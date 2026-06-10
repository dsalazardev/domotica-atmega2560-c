# Asignación de Pines — ATmega2560

**⚠️ Pines LCD confirmados (T-01 resuelto). Resto tentativo — a confirmar durante implementación. (T-02 resuelto)**

| Pin ATmega2560 | Señal | Periférico | Notas |
|---|---|---|---|---|
| PG5 (D4) | LCD_D4 | LCD | Bus datos 4-bit, digitalWrite |
| PE3 (D5) | LCD_D5 | LCD | |
| PH3 (D6) | LCD_D6 | LCD | |
| PH4 (D7) | LCD_D7 | LCD | |
| PH5 (D8) | LCD_RS | LCD | Register Select |
| PH6 (D9) | LCD_EN | LCD | Enable (flanco descendente) |
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
| PB5 (D11) | SERVO_PWM | Servomotor | OC1A, PWM servomotor garaje (Timer1, Fast PWM 50Hz) |
| PB4 (D10) | SONIDO_PWM | Sonido | OC2A, PWM+RC → señal analógica (Timer2) |
| PL5 (D44) | SONIDO_RELE | Sonido | Relé encendido equipo de sonido |
| PE4 (D2) | ILUM_PWM | Iluminación | OC3B, PWM dimerización (Timer3, Fast PWM) |
| PD7 (D38) | ALARMA_BUZZER | Alarma | Buzzer piezoeléctrico (toggle 1kHz en disparo) |
| PE0 (D0) | USART0_RX | PC | Recepción USART PC |
| PE1 (D1) | USART0_TX | PC | Transmisión USART PC |
| PD2 (D19) | USART1_RX | USART1 | Loopback → USART2 (esclavo) |
| PD3 (D18) | USART1_TX | USART1 | |
| PH0 (D17) | USART2_RX | USART2 | Esclavo HORNO — recibe de USART1-TX |
| PH1 (D16) | USART2_TX | USART2 | Responde al maestro USART1-RX |
| PJ0 (D15) | USART3_RX | USART3 | Esclavo SONIDO — recibe de USART1-TX |
| PJ1 (D14) | USART3_TX | USART3 | Responde al maestro USART1-RX |
| PC5 (A6) | CALEFACTOR | Temperatura | Relé calefactor |
| PC6 (A7) | VENTILADOR | Temperatura | Relé ventilador |
| PC7 (A8) | HORNO_RELE | Horno | Relé horno |

## Pines Registro PA0-PA1 Descartados por Bug en wrapper de Proteus

Los pines PA0 (D22) y PA1 (D23) **no funcionan como salidas digitales** en el modelo wrapper
ARDUINO MEGA 2560 de Proteus. Se descartaron para LCD_RS y LCD_EN.

## Conflictos Resueltos

| Conflicto | Pines | Resolución |
|---|---|---|
| LCD_D5 (D5) vs ILUM_OC3A (PE3) | D5/PE3 | Iluminación movida a OC3B/PE4 (D2). LCD_D5 usa PE3 solo como GPIO digital. |
| LCD_EN (D9) vs SONIDO_OC2B (PH6) | D9/PH6 | Sonido PWM movido a OC2A/PB4 (D10). LCD_EN usa PH6 solo como GPIO digital. |
| TEC_FILA3 vs OC5A (servomotor) | PL3 | Servomotor movido a Timer1/OC1A (PB5). PL3 dedicado solo a teclado |
| ILUM_PWM vs LCD_D5 | PE3/OC3A → PE4/OC3B | OC3B (PE4/D2) libre para iluminación dimerizada |

**Nota**: Todos los pines LCD usan `digitalWrite()` (no acceso directo a puerto) porque están
distribuidos en 5 puertos diferentes. La velocidad de digitalWrite es suficiente para el LCD
a 16MHz. Conflicto TEC_FILA3 vs OC5A (T-02) resuelto: servomotor movido a Timer1/OC1A (PB5).
