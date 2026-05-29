#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR  DDRA
#define LCD_CTRL_PORT PORTC
#define LCD_CTRL_DDR  DDRC
#define LCD_RS PC0  
#define LCD_EN PC1  

#define KEY_PORT PORTK
#define KEY_DDR  DDRK
#define KEY_PIN  PINK

void USART0_Init(void);
void USART0_Transmit(unsigned char data);
void LCD_Init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_PulseEnable(void);
unsigned char TECLADO_Scan(void);

const unsigned char teclado_mapa[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

ISR(USART0_RX_vect) {
    unsigned char uart_data = UDR0; 

    if (uart_data == '\r') {
        LCD_Cmd(0x01); 
        _delay_ms(2);
    } else if (uart_data != '\n') {
        LCD_Char(uart_data); 
    }
}

int main(void) {
    USART0_Init();
    LCD_Init();
    
    KEY_DDR  = 0x0F;  
    KEY_PORT = 0xFF;  
    
    unsigned char tecla_presionada = 0;
    unsigned char tecla_anterior = 0;

    sei(); 

    while (1) {
        tecla_presionada = TECLADO_Scan();
        
        if (tecla_presionada != 0 && tecla_anterior == 0) {
            USART0_Transmit(tecla_presionada); 
        }
        tecla_anterior = tecla_presionada;

        _delay_ms(10); 
    }
}

void USART0_Init(void) {
    UBRR0H = 0;
    UBRR0L = 104; 
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART0_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); 
    UDR0 = data;
}

unsigned char TECLADO_Scan(void) {
    for (unsigned char f = 0; f < 4; f++) {
        KEY_PORT = 0xFF & ~(1 << f); 
        _delay_us(5); 
        
        unsigned char col_bits = (KEY_PIN >> 4) & 0x0F;
        
        for (unsigned char c = 0; c < 4; c++) {
            if (!(col_bits & (1 << c))) {
                return teclado_mapa[f][c]; 
            }
        }
    }
    return 0; 
}

void LCD_Init(void) {
    LCD_DATA_DDR |= 0xF0; 
    LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_EN); 
    _delay_ms(20);
    
    LCD_Cmd(0x02); 
    LCD_Cmd(0x28); 
    LCD_Cmd(0x0C); 
    LCD_Cmd(0x01); 
    _delay_ms(2);
}

void LCD_PulseEnable(void) {
    LCD_CTRL_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_CTRL_PORT &= ~(1 << LCD_EN);
    _delay_us(100);
}

void LCD_Cmd(unsigned char cmd) {
    LCD_CTRL_PORT &= ~(1 << LCD_RS); 
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmd & 0xF0);
    LCD_PulseEnable();
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((cmd << 4) & 0xF0);
    LCD_PulseEnable();
}

void LCD_Char(unsigned char data) {
    LCD_CTRL_PORT |= (1 << LCD_RS); 
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
    LCD_PulseEnable();
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((data << 4) & 0xF0);
    LCD_PulseEnable();
}