# Asignación de Pines — ATmega2560

**⚠️ Pines LCD confirmados (T-01 resuelto). Resto tentativo — a confirmar durante implementación. (T-02 resuelto)**

| Pin ATmega2560 | Señal | Periférico | Notas |
|---|---|---|---|
| PA4 | LCD_D4 | LCD | Bus datos (nibble alto) |
| PA5 | LCD_D5 | LCD | |
| PA6 | LCD_D6 | LCD | |
| PA7 | LCD_D7 | LCD | |
| PA0 | LCD_RS | LCD | Register Select (0=instrucción, 1=dato) |
| PA1 | LCD_EN | LCD | Enable (flanco descendente) |
| PB1 | SPI_SCK | Módulo RFID | SPI Clock |
| PB2 | SPI_MOSI | Módulo RFID | SPI Master Out |
| PB3 | SPI_MISO | Módulo RFID | SPI Master In |
| PLx | SPI_CS_RFID | Módulo RFID | Chip Select adicional para RFID |
| PL0 | TEC_FILA0 | Teclado | Fila 0 (salida) |
| PL1 | TEC_FILA1 | Teclado | Fila 1 (salida) |
| PL2 | TEC_FILA2 | Teclado | Fila 2 (salida) |
| PL3 | TEC_FILA3 | Teclado | Fila 3 (salida) |
| PK0 | TEC_COL0 | Teclado | Columna 0 (entrada PULLUP) |
| PK1 | TEC_COL1 | Teclado | Columna 1 (entrada PULLUP) |
| PK2 | TEC_COL2 | Teclado | Columna 2 (entrada PULLUP) |
| PK3 | TEC_COL3 | Teclado | Columna 3 (entrada PULLUP) |
| PB5 | SERVO_PWM | Servomotor | OC1A, PWM servomotor garaje (Timer1, Fast PWM 50Hz) |
| PD2 | USART0_RX | PC | Recepción USART |
| PD3 | USART0_TX | PC | Transmisión USART |
| PD0 | I2C_SDA | Expansión I2C | Serial Data (opcional) |
| PD1 | I2C_SCL | Expansión I2C | Serial Clock (opcional) |

## Conflictos Resueltos

| Conflicto | Pines | Resolución |
|---|---|---|
| TEC_FILA3 vs OC5A (servomotor) | PL3 | Servomotor movido a Timer1/OC1A (PB5). PL3 dedicado solo a teclado |

**Nota**: El conflicto LCD_EN vs SPI_SCK está resuelto: LCD_EN movido a PA1, SPI_SCK usa PB1 exclusivamente. Conflicto TEC_FILA3 vs OC5A (T-02) resuelto: servomotor movido a Timer1/OC1A (PB5).
