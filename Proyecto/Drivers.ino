#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#ifndef UID_LEN
#define UID_LEN 4
#endif

#define USART_RX_BUF_SIZE 64
static volatile uint8_t usart_rx_buf[USART_RX_BUF_SIZE];
static volatile uint8_t usart_rx_head = 0;
static volatile uint8_t usart_rx_tail = 0;

#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_SET 0x28

static uint8_t lcd_fila = 0;
static uint8_t lcd_columna = 0;

static void lcd_esperar_ms(uint16_t ms) {
    unsigned long t = millis();
    while (millis() - t < ms);
}

static void lcd_nop_us(void) {
    for (uint16_t i = 0; i < 200; i++) {
        __asm__ __volatile__("nop\n\t");
    }
}

static void lcd_enviar4Bits(uint8_t valor) {
    digitalWrite(LCD_D4, bitRead(valor, 0));
    digitalWrite(LCD_D5, bitRead(valor, 1));
    digitalWrite(LCD_D6, bitRead(valor, 2));
    digitalWrite(LCD_D7, bitRead(valor, 3));
    digitalWrite(LCD_EN, HIGH);
    lcd_nop_us();
    digitalWrite(LCD_EN, LOW);
    lcd_esperar_ms(1);
}

void lcd_comando(uint8_t cmd) {
    digitalWrite(LCD_RS, LOW);
    lcd_enviar4Bits(cmd >> 4);
    lcd_enviar4Bits(cmd);
    lcd_esperar_ms(2);
}

void lcd_dato(uint8_t dato) {
    digitalWrite(LCD_RS, HIGH);
    lcd_enviar4Bits(dato >> 4);
    lcd_enviar4Bits(dato);
    lcd_esperar_ms(2);
}

void lcd_init(void) {
    pinMode(LCD_RS, OUTPUT); pinMode(LCD_EN, OUTPUT);
    pinMode(LCD_D4, OUTPUT); pinMode(LCD_D5, OUTPUT);
    pinMode(LCD_D6, OUTPUT); pinMode(LCD_D7, OUTPUT);

    lcd_esperar_ms(20);

    lcd_enviar4Bits(0x03); lcd_esperar_ms(5);
    lcd_enviar4Bits(0x03); lcd_esperar_ms(1);
    lcd_enviar4Bits(0x03);
    lcd_enviar4Bits(0x02);

    lcd_comando(0x28);
    lcd_comando(0x0C);
    lcd_comando(0x01);
    lcd_esperar_ms(2);

    lcd_fila = 0;
    lcd_columna = 0;
}

void lcd_posicion(uint8_t fila, uint8_t col) {
    if (fila == 0) {
        lcd_comando(0x80 + col);
    } else {
        lcd_comando(0xC0 + col);
    }
}

void lcd_borrar(void) {
    lcd_comando(LCD_CLEAR);
    lcd_esperar_ms(2);
    lcd_fila = 0;
    lcd_columna = 0;
    lcd_posicion(lcd_fila, lcd_columna);
}

void lcd_imprimir(const char* texto) {
    while (*texto) {
        lcd_dato(*texto);
        texto++;
    }
}

void lcd_salto_linea(void) {
    if (lcd_fila == 0) {
        lcd_fila = 1;
        lcd_columna = 0;
        lcd_posicion(lcd_fila, lcd_columna);
    } else {
        lcd_borrar();
    }
}

void lcd_procesar(char dato) {
    if (dato == '\r' || dato == '\n') {
        lcd_salto_linea();
        return;
    }

    if (dato == '\f') {
        lcd_borrar();
        return;
    }

    lcd_dato(dato);
    lcd_columna++;

    if (lcd_columna >= 16) {
        lcd_salto_linea();
    }
}

#define KEY_ROW_PORT PORTL
#define KEY_ROW_DDR  DDRL
#define KEY_COL_PORT PORTK
#define KEY_COL_DDR  DDRK
#define KEY_COL_PIN  PINK

static const char teclado_mapa[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void teclado_init(void) {
    KEY_ROW_DDR |= 0x0F;
    KEY_ROW_PORT = (KEY_ROW_PORT & 0xF0) | 0x0F;
    KEY_COL_DDR &= ~0x0F;
    KEY_COL_PORT |= 0x0F;
}

char teclado_scan(void) {
    static unsigned long ultimo_scan = 0;
    if (millis() - ultimo_scan < 50) return 0;
    ultimo_scan = millis();

    for (uint8_t f = 0; f < 4; f++) {
        KEY_ROW_PORT = (KEY_ROW_PORT & 0xF0) | (~(1 << f) & 0x0F);
        __asm__ __volatile__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
                             "nop\n\t" "nop\n\t");
        uint8_t col_bits = KEY_COL_PIN & 0x0F;
        for (uint8_t c = 0; c < 4; c++) {
            if (!(col_bits & (1 << c))) {
                char tecla = teclado_mapa[f][c];
                KEY_ROW_PORT = (KEY_ROW_PORT & 0xF0) | 0x0F;
                return tecla;
            }
        }
    }
    KEY_ROW_PORT = (KEY_ROW_PORT & 0xF0) | 0x0F;
    return 0;
}

#define SPI_DDR   DDRB
#define SPI_PORT  PORTB
#define SPI_SS    PB0
#define SPI_SCK   PB1
#define SPI_MOSI  PB2
#define SPI_MISO  PB3

void spi_master_init(void) {
    SPI_DDR |= (1 << SPI_SS) | (1 << SPI_SCK) | (1 << SPI_MOSI);
    SPI_DDR &= ~(1 << SPI_MISO);
    SPI_PORT |= (1 << SPI_SS);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_master_transmit(uint8_t data) {
    SPI_PORT &= ~(1 << SPI_SS);
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    SPI_PORT |= (1 << SPI_SS);
    return SPDR;
}

uint8_t spi_master_receive(void) {
    return spi_master_transmit(0x00);
}

#define MFRC522_CS_LOW()   (SPI_PORT &= ~(1 << SPI_SS))
#define MFRC522_CS_HIGH()  (SPI_PORT |= (1 << SPI_SS))

#define MFRC522_CMD_REG    0x01
#define MFRC522_COMIRQ_REG 0x04
#define MFRC522_ERR_REG    0x06
#define MFRC522_FIFO_REG   0x09
#define MFRC522_FIFOLEV_REG 0x0A
#define MFRC522_BITFRAM_REG 0x0D
#define MFRC522_TXCTRL_REG 0x14
#define MFRC522_TXASK_REG  0x15
#define MFRC522_MODE_REG   0x11
#define MFRC522_TMODE_REG  0x2A
#define MFRC522_TPRESC_REG 0x2B
#define MFRC522_TRELOADH   0x2C
#define MFRC522_TRELOADL   0x2D

#define MFRC522_IDLE       0x00
#define MFRC522_TRANSCEIVE 0x0C
#define MFRC522_SOFTRESET  0x0F

#define PICC_REQIDL    0x26
#define PICC_ANTICOLL1 0x93

static void mfrc522_write_reg(uint8_t addr, uint8_t val) {
    MFRC522_CS_LOW();
    SPDR = (addr << 1) & 0x7E;
    while (!(SPSR & (1 << SPIF)));
    SPDR = val;
    while (!(SPSR & (1 << SPIF)));
    MFRC522_CS_HIGH();
}

static uint8_t mfrc522_read_reg(uint8_t addr) {
    MFRC522_CS_LOW();
    SPDR = (addr << 1) | 0x80;
    while (!(SPSR & (1 << SPIF)));
    SPDR = 0x00;
    while (!(SPSR & (1 << SPIF)));
    uint8_t val = SPDR;
    MFRC522_CS_HIGH();
    return val;
}

static void mfrc522_set_bit(uint8_t addr, uint8_t mask) {
    mfrc522_write_reg(addr, mfrc522_read_reg(addr) | mask);
}

static void mfrc522_clear_bit(uint8_t addr, uint8_t mask) {
    mfrc522_write_reg(addr, mfrc522_read_reg(addr) & ~mask);
}

static void mfrc522_init(void) {
    mfrc522_write_reg(MFRC522_CMD_REG, MFRC522_SOFTRESET);
    unsigned long t = millis();
    while (millis() - t < 10);

    mfrc522_write_reg(MFRC522_TMODE_REG, 0x80);
    mfrc522_write_reg(MFRC522_TPRESC_REG, 0xA9);
    mfrc522_write_reg(MFRC522_TRELOADH, 0x03);
    mfrc522_write_reg(MFRC522_TRELOADL, 0xE8);

    mfrc522_write_reg(MFRC522_TXASK_REG, 0x40);
    mfrc522_write_reg(MFRC522_MODE_REG, 0x3D);

    mfrc522_set_bit(MFRC522_TXCTRL_REG, 0x03);
}

static bool mfrc522_request(void) {
    mfrc522_write_reg(MFRC522_BITFRAM_REG, 0x07);
    mfrc522_write_reg(MFRC522_FIFO_REG, PICC_REQIDL);
    mfrc522_write_reg(MFRC522_CMD_REG, MFRC522_TRANSCEIVE);
    mfrc522_set_bit(MFRC522_BITFRAM_REG, 0x80);

    unsigned long t = millis();
    while (millis() - t < 20) {
        uint8_t irq = mfrc522_read_reg(MFRC522_COMIRQ_REG);
        if (irq & 0x20) break;
        if (irq & 0x01) break;
    }

    mfrc522_clear_bit(MFRC522_BITFRAM_REG, 0x80);
    uint8_t err = mfrc522_read_reg(MFRC522_ERR_REG);
    if (err & 0x02) return false;

    uint8_t level = mfrc522_read_reg(MFRC522_FIFOLEV_REG);
    return (level >= 2);
}

static bool mfrc522_anticoll(uint8_t *uid) {
    mfrc522_write_reg(MFRC522_BITFRAM_REG, 0x00);
    mfrc522_write_reg(MFRC522_FIFOLEV_REG, 0x80);

    mfrc522_write_reg(MFRC522_FIFO_REG, PICC_ANTICOLL1);
    mfrc522_write_reg(MFRC522_FIFO_REG, 0x20);

    mfrc522_write_reg(MFRC522_CMD_REG, MFRC522_TRANSCEIVE);
    mfrc522_set_bit(MFRC522_BITFRAM_REG, 0x80);

    unsigned long t = millis();
    while (millis() - t < 20) {
        uint8_t irq = mfrc522_read_reg(MFRC522_COMIRQ_REG);
        if (irq & 0x20) break;
        if (irq & 0x01) break;
    }

    mfrc522_clear_bit(MFRC522_BITFRAM_REG, 0x80);
    uint8_t err = mfrc522_read_reg(MFRC522_ERR_REG);
    if (err & 0x02) return false;

    uint8_t level = mfrc522_read_reg(MFRC522_FIFOLEV_REG);
    if (level != 5) return false;

    for (uint8_t i = 0; i < 5; i++) {
        uid[i] = mfrc522_read_reg(MFRC522_FIFO_REG);
    }

    uint8_t check = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
    return (check == uid[4]);
}

bool mfrc522_leer_uid(uint8_t *uid_out) {
    if (!mfrc522_request()) return false;
    uint8_t buf[5];
    if (!mfrc522_anticoll(buf)) return false;
    for (uint8_t i = 0; i < UID_LEN; i++) {
        uid_out[i] = buf[i];
    }
    return true;
}

void usart_init(void) {
    UBRR0H = 0;
    UBRR0L = 104;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t usart_leer(void) {
    uint8_t c = 0;
    cli();
    if (usart_rx_head != usart_rx_tail) {
        c = usart_rx_buf[usart_rx_tail];
        usart_rx_tail = (usart_rx_tail + 1) % USART_RX_BUF_SIZE;
    }
    sei();
    return c;
}

uint8_t usart_disponible(void) {
    uint8_t disp;
    cli();
    disp = (usart_rx_head - usart_rx_tail) % USART_RX_BUF_SIZE;
    sei();
    return disp;
}

ISR(USART0_RX_vect) {
    uint8_t c = UDR0;
    uint8_t next = (usart_rx_head + 1) % USART_RX_BUF_SIZE;
    if (next != usart_rx_tail) {
        usart_rx_buf[usart_rx_head] = c;
        usart_rx_head = next;
    }
}

#define USART1_RX_BUF_SIZE 64
static volatile uint8_t usart1_rx_buf[USART1_RX_BUF_SIZE];
static volatile uint8_t usart1_rx_head = 0;
static volatile uint8_t usart1_rx_tail = 0;

void usart1_init(void) {
    UBRR1H = 0;
    UBRR1L = 104;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
    PORTD |= (1 << PD2);
}

void usart1_transmit(uint8_t data) {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = data;
}

uint8_t usart1_leer(void) {
    uint8_t c = 0;
    cli();
    if (usart1_rx_head != usart1_rx_tail) {
        c = usart1_rx_buf[usart1_rx_tail];
        usart1_rx_tail = (usart1_rx_tail + 1) % USART1_RX_BUF_SIZE;
    }
    sei();
    return c;
}

uint8_t usart1_disponible(void) {
    uint8_t disp;
    cli();
    disp = (usart1_rx_head - usart1_rx_tail) % USART1_RX_BUF_SIZE;
    sei();
    return disp;
}

void usart1_print(const char* s) {
    while (pgm_read_byte(s)) {
        usart1_transmit(pgm_read_byte(s++));
    }
}

void usart1_puts(const char* s) {
    while (*s) usart1_transmit(*s++);
}

ISR(USART1_RX_vect) {
    uint8_t c = UDR1;
    uint8_t next = (usart1_rx_head + 1) % USART1_RX_BUF_SIZE;
    if (next != usart1_rx_tail) {
        usart1_rx_buf[usart1_rx_head] = c;
        usart1_rx_head = next;
    }
}

#define USART2_RX_BUF_SIZE 64
static volatile uint8_t usart2_rx_buf[USART2_RX_BUF_SIZE];
static volatile uint8_t usart2_rx_head = 0;
static volatile uint8_t usart2_rx_tail = 0;

void usart2_init(void) {
    UBRR2H = 0;
    UBRR2L = 104;
    UCSR2B = (1 << RXEN2) | (1 << RXCIE2);
    UCSR2C = (1 << UCSZ21) | (1 << UCSZ20);
    DDRH &= ~(1 << PH1);
}

void usart2_transmit(uint8_t data) {
    while (!(UCSR2A & (1 << UDRE2)));
    UDR2 = data;
}

uint8_t usart2_leer(void) {
    uint8_t c = 0;
    cli();
    if (usart2_rx_head != usart2_rx_tail) {
        c = usart2_rx_buf[usart2_rx_tail];
        usart2_rx_tail = (usart2_rx_tail + 1) % USART2_RX_BUF_SIZE;
    }
    sei();
    return c;
}

uint8_t usart2_disponible(void) {
    uint8_t disp;
    cli();
    disp = (usart2_rx_head - usart2_rx_tail) % USART2_RX_BUF_SIZE;
    sei();
    return disp;
}

void usart2_print(const char* s) {
    while (pgm_read_byte(s)) {
        usart2_transmit(pgm_read_byte(s++));
    }
}

void usart2_puts(const char* s) {
    while (*s) usart2_transmit(*s++);
}

ISR(USART2_RX_vect) {
    uint8_t c = UDR2;
    uint8_t next = (usart2_rx_head + 1) % USART2_RX_BUF_SIZE;
    if (next != usart2_rx_tail) {
        usart2_rx_buf[usart2_rx_head] = c;
        usart2_rx_head = next;
    }
}

void usart_puts(const char* s) {
    while (*s) usart_transmit(*s++);
}

void usart_print(const char* s) {
    while (pgm_read_byte(s)) {
        usart_transmit(pgm_read_byte(s++));
    }
}

void usart_println(const char* s) {
    usart_print(s);
    usart_transmit('\n');
}

#define USART3_RX_BUF_SIZE 64
static volatile uint8_t usart3_rx_buf[USART3_RX_BUF_SIZE];
static volatile uint8_t usart3_rx_head = 0;
static volatile uint8_t usart3_rx_tail = 0;

void usart3_init(void) {
    UBRR3H = 0;
    UBRR3L = 104;
    UCSR3B = (1 << RXEN3) | (1 << RXCIE3);
    UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
    DDRJ &= ~(1 << PJ1);
}

void usart3_transmit(uint8_t data) {
    while (!(UCSR3A & (1 << UDRE3)));
    UDR3 = data;
}

uint8_t usart3_leer(void) {
    uint8_t c = 0;
    cli();
    if (usart3_rx_head != usart3_rx_tail) {
        c = usart3_rx_buf[usart3_rx_tail];
        usart3_rx_tail = (usart3_rx_tail + 1) % USART3_RX_BUF_SIZE;
    }
    sei();
    return c;
}

uint8_t usart3_disponible(void) {
    uint8_t disp;
    cli();
    disp = (usart3_rx_head - usart3_rx_tail) % USART3_RX_BUF_SIZE;
    sei();
    return disp;
}

void usart3_puts(const char* s) {
    while (*s) usart3_transmit(*s++);
}

void usart3_print(const char* s) {
    while (pgm_read_byte(s)) {
        usart3_transmit(pgm_read_byte(s++));
    }
}

ISR(USART3_RX_vect) {
    uint8_t c = UDR3;
    uint8_t next = (usart3_rx_head + 1) % USART3_RX_BUF_SIZE;
    if (next != usart3_rx_tail) {
        usart3_rx_buf[usart3_rx_head] = c;
        usart3_rx_head = next;
    }
}
