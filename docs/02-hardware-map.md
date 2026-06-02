# Mapa de Hardware — ATmega2560

## Periféricos y Asignación de Pines

| Periférico | Tipo | Protocolo | Puertos Probables | Función |
|---|---|---|---|---|
| LCD 16x2 | Salida | GPIO 4-bit | PA4-PA7 (D4-D7), PB0-PB1 (RS, EN) | Interfaz usuario |
| Teclado matricial 4x4 | Entrada | GPIO scan | PL0-PL3 (filas OUT), PK0-PK3 (cols IN) | Ingreso datos/navegación |
| Módulo RFID | E/S | SPI | PB0 (SS), PB1 (SCK), PB2 (MOSI), PB3 (MISO) | Lectura/escritura tarjetas |
| Sensor de humo | Entrada | GPIO o ADC | Pin digital o ADC | Detección de incendio |
| Reed switch (puer/vent) | Entrada | GPIO (INT) | Pin con INT0-7 o PCINT | Detección de intrusión |
| Imán puerta principal | Salida | GPIO + relé | Pin digital + transistor driver | Apertura puerta |
| Servomotor garaje | Salida | PWM | PL3 (OC5A) | Apertura garaje |
| Calefactor | Salida | GPIO (relé) o PWM | Pin digital/OCn | Control temperatura |
| Ventilador | Salida | GPIO (relé) o PWM | Pin digital/OCn | Control temperatura |
| Sensor temperatura | Entrada | ADC o 1-Wire | PF0-PF7 / PK0-PK7 / pin digital | Medición temperatura |
| Potenciómetro volumen | Entrada | ADC | Pin ADC (PF0-PF7, PK0-PK7) | Control volumen analógico |
| USART a PC | E/S | USART | PD2 (RXD0), PD3 (TXD0) | Comunicación remota |
| Señal analógica sonido | Salida | PWM+RC (DAC) o DAC | Pin OCn + filtro RC | Volumen proporcional |

## Pines PWM Disponibles

| Timer | Resolución | Pines OC |
|---|---|---|
| Timer0 | 8 bits | PB7 (OC0A), PG5 (OC0B) |
| Timer1 | 16 bits | PB5 (OC1A), PB6 (OC1B), PB7 (OC1C) |
| Timer2 | 8 bits | PB4 (OC2A), PH6 (OC2B) |
| Timer3 | 16 bits | PE3 (OC3A), PE4 (OC3B), PE5 (OC3C) |
| Timer4 | 16 bits | PH3 (OC4A), PH4 (OC4B), PH5 (OC4C) |
| Timer5 | 16 bits | PL3 (OC5A), PL4 (OC5B), PL5 (OC5C) |

## Pines USART Disponibles

| USART | TX | RX |
|---|---|---|
| USART0 | PE1 / PD3 | PE0 / PD2 |
| USART1 | PD3 | PD2 |
| USART2 | PH1 | PH0 |
| USART3 | PJ1 | PJ0 |
