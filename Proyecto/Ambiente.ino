#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define ILUM_DDR    DDRE
#define ILUM_PORT   PORTE
#define ILUM_PIN    PE3

#define ILUM_ADC_CH  1
#define ILUM_POLL_MS 100

#define TEMP_ADC_CH  0
#define TEMP_POLL_MS 500
#define TEMP_SETPOINT   24
#define TEMP_HISTERESIS 2

#define CALEFACTOR_DDR  DDRC
#define CALEFACTOR_PORT PORTC
#define CALEFACTOR_PIN  PC5

#define VENTILADOR_DDR  DDRC
#define VENTILADOR_PORT PORTC
#define VENTILADOR_PIN  PC6

#define HORNO_DDR  DDRC
#define HORNO_PORT PORTC
#define HORNO_PIN  PC7

#define SONIDO_DDR     DDRH
#define SONIDO_PORT    PORTH
#define SONIDO_RELE_PIN PH7
#define SONIDO_PWM_PIN  PH6

static unsigned long ultimo_poll_ilum = 0;
static unsigned long ultimo_poll_temp = 0;
static uint8_t ilum_nivel_actual = 255;
static uint8_t horno_activo = 0;
static uint8_t horno_temp = 0;
static unsigned long horno_timeout = 0;
static uint8_t sonido_activo = 0;
static uint8_t temperatura_actual = 25;

uint8_t temperatura_leer(void) { return temperatura_actual; }

uint8_t horno_activo_get(void) { return horno_activo; }
uint8_t sonido_activo_get(void) { return sonido_activo; }

void iluminacion_iniciar(void) {
    ILUM_DDR |= (1 << ILUM_PIN);
    TCCR3A = (1 << COM3A1) | (1 << WGM31);
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30);
    ICR3 = 2500;
    OCR3A = 0;
}

static void iluminacion_dimerizar(uint8_t nivel) {
    if (nivel > 100) nivel = 100;
    uint16_t duty = (nivel == 0) ? 0 : ((uint16_t)nivel * 2500UL) / 100;
    if (duty > 2500) duty = 2500;
    OCR3A = duty;
}

static uint16_t adc_leer(uint8_t canal) {
    ADMUX = (1 << REFS0) | (canal & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void temperatura_iniciar(void) {
    ADMUX = (1 << REFS0) | TEMP_ADC_CH;
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    CALEFACTOR_DDR |= (1 << CALEFACTOR_PIN);
    VENTILADOR_DDR |= (1 << VENTILADOR_PIN);
    CALEFACTOR_PORT &= ~(1 << CALEFACTOR_PIN);
    VENTILADOR_PORT &= ~(1 << VENTILADOR_PIN);
}

static void temperatura_controlar(void) {
    uint16_t adc_val = adc_leer(TEMP_ADC_CH);
    uint8_t temp_c = (adc_val * 500UL) / 1024;
    temperatura_actual = temp_c;

    if (temp_c < TEMP_SETPOINT - TEMP_HISTERESIS) {
        CALEFACTOR_PORT |= (1 << CALEFACTOR_PIN);
        VENTILADOR_PORT &= ~(1 << VENTILADOR_PIN);
    } else if (temp_c > TEMP_SETPOINT + TEMP_HISTERESIS) {
        CALEFACTOR_PORT &= ~(1 << CALEFACTOR_PIN);
        VENTILADOR_PORT |= (1 << VENTILADOR_PIN);
    }
}

void horno_encender(uint16_t tiempo_seg, uint8_t temp) {
    horno_activo = 1;
    horno_temp = temp;
    horno_timeout = millis() + (unsigned long)tiempo_seg * 1000UL;
    HORNO_DDR |= (1 << HORNO_PIN);
    HORNO_PORT |= (1 << HORNO_PIN);
}

void horno_apagar(void) {
    horno_activo = 0;
    HORNO_PORT &= ~(1 << HORNO_PIN);
}

void sonido_encender(void) {
    sonido_activo = 1;
    SONIDO_DDR |= (1 << SONIDO_RELE_PIN) | (1 << SONIDO_PWM_PIN);
    SONIDO_PORT |= (1 << SONIDO_RELE_PIN);

    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS20);
    OCR2B = 0;
}

void sonido_apagar(void) {
    sonido_activo = 0;
    TCCR2B = 0;
    OCR2B = 0;
    SONIDO_PORT &= ~(1 << SONIDO_RELE_PIN);
}

void sonido_volumen(uint8_t nivel) {
    if (nivel > 100) nivel = 100;
    OCR2B = (nivel * 255UL) / 100;
}

void ambiente_actualizar(void) {
    unsigned long ahora = millis();

    if (ahora - ultimo_poll_ilum >= ILUM_POLL_MS) {
        ultimo_poll_ilum = ahora;
        uint16_t adc_val = adc_leer(ILUM_ADC_CH);
        uint8_t nivel = (adc_val * 100UL) / 1023;
        if (nivel != ilum_nivel_actual) {
            ilum_nivel_actual = nivel;
            iluminacion_dimerizar(nivel);
        }
    }

    if (ahora - ultimo_poll_temp >= TEMP_POLL_MS) {
        ultimo_poll_temp = ahora;
        temperatura_controlar();
    }

    if (horno_activo && ahora >= horno_timeout) {
        horno_apagar();
    }
}
