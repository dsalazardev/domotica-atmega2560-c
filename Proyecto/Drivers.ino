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

#define LCD_RS PA0
#define LCD_EN PA1
#define LCD_D4 PA4
#define LCD_D5 PA5
#define LCD_D6 PA6
#define LCD_D7 PA7

#define LCD_DDR DDRA
#define LCD_PORT PORTA

#define LCD_DATA_MASK 0xF0
#define LCD_RS_MASK (1 << LCD_RS)
#define LCD_EN_MASK (1 << LCD_EN)
#define LCD_MASK (LCD_DATA_MASK | LCD_RS_MASK | LCD_EN_MASK)

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_SET 0x28

static uint8_t lcd_fila = 0;
static uint8_t lcd_columna = 0;
static unsigned long lcd_ultimo_comando = 0;

static void lcd_esperar(void) {
    unsigned long inicio = millis();
    while (millis() - inicio < 2);
}

static void lcd_pulso(void) {
    LCD_PORT |= LCD_EN_MASK;
    __asm__ __volatile__(
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
    );
    LCD_PORT &= ~LCD_EN_MASK;
}

static void lcd_nibble(uint8_t dato) {
    LCD_PORT = (LCD_PORT & 0x0F) | ((dato & 0x0F) << 4);
    lcd_pulso();
}

void lcd_comando(uint8_t cmd) {
    lcd_esperar();
    LCD_PORT &= ~LCD_RS_MASK;
    lcd_nibble(cmd >> 4);
    lcd_nibble(cmd);
    lcd_ultimo_comando = millis();
}

void lcd_dato(uint8_t dato) {
    lcd_esperar();
    LCD_PORT |= LCD_RS_MASK;
    lcd_nibble(dato >> 4);
    lcd_nibble(dato);
    lcd_ultimo_comando = millis();
}

void lcd_init(void) {
    LCD_DDR |= LCD_MASK;
    LCD_PORT &= ~LCD_MASK;

    unsigned long t = millis();
    while (millis() - t < 20);

    lcd_nibble(0x03);
    t = millis();
    while (millis() - t < 5);

    lcd_nibble(0x03);
    t = millis();
    while (millis() - t < 1);

    lcd_nibble(0x03);
    lcd_nibble(0x02);

    lcd_comando(LCD_FUNCTION_SET);
    lcd_comando(LCD_DISPLAY_ON);
    lcd_comando(LCD_ENTRY_MODE);
    lcd_borrar();
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
    lcd_esperar();
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

#define KEY_PORT PORTK
#define KEY_DDR  DDRK
#define KEY_PIN  PINK

static const char teclado_mapa[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void teclado_init(void) {
    KEY_DDR  = 0x0F;
    KEY_PORT = 0xFF;
}

char teclado_scan(void) {
    static unsigned long ultimo_scan = 0;
    if (millis() - ultimo_scan < 50) return 0;
    ultimo_scan = millis();

    for (uint8_t f = 0; f < 4; f++) {
        KEY_PORT = 0xFF & ~(1 << f);
        __asm__ __volatile__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
                             "nop\n\t" "nop\n\t");
        uint8_t col_bits = (KEY_PIN >> 4) & 0x0F;
        for (uint8_t c = 0; c < 4; c++) {
            if (!(col_bits & (1 << c))) {
                char tecla = teclado_mapa[f][c];
                KEY_PORT = 0xFF;
                return tecla;
            }
        }
    }
    KEY_PORT = 0xFF;
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

void usart_print(const char* s) {
    while (pgm_read_byte(s)) {
        usart_transmit(pgm_read_byte(s++));
    }
}

void usart_println(const char* s) {
    usart_print(s);
    usart_transmit('\n');
}
