## ADDED Requirements

### Requirement: Documentar restricciones de la profesora en AGENTS.md
El sistema de documentación SHALL incluir una sección "Restricciones de la Profesora (Nubia Liliana)" en AGENTS.md que contenga el mensaje textual completo, una tabla de implicaciones técnicas, y una nota sobre qué librerías SÍ están permitidas.

#### Scenario: Sección visible al leer AGENTS.md
- **WHEN** un desarrollador lee AGENTS.md de principio a fin
- **THEN** encuentra la sección de restricciones antes de Code Style & Conventions

#### Scenario: Tabla de implicaciones completa
- **WHEN** se revisa la sección de restricciones
- **THEN** contiene 5 filas: servomotor (sin Servo.h), teclado (sin Keypad.h), delays (sin _delay_*()), Arduino IDE (estructura .ino plana), sin software adicional

#### Scenario: Nota de librerías permitidas
- **WHEN** se lee la sección de restricciones
- **THEN** incluye una nota explícita de que `<avr/io.h>`, `<stdint.h>`, `millis()`, `micros()`, `Serial` y `EEPROM` SÍ están permitidos

### Requirement: Tech Stack debe reflejar Arduino IDE obligatorio
El Tech Stack en AGENTS.md SHALL listar "Arduino IDE (obligatorio para revisión)" como IDE principal y "CLion + PlatformIO" como opcional para desarrollo.

#### Scenario: Arduino IDE listado primero
- **WHEN** se lee la fila de IDE en Tech Stack
- **THEN** Arduino IDE aparece como opción obligatoria para revisión

### Requirement: Repository Structure debe mostrar estructura destino
El árbol de Repository Structure en AGENTS.md SHALL mostrar la estructura destino (todo .ino plano en Proyecto/) además de la estructura actual.

#### Scenario: Árbol destino visible
- **WHEN** se lee Repository Structure
- **THEN** se muestra un árbol con Proyecto.ino, LCD.ino, Teclado.ino, etc. en la raíz de Proyecto/

### Requirement: Code Style debe prohibir clases y delays
La sección Code Style & Conventions en AGENTS.md SHALL incluir reglas explícitas: sin clases C++, sin `_delay_ms()`, sin `_delay_us()`, sin `Servo.h`, sin `Keypad.h`. Solo funciones con prefijo.

#### Scenario: Prohibiciones listadas
- **WHEN** se lee Code Style & Conventions
- **THEN** se listan las 5 prohibiciones y se indica que el estilo permitido son funciones con prefijo de subsistema
