# Plan de Implementación — Domótica ATmega2560

## Estado Actual por Regla de Negocio

| Regla | Descripción | Estado | Prioridad |
|-------|-------------|--------|-----------|
| **R1** | Alarma dual (intrusión + humo) | ⚠️ Solo intrusión | Alta |
| **R2** | Activación/desactivación con código | ✅ Completo | — |
| **R3** | Acceso RFID a vivienda | ✅ Completo | — |
| **R4** | Enrolamiento de personas | ✅ Completo | — |
| **R5** | Borrado de personas | ✅ Completo | — |
| **R6** | Dos puntos de acceso (PP + garaje) | ✅ Completo | — |
| **R7** | Puerta principal con imán | ✅ Completo | — |
| **R8** | Puerta garaje con servomotor | ✅ Completo | — |
| **R9** | Habitación juegos con cuota | ✅ Completo | — |
| **R10** | Descuento de acceso por ingreso | ✅ Completo | — |
| **R11** | Carga de accesos por padres | ✅ Completo | — |
| **R12** | Iluminación dimerizada | ✅ Completo | — |
| **R13** | Control temperatura (calefactor + ventilador) | ✅ Completo | — |
| **R14** | Horno remoto con tiempo y temperatura | ⚠️ Sin loopback USART | Alta |
| **R15** | Sonido remoto con control de volumen | ⚠️ Sin loopback USART | Alta |
| **R16** | Lista de mercado consultable remotamente | ✅ USART OK | — |
| **R17** | Formato lista (producto + cantidad) | ✅ Completo | — |
| **R18** | LCD como interfaz de visualización | ✅ Completo | — |

### Completo: 14/18 | Parcial: 3/18 (R14, R15) | Pendiente: 1/18 (R1 humo)

---

## Bugs Conocidos

| # | Bug | Archivo | Impacto | Fix |
|---|-----|---------|---------|-----|
| B1 | Timer5 configurado a 16ms en vez de 1ms | Timer.ino:8 | Bajo (`timer_tick` nunca se lee) | Prescaler `/64` en vez de `/1024` |
| B2 | `alarma_activa()` retorna `false` en estado DISPARADA | Seguridad.ino:171 | Medio (ESTADO? muestra alarma apagada aunque esté disparada) | Agregar chequeo `|| alarma_estado == EST_ALARMA_DISPARADA` |
| B3 | Sensor de humo eliminado (R1 incompleta) | Seguridad.ino | Alto — requisito de la profesora | Reasignar pines y agregar lógica |
| B4 | Sin menú LCD para lista de mercado | Proyecto.ino | Bajo (solo USART) | Agregar entrada en menú principal |
| B5 | Pin assignment doc desactualizado | docs/12-pin-assignment.md | Bajo (código es correcto) | Actualizar documento |

---

## Plan por Fases

### FASE 0 — Correcciones Inmediatas

**Objetivo**: arreglar lo que está roto ahora y confirmar que funciona.

| Tarea | Archivos | Descripción | Estado |
|-------|----------|-------------|--------|
| 0.1 | Seguridad.ino, Ambiente.ino | Sensor de humo (PC4) y reasignar reed switches | Pendiente |
| 0.2 | Ambiente.ino | ADC con AVCC ref + AREF a VCC en Proteus | Pendiente (probar) |
| 0.3 | Ambiente.ino | Sonido D10: limpiar COM2A antes de forzar PORTB LOW | ✅ Hecho |
| 0.4 | Timer.ino | Timer5: prescaler `/64` para tick 1ms | Pendiente |
| 0.5 | Seguridad.ino | `alarma_activa()` incluya DISPARADA | Pendiente |

### FASE 1 — Loopback USART (Maestro-Esclavo)

**Objetivo**: cablear y probar la comunicación USART1 ↔ USART2 para comando remoto.

| Tarea | Descripción | Estado |
|-------|-------------|--------|
| 1.1 | **En Proteus**: conectar PD3 (USART1-TX) → PH0 (USART2-RX) | Pendiente |
| 1.2 | **En Proteus**: conectar PH1 (USART2-TX) → PD2 (USART1-RX) | Pendiente |
| 1.3 | Probar desde Virtual Terminal: `HORNO ON 180 30` debe responder con `OK: esclavo=OK: horno encendido` | Pendiente |
| 1.4 | Probar: `SONIDO ON 50` debe responder `OK: esclavo=OK: sonido encendido` | Pendiente |
| 1.5 | Probar: `SONIDO OFF` debe apagar sonido vía esclavo | Pendiente |
| 1.6 | Probar: `ESTADO?` debe mostrar alarma, horno, sonido, temp | Pendiente |
| 1.7 | Probar: `TEMP?` debe mostrar temperatura actual | Pendiente |

**Diagrama de conexión**:

```
Maestro (USART1)           Esclavo (USART2)
  PD3/TXD1 ─────────────→ PH0/RXD2
  PD2/RXD1 ←───────────── PH1/TXD2
```

**Flujo de comando remoto**:

```
PC (USART0) → Maestro → USART1-TX → USART2-RX → Esclavo
                                                  → ejecuta acción
                                                  → responde por USART2-TX
PC (USART0) ← Maestro ← USART1-RX ← USART2-TX ← Esclavo
```

### FASE 2 — Sensor de Humo (R1)

**Objetivo**: completar alarma dual (intrusión + incendio) según R1.

| Tarea | Descripción | Estado |
|-------|-------------|--------|
| 2.1 | Reasignar pines PORTC para 5 sensores: humo (PC4), reed PP (PC7), reed GAR (PC6), reed VENTANA_SAL (PC2), reed VENTANA_COC (PC0) | Pendiente |
| 2.2 | Agregar `SENSOR_HUMO` en defines y lógica en `alarma_actualizar()` | Pendiente |
| 2.3 | Cambiar HORNO a otro pin (ej: PL6) para liberar PC0 | Pendiente |
| 2.4 | Agregar componente en Proteus: botón momentáneo para simular humo | Pendiente |
| 2.5 | Probar: presionar botón humo → mensaje "ALERTA: Humo en la vivienda" por serial | Pendiente |

### FASE 3 — Lista de Mercado en LCD

**Objetivo**: agregar interfaz local para la lista (hoy solo USART).

| Tarea | Descripción | Estado |
|-------|-------------|--------|
| 3.1 | Agregar `5.List` en menú principal | Pendiente |
| 3.2 | Agregar estado `MENU_LISTA` y submenú: `1.Agr 2.Ver 3.Vac` | Pendiente |
| 3.3 | Implementar ingreso de nombre con teclado numérico (ABC/2, DEF/3, etc.) | Pendiente |
| 3.4 | Implementar "Ver lista" con navegación por items | Pendiente |
| 3.5 | Implementar "Vaciar lista" con confirmación | Pendiente |

### FASE 4 — Cierre y Verificación

**Objetivo**: asegurar que todo funciona y es entregable.

| Tarea | Descripción | Estado |
|-------|-------------|--------|
| 4.1 | Probar todos los caminos del menú completo | Pendiente |
| 4.2 | Probar todos los comandos USART desde PC | Pendiente |
| 4.3 | Verificar que compila en Arduino IDE (abrir carpeta `Proyecto/`) | Pendiente |
| 4.4 | Verificar que NO hay `_delay_ms()`, `Servo.h`, `Keypad.h`, `class` | Pendiente |
| 4.5 | Actualizar pin assignment doc (`docs/12-pin-assignment.md`) | Pendiente |
| 4.6 | Generar .zip de `Proyecto/` para entrega | Pendiente |

---

## Asignación de Pines Final (Propuesta)

Después de reasignar para incluir sensor de humo y liberar conflictos:

| Pin | Señal | Función | Puerto |
|-----|-------|---------|--------|
| D0 | PE0/RXD0 | USART0 RX (a PC) | PE0 |
| D1 | PE1/TXD0 | USART0 TX (a PC) | PE1 |
| D2 | PE4/OC3B | ILUMINACIÓN PWM (Timer3) | PE4 |
| D4 | PG5 | LCD D4 | PG5 |
| D5 | PE3/OC3A | LCD D5 | PE3 |
| D6 | PH3/OC4A | LCD D6 | PH3 |
| D7 | PH4/OC4B | LCD D7 | PH4 |
| D8 | PH5/OC4C | LCD RS | PH5 |
| D9 | PH6/OC2B | LCD E | PH6 |
| D10 | PB4/OC2A | SONIDO PWM (Timer2) | PB4 |
| D11 | PB5/OC1A | SERVO GARAJE (Timer1) | PB5 |
| D30 | PC7 | SENSOR PUERTA PRINCIPAL (reed) | PC7 |
| D31 | PC6 | SENSOR PUERTA GARAJE (reed) | PC6 |
| D32 | PC5 | CALEFACTOR | PC5 |
| D33 | PC2 | SENSOR VENTANA SALA (reed) | PC2 |
| D34 | PC3 | LED ALARMA (verde) | PC3 |
| D35 | PC4 | SENSOR HUMO | PC4 |
| D36 | PC1 | VENTILADOR | PC1 |
| D37 | PC0 | SENSOR VENTANA COCINA (reed) | PC0 |
| D44 | PL5 | SONIDO RELE | PL5 |
| D45 | PL4 | LED IMÁN (rojo) | PL4 |
| D46-D49 | PL0-PL3 | TECLADO FILAS | PL0-PL3 |
| A8-A11 | PK0-PK3 | TECLADO COLUMNAS | PK0-PK3 |
| A0 | PF0 | TEMPERATURA (ADC) / POT testing | PF0 |
| A1 | PF1 | ILUMINACIÓN LDR (sin uso actual) | PF1 |
| AREF | — | A VCC (5V) para ADC estable | — |

---

## Checklist de Requisitos Profesora

- [ ] **Sin `_delay_ms()` ni `_delay_us()`** — todo con `millis()`, timers o NOP loops
- [ ] **Sin `Servo.h`** — PWM manual con Timer1
- [ ] **Sin `Keypad.h`** — escaneo matricial manual con debounce
- [ ] **Sin `class`** — solo funciones con prefijo
- [ ] **Estructura plana .ino** — 6 pestañas en Arduino IDE
- [ ] **Compila directo en Arduino IDE** — sin PlatformIO, sin librerías externas
- [ ] **Comunicación serial** — USART0 con PC a 9600 baud
- [ ] **Protocolo maestro-esclavo** — USART1 ↔ USART2 loopback
- [ ] **Enrolamiento y borrado RFID** — menú + EEPROM
- [ ] **Habitación juegos** — cuota, descuento, recarga por padres
- [ ] **Control remoto** — horno y sonido desde PC
- [ ] **Lista de mercado** — consultable remotamente
