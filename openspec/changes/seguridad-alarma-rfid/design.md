## Context

Actualmente `Seguridad.ino` es un esqueleto vacío. El sistema tiene Drivers.ino (LCD, teclado, USART) y Timer.ino (timer5 tick 1ms) funcionales. Los subsistemas de seguridad (R1-R11) deben implementarse sin librerías externas, sin `_delay_*()` y compilar en Arduino IDE.

## Goals / Non-Goals

**Goals:**
- Alarma dual con sensores reed (puertas/ventanas) y humo, notificación por USART
- Validación de código de seguridad 4 dígitos para activar/desactivar alarma
- Comunicación SPI con módulo RFID MFRC522 para lectura/escritura de tarjetas
- Enrolamiento y borrado de UIDs autorizados almacenados en EEPROM
- Control de puerta principal (imán via GPIO) y garaje (servomotor Timer1/OC1A)
- Habitación de juegos con cuota de usos en tarjeta RFID, descuento y recarga
- Menú en LCD para navegar todas las funciones

**Non-Goals:**
- Subsistemas de ambiente (iluminación, temperatura, horno, sonido) — se implementan en otro cambio
- Lista de mercado — se implementa en otro cambio
- Comunicación I2C — no requerida para este subsistema

## Decisions

| Decisión | Opción elegida | Alternativas | Razón |
|---|---|---|---|
| Protocolo RFID | SPI (MFRC522) | I2C | SPI-Maestro.ino es el ejercicio de clase que conocen |
| Servomotor | Timer1/OC1A (PB5) | Timer5/OC5A (PL3) | PL3 compartido con TEC_FILA3 del teclado (docs/12-pin-assignment.md) |
| Almacenamiento UIDs | EEPROM interna ATmega2560 (4KB) | Memoria externa | Suficiente para ~40 UIDs de 4 bytes + metadata |
| Cuota juegos | EEPROM local indexada por UID | Escritura en tarjeta RFID via SPI | No se ha probado escritura RFID en Proteus. EEPROM funciona igual en simulación y físico. La cuota queda asociada al UID, no a la tarjeta física. |
| Código seguridad | 4 dígitos numéricos, almacenado en EEPROM | Alfanumérico, en RAM | Simple, suficiente para vivienda unifamiliar (Design Decision #2) |
| Timers alarma | millis() no-bloqueante y contador tick | Interrupciones PCINT/INT | Consistente con el resto del proyecto, evita delays |
| Sensor temperatura | LM35 (ADC) | DHT11, DS18B20 | Para determinar en cambio de ambiente si se usa LM35 (Design Decision #1) — en seguridad solo sensores reed y humo digital |

## Risks / Trade-offs

- [Riesgo] Módulo RFID MFRC522 no probado en Proteus → Mitigación: SPI-Maestro.ino ya funciona como referencia, adaptar pines y timing
- [Riesgo] EEPROM tiene vida útil de ~100k escrituras → Mitigación: enrolar/borrar son operaciones de administrador poco frecuentes
- [Riesgo] Servomotor puede requerir calibración de ángulos → Mitigación: usar constantes para ángulo min/max, ajustables en código
- [Riesgo] Cuota de juegos en EEPROM local: si se reemplaza la tarjeta del niño, se pierde la cuota acumulada → Mitigación: los padres pueden recargar la cuota desde el menú tras enrolar la nueva tarjeta

## Open Questions

- Pin exacto para sensor de humo (digital) — Design Decision #5: número de sensores
- Tiempo exacto de pulso para servomotor (ángulo 0° → 90°) — calibrar en Proteus
