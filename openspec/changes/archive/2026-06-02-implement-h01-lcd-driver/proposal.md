## Why

El proyecto domótica no tiene un solo archivo de código funcional — `main.ino` tiene un loop vacío. Sin un driver LCD funcional no se puede mostrar información al usuario (R18: *toda* la información se muestra por LCD), y sin el árbol de menús ningún subsistema puede operar. HU-01 (Driver LCD 4-bit) es la primera dependencia de toda la cadena de implementación. Además, el conflicto de pines T-01 (LCD_EN vs SPI_SCK en PB1) debe resolverse primero para evitar refactor costoso después.

## What Changes

- Resolver conflicto T-01: mover LCD_EN de PB1 a PA1, liberar PB1 para SPI_SCK
- Actualizar `docs/12-pin-assignment.md` con la asignación definitiva de pines LCD
- Crear driver LCD 16x2 en modo 4 bits (`LCD.h`, `LCD.cpp`)
- Modificar `main.ino` para inicializar LCD y mostrar mensaje de prueba
- Compilar y verificar con `pio run`

## Capabilities

### New Capabilities
- `lcd-4bit-driver`: Driver de bajo nivel para LCD 16x2 en modo 4 bits. Incluye inicialización, envío de comandos, escritura de datos, posicionamiento de cursor, borrado de pantalla e impresión de cadenas.

### Modified Capabilities

Ninguna — no existen specs previas.

## Impact

- **Archivos nuevos**: `Proyecto/include/LCD.h`, `Proyecto/src/LCD.cpp`
- **Archivos modificados**: `Proyecto/src/main.ino`, `docs/12-pin-assignment.md`
- **Pines modificados**: LCD_EN migra de PB1 a PA1. PB1 queda libre para SPI_SCK. RS se asigna a PA0.
- **Dependencias futuras**: HU-03 (Sistema de Menús) depende de este driver. Toda la Fase 2-7 depende de HU-03.
