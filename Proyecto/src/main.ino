#include <avr/io.h>
#include <util/delay.h>

#include "LCD.h"

void setup() {
    lcd_init();
    lcd_imprimir("Hola Mundo!");
}

void loop() {
    _delay_ms(50);
}
