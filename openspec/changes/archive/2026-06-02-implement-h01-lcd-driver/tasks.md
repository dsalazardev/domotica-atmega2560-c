## 1. Documentación — Resolver conflicto T-01

- [x] 1.1 Actualizar `docs/12-pin-assignment.md`: mover LCD_EN de PB1 a PA1, RS de PB0 a PA0. Agregar filas PA0 (LCD_RS) y PA1 (LCD_EN). Quitar conflicto LCD_EN vs SPI_SCK de la tabla de conflictos.

## 2. Crear Header del Driver LCD

- [x] 2.1 Crear `Proyecto/include/LCD.h` con include guard, defines de pines (LCD_RS PA0, LCD_EN PA1, LCD_D4 PA4, LCD_D5 PA5, LCD_D6 PA6, LCD_D7 PA7), defines de registros (LCD_DDR DDRA, LCD_PORT PORTA), prototipos de funciones (lcd_init, lcd_comando, lcd_dato, lcd_nibble, lcd_pulso, lcd_posicion, lcd_borrar, lcd_imprimir), y macros para comandos comunes (LCD_CLEAR 0x01, LCD_HOME 0x02, LCD_ENTRY_MODE 0x06, LCD_DISPLAY_ON 0x0C, LCD_FUNCTION_SET 0x28).

## 3. Implementar Driver LCD

- [x] 3.1 Crear `Proyecto/src/LCD.cpp` con implementación de `lcd_pulso()`: generar flanco descendente en LCD_EN (PA1 HIGH → _delay_us(1) → PA1 LOW)
- [x] 3.2 Implementar `lcd_nibble(uint8_t dato)`: colocar bits 0-3 en PA4-PA7, llamar lcd_pulso()
- [x] 3.3 Implementar `lcd_comando(uint8_t cmd)`: RS=0, enviar nibble alto (cmd>>4), enviar nibble bajo (cmd & 0x0F)
- [x] 3.4 Implementar `lcd_dato(uint8_t dato)`: RS=1, enviar nibble alto (dato>>4), enviar nibble bajo (dato & 0x0F)
- [x] 3.5 Implementar `lcd_init()`: configurar DDRA pines como salida, secuencia datasheet: esperar 40ms → 3x nibble 0x03 → nibble 0x02 → comando 0x28 → comando 0x0C → comando 0x06 → comando 0x01 → esperar 2ms
- [x] 3.6 Implementar `lcd_posicion(uint8_t fila, uint8_t col)`: calcular dirección DDRAM (0x00 + col para fila 0, 0x40 + col para fila 1), enviar como comando con bit 7 en 1 (0x80 | addr)
- [x] 3.7 Implementar `lcd_borrar()`: enviar comando LCD_CLEAR (0x01)
- [x] 3.8 Implementar `lcd_imprimir(char* texto)`: iterar sobre la cadena llamando lcd_dato() para cada carácter hasta encontrar null

## 4. Integración en main.ino

- [x] 4.1 Modificar `Proyecto/src/main.ino`: incluir `LCD.h`, en setup() llamar lcd_init() y lcd_imprimir("Hola Mundo!"), en loop() mantener _delay_ms(50). Eliminar código muerto (bucle for vacío).

## 5. Verificación

- [x] 5.1 Ejecutar `pio run` y verificar que compila sin errores ni warnings
