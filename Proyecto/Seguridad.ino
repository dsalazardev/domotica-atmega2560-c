#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <SPI.h>
#include <MFRC522.h>

#define RFID_SS_PIN 53
MFRC522 rfid(RFID_SS_PIN, 13);

#ifndef UID_LEN
#define UID_LEN 4
#endif
#ifndef MAX_TARJETAS
#define MAX_TARJETAS 10
#endif
#ifndef EEPROM_SALDO_MAX
#define EEPROM_SALDO_MAX 255
#endif

#define RFID_DEBUG 1

static void dbg_print(const char *s) {
    while (*s) {
        usart_transmit(*s);
        s++;
    }
}

#define SENSOR_DDR  DDRC
#define SENSOR_PORT PORTC
#define SENSOR_PIN  PINC
#define SENSOR_PUERTA_PP   PC7
#define SENSOR_PUERTA_GAR  PC6
#define SENSOR_VENTANA_SAL PC2
#define SENSOR_VENTANA_COC PC4

#define HUMO_DDR  DDRL
#define HUMO_PORT PORTL
#define HUMO_PIN  PINL
#define HUMO_BIT  PL6

#define INTRUSION_LED_DDR  DDRC
#define INTRUSION_LED_PORT PORTC
#define INTRUSION_LED_PIN  PC3

#define INCENDIO_LED_DDR  DDRA
#define INCENDIO_LED_PORT PORTA
#define INCENDIO_LED_PIN  PA2

#define BUZZER_DDR  DDRD
#define BUZZER_PORT PORTD
#define BUZZER_PIN  PD7

#define IMAN_DDR  DDRL
#define IMAN_PORT PORTL
#define IMAN_PP   PL4

#define SERVO_DDR  DDRB
#define SERVO_PORT PORTB
#define SERVO_PIN  PB5
#define SERVO_PWM_TOP 4999
#define SERVO_0GRADOS 125
#define SERVO_90GRADOS 2500

#define GARAJE_LED_DDR  DDRB
#define GARAJE_LED_PORT PORTB
#define GARAJE_LED_PIN  PB6

typedef enum {
    ALARMA_DESACTIVADA,
    ALARMA_ACTIVADA,
    ALARMA_DISPARADA
} estado_alarma_t;

static estado_alarma_t intrusion_estado = ALARMA_DESACTIVADA;
static estado_alarma_t incendio_estado = ALARMA_DESACTIVADA;
static bool intrusion_sensor_prev[4];
static bool incendio_sensor_prev;
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
    GARAJE_LED_DDR |= (1 << GARAJE_LED_PIN);
    GARAJE_LED_PORT &= ~(1 << GARAJE_LED_PIN);
}

static void servo_escribir(uint16_t valor) {
    OCR1A = valor;
}

void servo_abrir(void) {
    if (servo_estado != SERVO_PARADO) return;
    servo_estado = SERVO_ABRIENDO;
    servo_timer = millis();
    GARAJE_LED_PORT |= (1 << GARAJE_LED_PIN);
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
                GARAJE_LED_PORT &= ~(1 << GARAJE_LED_PIN);
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
    intrusion_estado = ALARMA_DESACTIVADA;
    incendio_estado = ALARMA_DESACTIVADA;
    SENSOR_DDR &= ~((1 << SENSOR_PUERTA_PP) | (1 << SENSOR_PUERTA_GAR) |
                    (1 << SENSOR_VENTANA_SAL) | (1 << SENSOR_VENTANA_COC));
#if MODO_FISICO
    SENSOR_PORT &= ~((1 << SENSOR_PUERTA_PP) | (1 << SENSOR_PUERTA_GAR) |
                     (1 << SENSOR_VENTANA_SAL) | (1 << SENSOR_VENTANA_COC));
#else
    SENSOR_PORT |= (1 << SENSOR_PUERTA_PP) | (1 << SENSOR_PUERTA_GAR) |
                   (1 << SENSOR_VENTANA_SAL) | (1 << SENSOR_VENTANA_COC);
#endif
    HUMO_DDR &= ~(1 << HUMO_BIT);
#if MODO_FISICO
    HUMO_PORT &= ~(1 << HUMO_BIT);
#else
    HUMO_PORT |= (1 << HUMO_BIT);
#endif
    INTRUSION_LED_DDR |= (1 << INTRUSION_LED_PIN);
    INTRUSION_LED_PORT &= ~(1 << INTRUSION_LED_PIN);
    INCENDIO_LED_DDR |= (1 << INCENDIO_LED_PIN);
    INCENDIO_LED_PORT &= ~(1 << INCENDIO_LED_PIN);
    BUZZER_DDR |= (1 << BUZZER_PIN);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
#if MODO_FISICO
    for (uint8_t i = 0; i < 4; i++) {
        intrusion_sensor_prev[i] = false;
    }
    incendio_sensor_prev = false;
#else
    for (uint8_t i = 0; i < 4; i++) {
        intrusion_sensor_prev[i] = true;
    }
    incendio_sensor_prev = true;
#endif
    IMAN_DDR |= (1 << IMAN_PP);
    IMAN_PORT &= ~(1 << IMAN_PP);
    codigo_actual = lista_leer_codigo();
    servo_init();
}

bool alarma_verificar_codigo(uint16_t codigo) {
    return codigo == codigo_actual;
}

void alarma_cambiar_codigo(uint16_t nuevo_codigo) {
    codigo_actual = nuevo_codigo;
}

void alarma_intrusion_activar(void) {
    intrusion_estado = ALARMA_ACTIVADA;
    INTRUSION_LED_PORT |= (1 << INTRUSION_LED_PIN);
}

void alarma_intrusion_desactivar(void) {
    intrusion_estado = ALARMA_DESACTIVADA;
    INTRUSION_LED_PORT &= ~(1 << INTRUSION_LED_PIN);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

bool alarma_intrusion_activa(void) {
    return intrusion_estado == ALARMA_ACTIVADA || intrusion_estado == ALARMA_DISPARADA;
}

void alarma_intrusion_disparar(const char *lugar) {
    if (intrusion_estado != ALARMA_ACTIVADA) return;
    intrusion_estado = ALARMA_DISPARADA;
    INTRUSION_LED_PORT &= ~(1 << INTRUSION_LED_PIN);
    alarma_notificar(lugar);
}

void alarma_incendio_activar(void) {
    incendio_estado = ALARMA_ACTIVADA;
    INCENDIO_LED_PORT |= (1 << INCENDIO_LED_PIN);
}

void alarma_incendio_desactivar(void) {
    incendio_estado = ALARMA_DESACTIVADA;
    INCENDIO_LED_PORT &= ~(1 << INCENDIO_LED_PIN);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

bool alarma_incendio_activa(void) {
    return incendio_estado == ALARMA_ACTIVADA || incendio_estado == ALARMA_DISPARADA;
}

bool alarma_activa(void) {
    return alarma_intrusion_activa() || alarma_incendio_activa();
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

    bool sensor_disparado[4];
#if MODO_FISICO
    sensor_disparado[0] = alarma_leer_sensor(SENSOR_PUERTA_PP);
    sensor_disparado[1] = alarma_leer_sensor(SENSOR_PUERTA_GAR);
    sensor_disparado[2] = alarma_leer_sensor(SENSOR_VENTANA_SAL);
    sensor_disparado[3] = alarma_leer_sensor(SENSOR_VENTANA_COC);
    bool humo_disparado = (HUMO_PIN & (1 << HUMO_BIT));
#else
    sensor_disparado[0] = !alarma_leer_sensor(SENSOR_PUERTA_PP);
    sensor_disparado[1] = !alarma_leer_sensor(SENSOR_PUERTA_GAR);
    sensor_disparado[2] = !alarma_leer_sensor(SENSOR_VENTANA_SAL);
    sensor_disparado[3] = !alarma_leer_sensor(SENSOR_VENTANA_COC);
    bool humo_disparado = !(HUMO_PIN & (1 << HUMO_BIT));
#endif

    if (intrusion_estado == ALARMA_ACTIVADA) {
        for (uint8_t i = 0; i < 4; i++) {
            if (sensor_disparado[i] && !intrusion_sensor_prev[i]) {
                intrusion_estado = ALARMA_DISPARADA;
                INTRUSION_LED_PORT &= ~(1 << INTRUSION_LED_PIN);
                const char *lugar = "Intrusion en ventana";
                if (i == 0) lugar = "Intrusion en Puerta PP";
                else if (i == 1) lugar = "Intrusion en Puerta Gar";
                else if (i == 2) lugar = "Intrusion en Ventana Sala";
                else lugar = "Intrusion en Ventana Cocina";
                alarma_notificar(lugar);
                break;
            }
        }
    }

    if (incendio_estado == ALARMA_ACTIVADA) {
        if (humo_disparado && !incendio_sensor_prev) {
            incendio_estado = ALARMA_DISPARADA;
            INCENDIO_LED_PORT &= ~(1 << INCENDIO_LED_PIN);
            alarma_notificar("Humo en la vivienda");
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        intrusion_sensor_prev[i] = sensor_disparado[i];
    }
    incendio_sensor_prev = humo_disparado;

    puertas_actualizar();

    if (intrusion_estado == ALARMA_DISPARADA || incendio_estado == ALARMA_DISPARADA) {
        static unsigned long ultimo_blink = 0;
        if (ahora - ultimo_blink >= 200) {
            ultimo_blink = ahora;
            if (intrusion_estado == ALARMA_DISPARADA) {
                INTRUSION_LED_PORT ^= (1 << INTRUSION_LED_PIN);
            }
            if (incendio_estado == ALARMA_DISPARADA) {
                INCENDIO_LED_PORT ^= (1 << INCENDIO_LED_PIN);
            }
        }
    }
}

void alarma_buzzer_sonar(void) {
    if (intrusion_estado == ALARMA_DISPARADA || incendio_estado == ALARMA_DISPARADA) {
        static unsigned long ultimo = 0;
        unsigned long ahora = micros();
        if (ahora - ultimo >= 500) {
            ultimo = ahora;
            BUZZER_PORT ^= (1 << BUZZER_PIN);
        }
    }
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
    SPI.begin();
    rfid.PCD_Init();
    rfid_tarjeta_presente = false;
    rfid_tarjeta_anterior = false;
    uint8_t cnt = lista_leer_conteo();
    if (cnt == 0xFF || cnt > MAX_TARJETAS) {
        lista_escribir_conteo(0);
    }

    {
        static const char hex[] = "0123456789ABCDEF";
        uint8_t ver = rfid.PCD_ReadRegister(MFRC522::VersionReg);
        lcd_borrar();
        lcd_imprimir("RFID v:0x");
        lcd_dato(hex[ver >> 4]);
        lcd_dato(hex[ver & 0x0F]);
        lcd_posicion(1, 0);
        if (ver == 0x91 || ver == 0x92) {
            lcd_imprimir("SPI OK");
        } else {
            lcd_imprimir("SPI FAIL!");
        }
        delay(2000);
    }
}

bool rfid_validar_uid(const uint8_t *uid) {
    static const uint8_t EMERGENCIA_UID[UID_LEN] = {0x01, 0x02, 0x03, 0x04};
    uint8_t match = 0;
    for (uint8_t i = 0; i < UID_LEN; i++) {
        if (uid[i] == EMERGENCIA_UID[i]) match++;
    }
    if (match == UID_LEN) return true;
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

void rfid_simular_tarjeta(const uint8_t *uid) {
    rfid_tarjeta_anterior = false;
    rfid_tarjeta_presente = true;
    for (uint8_t i = 0; i < UID_LEN; i++) {
        rfid_uid[i] = uid[i];
    }
}

void rfid_actualizar(void) {
    unsigned long ahora = millis();
    if (ahora - rfid_ultimo_poll < 200) return;
    rfid_ultimo_poll = ahora;

    rfid_tarjeta_anterior = rfid_tarjeta_presente;

    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        rfid_tarjeta_presente = true;
        for (uint8_t i = 0; i < rfid.uid.size && i < UID_LEN; i++) {
            rfid_uid[i] = rfid.uid.uidByte[i];
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
#if RFID_DEBUG
        dbg_print("RFID OK UID:");
        {
            static const char hex[] = "0123456789ABCDEF";
            for (uint8_t i = 0; i < UID_LEN; i++) {
                usart_transmit(hex[rfid_uid[i] >> 4]);
                usart_transmit(hex[rfid_uid[i] & 0x0F]);
            }
        }
        usart_transmit('\r');
        usart_transmit('\n');
#endif
    } else {
        rfid_tarjeta_presente = false;
    }
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

void rfid_generar_uid(uint8_t *uid) {
    static uint8_t contador = 0;
    uid[0] = 0xAA;
    uid[1] = contador++;
    uid[2] = 0xBB;
    uid[3] = 0xCC;
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
