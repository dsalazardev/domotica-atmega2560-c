#ifndef PROYECTO_LCD_H
#define PROYECTO_LCD_H

#include <stdint.h>

#define LCD_RS PA0
#define LCD_EN PA1
#define LCD_D4 PA4
#define LCD_D5 PA5
#define LCD_D6 PA6
#define LCD_D7 PA7

#define LCD_DDR DDRA
#define LCD_PORT PORTA

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_SET 0x28

void lcd_init(void);
void lcd_comando(uint8_t cmd);
void lcd_dato(uint8_t dato);
void lcd_nibble(uint8_t dato);
void lcd_pulso(void);
void lcd_posicion(uint8_t fila, uint8_t col);
void lcd_borrar(void);
void lcd_imprimir(const char* texto);

#endif
