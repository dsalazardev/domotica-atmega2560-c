# Protocolo SPI — Referencia

## Código de Clase de Referencia
`DOCUMENTOS/CODIGO-CLASES/SPI-Maestro/SPI-Maestro.ino`

## Pines del ATmega2560

| Pin | Señal SPI | Función |
|---|---|---|
| PB0 | SS | Slave Select (activo bajo) |
| PB1 | SCK | Serial Clock |
| PB2 | MOSI | Master Out Slave In |
| PB3 | MISO | Master In Slave Out |

**Nota**: Para el módulo RFID se necesitará un CS (Chip Select) adicional usando otro pin GPIO como SS. El PB0 queda como SS maestro y un pin extra (ej: PLx) como CS para el módulo RFID.

## Registros

| Registro | Bits clave | Descripción |
|---|---|---|
| SPCR | SPE, MSTR, SPR1, SPR0, CPOL, CPHA | Control SPI |
| SPSR | SPIF, WCOL, SPI2X | Status SPI |
| SPDR | SPDR[7:0] | Dato SPI |

## Inicialización (Maestro)
```c
SPI_DDR |= (1 << SS) | (1 << SCK) | (1 << MOSI);
SPI_DDR &= ~(1 << MISO);
SPI_PORT |= (1 << SS);
SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
```

## Transferencia Full-Dúplex
```c
char spi_transfer(char data) {
    SPI_PORT &= ~(1 << SS);  // CS low
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    SPI_PORT |= (1 << SS);   // CS high
    return SPDR;
}
```
SPI es full-dúplex: cada transmisión recibe un dato simultáneamente.
