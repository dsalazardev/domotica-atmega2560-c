#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Seguridad.h"

Seguridad seguridad;

void setup() {
    seguridad.iniciar();
    sei();
}

void loop() {
    seguridad.verificar();
    _delay_ms(50);
}
