#include <avr/io.h>
#include <util/delay.h>

#include "../include/LCD.h"

#define LCD_DATA_MASK 0xF0
#define LCD_RS_MASK (1 << LCD_RS)
#define LCD_EN_MASK (1 << LCD_EN)
#define LCD_MASK (LCD_DATA_MASK | LCD_RS_MASK | LCD_EN_MASK)

static uint8_t lcd_fila = 0;
static uint8_t lcd_columna = 0;

void lcd_pulso(void)
{
    LCD_PORT |= LCD_EN_MASK;
    _delay_us(1);
    LCD_PORT &= ~LCD_EN_MASK;
    _delay_us(100);
}

void lcd_nibble(uint8_t dato)
{
    LCD_PORT = (LCD_PORT & 0x0F) | ((dato & 0x0F) << 4);
    lcd_pulso();
}

void lcd_comando(uint8_t cmd)
{
    LCD_PORT &= ~LCD_RS_MASK;
    lcd_nibble(cmd >> 4);
    lcd_nibble(cmd);
    _delay_ms(2);
}

void lcd_dato(uint8_t dato)
{
    LCD_PORT |= LCD_RS_MASK;
    lcd_nibble(dato >> 4);
    lcd_nibble(dato);
    _delay_us(100);
}

void lcd_init(void)
{
    LCD_DDR |= LCD_MASK;
    LCD_PORT &= ~LCD_MASK;

    _delay_ms(20);

    LCD_PORT &= ~LCD_RS_MASK;

    lcd_nibble(0x03);
    _delay_ms(5);

    lcd_nibble(0x03);
    _delay_us(150);

    lcd_nibble(0x03);
    lcd_nibble(0x02);

    lcd_comando(LCD_FUNCTION_SET);
    lcd_comando(LCD_DISPLAY_ON);
    lcd_comando(LCD_ENTRY_MODE);
    lcd_borrar();
}

void lcd_posicion(uint8_t fila, uint8_t col)
{
    if (fila == 0) {
        lcd_comando(0x80 + col);
    } else {
        lcd_comando(0xC0 + col);
    }
}

void lcd_borrar(void)
{
    lcd_comando(LCD_CLEAR);
    _delay_ms(2);
    lcd_fila = 0;
    lcd_columna = 0;
    lcd_posicion(lcd_fila, lcd_columna);
}

void lcd_imprimir(const char* texto)
{
    while (*texto) {
        lcd_dato(*texto);
        texto++;
    }
}
