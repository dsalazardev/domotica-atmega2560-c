## ADDED Requirements

### Requirement: Inicializar LCD en modo 4 bits
El sistema SHALL inicializar el LCD 16x2 en modo 4 bits siguiendo la secuencia del datasheet ST7066: 3 fases 8-bit → comando 0x28 → display on → entry mode → clear.

#### Scenario: Inicialización correcta
- **WHEN** se ejecuta `lcd_init()` en setup()
- **THEN** el LCD queda listo para recibir comandos y datos en modo 4 bits, 2 líneas, 5x8

#### Scenario: Tiempos de espera cumplidos
- **WHEN** se ejecuta `lcd_init()`
- **THEN** se esperan >40ms tras VDD, >5ms tras primer nibble, >1ms tras segundo nibble, >2ms tras clear

### Requirement: Enviar comando al LCD
El sistema SHALL enviar comandos de control al LCD (RS=0) a través de `lcd_comando(uint8_t cmd)`.

#### Scenario: Comando enviado correctamente
- **WHEN** se llama `lcd_comando(0x01)` (clear display)
- **THEN** el LCD ejecuta el comando y la pantalla se limpia

### Requirement: Enviar dato al LCD
El sistema SHALL enviar caracteres a mostrar al LCD (RS=1) a través de `lcd_dato(uint8_t dato)`.

#### Scenario: Carácter mostrado
- **WHEN** se llama `lcd_dato('A')`
- **THEN** el carácter 'A' aparece en la posición actual del cursor

### Requirement: Posicionar cursor
El sistema SHALL posicionar el cursor en una fila y columna específicas mediante `lcd_posicion(uint8_t fila, uint8_t col)`.

#### Scenario: Cursor en fila 0 columna 0
- **WHEN** se llama `lcd_posicion(0, 0)`
- **THEN** el cursor se mueve al inicio de la primera línea

#### Scenario: Cursor en fila 1 columna 5
- **WHEN** se llama `lcd_posicion(1, 5)`
- **THEN** el cursor se mueve a la columna 5 de la segunda línea

### Requirement: Borrar pantalla
El sistema SHALL borrar toda la pantalla y retornar el cursor a la posición inicial mediante `lcd_borrar()`.

#### Scenario: Pantalla limpiada
- **WHEN** se llama `lcd_borrar()`
- **THEN** la pantalla se limpia y el cursor vuelve a (0,0)

### Requirement: Imprimir cadena de texto
El sistema SHALL imprimir una cadena terminada en null en la posición actual del cursor mediante `lcd_imprimir(char* texto)`.

#### Scenario: Texto corto mostrado
- **WHEN** se llama `lcd_imprimir("Hola")`
- **THEN** el LCD muestra "Hola" en la posición actual del cursor

#### Scenario: Texto de 16 caracteres (línea completa)
- **WHEN** se llama `lcd_imprimir("0123456789ABCDEF")`
- **THEN** el LCD muestra los 16 caracteres en la línea actual

### Requirement: Enviar nibble al LCD
El sistema SHALL enviar 4 bits (un nibble) al LCD a través de PA4-PA7 y generar un pulso enable en PA1 mediante `lcd_nibble(uint8_t dato)`.

#### Scenario: Nibble con pulso enable
- **WHEN** se llama `lcd_nibble(0x0A)`
- **THEN** los bits 0-3 del parámetro se colocan en PA4-PA7 y se genera un pulso enable (flanco descendente) en PA1

### Requirement: Prueba de integración en main.ino
El sistema SHALL inicializar el LCD en setup() y mostrar un mensaje "Hola Mundo!" en la línea 1.

#### Scenario: Mensaje de bienvenida
- **WHEN** el programa inicia
- **THEN** el LCD muestra "Hola Mundo!" en la fila 0

#### Scenario: Compilación exitosa
- **WHEN** se ejecuta `pio run`
- **THEN** la compilación termina sin errores ni warnings
