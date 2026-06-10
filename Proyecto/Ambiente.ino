#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ILUM_DDR    DDRE
#define ILUM_PORT   PORTE
#define ILUM_PIN    PE4

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
#define VENTILADOR_PIN  PC1

#define HORNO_DDR  DDRC
#define HORNO_PORT PORTC
#define HORNO_PIN  PC0

#define SONIDO_RELE_DDR  DDRL
#define SONIDO_RELE_PORT PORTL
#define SONIDO_RELE_PIN  PL5

#define SONIDO_PWM_DDR   DDRB
#define SONIDO_PWM_PORT  PORTB
#define SONIDO_PWM_PIN   PB4

static unsigned long ultimo_poll_temp = 0;
static uint8_t ilum_nivel_actual = 0;
static uint8_t horno_activo = 0;
static uint8_t horno_temp = 0;
static unsigned long horno_timeout = 0;
static uint8_t sonido_activo = 0;
static uint8_t temperatura_actual = 25;
static uint8_t sonido_nivel_actual = 0;

uint8_t temperatura_leer(void) { return temperatura_actual; }

uint8_t horno_activo_get(void) { return horno_activo; }
uint8_t sonido_nivel_get(void) { return sonido_nivel_actual; }
uint8_t sonido_activo_get(void) { return sonido_activo; }

void iluminacion_iniciar(void) {
    ILUM_DDR |= (1 << ILUM_PIN);
    TCCR3A = (1 << COM3B1) | (1 << WGM31);
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30);
    ICR3 = 2500;
    OCR3B = 0;
}

uint8_t ilum_nivel_get(void) { return ilum_nivel_actual; }

void iluminacion_dimerizar(uint8_t nivel) {
    if (nivel > 100) nivel = 100;
    uint16_t duty = (nivel == 0) ? 0 : ((uint16_t)nivel * 2500UL) / 100;
    if (duty > 2500) duty = 2500;
    OCR3B = duty;
    ilum_nivel_actual = nivel;
}

uint16_t adc_leer(uint8_t canal) {
    ADMUX = (1 << REFS0) | (canal & 0x07);
    ADCSRA |= (1 << ADSC);
    unsigned long t = millis();
    while (ADCSRA & (1 << ADSC)) {
        if (millis() - t > 100) break;
    }
    return ADC;
}

void temperatura_iniciar(void) {
    CALEFACTOR_DDR |= (1 << CALEFACTOR_PIN);
    VENTILADOR_DDR |= (1 << VENTILADOR_PIN);
    CALEFACTOR_PORT &= ~(1 << CALEFACTOR_PIN);
    VENTILADOR_PORT &= ~(1 << VENTILADOR_PIN);
}

static void temperatura_controlar(void) {
    uint8_t t = temperatura_actual;

    if (t < TEMP_SETPOINT - TEMP_HISTERESIS) {
        CALEFACTOR_PORT |= (1 << CALEFACTOR_PIN);
        VENTILADOR_PORT &= ~(1 << VENTILADOR_PIN);
    } else if (t > TEMP_SETPOINT + TEMP_HISTERESIS) {
        CALEFACTOR_PORT &= ~(1 << CALEFACTOR_PIN);
        VENTILADOR_PORT |= (1 << VENTILADOR_PIN);
    }
}

void temperatura_ajustar(int8_t delta) {
    int16_t nueva = (int16_t)temperatura_actual + delta;
    if (nueva > 99) nueva = 99;
    if (nueva < 0) nueva = 0;
    temperatura_actual = (uint8_t)nueva;
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

void sonido_set_volumen(uint8_t nivel) {
    if (nivel > 100) nivel = 100;

    if (nivel == 0) {
        sonido_activo = 0;
        sonido_nivel_actual = 0;
        TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0));
        TCCR2B = 0;
        OCR2A = 0;
        SONIDO_RELE_PORT &= ~(1 << SONIDO_RELE_PIN);
        SONIDO_RELE_DDR |= (1 << SONIDO_RELE_PIN);
        SONIDO_PWM_PORT &= ~(1 << SONIDO_PWM_PIN);
        SONIDO_PWM_DDR |= (1 << SONIDO_PWM_PIN);
        return;
    }

    sonido_activo = 1;
    sonido_nivel_actual = nivel;

    SONIDO_RELE_DDR |= (1 << SONIDO_RELE_PIN);
    SONIDO_RELE_PORT |= (1 << SONIDO_RELE_PIN);

    SONIDO_PWM_DDR |= (1 << SONIDO_PWM_PIN);
    TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS21);
    OCR2A = (nivel * 255UL) / 100;
}

void sonido_apagar(void) {
    sonido_set_volumen(0);
}

void ambiente_actualizar(void) {
    unsigned long ahora = millis();

    if (ahora - ultimo_poll_temp >= TEMP_POLL_MS) {
        ultimo_poll_temp = ahora;
        temperatura_controlar();
    }

    if (horno_activo && ahora >= horno_timeout) {
        horno_apagar();
    }
}

#define SLAVE_LINEA_BUF 48
static char slave_linea[SLAVE_LINEA_BUF];
static uint8_t slave_pos = 0;

static void slave_enviar_respuesta(const char* msg) {
    UCSR2B |= (1 << TXEN2);
    usart2_print(msg);
    usart2_transmit('\n');
    UCSR2B &= ~(1 << TXEN2);
    DDRH &= ~(1 << PH1);
}

static void slave_procesar_linea(char* linea) {
    char* tok = strtok(linea, " ");
    if (tok == NULL) return;

    if (strcmp_P(tok, PSTR("HORNO")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { slave_enviar_respuesta(PSTR("ERROR: comando invalido")); return; }
        if (strcmp_P(tok, PSTR("ON")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { slave_enviar_respuesta(PSTR("ERROR: falta temperatura")); return; }
            uint16_t temp = 0;
            while (*tok >= '0' && *tok <= '9') temp = temp * 10 + (*tok++ - '0');
            tok = strtok(NULL, " ");
            if (tok == NULL) { slave_enviar_respuesta(PSTR("ERROR: falta tiempo")); return; }
            uint16_t tsec = 0;
            while (*tok >= '0' && *tok <= '9') tsec = tsec * 10 + (*tok++ - '0');
            horno_encender(tsec, (uint8_t)temp);
            slave_enviar_respuesta(PSTR("OK: horno encendido"));
        } else if (strcmp_P(tok, PSTR("OFF")) == 0) {
            horno_apagar();
            slave_enviar_respuesta(PSTR("OK: horno apagado"));
        } else {
            slave_enviar_respuesta(PSTR("ERROR: comando horno invalido"));
        }
    }
}

void ambiente_slave_procesar(void) {
    while (usart2_disponible() > 0) {
        char c = usart2_leer();
        if (c == '\n' || c == '\r') {
            if (slave_pos > 0) {
                slave_linea[slave_pos] = '\0';
                slave_procesar_linea(slave_linea);
                slave_pos = 0;
            }
        } else if (slave_pos < SLAVE_LINEA_BUF - 1) {
            slave_linea[slave_pos++] = c;
        }
    }
}

#define SONIDO_SLAVE_BUF 48
static char sonido_slave_linea[SONIDO_SLAVE_BUF];
static uint8_t sonido_slave_pos = 0;

static void sonido_slave_enviar_respuesta(const char* msg) {
    UCSR3B |= (1 << TXEN3);
    usart3_print(msg);
    usart3_transmit('\n');
    UCSR3B &= ~(1 << TXEN3);
    DDRJ &= ~(1 << PJ1);
}

static void sonido_slave_procesar_linea(char* linea) {
    char* tok = strtok(linea, " ");
    if (tok == NULL) return;

    if (strcmp_P(tok, PSTR("SONIDO")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { sonido_slave_enviar_respuesta(PSTR("ERROR: comando invalido")); return; }
        if (strcmp_P(tok, PSTR("ON")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { sonido_slave_enviar_respuesta(PSTR("ERROR: falta volumen")); return; }
            uint8_t vol = 0;
            while (*tok >= '0' && *tok <= '9') vol = vol * 10 + (*tok++ - '0');
            sonido_set_volumen(vol);
            sonido_slave_enviar_respuesta(PSTR("OK: sonido encendido"));
        } else if (strcmp_P(tok, PSTR("OFF")) == 0) {
            sonido_apagar();
            sonido_slave_enviar_respuesta(PSTR("OK: sonido apagado"));
        } else {
            sonido_slave_enviar_respuesta(PSTR("ERROR: comando sonido invalido"));
        }
    }
}

void sonido_slave_procesar(void) {
    while (usart3_disponible() > 0) {
        char c = usart3_leer();
        if (c == '\n' || c == '\r') {
            if (sonido_slave_pos > 0) {
                sonido_slave_linea[sonido_slave_pos] = '\0';
                sonido_slave_procesar_linea(sonido_slave_linea);
                sonido_slave_pos = 0;
            }
        } else if (sonido_slave_pos < SONIDO_SLAVE_BUF - 1) {
            sonido_slave_linea[sonido_slave_pos++] = c;
        }
    }
}
