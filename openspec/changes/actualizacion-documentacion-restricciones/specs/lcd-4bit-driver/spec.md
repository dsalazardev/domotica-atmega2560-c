## MODIFIED Requirements

### Requirement: Inicializar LCD en modo 4 bits
El sistema SHALL inicializar el LCD 16x2 en modo 4 bits siguiendo la secuencia del datasheet ST7066: 3 fases 8-bit → comando 0x28 → display on → entry mode → clear. Todas las esperas de temporización DEBEN ser no-bloqueantes (sin `_delay_ms()` ni `_delay_us()`).

#### Scenario: Inicialización correcta
- **WHEN** se ejecuta `lcd_init()` en setup()
- **THEN** el LCD queda listo para recibir comandos y datos en modo 4 bits, 2 líneas, 5x8

#### Scenario: Tiempos de espera sin delays
- **WHEN** se ejecuta `lcd_init()`
- **THEN** las esperas se realizan con millis(), micros() o NOPs, sin usar _delay_*()

### Requirement: Enviar comando al LCD
El sistema SHALL enviar comandos de control al LCD (RS=0) a través de `lcd_comando(uint8_t cmd)`, con temporización no-bloqueante.

#### Scenario: Comando enviado correctamente
- **WHEN** se llama `lcd_comando(0x01)` (clear display)
- **THEN** el LCD ejecuta el comando sin usar _delay_ms()

### Requirement: Enviar nibble al LCD
El sistema SHALL enviar 4 bits al LCD a través de PA4-PA7 y generar un pulso enable en PA1 mediante `lcd_nibble(uint8_t dato)`. El pulso enable se genera con temporización por NOPs (sin _delay_us()).

#### Scenario: Nibble con pulso enable NOP-based
- **WHEN** se llama `lcd_nibble(0x0A)`
- **THEN** los bits 0-3 se colocan en PA4-PA7 y se genera un pulso enable mediante NOPs
