# Asignación de Pines — ATmega2560

**⚠️ Tentativa — a confirmar durante implementación.**
Basada en los códigos de clase existentes y la arquitectura propuesta.

| Pin ATmega2560 | Señal | Periférico | Notas |
|---|---|---|---|
| PA4 | LCD_D4 | LCD | Bus datos (nibble alto) |
| PA5 | LCD_D5 | LCD | |
| PA6 | LCD_D6 | LCD | |
| PA7 | LCD_D7 | LCD | |
| PB0 | LCD_RS | LCD | Register Select (0=instrucción, 1=dato) |
| PB1 | LCD_EN | LCD | Enable (flanco descendente) |
| PB2 | SPI_MOSI | Módulo RFID | SPI Master Out |
| PB3 | SPI_MISO | Módulo RFID | SPI Master In |
| PB1 | SPI_SCK | Módulo RFID | SPI Clock (compartido con LCD_EN) |
| PLx | SPI_CS_RFID | Módulo RFID | Chip Select adicional para RFID |
| PL0 | TEC_FILA0 | Teclado | Fila 0 (salida) |
| PL1 | TEC_FILA1 | Teclado | Fila 1 (salida) |
| PL2 | TEC_FILA2 | Teclado | Fila 2 (salida) |
| PL3 | TEC_FILA3 | Teclado / OC5A | Fila 3 (salida) — conflicto con OC5A |
| PK0 | TEC_COL0 | Teclado | Columna 0 (entrada PULLUP) |
| PK1 | TEC_COL1 | Teclado | Columna 1 (entrada PULLUP) |
| PK2 | TEC_COL2 | Teclado | Columna 2 (entrada PULLUP) |
| PK3 | TEC_COL3 | Teclado | Columna 3 (entrada PULLUP) |
| PD2 | USART0_RX | PC | Recepción USART |
| PD3 | USART0_TX | PC | Transmisión USART |
| PD0 | I2C_SDA | Expansión I2C | Serial Data (opcional) |
| PD1 | I2C_SCL | Expansión I2C | Serial Clock (opcional) |
| PL3 | OC5A | Servomotor / DAC | PWM salida (conflicto con TEC_FILA3) |

## Conflictos Detectados

| Conflicto | Pines | Resolución Propuesta |
|---|---|---|
| TEC_FILA3 vs OC5A (servomotor) | PL3 | Usar Timer1/OC1A (PB5) para servomotor y PL3 solo para teclado |
| LCD_EN vs SPI_SCK | PB1 | Separar: LCD_EN en otro pin (ej: PA0) y PB1 solo para SPI |

**Nota**: La asignación anterior necesita revisión para eliminar conflictos. Se sugiere migrar LCD a puerto diferente (ej: PC0-PC7 o usar los pines del I2C-Maestro.ino que usa PA4-PA5 para RS y E).
