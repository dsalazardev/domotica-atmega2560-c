# Protocolo I2C (TWI) — Referencia

## Código de Clase de Referencia
`DOCUMENTOS/CODIGO-CLASES/I2C-Maestro/I2C-Maestro.ino`

## Pines del ATmega2560

| Pin | Señal | Función |
|---|---|---|
| PD0 | SDA | Serial Data (requiere pull-up externo) |
| PD1 | SCL | Serial Clock (requiere pull-up externo) |

## Registros

| Registro | Descripción |
|---|---|
| TWBR | TWI Bit Rate Register |
| TWSR | TWI Status Register (prescaler bits TWPS) |
| TWCR | TWI Control Register (TWINT, TWSTA, TWSTO, TWEN, TWEA) |
| TWDR | TWI Data Register |

## Velocidad
```
F_TWI = F_CPU / (16 + 2 * TWBR * 4^TWPS)
Para 100kHz con F_CPU = 16MHz, TWPS = 0:
  TWBR = (16MHz / 100kHz - 16) / 2 = 72
```

## Inicialización (Maestro)
```c
DDRD &= ~((1 << PD0) | (1 << PD1));
PORTD |= (1 << PD0) | (1 << PD1);
TWBR = 72;
TWSR = 0x00;       // Prescaler = 1
TWCR = (1 << TWEN);
```

## Transmisión
```c
void i2c_transmit(char slave_addr, char data) {
    // START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    // Dirección esclavo + escritura
    TWDR = (slave_addr << 1);
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    // Dato
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    // STOP
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
```

## Solicitud de Datos (Esclavo → Maestro)
```c
char i2c_request(char slave_addr) {
    // START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    // Dirección esclavo + lectura
    TWDR = (slave_addr << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    // Recibir
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    char data = TWDR;
    // STOP
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    return data;
}
```
