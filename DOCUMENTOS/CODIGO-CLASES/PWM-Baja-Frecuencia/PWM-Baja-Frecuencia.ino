#include <avr/io.h>
#include <stdint.h>

void setup_pwm_lento() {
    DDRL |= (1 << PL3);

    TCCR5A = (1 << COM5A1) | (1 << WGM51);
    TCCR5B = (1 << WGM53) | (1 << WGM52) | (1 << CS52) | (1 << CS50);

    ICR5 = 15624; 

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

int main() {
    setup_pwm_lento();

    while (1) {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));

        uint16_t valor_adc = ADC; 
        
        uint32_t calculo = ((uint32_t)valor_adc * 15624UL) / 1023UL;
        
        OCR5A = (uint16_t)calculo;
    }
}