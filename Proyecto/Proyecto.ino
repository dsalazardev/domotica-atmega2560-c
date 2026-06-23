#include <avr/io.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef COD_DIGITOS
#define COD_DIGITOS 4
#endif

// Modo fisico: switches entregan VCC al activarse (activo-alto)
// Modo Proteus: botones conectan a GND al presionarse (activo-bajo)
// Cambiar a 0 para compilar para Proteus/simulacion
#define MODO_FISICO 1

typedef enum {
    MENU_PRINCIPAL,
    MENU_SEGURIDAD,
    MENU_SEG_INTRUSION,
    MENU_SEG_INCENDIO,
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
    MENU_AMBIENTE,
    MENU_AMB_ILUM,
    MENU_AMB_TEMP,
    MENU_AMB_HORNO,
    MENU_AMB_SONIDO,
    MENU_MENSAJE
} estado_menu_t;

static estado_menu_t menu_estado = MENU_PRINCIPAL;
static unsigned long menu_mensaje_hasta = 0;
static estado_menu_t menu_volver_a = MENU_PRINCIPAL;
static uint8_t menu_seleccion = 0;
static uint8_t entrada_digitos[COD_DIGITOS];
static uint8_t entrada_pos;
static bool codigo_pendiente = false;
static uint8_t menu_codigo_nuevo[COD_DIGITOS];

typedef enum {
    MAESTRO_OCIOSO,
    MAESTRO_ESPERANDO_HORNO,
    MAESTRO_ESPERANDO_SONIDO
} estado_maestro_t;

static estado_maestro_t maestro_estado = MAESTRO_OCIOSO;
static unsigned long maestro_tiempo_envio = 0;
static char maestro_respuesta[48];
static uint8_t maestro_resp_pos = 0;

static void mostrar_numero(uint8_t n) {
    if (n >= 100) lcd_dato('0' + n / 100);
    else lcd_dato(' ');
    if (n >= 10) lcd_dato('0' + (n / 10) % 10);
    else lcd_dato(' ');
    lcd_dato('0' + n % 10);
}

static void mostrar_mensaje(const char *msg, uint16_t duracion_ms, estado_menu_t volver_a) {
    lcd_borrar();
    lcd_imprimir(msg);
    menu_volver_a = volver_a;
    menu_estado = MENU_MENSAJE;
    menu_mensaje_hasta = millis() + duracion_ms;
}

static void menu_mostrar_principal(void) {
    lcd_borrar();
    lcd_imprimir("1.Seg 2.Acc 3.Jue");
    lcd_posicion(1, 0);
    lcd_imprimir("4.Amb           ");
}

static void menu_mostrar_seg_intrusion(void) {
    lcd_borrar();
    lcd_imprimir("1.Act 2.Des 3.At");
}

static void menu_mostrar_seg_incendio(void) {
    lcd_borrar();
    lcd_imprimir("1.Act 2.Des 3.At");
}

static void menu_mostrar_seguridad(void) {
    lcd_borrar();
    lcd_imprimir("1.Intrus 2.Incen");
    lcd_posicion(1, 0);
    lcd_imprimir("3.Codigo     ");
}

static void menu_mostrar_acceso(void) {
    lcd_borrar();
    lcd_imprimir("1.Enr 2.Bor 3.Puer");
}

static void menu_mostrar_juegos(void) {
    lcd_borrar();
    lcd_imprimir("1.Ing 2.Rec 3.Con");
}

static void menu_mostrar_ambiente(void) {
    lcd_borrar();
    lcd_imprimir("1.Ilum 2.Temp");
    lcd_posicion(1, 0);
    lcd_imprimir("3.Horn 4.Soni");
}

static void menu_mostrar_ilum(void) {
    lcd_borrar();
    uint8_t n = ilum_nivel_get();
    lcd_imprimir("Ilum: ");
    if (n < 10) lcd_dato(' ');
    else lcd_dato('0' + n / 10);
    lcd_dato('0' + n % 10);
    lcd_imprimir("%  ");
    for (uint8_t i = 0; i < n / 10; i++) lcd_dato('*');
    lcd_posicion(1, 0);
    lcd_imprimir("A+10 B-10 C=0 D<");
}

static void menu_mostrar_temp(void) {
    lcd_borrar();
    uint8_t t = temperatura_leer();
    lcd_imprimir("TEMP:");
    if (t < 10) lcd_dato(' ');
    lcd_dato('0' + t / 10 % 10);
    lcd_dato('0' + t % 10);
    lcd_imprimir("C");
    lcd_posicion(1, 0);
    for (uint8_t i = 0; i < t / 4; i++) lcd_dato('*');
    lcd_imprimir(" A+1 B-1 D<");
}

static void menu_mostrar_horno(void) {
    lcd_borrar();
    if (horno_activo_get()) lcd_imprimir("Horno: ON");
    else lcd_imprimir("Horno: OFF");
    lcd_posicion(1, 0); lcd_imprimir("A=On B=Off D=Salir");
}

static void menu_mostrar_sonido_valor(uint8_t n) {
    lcd_borrar();
    lcd_imprimir("Soni: ");
    if (n < 10) lcd_dato(' ');
    else lcd_dato('0' + n / 10);
    lcd_dato('0' + n % 10);
    lcd_imprimir("%  ");
    for (uint8_t i = 0; i < n / 10; i++) lcd_dato('*');
    lcd_posicion(1, 0);
    lcd_imprimir("A+10 B-10 C=0 D<");
}

static void menu_mostrar_sonido(void) {
    menu_mostrar_sonido_valor(sonido_nivel_get());
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

static void menu_procesar_ambiente(char tecla) {
    switch (tecla) {
        case '1': menu_estado = MENU_AMB_ILUM; menu_mostrar_ilum(); break;
        case '2': menu_estado = MENU_AMB_TEMP; menu_mostrar_temp(); break;
        case '3': menu_estado = MENU_AMB_HORNO; menu_mostrar_horno(); break;
        case '4': menu_estado = MENU_AMB_SONIDO; menu_mostrar_sonido(); break;
        case 'B': menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); break;
        default: break;
    }
}

static void menu_procesar_main(char tecla) {
    switch (tecla) {
        case '1': menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); break;
        case '2': menu_estado = MENU_ACCESO; menu_mostrar_acceso(); break;
        case '4': menu_estado = MENU_AMBIENTE; menu_mostrar_ambiente(); break;
        case '3': menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); break;
        default: break;
    }
}

static void menu_procesar_seg_intrusion(char tecla) {
    switch (tecla) {
        case '1': menu_solicitar_codigo("Cod Act Int:"); menu_seleccion = 101; break;
        case '2': menu_solicitar_codigo("Cod Des Int:"); menu_seleccion = 102; break;
        case '3': case 'B': menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); break;
        default: break;
    }
}

static void menu_procesar_seg_incendio(char tecla) {
    switch (tecla) {
        case '1': menu_solicitar_codigo("Cod Act Inc:"); menu_seleccion = 103; break;
        case '2': menu_solicitar_codigo("Cod Des Inc:"); menu_seleccion = 104; break;
        case '3': case 'B': menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); break;
        default: break;
    }
}

static void menu_procesar_seguridad(char tecla) {
    switch (tecla) {
        case '1': menu_estado = MENU_SEG_INTRUSION; menu_mostrar_seg_intrusion(); break;
        case '2': menu_estado = MENU_SEG_INCENDIO; menu_mostrar_seg_incendio(); break;
        case '3': menu_solicitar_codigo("Cod Actual:"); menu_seleccion = 3; break;
        case 'B': menu_estado = MENU_PRINCIPAL; menu_mostrar_principal(); break;
        default: break;
    }
}

static void menu_codigo_ingresado(void) {
    codigo_pendiente = false;
    uint16_t cod = entrada_a_codigo();
    switch (menu_seleccion) {
        case 101:
            if (alarma_verificar_codigo(cod)) { alarma_intrusion_activar(); mostrar_mensaje("Intrusion Activada", 2000, MENU_SEG_INTRUSION); }
            else { mostrar_mensaje("Codigo incorrecto", 2000, MENU_SEG_INTRUSION); }
            break;
        case 102:
            if (alarma_verificar_codigo(cod)) { alarma_intrusion_desactivar(); mostrar_mensaje("Intrusion Desactivada", 2000, MENU_SEG_INTRUSION); }
            else { mostrar_mensaje("Codigo incorrecto", 2000, MENU_SEG_INTRUSION); }
            break;
        case 103:
            if (alarma_verificar_codigo(cod)) { alarma_incendio_activar(); mostrar_mensaje("Incendio Activada", 2000, MENU_SEG_INCENDIO); }
            else { mostrar_mensaje("Codigo incorrecto", 2000, MENU_SEG_INCENDIO); }
            break;
        case 104:
            if (alarma_verificar_codigo(cod)) { alarma_incendio_desactivar(); mostrar_mensaje("Incendio Desactivada", 2000, MENU_SEG_INCENDIO); }
            else { mostrar_mensaje("Codigo incorrecto", 2000, MENU_SEG_INCENDIO); }
            break;
        case 3:
            if (alarma_verificar_codigo(cod)) {
                menu_estado = MENU_SEG_CAMBIAR_COD_NUEVO1;
                lcd_borrar(); lcd_imprimir("Nuevo codigo:"); lcd_posicion(1, 0);
                entrada_pos = 0;
                for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
            } else {
                mostrar_mensaje("Cod actual incorrecto", 2000, MENU_SEGURIDAD);
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
        mostrar_mensaje("Codigo cambiado", 2000, MENU_SEGURIDAD);
    } else {
        mostrar_mensaje("Confirmacion fallida", 2000, MENU_SEGURIDAD);
    }
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
                mostrar_mensaje("No hay tarjetas", 2000, MENU_ACCESO);
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
            mostrar_mensaje("Acceso denegado", 2000, MENU_ACCESO);
        }
    } else if (tecla == 'B') {
        menu_estado = MENU_ACCESO; menu_mostrar_acceso();
    }
}

static void menu_procesar_puerta_sel(char tecla) {
    if (tecla == 'A') { puerta_abrir_principal(); mostrar_mensaje("Puerta abierta", 2000, MENU_ACCESO); }
    else if (tecla == 'B') { servo_abrir(); mostrar_mensaje("Garaje abierto", 2000, MENU_ACCESO); }
    else { mostrar_mensaje("Op invalida", 1000, MENU_ACCESO); }
}

static void menu_procesar_borrar_sel(char tecla) {
    if (tecla >= '1' && tecla <= '9') {
        uint8_t idx = tecla - '1';
        if (idx < rfid_get_conteo()) {
            rfid_borrar(idx);
            mostrar_mensaje("Tarjeta eliminada", 2000, MENU_ACCESO);
        } else {
            mostrar_mensaje("Indice invalido", 2000, MENU_ACCESO);
        }
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

static void menu_procesar_ilum(char tecla) {
    if (tecla == 'A') { uint8_t n = ilum_nivel_get() + 10; if (n > 100) n = 100; iluminacion_dimerizar(n); menu_mostrar_ilum(); }
    else if (tecla == 'B') { uint8_t n = ilum_nivel_get(); if (n >= 10) n -= 10; else n = 0; iluminacion_dimerizar(n); menu_mostrar_ilum(); }
    else if (tecla == 'C') { iluminacion_dimerizar(0); menu_mostrar_ilum(); }
    else if (tecla == 'D') { menu_estado = MENU_AMBIENTE; menu_mostrar_ambiente(); }
    else if (tecla >= '0' && tecla <= '9') {
        static uint8_t tmp = 0;
        tmp = tmp * 10 + (tecla - '0');
        if (tmp > 100) tmp = tecla - '0';
        iluminacion_dimerizar(tmp);
        if (tecla == '0' || tecla == '1') tmp = 0;
        menu_mostrar_ilum();
    }
}

static void menu_procesar_temp(char tecla) {
    if (tecla == 'A') { temperatura_ajustar(1); menu_mostrar_temp(); }
    else if (tecla == 'B') { temperatura_ajustar(-1); menu_mostrar_temp(); }
    else if (tecla == 'D') { menu_estado = MENU_AMBIENTE; menu_mostrar_ambiente(); }
    else { menu_mostrar_temp(); }
}

static void menu_procesar_horno(char tecla) {
    if (tecla == 'A') { horno_encender(60, 180); menu_mostrar_horno(); }
    else if (tecla == 'B') { horno_apagar(); menu_mostrar_horno(); }
    else if (tecla == 'D') { menu_estado = MENU_AMBIENTE; menu_mostrar_ambiente(); }
    else { menu_mostrar_horno(); }
}

static void menu_procesar_sonido(char tecla) {
    uint8_t n;
    if (tecla == 'A') {
        n = sonido_nivel_get(); if (n <= 90) n += 10; else n = 100;
        menu_mostrar_sonido_valor(n);
        sonido_set_volumen(n);
    } else if (tecla == 'B') {
        n = sonido_nivel_get(); if (n >= 10) n -= 10; else n = 0;
        menu_mostrar_sonido_valor(n);
        sonido_set_volumen(n);
    } else if (tecla == 'C') {
        menu_mostrar_sonido_valor(0);
        sonido_set_volumen(0);
    } else if (tecla == 'D') { menu_estado = MENU_AMBIENTE; menu_mostrar_ambiente(); }
    else { menu_mostrar_sonido(); }
}

static void menu_procesar_juegos_recarga(void) {
    codigo_pendiente = false;
    uint16_t cod = entrada_a_codigo();
    if (alarma_verificar_codigo(cod)) {
        lcd_borrar(); lcd_imprimir("Acerca tarjeta...");
        menu_seleccion = 11;
        menu_estado = MENU_JUEG_RECARGAR;
    } else {
        mostrar_mensaje("Codigo incorrecto", 2000, MENU_JUEGOS);
    }
}

static void menu_procesar_recarga_accesos(void) {
    uint8_t accesos = 0;
    for (uint8_t i = 0; i < entrada_pos; i++) {
        accesos = accesos * 10 + entrada_digitos[i];
    }
    if (accesos > 100) accesos = 100;
    const uint8_t* uid = rfid_get_uid();
    if (juegos_cargar_accesos(uid, accesos)) {
        uint8_t saldo = juegos_consultar_saldo(uid);
        lcd_borrar(); lcd_imprimir("Recarga exitosa");
        lcd_posicion(1, 0); lcd_imprimir("Saldo: ");
        mostrar_numero(saldo); lcd_imprimir(" usos");
        menu_mensaje_hasta = millis() + 3000;
        menu_estado = MENU_MENSAJE;
        menu_volver_a = MENU_JUEGOS;
    } else {
        mostrar_mensaje("Error", 2000, MENU_JUEGOS);
    }
}

void menu_iniciar(void) {
    menu_estado = MENU_PRINCIPAL;
    menu_mostrar_principal();
}

static const uint8_t RFID_UID_VALIDO[4] = {0x01, 0x02, 0x03, 0x04};
static const uint8_t RFID_UID_INVALIDO[4] = {0xAB, 0xCD, 0xEF, 0x12};

void menu_procesar_tecla(char tecla) {
    if (menu_estado == MENU_MENSAJE) {
        return;
    }

    if (tecla == '*' && menu_estado != MENU_CODIGO_INPUT &&
        menu_estado != MENU_SEG_CAMBIAR_COD_NUEVO1 &&
        menu_estado != MENU_SEG_CAMBIAR_COD_NUEVO2) {
        if (menu_estado == MENU_ACC_ENROLAR) {
            rfid_simular_tarjeta(RFID_UID_VALIDO);
        } else {
            rfid_simular_tarjeta(RFID_UID_VALIDO);
        }
    }
    if (tecla == '#' && menu_estado != MENU_CODIGO_INPUT &&
        menu_estado != MENU_SEG_CAMBIAR_COD_NUEVO1 &&
        menu_estado != MENU_SEG_CAMBIAR_COD_NUEVO2) {
        rfid_simular_tarjeta(RFID_UID_INVALIDO);
    }

    if (menu_estado == MENU_CODIGO_INPUT) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) {
            codigo_pendiente = false;
            if (menu_seleccion >= 100) { menu_codigo_ingresado(); }
            else if (menu_seleccion >= 10) { menu_procesar_juegos_recarga(); }
            else { menu_codigo_ingresado(); }
        } else if (tecla == 'B') {
            if (menu_seleccion >= 103) { menu_estado = MENU_SEG_INCENDIO; menu_mostrar_seg_incendio(); }
            else if (menu_seleccion >= 101) { menu_estado = MENU_SEG_INTRUSION; menu_mostrar_seg_intrusion(); }
            else if (menu_seleccion >= 10) { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
            else { menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); }
        }
        return;
    }

    if (menu_estado == MENU_SEG_CAMBIAR_COD_NUEVO1) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) menu_procesar_cambio_nuevo1();
        else if (tecla == 'B') { menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); }
        return;
    }

    if (menu_estado == MENU_SEG_CAMBIAR_COD_NUEVO2) {
        if (menu_procesar_ingreso_codigo(tecla) && codigo_pendiente) menu_procesar_cambio_nuevo2();
        else if (tecla == 'B') { menu_estado = MENU_SEGURIDAD; menu_mostrar_seguridad(); }
        return;
    }

    if (menu_estado == MENU_ACC_ENROLAR) {
        if (rfid_tarjeta_nueva()) {
            if (rfid_enrolar(rfid_get_uid())) mostrar_mensaje("Persona enrolada", 2000, MENU_ACCESO);
            else mostrar_mensaje("Tarjeta ya existe", 2000, MENU_ACCESO);
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
                mostrar_mensaje("Acceso denegado", 2000, MENU_JUEGOS);
            } else {
                uint8_t saldo = juegos_consultar_saldo(uid);
                if (saldo == 0) {
                    mostrar_mensaje("Sin accesos. Pide a tus padres", 3000, MENU_JUEGOS);
                } else {
                    juegos_descontar_acceso(uid);
                    lcd_borrar(); lcd_imprimir("Bienvenido!");
                    lcd_posicion(1, 0); lcd_imprimir("Te quedan ");
                    mostrar_numero(saldo - 1); lcd_imprimir(" usos");
                    menu_volver_a = MENU_JUEGOS;
                    menu_mensaje_hasta = millis() + 3000;
                    menu_estado = MENU_MENSAJE;
                    puerta_abrir_principal();
                }
            }
        } else if (tecla == 'B') { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
        return;
    }

    if (menu_estado == MENU_JUEG_RECARGAR) {
        static bool esperando_tarjeta = true;
        if (esperando_tarjeta && rfid_tarjeta_nueva()) {
            if (!rfid_validar_uid(rfid_get_uid())) {
                mostrar_mensaje("Tarjeta no valida", 2000, MENU_JUEGOS);
            } else {
                lcd_borrar(); lcd_imprimir("Accesos a sumar:");
                lcd_posicion(1, 0);
                entrada_pos = 0;
                for (uint8_t i = 0; i < COD_DIGITOS; i++) entrada_digitos[i] = 0;
                esperando_tarjeta = false;
            }
        } else if (!esperando_tarjeta) {
            if (tecla >= '0' && tecla <= '9' && entrada_pos < COD_DIGITOS) {
                entrada_digitos[entrada_pos] = tecla - '0';
                lcd_dato(tecla);
                entrada_pos++;
            } else if (tecla == 'A' && entrada_pos > 0) {
                esperando_tarjeta = true;
                menu_procesar_recarga_accesos();
            } else if (tecla == 'B' || tecla == '#') {
                esperando_tarjeta = true;
                menu_estado = MENU_JUEGOS; menu_mostrar_juegos();
            }
        } else if (tecla == 'B') {
            menu_estado = MENU_JUEGOS; menu_mostrar_juegos();
        }
        return;
    }

    if (menu_estado == MENU_JUEG_CONSULTAR) {
        if (rfid_tarjeta_nueva()) {
            const uint8_t* uid = rfid_get_uid();
            if (!rfid_validar_uid(uid)) {
                mostrar_mensaje("Acceso denegado", 2000, MENU_JUEGOS);
            } else {
                uint8_t saldo = juegos_consultar_saldo(uid);
                lcd_borrar(); lcd_imprimir("Saldo: ");
                mostrar_numero(saldo); lcd_imprimir(" usos");
                menu_volver_a = MENU_JUEGOS;
                menu_mensaje_hasta = millis() + 3000;
                menu_estado = MENU_MENSAJE;
            }
        } else if (tecla == 'B') { menu_estado = MENU_JUEGOS; menu_mostrar_juegos(); }
        return;
    }

    switch (menu_estado) {
        case MENU_PRINCIPAL: menu_procesar_main(tecla); break;
        case MENU_SEGURIDAD: menu_procesar_seguridad(tecla); break;
        case MENU_SEG_INTRUSION: menu_procesar_seg_intrusion(tecla); break;
        case MENU_SEG_INCENDIO: menu_procesar_seg_incendio(tecla); break;
        case MENU_ACCESO: menu_procesar_acceso(tecla); break;
        case MENU_JUEGOS: menu_procesar_juegos(tecla); break;
        case MENU_AMBIENTE: menu_procesar_ambiente(tecla); break;
        case MENU_AMB_ILUM: menu_procesar_ilum(tecla); break;
        case MENU_AMB_TEMP: menu_procesar_temp(tecla); break;
        case MENU_AMB_HORNO: menu_procesar_horno(tecla); break;
        case MENU_AMB_SONIDO: menu_procesar_sonido(tecla); break;
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
    usart_transmit(alarma_intrusion_activa() ? '1' : '0');
    usart_print(PSTR(" INCENDIO="));
    usart_transmit(alarma_incendio_activa() ? '1' : '0');
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
        if (cant >= 100) usart_transmit('0' + cant / 100);
        if (cant >= 10) usart_transmit('0' + (cant / 10) % 10);
        usart_transmit('0' + cant % 10);
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
    char comando_orig[48];
    strncpy(comando_orig, linea, sizeof(comando_orig) - 1);
    comando_orig[sizeof(comando_orig) - 1] = '\0';

    char* tok = strtok(linea, " ");
    if (tok == NULL) return;

    if (strcmp_P(tok, PSTR("ALARMA")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        if (strcmp_P(tok, PSTR("INCENDIO")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            bool activar = (strcmp_P(tok, PSTR("ON")) == 0);
            bool desactivar = (strcmp_P(tok, PSTR("OFF")) == 0);
            if (!activar && !desactivar) { usart_respuesta_error(); return; }
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint16_t cod = parse_uint16(tok);
            if (alarma_verificar_codigo(cod)) {
                if (activar) { alarma_incendio_activar(); usart_respuesta_ok(PSTR("incendio activado")); }
                else { alarma_incendio_desactivar(); usart_respuesta_ok(PSTR("incendio desactivado")); }
            } else {
                usart_respuesta_ok(PSTR("codigo incorrecto"));
            }
        } else {
            bool activar = (strcmp_P(tok, PSTR("ON")) == 0);
            bool desactivar = (strcmp_P(tok, PSTR("OFF")) == 0);
            if (!activar && !desactivar) { usart_respuesta_error(); return; }
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint16_t cod = parse_uint16(tok);
            if (alarma_verificar_codigo(cod)) {
                if (activar) { alarma_intrusion_activar(); usart_respuesta_ok(PSTR("intrusion activada")); }
                else { alarma_intrusion_desactivar(); usart_respuesta_ok(PSTR("intrusion desactivada")); }
            } else {
                usart_respuesta_ok(PSTR("codigo incorrecto"));
            }
        }
    } else if (strcmp_P(tok, PSTR("HORNO")) == 0) {
        maestro_enviar_comando(comando_orig);
    } else if (strcmp_P(tok, PSTR("SONIDO")) == 0) {
        maestro_enviar_comando_sonido(comando_orig);
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
    } else if (strcmp_P(tok, PSTR("RFID")) == 0) {
        tok = strtok(NULL, " ");
        if (tok == NULL) { usart_respuesta_error(); return; }
        if (strcmp_P(tok, PSTR("ENROL")) == 0) {
            uint8_t uid[4];
            tok = strtok(NULL, " ");
            if (tok != NULL && strcmp_P(tok, PSTR("*")) == 0) {
                memcpy(uid, RFID_UID_VALIDO, 4);
            } else {
                rfid_generar_uid(uid);
            }
            if (rfid_enrolar(uid)) {
                usart_print(PSTR("OK: enrolado idx="));
                uint8_t cnt = rfid_get_conteo();
                usart_transmit('0' + cnt - 1);
                usart_transmit('\n');
            } else {
                usart_respuesta_ok(PSTR("error al enrolar"));
            }
        } else if (strcmp_P(tok, PSTR("LISTA")) == 0) {
            uint8_t cnt = rfid_get_conteo();
            usart_print(PSTR("OK: "));
            static const char hex[] = "0123456789ABCDEF";
            for (uint8_t i = 0; i < cnt; i++) {
                if (i > 0) usart_transmit(' ');
                uint8_t uid[4];
                lista_leer_uid(i, uid);
                usart_transmit(hex[uid[0] >> 4]);
                usart_transmit(hex[uid[0] & 0x0F]);
                usart_transmit(':');
                usart_transmit(hex[uid[1] >> 4]);
                usart_transmit(hex[uid[1] & 0x0F]);
                usart_transmit(':');
                usart_transmit(hex[uid[2] >> 4]);
                usart_transmit(hex[uid[2] & 0x0F]);
                usart_transmit(':');
                usart_transmit(hex[uid[3] >> 4]);
                usart_transmit(hex[uid[3] & 0x0F]);
            }
            usart_transmit('\n');
        } else if (strcmp_P(tok, PSTR("BORRAR")) == 0) {
            tok = strtok(NULL, " ");
            if (tok == NULL) { usart_respuesta_error(); return; }
            uint8_t idx = (uint8_t)parse_uint16(tok);
            if (rfid_borrar(idx)) {
                usart_respuesta_ok(PSTR("borrado"));
            } else {
                usart_respuesta_ok(PSTR("indice invalido"));
            }
        } else {
            usart_respuesta_error();
        }
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

static void maestro_enviar_comando(const char* cmd) {
    usart1_puts(cmd);
    usart1_transmit('\n');
    maestro_estado = MAESTRO_ESPERANDO_HORNO;
    maestro_tiempo_envio = millis();
    maestro_resp_pos = 0;
}

static void maestro_enviar_comando_sonido(const char* cmd) {
    usart1_puts(cmd);
    usart1_transmit('\n');
    maestro_estado = MAESTRO_ESPERANDO_SONIDO;
    maestro_tiempo_envio = millis();
    maestro_resp_pos = 0;
}

static void maestro_procesar_respuesta(void) {
    if (maestro_estado != MAESTRO_ESPERANDO_HORNO &&
        maestro_estado != MAESTRO_ESPERANDO_SONIDO) return;

    // 1. Leer bytes disponibles primero
    while (usart1_disponible() > 0) {
        char c = usart1_leer();
        if (c == '\n' || c == '\r') {
            if (maestro_resp_pos > 0) {
                maestro_respuesta[maestro_resp_pos] = '\0';
                if (maestro_estado == MAESTRO_ESPERANDO_HORNO) {
                    usart_print(PSTR("OK: horno="));
                } else {
                    usart_print(PSTR("OK: sonido="));
                }
                usart_puts(maestro_respuesta);
                usart_transmit('\n');
                maestro_estado = MAESTRO_OCIOSO;
            }
        } else if (maestro_resp_pos < sizeof(maestro_respuesta) - 1) {
            maestro_respuesta[maestro_resp_pos++] = c;
        }
    }

    // 2. Timeout 1000ms
    if (maestro_estado != MAESTRO_OCIOSO && millis() - maestro_tiempo_envio >= 1000) {
        if (maestro_resp_pos > 0) {
            maestro_respuesta[maestro_resp_pos] = '\0';
            if (maestro_estado == MAESTRO_ESPERANDO_HORNO) {
                usart_print(PSTR("OK: horno="));
            } else {
                usart_print(PSTR("OK: sonido="));
            }
            usart_puts(maestro_respuesta);
            usart_transmit('\n');
        } else {
            if (maestro_estado == MAESTRO_ESPERANDO_HORNO) {
                usart_respuesta_ok(PSTR("ERROR: timeout horno"));
            } else {
                usart_respuesta_ok(PSTR("ERROR: timeout sonido"));
            }
        }
        maestro_estado = MAESTRO_OCIOSO;
    }
}

void setup() {
    lcd_init();
    teclado_init();
    usart_init();
    usart1_init();
    usart2_init();
    usart3_init();
    timer_init();
    spi_master_init();
    alarma_init();
    rfid_init();
    iluminacion_iniciar();
    temperatura_iniciar();
    lista_actualizar();
    menu_iniciar();
}

static void menu_volver(void) {
    switch (menu_volver_a) {
        case MENU_SEGURIDAD: menu_mostrar_seguridad(); break;
        case MENU_SEG_INTRUSION: menu_mostrar_seg_intrusion(); break;
        case MENU_SEG_INCENDIO: menu_mostrar_seg_incendio(); break;
        case MENU_ACCESO: menu_mostrar_acceso(); break;
        case MENU_JUEGOS: menu_mostrar_juegos(); break;
        case MENU_AMBIENTE: menu_mostrar_ambiente(); break;
        case MENU_AMB_ILUM: menu_mostrar_ilum(); break;
        case MENU_AMB_TEMP: menu_mostrar_temp(); break;
        case MENU_AMB_HORNO: menu_mostrar_horno(); break;
        case MENU_AMB_SONIDO: menu_mostrar_sonido(); break;
        default: menu_mostrar_principal(); break;
    }
}

void loop() {
    unsigned long ahora = millis();

    if (menu_estado == MENU_MENSAJE && ahora >= menu_mensaje_hasta) {
        menu_estado = menu_volver_a;
        menu_volver();
    }

    procesar_comandos_usart();

    alarma_buzzer_sonar();

    static unsigned long ultimo_tick = 0;
    if (ahora - ultimo_tick >= 50) {
        ultimo_tick = ahora;

        static char ultima_tecla = 0;
        char tecla = teclado_scan();
        if (tecla != 0 && tecla != ultima_tecla) {
            menu_procesar_tecla(tecla);
        }
        ultima_tecla = tecla;

        alarma_actualizar();
        rfid_actualizar();
        ambiente_actualizar();
        ambiente_slave_procesar();
        sonido_slave_procesar();
        maestro_procesar_respuesta();
    }
}
