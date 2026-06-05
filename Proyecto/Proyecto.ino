#include <avr/io.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef COD_DIGITOS
#define COD_DIGITOS 4
#endif

typedef enum {
    MENU_PRINCIPAL,
    MENU_SEGURIDAD,
    MENU_SEG_CAMBIAR_COD_NUEVO1,
    MENU_SEG_CAMBIAR_COD_NUEVO2,
    MENU_ACCESO,
    MENU_ACC_ENROLAR,
    MENU_ACC_BORRAR_SEL,
    MENU_ACC_PUERTA,
    MENU_ACC_PUERTA_SEL,
    MENU_JUEGOS,
    MENU_JUEG_INGRESAR,
    MENU_JUEG_RECARGAR,
    MENU_JUEG_CONSULTAR,
    MENU_CODIGO_INPUT,
    MENU_MENSAJE
} estado_menu_t;

static estado_menu_t menu_estado = MENU_PRINCIPAL;
static unsigned long menu_mensaje_hasta = 0;
static uint8_t menu_seleccion = 0;
static uint8_t entrada_digitos[COD_DIGITOS];
static uint8_t entrada_pos;
static bool codigo_pendiente = false;
static uint8_t menu_codigo_nuevo[COD_DIGITOS];

static void mostrar_numero(uint8_t n) {
    if (n >= 100) lcd_dato('0' + n / 100);
    else lcd_dato(' ');
    if (n >= 10) lcd_dato('0' + (n / 10) % 10);
    else lcd_dato(' ');
    lcd_dato('0' + n % 10);
}

static void mostrar_mensaje(const char *msg, uint16_t duracion_ms) {
    lcd_borrar();
    lcd_imprimir(msg);
    menu_estado = MENU_MENSAJE;
    menu_mensaje_hasta = millis() + duracion_ms;
}

static void menu_mostrar_principal(void) {
    lcd_borrar();
    lcd_imprimir("1.Seg 2.Acc 3.Jue");
}

static void menu_mostrar_seguridad(void) {
    lcd_borrar();
    lcd_imprimir("1.Act 2.Des 3.Cod");
}

static void menu_mostrar_acceso(void) {
    lcd_borrar();
    lcd_imprimir("1.Enr 2.Bor 3.Puer");
}

static void menu_mostrar_juegos(void) {
    lcd_borrar();
    lcd_imprimir("1.Ing 2.Rec 3.Con");
}

static void menu_solicitar_codigo(const char *titulo) {
    lcd_borrar();
    lcd_imprimir(titulo);
    lcd_posicion(1, 0);
    entrada_pos = 0;
    for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
    menu_estado = MENU_CODIGO_INPUT;
    codigo_pendiente = false;
}

static uint16_t entrada_a_codigo(void) {
    uint16_t val = 0;
    for (uint8_t i = 0; i < COD_DIGITOS; i++) val = val * 10 + entrada_digitos[i];
    return val;
}

static bool menu_procesar_ingreso_codigo(char tecla) {
    if (tecla >= '0' && tecla <= '9' && entrada_pos < COD_DIGITOS) {
        entrada_digitos[entrada_pos] = tecla - '0';
        lcd_dato(tecla);
        entrada_pos++;
        if (entrada_pos == COD_DIGITOS) codigo_pendiente = true;
        return true;
    }
    if (tecla == '*' && entrada_pos > 0) {
        entrada_pos--;
        entrada_digitos[entrada_pos] = 0;
        lcd_posicion(1, entrada_pos);
        lcd_dato(' ');
        lcd_posicion(1, entrada_pos);
        return true;
    }
    if ((tecla == '#' || tecla == 'A') && entrada_pos == COD_DIGITOS) {
        codigo_pendiente = true;
        return true;
    }
    return false;
}

static void menu_procesar_main(char tecla) {
    switch (tecla) {
        case '1': menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); break;
        case '2': menu_estado = MENU_ACCESO; menu_mostrar_acceso(); break;
        case '3': menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); break;
        default: break;
    }
}

static void menu_procesar_seguridad(char tecla) {
    switch (tecla) {
        case '1': menu_solicitar_codigo("Cod Activar:"); menu_seleccion = 1; break;
        case '2': menu_solicitar_codigo("Cod Desactivar:"); menu_seleccion = 2; break;
        case '3': menu_solicitar_codigo("Cod Actual:"); menu_seleccion = 3; break;
        case 'B': menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); break;
        default: break;
    }
}

static void menu_codigo_ingresado(void) {
    codigo_pendiente = false;
    uint16_t cod = entrada_a_codigo();
    switch (menu_seleccion) {
        case 1:
            if (alarma_verificar_codigo(cod)) { alarma_activar(); mostrar_mensaje("Alarma Activada", 2000); }
            else { mostrar_mensaje("Codigo incorrecto", 2000); }
            menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad();
            break;
        case 2:
            if (alarma_verificar_codigo(cod)) { alarma_desactivar(); mostrar_mensaje("Alarma Desactivada", 2000); }
            else { mostrar_mensaje("Codigo incorrecto", 2000); }
            menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad();
            break;
        case 3:
            if (alarma_verificar_codigo(cod)) {
                menu_estado = MENU_SEG_CAMBIAR_COD_NUEVO1;
                lcd_borrar(); lcd_imprimir("Nuevo codigo:"); lcd_posicion(1, 0);
                entrada_pos = 0;
                for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
            } else {
                mostrar_mensaje("Cod actual incorrecto", 2000);
                menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad();
            }
            break;
        default:
            menu_estado = MENU_PRINCIPAL; menu_mostrar_principal();
            break;
    }
}

static void menu_procesar_cambio_nuevo1(void) {
    codigo_pendiente = false;
    for (uint8_t i = 0; i < COD_DIGITOS; i++) menu_codigo_nuevo[i] = entrada_digitos[i];
    menu_estado = MENU_SEG_CAMBIAR_COD_NUEVO2;
    lcd_borrar(); lcd_imprimir("Confirmar codigo:"); lcd_posicion(1, 0);
    entrada_pos = 0;
    for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
}

static void menu_procesar_cambio_nuevo2(void) {
    codigo_pendiente = false;
    bool coincide = true;
    for (uint8_t i = 0; i < COD_DIGITOS; i++) {
        if (menu_codigo_nuevo[i] != entrada_digitos[i]) { coincide = false; break; }
    }
    if (coincide) {
        uint16_t nuevo_cod = 0;
        for (uint8_t i = 0; i < COD_DIGITOS; i++) nuevo_cod = nuevo_cod * 10 + menu_codigo_nuevo[i];
        alarma_cambiar_codigo(nuevo_cod);
        lista_escribir_codigo(nuevo_cod);
        mostrar_mensaje("Codigo cambiado", 2000);
    } else {
        mostrar_mensaje("Confirmacion fallida", 2000);
    }
    menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad();
}

static void menu_procesar_acceso(char tecla) {
    switch (tecla) {
        case '1':
            lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
            menu_estado = MENU_ACC_ENROLAR;
            break;
        case '2': {
            uint8_t cnt = rfid_get_conteo();
            if (cnt == 0) {
                mostrar_mensaje("No hay tarjetas", 2000);
                menu_estado = MENU_ACCESO; menu_mostrar_acceso();
            } else {
                lcd_borrar(); lcd_imprimir("Sel tarjeta 1-9");
                lcd_posicion(1, 0); lcd_imprimir("Cnt:"); lcd_dato('0' + cnt);
                menu_estado = MENU_ACC_BORRAR_SEL;
            }
            break;
        }
        case '3':
            lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
            menu_estado = MENU_ACC_PUERTA;
            break;
        case 'B': menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); break;
        default: break;
    }
}

static void menu_procesar_acceso_puerta(char tecla) {
    if (rfid_tarjeta_nueva()) {
        if (rfid_validar_uid(rfid_get_uid())) {
            lcd_borrar(); lcd_imprimir("PP=A GAR=B");
            menu_estado = MENU_ACC_PUERTA_SEL;
        } else {
            mostrar_mensaje("Acceso denegado", 2000);
            menu_estado = MENU_ACCESO; menu_mostrar_acceso();
        }
    } else if (tecla == 'B') {
        menu_estado = MENU_ACCESO; menu_mostrar_acceso();
    }
}

static void menu_procesar_puerta_sel(char tecla) {
    if (tecla == 'A') { puerta_abrir_principal(); mostrar_mensaje("Puerta abierta", 2000); }
    else if (tecla == 'B') { servo_abrir(); mostrar_mensaje("Garaje abierto", 2000); }
    else { mostrar_mensaje("Op invalida", 1000); }
    menu_estado = MENU_ACCESO; menu_mostrar_acceso();
}

static void menu_procesar_borrar_sel(char tecla) {
    if (tecla >= '1' && tecla <= '9') {
        uint8_t idx = tecla - '1';
        if (idx < rfid_get_conteo()) {
            rfid_borrar(idx);
            mostrar_mensaje("Tarjeta eliminada", 2000);
        } else {
            mostrar_mensaje("Indice invalido", 2000);
        }
        menu_estado = MENU_ACCESO; menu_mostrar_acceso();
    } else if (tecla == 'B') {
        menu_estado = MENU_ACCESO; menu_mostrar_acceso();
    }
}

static void menu_procesar_juegos(char tecla) {
    switch (tecla) {
        case '1':
            lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
            menu_estado = MENU_JUEG_INGRESAR;
            break;
        case '2':
            menu_solicitar_codigo("Codigo padres:");
            menu_seleccion = 10;
            break;
        case '3':
            lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
            menu_estado = MENU_JUEG_CONSULTAR;
            break;
        case 'B': menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); break;
        default: break;
    }
}

static void menu_procesar_juegos_recarga(void) {
    codigo_pendiente = false;
    uint16_t cod = entrada_a_codigo();
    if (alarma_verificar_codigo(cod)) {
        lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
        menu_seleccion = 11;
        menu_estado = MENU_JUEG_RECARGAR;
    } else {
        mostrar_mensaje("Codigo incorrecto", 2000);
        menu_estado = MENU_JUEGOS; menu_mostrar_juegos();
    }
}

static void menu_procesar_recarga_accesos(void) {
    uint8_t accesos = (uint8_t)entrada_a_codigo();
    if (accesos > 100) accesos = 100;
    juegos_cargar_accesos(rfid_get_uid(), accesos);
    lcd_borrar(); lcd_imprimir("Recarga exitosa");
    lcd_posicion(1, 0); lcd_imprimir("Saldo: ");
    mostrar_numero(accesos); lcd_imprimir(" usos");
    menu_mensaje_hasta = millis() + 3000;
    menu_estado = MENU_MENSAJE;
}

void menu_iniciar(void) {
    menu_estado = MENU_PRINCIPAL;
    menu_mostrar_principal();
}

void menu_procesar_tecla(char tecla) {
    if (menu_estado == MENU_MENSAJE) {
        if (millis() >= menu_mensaje_hasta) { menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); }
        return;
    }

    if (menu_estado == MENU_CODIGO_INPUT) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) {
            codigo_pendiente = false;
            if (menu_seleccion == 12) { menu_seleccion = 0; menu_procesar_recarga_accesos(); }
            else if (menu_seleccion >= 10) { menu_procesar_juegos_recarga(); }
            else { menu_codigo_ingresado(); }
        }
        return;
    }

    if (menu_estado == MENU_SEG_CAMBIAR_COD_NUEVO1) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) menu_procesar_cambio_nuevo1();
        return;
    }

    if (menu_estado == MENU_SEG_CAMBIAR_COD_NUEVO2) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) menu_procesar_cambio_nuevo2();
        return;
    }

    if (menu_estado == MENU_ACC_ENROLAR) {
        if (rfid_tarjeta_nueva()) {
            if (rfid_enrolar(rfid_get_uid())) mostrar_mensaje("Persona enrolada", 2000);
            else mostrar_mensaje("Tarjeta ya existe", 2000);
            menu_estado = MENU_ACCESO; menu_mostrar_acceso();
        } else if (tecla == 'B') { menu_estado = MENU_ACCESO; menu_mostrar_acceso(); }
        return;
    }

    if (menu_estado == MENU_ACC_BORRAR_SEL) { menu_procesar_borrar_sel(tecla); return; }
    if (menu_estado == MENU_ACC_PUERTA) { menu_procesar_acceso_puerta(tecla); return; }
    if (menu_estado == MENU_ACC_PUERTA_SEL) { menu_procesar_puerta_sel(tecla); return; }

    if (menu_estado == MENU_JUEG_INGRESAR) {
        if (rfid_tarjeta_nueva()) {
            const uint8_t* uid = rfid_get_uid();
            if (!rfid_validar_uid(uid)) {
                mostrar_mensaje("Acceso denegado", 2000);
            } else {
                uint8_t saldo = juegos_consultar_saldo(uid);
                if (saldo == 0) {
                    mostrar_mensaje("Sin accesos. Pide a tus padres", 3000);
                } else {
                    juegos_descontar_acceso(uid);
                    lcd_borrar(); lcd_imprimir("Bienvenido!");
                    lcd_posicion(1, 0); lcd_imprimir("Te quedan ");
                    mostrar_numero(saldo - 1); lcd_imprimir(" usos");
                    menu_mensaje_hasta = millis() + 3000;
                    menu_estado = MENU_MENSAJE;
                    puerta_abrir_principal();
                }
            }
            menu_estado = MENU_JUEGOS; menu_mostrar_juegos();
        } else if (tecla == 'B') { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
        return;
    }

    if (menu_estado == MENU_JUEG_RECARGAR) {
        if (rfid_tarjeta_nueva()) {
            lcd_borrar(); lcd_imprimir("Accesos a sumar:");
            lcd_posicion(1, 0); entrada_pos = 0;
            for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
            menu_seleccion = 12;
            menu_estado = MENU_CODIGO_INPUT;
            codigo_pendiente = false;
        } else if (tecla == 'B') { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
        return;
    }

    if (menu_estado == MENU_JUEG_CONSULTAR) {
        if (rfid_tarjeta_nueva()) {
            const uint8_t* uid = rfid_get_uid();
            if (!rfid_validar_uid(uid)) {
                mostrar_mensaje("Acceso denegado", 2000);
            } else {
                uint8_t saldo = juegos_consultar_saldo(uid);
                lcd_borrar(); lcd_imprimir("Saldo: ");
                mostrar_numero(saldo); lcd_imprimir(" usos");
                menu_mensaje_hasta = millis() + 3000;
                menu_estado = MENU_MENSAJE;
            }
            menu_estado = MENU_JUEGOS; menu_mostrar_juegos();
        } else if (tecla == 'B') { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
        return;
    }

    switch (menu_estado) {
        case MENU_PRINCIPAL: menu_procesar_main(tecla); break;
        case MENU_SEGURIDAD: menu_procesar_seguridad(tecla); break;
        case MENU_ACCESO: menu_procesar_acceso(tecla); break;
        case MENU_JUEGOS: menu_procesar_juegos(tecla); break;
        default: break;
    }
}

static void usart_respuesta_ok(const char* msg) {
    usart_print(PSTR("OK: "));
    usart_print(msg);
    usart_transmit('\n');
}

static void usart_respuesta_error(void) {
    usart_print(PSTR("ERROR: comando invalido\n"));
}

static uint16_t parse_uint16(const char* s) {
    uint16_t v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v * 10 + (*s++ - '0');
    }
    return v;
}

static void usart_envia_estado(void) {
    usart_print(PSTR("OK: ALARMA="));
    usart_transmit(alarma_activa() ? '1' : '0');
    usart_print(PSTR(" HORNO="));
    usart_transmit(horno_activo_get() ? '1' : '0');
    usart_print(PSTR(" SONIDO="));
    usart_transmit(sonido_activo_get() ? '1' : '0');
    usart_print(PSTR(" TEMP="));
    uint8_t t = temperatura_leer();
    usart_transmit('0' + t / 10);
    usart_transmit('0' + t % 10);
    usart_transmit('\n');
}

static void usart_envia_lista(void) {
    uint8_t n = lista_total();
    usart_print(PSTR("OK: "));
    for (uint8_t i = 0; i < n; i++) {
        if (i > 0) usart_transmit(',');
        usart_transmit('"');
        const char* nom = lista_obtener_nombre(i);
        while (*nom) usart_transmit(*nom++);
        usart_print(PSTR("\"x"));
        uint8_t cant = lista_obtener_cantidad(i);
        usart_transmit('0' + cant);
    }
    usart_transmit('\n');
}

static void usart_envia_temp(void) {
    usart_print(PSTR("OK: "));
    uint8_t t = temperatura_leer();
    usart_transmit('0' + t / 10);
    usart_transmit('0' + t % 10);
    usart_print(PSTR(" C\n"));
}

static void procesar_comando(char* linea) {
    char* tok = strtok(linea, " ");
    if (tok == NULL) return;

    if (strcmp_P(tok, PSTR("ALARMA")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        uint16_t cod = parse_uint16(tok);
        if (strcmp_P(tok, PSTR("ON")) == 0) {
            if (alarma_verificar_codigo(cod)) {
                alarma_activar();
                usart_respuesta_ok(PSTR("alarma activada"));
            } else {
                usart_respuesta_ok(PSTR("codigo incorrecto"));
            }
        } else if (strcmp_P(tok, PSTR("OFF")) == 0) {
            if (alarma_verificar_codigo(cod)) {
                alarma_desactivar();
                usart_respuesta_ok(PSTR("alarma desactivada"));
            } else {
                usart_respuesta_ok(PSTR("codigo incorrecto"));
            }
        } else {
            usart_respuesta_error();
        }
    } else if (strcmp_P(tok, PSTR("HORNO")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        if (strcmp_P(tok, PSTR("ON")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint16_t temp = parse_uint16(tok);
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint16_t tsec = parse_uint16(tok);
            horno_encender(tsec, temp);
            usart_respuesta_ok(PSTR("horno encendido"));
        } else if (strcmp_P(tok, PSTR("OFF")) == 0) {
            horno_apagar();
            usart_respuesta_ok(PSTR("horno apagado"));
        } else {
            usart_respuesta_error();
        }
    } else if (strcmp_P(tok, PSTR("SONIDO")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        if (strcmp_P(tok, PSTR("ON")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint8_t vol = (uint8_t)parse_uint16(tok);
            sonido_encender();
            sonido_volumen(vol);
            usart_respuesta_ok(PSTR("sonido encendido"));
        } else if (strcmp_P(tok, PSTR("OFF")) == 0) {
            sonido_apagar();
            usart_respuesta_ok(PSTR("sonido apagado"));
        } else {
            usart_respuesta_error();
        }
    } else if (strcmp_P(tok, PSTR("TEMP?")) == 0) {
        usart_envia_temp();
    } else if (strcmp_P(tok, PSTR("LISTA?")) == 0) {
        usart_envia_lista();
    } else if (strcmp_P(tok, PSTR("LISTA+")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        const char* nom = tok;
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        uint8_t cant = (uint8_t)parse_uint16(tok);
        lista_agregar(nom, cant);
        usart_respuesta_ok(PSTR("item agregado"));
    } else if (strcmp_P(tok, PSTR("LISTA-")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        uint8_t idx = (uint8_t)parse_uint16(tok);
        lista_eliminar(idx);
        usart_respuesta_ok(PSTR("item eliminado"));
    } else if (strcmp_P(tok, PSTR("ESTADO?")) == 0) {
        usart_envia_estado();
    } else {
        usart_respuesta_error();
    }
}

static void procesar_comandos_usart(void) {
    static char linea[48];
    static uint8_t pos = 0;

    while (usart_disponible() > 0) {
        char c = usart_leer();
        if (c == '\n' || c == '\r') {
            if (pos > 0) {
                linea[pos] = '\0';
                procesar_comando(linea);
                pos = 0;
            }
        } else if (pos < sizeof(linea) - 1) {
            linea[pos++] = c;
        }
    }
}

void setup() {
    lcd_init();
    teclado_init();
    usart_init();
    timer_init();
    spi_master_init();
    alarma_init();
    rfid_init();
    iluminacion_iniciar();
    temperatura_iniciar();
    lista_actualizar();
    menu_iniciar();
}

void loop() {
    static unsigned long ultimo_tick = 0;
    unsigned long ahora = millis();

    procesar_comandos_usart();

    if (ahora - ultimo_tick >= 50) {
        ultimo_tick = ahora;

        char tecla = teclado_scan();
        if (tecla != 0) {
            menu_procesar_tecla(tecla);
        }

        alarma_actualizar();
        rfid_actualizar();
        ambiente_actualizar();
    }
}
