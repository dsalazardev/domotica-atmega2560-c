#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t numeros7Seg[] = {
    0xF9, 0xA4, 0xB0, 0x88,
    0x99, 0x92, 0x82, 0x83,
    0xF8, 0x80, 0x90, 0xC6,
    0x86, 0xC0, 0x8E, 0xA1
};

volatile uint8_t filaActual = 0;

void setup() {
    DDRA = 0xFF;
    PORTA = 0xFF;

    DDRL |= 0x0F;
    DDRK &= ~0x0F;
    PORTK |= 0x0F;

    PCICR |= (1 << PCIE2);
    PCMSK2 |= 0x0F;

    sei();
}

ISR(PCINT2_vect) {
    uint8_t col = PINK & 0x0F;
    if (col != 0x0F) {
        uint8_t c = 0;
        if      (col == 0x0E) c = 0;
        else if (col == 0x0D) c = 1;
        else if (col == 0x0B) c = 2;
        else if (col == 0x07) c = 3;
        else return;

        PORTA = numeros7Seg[(filaActual * 4) + c];
    }
}

int main() {
    setup();
    while (1) {
        for (filaActual = 0; filaActual < 4; filaActual++) {
            PORTL = (PORTL | 0x0F) & ~(1 << filaActual);
            for (volatile uint16_t v = 0; v < 4000; v++);
        }
    }
    return 0;
}