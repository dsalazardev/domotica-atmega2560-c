## Context

Ambiente.ino debe implementar 4 subsistemas independientes que comparten el mismo archivo y el mismo ciclo de `ambiente_actualizar()`. Cada subsistema tiene su propio timer hardware o usa `millis()` para temporización no-bloqueante. El archivo sigue el mismo patrón que Seguridad.ino: funciones con prefijo de subsistema, variables static, sin clases.

## Goals / Non-Goals

**Goals:**
- Iluminación dimerizada por PWM (Timer3/OC3A) controlada por potenciómetro (ADC)
- Control de temperatura on/off con histéresis (LM35 + ADC, calefactor + ventilador)
- Horno remoto con temporizador de cuenta regresiva
- Sonido remoto con volumen analógico (PWM + filtro RC)
- Todas las funciones no-bloqueantes, sin `_delay_*()`

**Non-Goals:**
- Menú LCD para ambiente (iluminación, temperatura) — se implementa en otro cambio en Proyecto.ino
- Parser de comandos USART para horno/sonido — se implementa en otro cambio en Proyecto.ino
- Lista de mercado (R16-R17) — pendiente para Lista.ino
- Control PID de temperatura — solo histéresis on/off

## Decisions

| Decisión | Opción elegida | Alternativas | Razón |
|---|---|---|---|
| Sensor temperatura | LM35 (ADC0) | DHT11, DS18B20 | El ejercicio de clase usa LM35, ADC ya disponible, sin protocolos adicionales |
| Potenciómetro iluminación | ADC1 (PF1) | ADC0 compartido | ADC0 dedicado a LM35, ADC1 libre |
| Setpoint temperatura | 24°C fijo (#define) | Configurable por menú/USART | Simple, suficiente para vivienda. Configurable en iteración futura si se requiere |
| Control iluminación | Solo potenciómetro (ADC) | Solo teclado, ambos | Decisión del usuario: ajuste analógico continuo |
| PWM sonido | Timer2 Fast PWM, 8-bit, sin prescaler | Timer4, Timer0 | Timer2 es 8-bit y suficiente para ~62.5kHz > 10kHz requerido. No interfiere con otros timers |
| Frecuencia PWM iluminación | ~100Hz (prescaler 64, TOP=2500) | ~1kHz, ~50Hz | 100Hz evita parpadeo visible, compatible con dimerización LED/incandescente |
| Temporización horno | `timer_tick` (Timer5) | `millis()` | Consistente con el sistema de tick existente. Se usa `millis()` para evitar desbordamiento en cuentas largas |
| Pin relés (calefactor, ventilador, horno) | PORTC (PC5-PC7) | PORTx otros | PORTC no usado por otros subsistemas (sensores alarma usan PC0-PC4 como entrada) |

## Pin Assignment

| Pin | Señal | Periférico |
|---|---|---|
| PE3 | OC3A | Iluminación PWM |
| PF0 | ADC0 | LM35 temperatura |
| PF1 | ADC1 | Potenciómetro iluminación |
| PC5 | CALEFACTOR | Relé calefactor |
| PC6 | VENTILADOR | Relé ventilador |
| PC7 | HORNO | Relé horno |
| PH6 | OC2B | Sonido PWM |
| PH7 | SONIDO_RELE | Relé equipo sonido |

## Risks / Trade-offs

- [Riesgo] LM35 en Proteus: la simulación ADC puede no ser precisa → Mitigación: verificar con valores fijos de voltaje
- [Riesgo] Filtro RC para sonido: no simulado en Proteus → Mitigación: la señal PWM es correcta, el filtro se añade en hardware físico
- [Riesgo] Timer3 conflicto: si Seguridad.ino intenta usar Timer3 → Mitigación: Seguridad.ino usa Timer1 (servo) y millis() (alarma). Timer3 exclusivo para iluminación
- [Riesgo] Horno temporizador: tiempos largos (minutos) pueden desbordar millis() → Mitigación: contar en segundos con división, reiniciar millis() no es problema porque se usa diferencia

## Open Questions

- ¿Se necesita protección (Kp) para que calefactor y ventilador no se activen simultáneamente? Ya resuelto: lógica mutuamente excluyente en `temperatura_controlar()`
