## Context

Estado actual: el proyecto compila pero `main.ino` tiene un loop vacío. El LCD 16x2 (ST7066) y teclado matricial son la única interfaz con el usuario (R18). No existe ningún driver de hardware implementado.

El código de clase `I2C-Maestro.ino` (298 líneas) contiene un driver LCD 4-bit funcional con init, comando, dato, nibble, pulso, posicionar, borrar e imprimir. También incluye patrón de teclado matricial. El datasheet del ST7066 especifica la secuencia exacta de inicialización en modo 4 bits.

El conflicto T-01 está documentado en `docs/12-pin-assignment.md`: PB1 compartido entre LCD_EN y SPI_SCK. La resolución acordada es mover LCD_EN a PA1.

## Goals / Non-Goals

**Goals:**
- Resolver conflicto T-01 actualizando `docs/12-pin-assignment.md`
- Implementar driver LCD 16x2 en modo 4 bits funcional y probado
- Verificar compilación con `pio run`

**Non-Goals:**
- No implementar otros subsistemas (teclado, menús, RFID, etc.)
- No probar en hardware físico (solo simulación Proteus)
- No modificar platformio.ini ni CMakeLists.txt

## Decisions

### Decisión 1: Asignación definitiva de pines LCD
- **Decisión**: PA0=RS, PA1=EN, PA4-PA7=D4-D7
- **Alternativa considerada**: PC0-PC5 para datos (rechazado: requiere más cambios en código de clase de referencia)
- **Por qué**: Compatible con el patrón del I2C-Maestro.ino que usa PORTA, solo se reasignan RS (antes PB0→PA0) y EN (antes PB1→PA1). Libera PB1 para SPI.

### Decisión 2: Arquitectura del driver
- **Decisión**: Dos archivos: `LCD.h` (header con prototipos y defines) y `LCD.cpp` (implementación)
- **Alternativa considerada**: Driver monolítico en main.ino (rechazado: no escala para 20 HUs futuras)
- **Por qué**: Consistente con el estándar del proyecto (clases en PascalCase, .h/.cpp separados)

### Decisión 3: Control de pines LCD
- **Decisión**: Manipulación directa de registros (PORTA, DDRA) sin Arduino digitalWrite()
- **Alternativa considerada**: Usar digitalWrite() del framework Arduino (rechazado: más lento, más código)
- **Por qué**: El ATmega2560 tiene recursos limitados (8KB SRAM). Control directo de registros es más eficiente y es el patrón usado en los códigos de clase de referencia.

### Decisión 4: Tiempos de inicialización
- **Decisión**: Usar `_delay_ms()` de avr-libc/delay.h para los retardos de init
- **Alternativa considerada**: Timers hardware (rechazado: desperdicio de recurso para retardos de una sola vez)
- **Por qué**: Los retardos de init LCD ocurren una sola vez en setup(). No hay razón para ocupar un timer. La librería `<util/delay.h>` es precisa con F_CPU definido.

## Risks / Trade-offs

| Riesgo | Mitigación |
|---|---|
| Pines incorrectos en Proteus | La asignación es tentativa; verificar en simulación antes de soldar |
| Tiempos de init fuera de spec | Seguir estrictamente la secuencia del datasheet ST7066 con márgenes de seguridad (2x los mínimos) |
| Conflicto con otros periféricos en PORTA | PORTA se dedica exclusivamente a LCD; ningún otro periférico lo usa |
| Endianness de nibbles en modo 4 bits | El código de referencia I2C-Maestro.ino ya maneja esto correctamente |
