#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Seguridad.h"

Seguridad seguridad;

void setup() {
    sei();
}

void loop() {
    _delay_ms(50);
}
