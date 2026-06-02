# AGENTS.md — domotica-atmega2560-c

## Project Overview

Sistema domótico para vivienda unifamiliar basado en ATmega2560 (Arduino Mega) con control de seguridad, acceso RFID, iluminación dimerizada, temperatura, horno remoto, equipo de sonido y lista de mercado. Interfaz mediante LCD 16x2 y teclado matricial 4x4. Comunicación con PC vía USART.

## Tech Stack

| Componente | Especificación |
|---|---|
| Microcontrolador | ATmega2560 (16MHz, 256KB Flash, 8KB SRAM, 4KB EEPROM) |
| Lenguaje | C++11 |
| Framework | Arduino (AVR-GCC) |
| Build System | PlatformIO (con respaldo CMake para CLion) |
| IDE | CLion + PlatformIO plugin |
| Simulación | Proteus (sin placa física aún) |

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
  ├── src/
  │   ├── main.ino                  ← Punto de entrada (loop vacío)
  │   └── Seguridad.cpp             ← Clase Seguridad (esqueleto)
  ├── include/
  │   └── Seguridad.h               ← Clase Seguridad (vacía)
  ├── platformio.ini                ← Config PlatformIO
  └── CMakeLists.txt                ← Config CMake/CLion
/DOCUMENTOS/                         ← Material de referencia
  ├── MARKDOWN/                     ← Notas y apuntes de clase
  ├── DOCUMENTACIÓN/                ← PDFs y PPTX originales
  └── CODIGO-CLASES/                ← Códigos de ejemplo funcionales
/openspec/                           ← Planificación OpenSpec
.opencode/                           ← Configuración del asistente
```

## Source Files

| Archivo | Propósito | Estado |
|---|---|---|
| `Proyecto/src/main.ino` | Punto de entrada Arduino, loop principal | Esqueleto (loop vacío) |
| `Proyecto/src/Seguridad.cpp` | Implementación subsistema seguridad | Esqueleto (solo include) |
| `Proyecto/include/Seguridad.h` | Declaración clase Seguridad | Esqueleto (clase vacía) |
| `Proyecto/platformio.ini` | Configuración PlatformIO | Completado |
| `Proyecto/CMakeLists.txt` | Configuración CMake/CLion | Completado |

## Setup Commands

```bash
pio run                    # Compilar
pio run --target clean     # Limpiar build
pio run --target upload    # Subir a Proteus
pio device monitor         # Monitor serial (9600 baud)
```

## Code Style & Conventions

- Lenguaje: C++11 estándar
- Archivos `.ino` para entry point Arduino, `.h`/`.cpp` para módulos
- Comentarios en español (alineado con documentación y clases)
- Clases en `PascalCase`, funciones en `camelCase`, constantes en `UPPER_SNAKE`
- Include guards en todos los headers (`#ifndef`/`#define`/`#endif`)
- Prefijo de funciones de hardware con el subsistema (ej: `lcd_comando()`, `rfid_leer()`)

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
7. **Consultar códigos de clase** en `DOCUMENTOS/CODIGO-CLASES/` como referencia de drivers funcionales
8. **Documentar decisiones de diseño** a medida que se toman — actualizar `docs/` y `openspec/` según corresponda
9. **Las Design Decisions Pending** requieren decisión explícita — no asumir valores por defecto
