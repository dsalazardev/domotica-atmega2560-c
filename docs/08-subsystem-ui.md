# Subsistema: Interfaz de Usuario — LCD y Teclado

## Reglas Asociadas
- **R18**: Toda la información al usuario se muestra por LCD

## Hardware
- LCD 16x2 (Vishay LCD-016N002B-CFH-ET, controlador ST7066)
- Teclado matricial 4x4
- Conexión LCD en modo 4 bits: ahorra 4 pines del microcontrolador
- Teclado sin Keypad.h: escaneo manual round-robin con millis(), debounce por muestreo
- LCD sin delays: temporización non-blocking con NOPs + millis() + micros()

## Árbol de Menús

```
Pantalla de Inicio (bienvenida / estado)
│
├── 1. Seguridad
│   ├── 1.1 Activar Alarma        (pedir código)
│   ├── 1.2 Desactivar Alarma     (pedir código)
│   └── 1.3 Estado Alarmas        (mostrar estado actual)
│
├── 2. RFID
│   ├── 2.1 Enrolar Persona       (pedir código admin)
│   └── 2.2 Borrar Persona        (pedir código admin)
│
├── 3. Habitación Juegos
│   ├── 3.1 Recargar Accesos      (pedir código admin)
│   └── 3.2 Estado Tarjeta        (leer y mostrar saldo)
│
├── 4. Iluminación
│   (ajuste de dimerización: + / -)
│
├── 5. Temperatura
│   ├── 5.1 Temp. Actual
│   └── 5.2 Set Point             (ajustar temperatura deseada)
│
├── 6. Horno
│   (estado: apagado/encendido + tiempo restante)
│
├── 7. Sonido
│   (estado: apagado/encendido + volumen)
│
├── 8. Lista de Mercado
│   ├── 8.1 Agregar Item
│   ├── 8.2 Ver Lista
│   └── 8.3 Limpiar Lista
│
└── 9. Configuración
    └── 9.1 Cambiar Código Admin
```

## Mapeo de Teclado Matricial 4x4

La matriz de teclas sigue el patrón de los códigos de clase. El proyecto implementa su propio escaneo sin `Keypad.h` ni delays, usando round-robin con temporización por `millis()`:

```
    Col0  Col1  Col2  Col3
F0:  '1'   '2'   '3'   'A'    ← Navegación / Confirmar
F1:  '4'   '5'   '6'   'B'    ← Navegación / Volver
F2:  '7'   '8'   '9'   'C'    ← Navegación / Cancelar
F3:  '*'   '0'   '#'   'D'    ← Funciones especiales
```

### Acciones por tecla (propuesta)
- **'0'-'9'**: Ingreso numérico / selección de menú
- **'A'**: Enter / Confirmar (tecla de acción, pasa a siguiente pantalla)
- **'B'**: Atrás / Cancelar (vuelve al menú anterior)
- **'C'**: Salir (vuelve a pantalla principal)
- **'D'**: Opción especial (depende del contexto, ej: borrar lista)
- **'*'** : Borrar (retroceso en ingreso de texto)
- **'#'** : Siguiente (cambiar campo, avanzar cursor)

## Inicialización LCD (Modo 4 bits)

```
1. Esperar >40ms tras VDD = 4.5V
2. Enviar 0x03 (nibble)           → init 8-bit mode
3. Esperar >5ms
4. Enviar 0x03 (nibble)           → confirm 8-bit
5. Esperar >1ms
6. Enviar 0x03 (nibble)           → confirm 8-bit
7. Enviar 0x02 (nibble)           → cambiar a 4-bit mode
8. Enviar comando 0x28            → Function Set: 4-bit, 2 líneas, 5x8
9. Enviar comando 0x0C            → Display ON, cursor OFF, blink OFF
10. Enviar comando 0x06           → Entry Mode: incrementar, no shift
11. Enviar comando 0x01           → Clear display
12. Esperar >2ms
```

**Nota**: Todas las esperas de la secuencia de inicialización se implementan con `millis()` (≥1ms) o NOP loops (<1ms), no con `_delay_ms()` / `_delay_us()`.

## Funciones del Driver LCD

| Función | Propósito |
|---|---|
| `lcd_init()` | Inicializar LCD en modo 4 bits |
| `lcd_comando(uint8_t cmd)` | Enviar instrucción de control |
| `lcd_dato(uint8_t dato)` | Enviar carácter a mostrar |
| `lcd_nibble(uint8_t dato)` | Enviar 4 bits + pulso enable |
| `lcd_posicion(uint8_t fila, uint8_t col)` | Posicionar cursor |
| `lcd_borrar()` | Limpiar pantalla |
| `lcd_imprimir(char* texto)` | Imprimir cadena |
