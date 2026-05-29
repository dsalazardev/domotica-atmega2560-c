#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LCD_PORT PORTA
#define LCD_DDR  DDRA
#define LCD_RS   PA4
#define LCD_E    PA5

#define LCD_USED_MASK ((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << LCD_RS) | (1 << LCD_E))

#define SPI_DDR   DDRB
#define SPI_PORT  PORTB
#define SPI_SS    PB0
#define SPI_SCK   PB1
#define SPI_MOSI  PB2
#define SPI_MISO  PB3

uint8_t contador_caracteres = 0;

const char mapa_teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};


void spi_master_init(void);
char spi_master_transmit(char data);
void teclado_init(void);
char teclado_leer(void);
void lcd_init(void);
void lcd_pulso(void);
void lcd_nibble(uint8_t dato);
void lcd_comando(uint8_t comando);
void lcd_dato(uint8_t dato);
void lcd_procesar(char tecla);

int main(void)
{
    lcd_init();
    teclado_init();
    spi_master_init();

    char tecla_local = 0;
    char tecla_remota = 0;

    while (1)
    {
        tecla_local = teclado_leer();

        if (tecla_local != 0)
        {
            tecla_remota = spi_master_transmit(tecla_local);
        }
        else
        {
            tecla_remota = spi_master_transmit(0x00);
        }

        if (tecla_remota != 0)
        {
            lcd_procesar(tecla_remota);
        }

        _delay_ms(50);
    }
}

void spi_master_init(void)
{
    SPI_DDR |= (1 << SPI_SS) | (1 << SPI_SCK) | (1 << SPI_MOSI);
    SPI_DDR &= ~(1 << SPI_MISO);

    SPI_PORT |= (1 << SPI_SS);

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

char spi_master_transmit(char data)
{
    SPI_PORT &= ~(1 << SPI_SS);

    SPDR = data;
    while (!(SPSR & (1 << SPIF)));

    SPI_PORT |= (1 << SPI_SS);

    return SPDR;
}

void teclado_init(void)
{
    DDRK = 0x0F;
    PORTK = 0xFF;
}

char teclado_leer(void)
{
    uint8_t fila;
    uint8_t columna;
    uint8_t lectura;

    for (fila = 0; fila < 4; fila++)
    {
        PORTK = 0xF0 | (0x0F & ~(1 << fila));
        
        _delay_us(5);
        
        lectura = (PINK >> 4) & 0x0F;

        if (lectura != 0x0F)
        {
            for (columna = 0; columna < 4; columna++)
            {
                if (!(lectura & (1 << columna)))
                {
                    _delay_ms(20);

                    if (!(((PINK >> 4) & 0x0F) & (1 << columna)))
                    {
                        char tecla = mapa_teclas[fila][columna];

                        while (!(((PINK >> 4) & 0x0F) & (1 << columna)));

                        _delay_ms(20);

                        PORTK = 0xFF;

                        return tecla;
                    }
                }
            }
        }
    }

    PORTK = 0xFF;
    return 0;
}

void lcd_pulso(void)
{
    LCD_PORT |= (1 << LCD_E);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_E);
    _delay_us(100);
}

void lcd_nibble(uint8_t dato)
{
    LCD_PORT = (LCD_PORT & 0xF0) | (dato & 0x0F);
    lcd_pulso();
}

void lcd_comando(uint8_t comando)
{
    LCD_PORT &= ~(1 << LCD_RS);

    lcd_nibble(comando >> 4);
    lcd_nibble(comando);

    _delay_ms(2);
}

void lcd_dato(uint8_t dato)
{
    LCD_PORT |= (1 << LCD_RS);

    lcd_nibble(dato >> 4);
    lcd_nibble(dato);

    _delay_us(100);
}

void lcd_init(void)
{
    LCD_DDR |= LCD_USED_MASK;
    LCD_PORT &= ~LCD_USED_MASK;

    _delay_ms(20);

    LCD_PORT &= ~(1 << LCD_RS);

    lcd_nibble(0x03);
    _delay_ms(5);

    lcd_nibble(0x03);
    _delay_us(150);

    lcd_nibble(0x03);
    lcd_nibble(0x02);

    lcd_comando(0x28);
    lcd_comando(0x0C);
    lcd_comando(0x06);
    lcd_comando(0x01);

    contador_caracteres = 0;
}

void lcd_procesar(char tecla)
{
    if ((tecla == '*') || (tecla == 'F'))
    {
        lcd_comando(0x01);
        contador_caracteres = 0;
        return;
    }

    if (tecla == 'E')
    {
        lcd_comando(0xC0);
        contador_caracteres = 16;
        return;
    }

    if (contador_caracteres >= 32)
    {
        lcd_comando(0x01);
        contador_caracteres = 0;
    }

    lcd_dato(tecla);

    contador_caracteres++;

    if (contador_caracteres == 16)
    {
        lcd_comando(0xC0);
    }
}