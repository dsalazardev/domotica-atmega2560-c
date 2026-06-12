#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <avr/eeprom.h>

#define EEPROM_CODIGO     0x00
#define EEPROM_RFID_CNT   0x02
#define EEPROM_RFID_LIST  0x10
#define EEPROM_SALDO_LIST 0x50
#define EEPROM_LISTA_CNT  0x60
#define EEPROM_LISTA_DATA 0x61

#define MAX_TARJETAS  10
#define UID_LEN       4
#define COD_DIGITOS   4
#define EEPROM_SALDO_MAX 255

#define LISTA_MAX 20
#define LISTA_NOMBRE_LEN 20

static char lista_nombres[LISTA_MAX][LISTA_NOMBRE_LEN];
static uint8_t lista_cantidades[LISTA_MAX];
static uint8_t total_productos = 0;
static bool lista_cargada = false;

static void lista_guardar_eeprom(void) {
    eeprom_write_byte((uint8_t*)EEPROM_LISTA_CNT, total_productos);
    for (uint8_t i = 0; i < total_productos; i++) {
        uint16_t addr = EEPROM_LISTA_DATA + i * (LISTA_NOMBRE_LEN + 1);
        for (uint8_t j = 0; j < LISTA_NOMBRE_LEN; j++) {
            eeprom_write_byte((uint8_t*)(addr + j), lista_nombres[i][j]);
        }
        eeprom_write_byte((uint8_t*)(addr + LISTA_NOMBRE_LEN), lista_cantidades[i]);
    }
}

void lista_agregar(const char* nombre, uint8_t cantidad) {
    if (total_productos >= LISTA_MAX) return;
    uint8_t i = total_productos++;
    strcpy(lista_nombres[i], nombre);
    lista_cantidades[i] = cantidad;
    lista_guardar_eeprom();
}

void lista_eliminar(uint8_t indice) {
    if (indice >= total_productos) return;
    for (uint8_t i = indice; i < total_productos - 1; i++) {
        strcpy(lista_nombres[i], lista_nombres[i + 1]);
        lista_cantidades[i] = lista_cantidades[i + 1];
    }
    total_productos--;
    lista_guardar_eeprom();
}

void lista_vaciar(void) {
    total_productos = 0;
    lista_guardar_eeprom();
}

uint8_t lista_total(void) {
    return total_productos;
}

const char* lista_obtener_nombre(uint8_t indice) {
    if (indice >= total_productos) return "";
    return lista_nombres[indice];
}

uint8_t lista_obtener_cantidad(uint8_t indice) {
    if (indice >= total_productos) return 0;
    return lista_cantidades[indice];
}

void lista_actualizar(void) {
    if (lista_cargada) return;
    total_productos = eeprom_read_byte((uint8_t*)EEPROM_LISTA_CNT);
    if (total_productos > LISTA_MAX) total_productos = 0;
    for (uint8_t i = 0; i < total_productos; i++) {
        uint16_t addr = EEPROM_LISTA_DATA + i * (LISTA_NOMBRE_LEN + 1);
        memset(lista_nombres[i], 0, LISTA_NOMBRE_LEN);
        for (uint8_t j = 0; j < LISTA_NOMBRE_LEN; j++) {
            uint8_t b = eeprom_read_byte((uint8_t*)(addr + j));
            if (b == 0xFF) break;
            lista_nombres[i][j] = b;
        }
        lista_cantidades[i] = eeprom_read_byte((uint8_t*)(addr + LISTA_NOMBRE_LEN));
    }
    lista_cargada = true;
}

uint16_t lista_leer_codigo(void) {
    return eeprom_read_word((uint16_t*)EEPROM_CODIGO);
}

void lista_escribir_codigo(uint16_t cod) {
    eeprom_write_word((uint16_t*)EEPROM_CODIGO, cod);
}

uint8_t lista_leer_conteo(void) {
    return eeprom_read_byte((uint8_t*)EEPROM_RFID_CNT);
}

void lista_escribir_conteo(uint8_t cnt) {
    eeprom_write_byte((uint8_t*)EEPROM_RFID_CNT, cnt);
}

void lista_leer_uid(uint8_t idx, uint8_t *uid) {
    uint16_t addr = EEPROM_RFID_LIST + idx * UID_LEN;
    for (uint8_t i = 0; i < UID_LEN; i++) {
        uid[i] = eeprom_read_byte((uint8_t*)(addr + i));
    }
}

void lista_escribir_uid(uint8_t idx, const uint8_t *uid) {
    uint16_t addr = EEPROM_RFID_LIST + idx * UID_LEN;
    for (uint8_t i = 0; i < UID_LEN; i++) {
        eeprom_write_byte((uint8_t*)(addr + i), uid[i]);
    }
}

uint8_t lista_leer_saldo(uint8_t idx) {
    return eeprom_read_byte((uint8_t*)(EEPROM_SALDO_LIST + idx));
}

void lista_escribir_saldo(uint8_t idx, uint8_t saldo) {
    eeprom_write_byte((uint8_t*)(EEPROM_SALDO_LIST + idx), saldo);
}
