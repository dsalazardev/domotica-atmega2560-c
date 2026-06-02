# Patrones de Código Reutilizables

## Patrón LCD 4-bit (6 referencias)

| Archivo | Funciones clave |
|---|---|
| `Calculadora-con-LCD.ino` | `enviar4Bits()`, `enviarComando()`, `enviarLetra()`, `imprimirTexto()` |
| `I2C-Maestro.ino` | `lcd_init()`, `lcd_comando()`, `lcd_dato()`, `lcd_nibble()`, `lcd_pulso()` |
| `SPI-Maestro.ino` | `lcd_init()`, `lcd_comando()`, `lcd_dato()`, `lcd_procesar()` |
| `USART.ino` | `LCD_Init()`, `LCD_Cmd()`, `LCD_Char()`, `LCD_PulseEnable()` |
| `LCD-1.ino` | `enviar4Bits()`, `enviarComando()`, `enviarLetra()` |
| `Teclado-con-LCD.ino` | `enviar4Bits()`, `enviarComando()`, `enviarLetra()`, `imprimirTexto()` |

**Secuencia de inicialización**: 3 fases 8-bit → función set 4-bit → display on → clear.

## Patrón Teclado Matricial (3 referencias)

| Archivo | Técnica |
|---|---|
| `Calculadora-con-LCD.ino` | Scan por filas (PORTL), leer columnas (PINK), debounce while |
| `I2C-Maestro.ino` | Scan con debounce de 20ms, evitar rebotes |
| `SPI-Maestro.ino` | Scan con debounce, esperar liberación de tecla |

**Estructura**: PORTL salida (filas), PINK entrada con pull-up (columnas), matriz 4x4.

## Patrón USART TX+RX (1 referencia)

| Archivo | Configuración |
|---|---|
| `USART.ino` | 9600 baud, 8N1, interrupción RX (ISR(USART0_RX_vect)), polling TX |

**Registros**: UBRR0=104, UCSR0B=(RXCIE0|RXEN0|TXEN0), UCSR0C=(UCSZ01|UCSZ00).

## Patrón SPI Maestro (1 referencia)

| Archivo | Características |
|---|---|
| `SPI-Maestro.ino` | Full-duplex, init + transmit, polling SPIF |

**Pines**: PB0(SS), PB1(SCK), PB2(MOSI), PB3(MISO). **Registros**: SPCR, SPSR, SPDR.

## Patrón I2C Maestro (1 referencia)

| Archivo | Características |
|---|---|
| `I2C-Maestro.ino` | Init, transmitir, solicitar datos con handshake TWINT |

**Pines**: PD0(SDA), PD1(SCL). **Registros**: TWBR, TWSR, TWCR, TWDR.

## Patrón PWM con ADC (1 referencia)

| Archivo | Características |
|---|---|
| `PWM-Baja-Frecuencia.ino` | Timer5 Fast PWM, ICR5 como TOP, ADC → OCR5A |

**Pines**: PL3(OC5A). **Frecuencia**: ICR5=15624 → ~1Hz con prescaler /1024.

## Patrón Interrupción por Cambio (PCINT) (1 referencia)

| Archivo | Características |
|---|---|
| `Teclado_con_7_segmentos.ino` | PCINT2_vect, grupo PK0-PK3 (PCINT16-19) |

**Registros**: PCICR, PCMSK2. Útil para detección inmediata de tecla o sensor.
