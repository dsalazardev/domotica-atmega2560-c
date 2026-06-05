#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UID_LEN
#define UID_LEN 4
#endif
#ifndef MAX_TARJETAS
#define MAX_TARJETAS 10
#endif
#ifndef EEPROM_SALDO_MAX
#define EEPROM_SALDO_MAX 255
#endif

#define SENSOR_DDR  DDRC
#define SENSOR_PIN  PINC
#define SENSOR_PUERTA_PP   PC0
#define SENSOR_PUERTA_GAR  PC1
#define SENSOR_VENTANA_SAL PC2
#define SENSOR_VENTANA_COC PC3
#define SENSOR_HUMO        PC4

#define IMAN_DDR  DDRL
#define IMAN_PORT PORTL
#define IMAN_PP   PL4

#define SERVO_DDR  DDRB
#define SERVO_PORT PORTB
#define SERVO_PIN  PB5
#define SERVO_PWM_TOP 4999
#define SERVO_0GRADOS 250
#define SERVO_90GRADOS 375

typedef enum {
    EST_ALARMA_DESACTIVADA,
    EST_ALARMA_ACTIVADA,
    EST_ALARMA_DISPARADA
} estado_alarma_t;

static estado_alarma_t alarma_estado = EST_ALARMA_DESACTIVADA;
static bool alarma_sensor_prev[5];
static unsigned long alarma_ultimo_poll = 0;
static uint16_t codigo_actual;

static bool puerta_iman_activo = false;
static unsigned long puerta_iman_hasta = 0;

typedef enum {
    SERVO_PARADO,
    SERVO_ABRIENDO,
    SERVO_ESPERANDO,
    SERVO_CERRANDO
} estado_servo_t;

static estado_servo_t servo_estado = SERVO_PARADO;
static unsigned long servo_timer = 0;

static bool rfid_tarjeta_presente = false;
static bool rfid_tarjeta_anterior = false;
static uint8_t rfid_uid[UID_LEN];
static unsigned long rfid_ultimo_poll = 0;

void servo_init(void) {
    SERVO_DDR |= (1 << SERVO_PIN);
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = SERVO_PWM_TOP;
    OCR1A = SERVO_0GRADOS;
}

static void servo_escribir(uint16_t valor) {
    OCR1A = valor;
}

void servo_abrir(void) {
    if (servo_estado != SERVO_PARADO) return;
    servo_estado = SERVO_ABRIENDO;
    servo_timer = millis();
}

void servo_cerrar(void) {
    if (servo_estado != SERVO_PARADO) return;
    servo_estado = SERVO_CERRANDO;
    servo_timer = millis();
}

static void servo_actualizar(void) {
    unsigned long ahora = millis();
    switch (servo_estado) {
        case SERVO_ABRIENDO:
            servo_escribir(SERVO_90GRADOS);
            servo_estado = SERVO_ESPERANDO;
            servo_timer = ahora;
            break;
        case SERVO_ESPERANDO:
            if (ahora - servo_timer >= 3000) {
                servo_escribir(SERVO_0GRADOS);
                servo_estado = SERVO_CERRANDO;
                servo_timer = ahora;
            }
            break;
        case SERVO_CERRANDO:
            if (ahora - servo_timer >= 1000) {
                servo_estado = SERVO_PARADO;
            }
            break;
        default:
            break;
    }
}

void puerta_abrir_principal(void) {
    if (puerta_iman_activo) return;
    IMAN_PORT |= (1 << IMAN_PP);
    puerta_iman_activo = true;
    puerta_iman_hasta = millis() + 3000;
}

static void puertas_actualizar(void) {
    if (puerta_iman_activo && millis() >= puerta_iman_hasta) {
        IMAN_PORT &= ~(1 << IMAN_PP);
        puerta_iman_activo = false;
    }
    servo_actualizar();
}

void alarma_init(void) {
    alarma_estado = EST_ALARMA_DESACTIVADA;
    SENSOR_DDR &= ~((1 << SENSOR_PUERTA_PP) | (1 << SENSOR_PUERTA_GAR) |
                    (1 << SENSOR_VENTANA_SAL) | (1 << SENSOR_VENTANA_COC) |
                    (1 << SENSOR_HUMO));
    for (uint8_t i = 0; i < 5; i++) {
        alarma_sensor_prev[i] = true;
    }
    IMAN_DDR |= (1 << IMAN_PP);
    IMAN_PORT &= ~(1 << IMAN_PP);
    codigo_actual = lista_leer_codigo();
    if (codigo_actual == 0xFFFF || codigo_actual == 0x0000) {
        codigo_actual = 1234;
        lista_escribir_codigo(codigo_actual);
    }
    servo_init();
}

bool alarma_verificar_codigo(uint16_t codigo) {
    return codigo == codigo_actual;
}

void alarma_cambiar_codigo(uint16_t nuevo_codigo) {
    codigo_actual = nuevo_codigo;
}

void alarma_activar(void) {
    alarma_estado = EST_ALARMA_ACTIVADA;
}

void alarma_desactivar(void) {
    alarma_estado = EST_ALARMA_DESACTIVADA;
}

bool alarma_activa(void) {
    return alarma_estado == EST_ALARMA_ACTIVADA;
}

static void alarma_notificar(const char *texto) {
    usart_transmit('A');
    usart_transmit('L');
    usart_transmit('E');
    usart_transmit('R');
    usart_transmit('T');
    usart_transmit('A');
    usart_transmit(':');
    usart_transmit(' ');
    while (*texto) {
        usart_transmit(*texto);
        texto++;
    }
    usart_transmit('\r');
    usart_transmit('\n');
}

static bool alarma_leer_sensor(uint8_t pin) {
    return SENSOR_PIN & (1 << pin);
}

void alarma_actualizar(void) {
    unsigned long ahora = millis();
    if (ahora - alarma_ultimo_poll < 100) return;
    alarma_ultimo_poll = ahora;

    bool sensor_disparado[5];
    sensor_disparado[0] = !alarma_leer_sensor(SENSOR_PUERTA_PP);
    sensor_disparado[1] = !alarma_leer_sensor(SENSOR_PUERTA_GAR);
    sensor_disparado[2] = !alarma_leer_sensor(SENSOR_VENTANA_SAL);
    sensor_disparado[3] = !alarma_leer_sensor(SENSOR_VENTANA_COC);
    sensor_disparado[4] =  alarma_leer_sensor(SENSOR_HUMO);

    if (alarma_estado == EST_ALARMA_ACTIVADA) {
        for (uint8_t i = 0; i < 5; i++) {
            if (sensor_disparado[i] && !alarma_sensor_prev[i]) {
                alarma_estado = EST_ALARMA_DISPARADA;
                if (i == 4) {
                    alarma_notificar("Humo en la vivienda");
                } else {
                    const char *lugar = "Intrusion en ventana";
                    if (i == 0) lugar = "Intrusion en Puerta PP";
                    else if (i == 1) lugar = "Intrusion en Puerta Gar";
                    else if (i == 2) lugar = "Intrusion en Ventana Sala";
                    else lugar = "Intrusion en Ventana Cocina";
                    alarma_notificar(lugar);
                }
                break;
            }
        }
    }

    for (uint8_t i = 0; i < 5; i++) {
        alarma_sensor_prev[i] = sensor_disparado[i];
    }

    puertas_actualizar();
}

static bool rfid_uid_igual(const uint8_t *a, const uint8_t *b) {
    for (uint8_t i = 0; i < UID_LEN; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

static int8_t rfid_buscar_uid(const uint8_t *uid) {
    uint8_t cnt = lista_leer_conteo();
    uint8_t buf[UID_LEN];
    for (uint8_t i = 0; i < cnt; i++) {
        lista_leer_uid(i, buf);
        if (rfid_uid_igual(buf, uid)) return i;
    }
    return -1;
}

void rfid_init(void) {
    mfrc522_init();
    rfid_tarjeta_presente = false;
    rfid_tarjeta_anterior = false;
}

bool rfid_validar_uid(const uint8_t *uid) {
    return (rfid_buscar_uid(uid) >= 0);
}

bool rfid_enrolar(const uint8_t *uid) {
    if (rfid_buscar_uid(uid) >= 0) return false;
    uint8_t cnt = lista_leer_conteo();
    if (cnt >= MAX_TARJETAS) return false;
    lista_escribir_uid(cnt, uid);
    lista_escribir_saldo(cnt, 0);
    lista_escribir_conteo(cnt + 1);
    return true;
}

bool rfid_borrar(uint8_t idx) {
    uint8_t cnt = lista_leer_conteo();
    if (idx >= cnt) return false;
    for (uint8_t i = idx; i < cnt - 1; i++) {
        uint8_t buf[UID_LEN];
        lista_leer_uid(i + 1, buf);
        lista_escribir_uid(i, buf);
        uint8_t saldo = lista_leer_saldo(i + 1);
        lista_escribir_saldo(i, saldo);
    }
    lista_escribir_conteo(cnt - 1);
    return true;
}

void rfid_actualizar(void) {
    unsigned long ahora = millis();
    if (ahora - rfid_ultimo_poll < 200) return;
    rfid_ultimo_poll = ahora;

    rfid_tarjeta_anterior = rfid_tarjeta_presente;
    rfid_tarjeta_presente = mfrc522_leer_uid(rfid_uid);
}

bool rfid_tarjeta_nueva(void) {
    return rfid_tarjeta_presente && !rfid_tarjeta_anterior;
}

const uint8_t* rfid_get_uid(void) {
    return rfid_uid;
}

uint8_t rfid_get_conteo(void) {
    return lista_leer_conteo();
}

bool juegos_cargar_accesos(const uint8_t *uid, uint8_t accesos) {
    int8_t idx = rfid_buscar_uid(uid);
    if (idx < 0) return false;
    uint8_t saldo_actual = lista_leer_saldo(idx);
    if ((uint16_t)saldo_actual + accesos > EEPROM_SALDO_MAX) {
        lista_escribir_saldo(idx, EEPROM_SALDO_MAX);
    } else {
        lista_escribir_saldo(idx, saldo_actual + accesos);
    }
    return true;
}

uint8_t juegos_consultar_saldo(const uint8_t *uid) {
    int8_t idx = rfid_buscar_uid(uid);
    if (idx < 0) return 0;
    return lista_leer_saldo(idx);
}

bool juegos_descontar_acceso(const uint8_t *uid) {
    int8_t idx = rfid_buscar_uid(uid);
    if (idx < 0) return false;
    uint8_t saldo = lista_leer_saldo(idx);
    if (saldo == 0) return false;
    lista_escribir_saldo(idx, saldo - 1);
    return true;
}
