#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t timer_tick = 0;

void timer_init(void) {
    TCCR5A = 0;
    TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50);
    OCR5A = 249;
    TIMSK5 = (1 << OCIE5A);
    sei();
}

ISR(TIMER5_COMPA_vect) {
    timer_tick++;
}
