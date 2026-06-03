## Why

La documentación del proyecto (AGENTS.md + 13 docs/) refleja una estructura PlatformIO con src/ + include/ + clases C++ + delays bloqueantes que ya no es válida. La profesora Nubia Liliana impuso restricciones: sin Servo.h, sin Keypad.h, sin `_delay_*()`, solo archivos .ino planos, y revisión exclusiva en Arduino IDE. La documentación debe actualizarse para reflejar estas restricciones o guiará al lector hacia implementaciones incorrectas.

## What Changes

- **AGENTS.md**: ~7 cambios — Tech Stack, Repository Structure, Source Files, Setup Commands, Code Style, Agent Guidelines, y nueva sección "Restricciones de la Profesora"
- **docs/02-hardware-map.md**: 2 cambios — corregir pines del LCD (PA0/PA1, no PB0/PB1) y mover servomotor de PL3 a PB5
- **docs/04-subsystem-alarm.md**: 1 cambio — "delay" → "temporización no-bloqueante con millis()"
- **docs/05-subsystem-rfid-access.md**: 1 cambio — "Timer1 o Timer5" → "Timer1/OC1A en PB5"
- **docs/08-subsystem-ui.md**: 3 cambios — notas sobre teclado sin Keypad.h, LCD sin delays, escaneo round-robin
- **docs/12-pin-assignment.md**: 6 cambios — PL3 ya no es compartido, agregar PB5, marcar conflictos como resueltos
- **docs/13-code-patterns.md**: 4 cambios — banner de advertencia + notas en 3 patrones

## Capabilities

### New Capabilities
- `doc-restricciones-profesora`: documento de referencia central con las restricciones de la profesora, sus implicaciones técnicas, y qué está permitido vs prohibido

### Modified Capabilities
- `lcd-4bit-driver`: los requisitos de timing agregan la condición "sin _delay_*()" — se añade un spec delta

## Impact

- **AGENTS.md**: reescritura parcial de 5 secciones + 1 sección nueva
- **docs/02-hardware-map.md**: corrección de error en pines LCD + actualización de pin de servomotor
- **docs/04 thru 13**: cambios menores (1-6 líneas cada uno, salvo 13-code-patterns.md que recibe un banner completo)
- **Ningún archivo de código** se toca. Solo documentación .md.
