#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define LCD_PORT PORTA
#define LCD_DDR   DDRA
#define LCD_RS    PA4
#define LCD_E     PA5

#define LCD_DATA_MASK 0x0F
#define LCD_CTRL_MASK ((1 << LCD_RS) | (1 << LCD_E))
#define LCD_MASK      (LCD_DATA_MASK | LCD_CTRL_MASK)

#define LCD_COLUMNAS 16

#define SLAVE_ADDR 0x20
#define MI_ADDR    0x10

#define TECLA_SALTO_LINEA 'E'
#define TECLA_BORRAR      'F'

#define KEY_PORT PORTK
#define KEY_DDR  DDRK
#define KEY_PIN  PINK

uint8_t lcd_fila = 0;
uint8_t lcd_columna = 0;

void i2c_master_init(void);
void i2c_master_transmit(char data);
char i2c_master_request_data(void);
void teclado_init(void);
void lcd_init(void);
void lcd_comando(uint8_t comando);
void lcd_dato(uint8_t dato);
void lcd_nibble(uint8_t dato);
void lcd_pulso(void);
void lcd_posicion(uint8_t fila, uint8_t columna);
void lcd_borrar(void);
void lcd_salto_linea(void);
void lcd_retroceso(void);
void lcd_procesar(char dato);
uint8_t teclado_scan(void);

const char teclado_mapa[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};

int main(void)
{
    lcd_init();
    teclado_init();
    i2c_master_init();

    char tecla_presionada = 0;
    char tecla_anterior = 0;
    char tecla_remota = 0;

    while (1)
    {
        tecla_presionada = teclado_scan();

        if (tecla_presionada != 0 && tecla_anterior == 0) {
            i2c_master_transmit(tecla_presionada);
        }
        tecla_anterior = tecla_presionada;

        _delay_ms(20);

        tecla_remota = i2c_master_request_data();
        
        if (tecla_remota != 0) {
            lcd_procesar(tecla_remota);
        }

        _delay_ms(30);
    }
}

void i2c_master_init(void)
{
    DDRD &= ~((1 << PD0) | (1 << PD1));
    PORTD |= (1 << PD0) | (1 << PD1);

    TWBR = 72; 
    TWSR = 0x00;
    TWCR = (1 << TWEN);
}

void i2c_master_transmit(char data)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWDR = (SLAVE_ADDR << 1);
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

char i2c_master_request_data(void)
{
    char data = 0;

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWDR = (SLAVE_ADDR << 1) | 0x01; 
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    TWCR = (1 << TWINT) | (1 << TWEN); 
    while (!(TWCR & (1 << TWINT)));
    data = TWDR;

    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    
    return data;
}

void teclado_init(void)
{
    KEY_DDR  = 0x0F;
    KEY_PORT = 0xFF;
}

uint8_t teclado_scan(void)
{
    for (uint8_t f = 0; f < 4; f++) {
        KEY_PORT = 0xFF & ~(1 << f); 
        _delay_us(5); 
        
        uint8_t col_bits = (KEY_PIN >> 4) & 0x0F;
        
        for (uint8_t c = 0; c < 4; c++) {
            if (!(col_bits & (1 << c))) {
                _delay_ms(20); 
                if (!((KEY_PIN >> 4) & (1 << c))) {
                    return teclado_mapa[f][c]; 
                }
            }
        }
    }
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

void lcd_posicion(uint8_t fila, uint8_t columna)
{
    if (fila == 0) {
        lcd_comando(0x80 + columna);
    } else {
        lcd_comando(0xC0 + columna);
    }
}

void lcd_borrar(void)
{
    lcd_comando(0x01);
    lcd_fila = 0;
    lcd_columna = 0;
    lcd_posicion(lcd_fila, lcd_columna);
}

void lcd_salto_linea(void)
{
    if (lcd_fila == 0) {
        lcd_fila = 1;
        lcd_columna = 0;
        lcd_posicion(lcd_fila, lcd_columna);
    } else {
        lcd_borrar();
    }
}

void lcd_retroceso(void)
{
    if (lcd_columna > 0) {
        lcd_columna--;
    } else if (lcd_fila > 0) {
        lcd_fila = 0;
        lcd_columna = LCD_COLUMNAS - 1;
    } else {
        return;
    }

    lcd_posicion(lcd_fila, lcd_columna);
    lcd_dato(' ');
    lcd_posicion(lcd_fila, lcd_columna);
}

void lcd_procesar(char dato)
{
    static uint8_t ultimo_fue_cr = 0;

    if (dato == '\r') {
        ultimo_fue_cr = 1;
        lcd_salto_linea();
        return;
    }

    if (dato == '\n') {
        if (ultimo_fue_cr) {
            ultimo_fue_cr = 0;
            return;
        }
        lcd_salto_linea();
        return;
    }

    ultimo_fue_cr = 0;

    if (dato == TECLA_SALTO_LINEA) {
        lcd_salto_linea();
        return;
    }

    if (dato == TECLA_BORRAR || dato == '\f') {
        lcd_borrar();
        return;
    }

    if (dato == '\b') {
        lcd_retroceso();
        return;
    }

    lcd_dato(dato);
    lcd_columna++;

    if (lcd_columna >= LCD_COLUMNAS) {
        lcd_salto_linea();
    }
}

void lcd_init(void)
{
    LCD_DDR |= LCD_MASK;
    LCD_PORT &= ~LCD_MASK;

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
    lcd_borrar();
}