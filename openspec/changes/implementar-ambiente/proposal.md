## Why

Actualmente `Ambiente.ino` es un esqueleto vacío de 33 líneas. El sistema ya tiene Drivers.ino (LCD, teclado, USART, SPI), Timer.ino (tick 1ms), Seguridad.ino (alarma, RFID, juegos) y Lista.ino (EEPROM) funcionales. Faltan los subsistemas de iluminación dimerizada (R12), control de temperatura (R13), horno remoto (R14) y equipo de sonido remoto (R15).

## What Changes

- Implementar `Ambiente.ino` completo (~200 líneas) con los 4 subsistemas
- El archivo se añade al sketch plano existente (6 archivos .ino)
- Proyecto.ino: agregar llamadas a `ambiente_actualizar()` en loop y `iluminacion_iniciar()`, `temperatura_iniciar()` en setup
- No requiere cambios en Drivers.ino, Timer.ino, Seguridad.ino ni Lista.ino

## Capabilities

### New Capabilities
- `iluminacion-dimerizada`: Timer3 Fast PWM en PE3 (OC3A), control por potenciómetro (ADC)
- `temperatura-control`: LM35 + ADC, histéresis ±2°C, calefactor y ventilador
- `horno-remoto`: Relé + cuenta regresiva con timer_tick, encendido/apagado remoto
- `sonido-remoto`: Timer2 Fast PWM (62.5kHz) + filtro RC, relé de encendido

## Impact

- `Proyecto/Ambiente.ino`: 33 → ~250 líneas
- `Proyecto/Proyecto.ino`: +3 líneas en setup, +1 línea en loop
- `docs/06-subsystem-environment.md`: actualizar si hay cambios de diseño
- Pines nuevos: PE3 (OC3A iluminación), PF0 (ADC0 LM35), PF1 (ADC1 potenciómetro), PC5 (calefactor), PC6 (ventilador), PC7 (horno), PH6 (OC2B sonido), PH7 (relé sonido)
- Sin cambios en otros archivos
