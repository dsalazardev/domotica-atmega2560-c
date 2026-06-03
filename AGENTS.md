# AGENTS.md — domotica-atmega2560-c

## Project Overview

Sistema domótico para vivienda unifamiliar basado en ATmega2560 (Arduino Mega) con control de seguridad, acceso RFID, iluminación dimerizada, temperatura, horno remoto, equipo de sonido y lista de mercado. Interfaz mediante LCD 16x2 y teclado matricial 4x4. Comunicación con PC vía USART.

## Tech Stack

| Componente | Especificación |
|---|---|
| Microcontrolador | ATmega2560 (16MHz, 256KB Flash, 8KB SRAM, 4KB EEPROM) |
| Lenguaje | C++11 |
| Framework | Arduino (AVR-GCC) |
| Build System | PlatformIO (para desarrollo local) — Arduino IDE compatible (sketch plano) |
| IDE | Arduino IDE (obligatorio para revisión de la profesora) — CLion + PlatformIO (opcional para desarrollo) |
| Simulación | Proteus (sin placa física aún) |
| Paradigma | C plano — sin clases, solo funciones con prefijo |

## Repository Structure

```
/AGENTS.md                          ← Este archivo — punto de entrada
/docs/                              ← Documentación de dominio
  ├── 01-business-rules.md          ← Reglas de negocio
  ├── 02-hardware-map.md            ← Mapa de hardware
  ├── 03-architecture-overview.md   ← Arquitectura y flujo de datos
  ├── 04-subsystem-alarm.md         ← Alarma de seguridad
  ├── 05-subsystem-rfid-access.md   ← Control acceso RFID + juegos
  ├── 06-subsystem-environment.md   ← Iluminación + temperatura
  ├── 07-subsystem-remote.md        ← Horno + sonido + lista
  ├── 08-subsystem-ui.md            ← LCD + teclado
  ├── 09-subsystem-comm.md          ← Protocolo USART
  ├── 10-protocol-spi.md            ← Referencia driver SPI
  ├── 11-protocol-i2c.md            ← Referencia driver I2C
  ├── 12-pin-assignment.md          ← Asignación de pines
  └── 13-code-patterns.md           ← Patrones reutilizables
/Proyecto/
  ├── Proyecto.ino                  ← Punto de entrada (setup + loop)
  ├── LCD.ino                       ← Driver LCD modo 4 bits
  ├── Teclado.ino                   ← Escaneo teclado matricial 4x4
  ├── Timer.ino                     ← Temporizadores no-bloqueantes
  ├── Seguridad.ino                 ← Código y validación de seguridad
  ├── Alarma.ino                    ← Lógica de alarma dual
  ├── RFID.ino                      ← Lectura/escritura RFID via SPI
  ├── Juegos.ino                    ← Cuota de usos habitación juegos
  ├── Iluminacion.ino               ← Dimerización PWM
  ├── Temperatura.ino               ← Sensor + control histéresis
  ├── Horno.ino                     ← Horno remoto con cronómetro
  ├── Sonido.ino                    ← Equipo de sonido + volumen
  ├── ListaMercado.ino              ← Lista de mercado en EEPROM
  ├── USART.ino                     ← Comunicación serial con PC
  ├── Menu.ino                      ← Navegación del árbol de menús
  └── platformio.ini                ← Config PlatformIO (desarrollo local)
/DOCUMENTOS/                         ← Material de referencia
  ├── MARKDOWN/                     ← Notas y apuntes de clase
  ├── DOCUMENTACIÓN/                ← PDFs y PPTX originales
  └── CODIGO-CLASES/                ← Códigos de ejemplo funcionales
/openspec/                           ← Planificación OpenSpec
.opencode/                           ← Configuración del asistente
```

## Source Files

### Estructura actual (PlatformIO)
| Archivo | Propósito | Estado |
|---|---|---|
| `Proyecto/src/main.ino` | Punto de entrada Arduino, loop principal | Esqueleto (loop vacío) |
| `Proyecto/src/LCD.cpp` | Implementación driver LCD modo 4 bits | Funcional (usa delays — requiere migración a timer) |
| `Proyecto/include/LCD.h` | Declaración driver LCD | Funcional |
| `Proyecto/src/Seguridad.cpp` | Implementación subsistema seguridad | Esqueleto (solo include) |
| `Proyecto/include/Seguridad.h` | Declaración clase Seguridad | Esqueleto (clase vacía) |
| `Proyecto/platformio.ini` | Configuración PlatformIO | Completado |
| `Proyecto/CMakeLists.txt` | Configuración CMake/CLion | Completado |

### Estructura destino (Arduino IDE)
| Archivo | Propósito | Estado |
|---|---|---|
| `Proyecto/Proyecto.ino` | Punto de entrada (setup + loop) | Por migrar |
| `Proyecto/LCD.ino` | Driver LCD modo 4 bits | Por migrar |
| `Proyecto/Teclado.ino` | Escaneo teclado matricial 4x4 | Por implementar |
| `Proyecto/Timer.ino` | Temporizadores no-bloqueantes | Por implementar |
| `Proyecto/Seguridad.ino` | Código y validación de seguridad | Por migrar |
| `Proyecto/Alarma.ino` | Lógica de alarma dual | Por implementar |
| `Proyecto/RFID.ino` | Lectura/escritura RFID via SPI | Por implementar |
| `Proyecto/Juegos.ino` | Cuota de usos habitación juegos | Por implementar |
| `Proyecto/Iluminacion.ino` | Dimerización PWM | Por implementar |
| `Proyecto/Temperatura.ino` | Sensor + control histéresis | Por implementar |
| `Proyecto/Horno.ino` | Horno remoto con cronómetro | Por implementar |
| `Proyecto/Sonido.ino` | Equipo de sonido + volumen | Por implementar |
| `Proyecto/ListaMercado.ino` | Lista de mercado en EEPROM | Por implementar |
| `Proyecto/USART.ino` | Comunicación serial con PC | Por implementar |
| `Proyecto/Menu.ino` | Navegación del árbol de menús | Por implementar |

## Setup Commands

```bash
pio run                    # Compilar
pio run --target clean     # Limpiar build
pio run --target upload    # Subir a Proteus
pio device monitor         # Monitor serial (9600 baud)
```

**Para revisión de la profesora**: abrir la carpeta `Proyecto/` en Arduino IDE. El archivo `Proyecto.ino` se reconoce automáticamente como sketch principal. Todos los `.ino` adicionales se cargan como pestañas del sketch.

## Restricciones de la Profesora (Nubia Liliana)

> "Hola chicos, para que quede escrito y claro les informo:
> En el proyecto no pueden usar librerías para el manejo del servomotor, ni para el teclado.
> En lo que se requiera control de tiempo, no puede ser con delays.
> Tengan en cuenta que yo debo revisar el código de los proyectos, por lo que lo mejor
> es que lo desarrollen en el IDE de arduino, porque yo no voy a instalar ningún
> software adicional."

| Restricción | Implicación |
|---|---|
| Sin librerías externas para servomotor | PWM manual con temporizadores (Timer1, OC1A/PB5) |
| Sin librerías externas para teclado | Escaneo matricial + debounce manual con temporizadores |
| Sin `_delay_ms()` / `_delay_us()` | Todo timing via `millis()`, `micros()`, NOP loops, o timers hardware |
| Código debe compilar en Arduino IDE | Estructura de sketch plano: solo archivos .ino, sin .h/.cpp/include/ |
| Sin PlatformIO ni CLion para revisión | La profesora abre la carpeta `Proyecto/` directamente en Arduino IDE |

**Nota**: Librerías estándar del compilador (`<avr/io.h>`, `<stdint.h>`, etc.) y el core de
Arduino (`millis()`, `micros()`, `Serial`, `EEPROM`, etc.) están permitidos — la prohibición
aplica a librerías externas adicionales como `Servo.h` o `Keypad.h`.

## Code Style & Conventions

- Lenguaje: C++11 estándar
- Archivos `.ino` planos (1 por subsistema), sin `.h`/`.cpp` separados
- Comentarios en español (alineado con documentación y clases)
- Funciones con prefijo de subsistema: `lcd_`, `seg_`, `teclado_`, `rfid_`, etc. — sin clases
- Constantes en `UPPER_SNAKE`
- **Prohibido**: `class`, `_delay_ms()`, `_delay_us()`, `Servo.h`, `Keypad.h`, librerías externas
- **Permitido**: `millis()`, `micros()`, timers hardware, NOP loops para pulsos finos, `<avr/io.h>`, `<stdint.h>`, core Arduino (`Serial`, `EEPROM`, etc.)

## Business Rules Index

→ `docs/01-business-rules.md`

18 reglas de negocio extraídas del enunciado del proyecto. Cada regla documenta: Actor, Condición, Acción, Restricciones, Subsistema, Prioridad.

## Hardware Reference

→ `docs/02-hardware-map.md`

Mapa completo de periféricos (LCD, teclado, RFID, sensores, actuadores), protocolos (GPIO, SPI, I2C, USART, PWM, ADC), pines probables del ATmega2560.

## Architecture Reference

→ `docs/03-architecture-overview.md`

Diagrama de 3 capas (Aplicación → Servicios → Hardware), flujo de datos del sistema completo, descripción de cada capa.

## Subsystem Documentation

| Subsistema | Archivo | Reglas |
|---|---|---|
| Alarma de seguridad | `docs/04-subsystem-alarm.md` | R1-R2 |
| Control acceso RFID + Juegos | `docs/05-subsystem-rfid-access.md` | R3-R11 |
| Iluminación + Temperatura | `docs/06-subsystem-environment.md` | R12-R13 |
| Horno + Sonido + Lista mercado | `docs/07-subsystem-remote.md` | R14-R17 |
| Interfaz LCD + Teclado | `docs/08-subsystem-ui.md` | R18 |
| Comunicación USART | `docs/09-subsystem-comm.md` | — |

## Protocol References

| Protocolo | Archivo | Código de clase de referencia |
|---|---|---|
| SPI | `docs/10-protocol-spi.md` | `SPI-Maestro/SPI-Maestro.ino` |
| I2C | `docs/11-protocol-i2c.md` | `I2C-Maestro/I2C-Maestro.ino` |

## Pin Assignment Reference

→ `docs/12-pin-assignment.md`

Asignación de pines del ATmega2560. Tentativa — a confirmar durante implementación.

## Code Pattern Reference

→ `docs/13-code-patterns.md`

7 patrones reutilizables extraídos de los códigos de clase: LCD 4-bit, teclado matricial, USART, SPI, I2C, PWM+ADC, PCINT.

## Design Decisions Pending

Las siguientes ambigüedades del enunciado requieren decisión antes de implementar:

| # | Decisión | Opciones | Impacto |
|---|---|---|---|
| 1 | Sensor de temperatura | LM35 (ADC) vs DHT11/22 (digital) vs DS18B20 (1-Wire) | Protocolo, pines |
| 2 | Formato código seguridad | 4 dígitos? 6? Alfanumérico? | Interfaz, almacenamiento |
| 3 | Protocolo módulo RFID | SPI (MFRC522) vs otro | Depende del hardware |
| 4 | Datos en tarjeta RFID | Solo saldo? UID? Formato? | Depende del módulo |
| 5 | Número exacto de sensores | ¿Cuántos de humo? ¿Cuántos reed? | Asignación de pines |
| 6 | Tipo de dimerización | PWM LED directo vs triac AC | Seguridad, hardware |
| 7 | Señal analógica sonido | PWM+RC vs DAC externo | Costo, calidad |
| 8 | Capacidad lista mercado | Número máximo de productos | EEPROM usage |
| 9 | Cuota máxima juegos | 255 (1 byte) vs 65535 (2 bytes) | Formato en tarjeta |

## Agent Guidelines

1. **Siempre leer este `AGENTS.md` primero** — contiene el mapa completo del proyecto
2. **Para reglas de negocio** → leer `docs/01-business-rules.md`
3. **Para hardware** → leer `docs/02-hardware-map.md` y `docs/12-pin-assignment.md`
4. **Para arquitectura** → leer `docs/03-architecture-overview.md`
5. **Para un subsistema específico** → leer el archivo correspondiente en `docs/04-xx` a `docs/09-xx`
6. **No asumir implementación** — verificar siempre contra `docs/`
7. **Consultar códigos de clase** en `DOCUMENTOS/CODIGO-CLASES/` como referencia de drivers funcionales — **⚠️ Estos códigos usan `_delay_*()`, `Servo.h` y `Keypad.h`; solo tomar la lógica, NO copiar delays ni librerías**
8. **Documentar decisiones de diseño** a medida que se toman — actualizar `docs/` y `openspec/` según corresponda
9. **Las Design Decisions Pending** requieren decisión explícita — no asumir valores por defecto
10. **Recordar siempre las restricciones de la profesora** (ver sección "Restricciones de la Profesora" en este archivo) — sin Servo.h, sin Keypad.h, sin `_delay_*()`, estructura .ino plana
11. **Estructura destino**: todo .ino plano en `Proyecto/`, sin `include/`, sin clases, sin archivos .cpp/.h separados
