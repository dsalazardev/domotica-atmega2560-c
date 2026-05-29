#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define RS PB0
#define EN PB1

long num1 = 0, num2 = 0;
char operacion = 0;
bool segundoNumero = false;

void enviar4Bits(uint8_t valor) {
    PORTD = (PORTD & 0x0F) | (valor << 4);
    PORTB |= (1 << EN);
    _delay_ms(1);
    PORTB &= ~(1 << EN);
    _delay_ms(1);
}

void enviarComando(uint8_t cmd) {
    PORTB &= ~(1 << RS);
    enviar4Bits(cmd >> 4);
    enviar4Bits(cmd & 0x0F);
    _delay_ms(2);
}

void enviarLetra(char letra) {
    PORTB |= (1 << RS);
    enviar4Bits(letra >> 4);
    enviar4Bits(letra & 0x0F);
    _delay_ms(1);
}

void imprimirTexto(char* texto) {
    while (*texto) {
        enviarLetra(*texto++);
    }
}

void calcular() {
    long resultado = 0;
    if (operacion == '+') resultado = num1 + num2;
    if (operacion == '-') resultado = num1 - num2;
    if (operacion == '*') resultado = num1 * num2;
    if (operacion == '/') resultado = (num2 != 0) ? num1 / num2 : 0;

    enviarComando(0xC0);
    enviarLetra('=');
    char buffer[16];
    itoa(resultado, buffer, 10);
    imprimirTexto(buffer);
}

char leerTeclado() {
    char teclas[4][4] = {
        {'1','2','3','+'},
        {'4','5','6','-'},
        {'7','8','9','*'},
        {'C','0','=','/'}
    };
    for (int f = 0; f < 4; f++) {
        PORTL = ~(1 << f);
        _delay_ms(5);
        uint8_t col = PINK & 0x0F;
        if (col != 0x0F) {
            while((PINK & 0x0F) != 0x0F);
            if (col == 0x0E) return teclas[f][0];
            if (col == 0x0D) return teclas[f][1];
            if (col == 0x0B) return teclas[f][2];
            if (col == 0x07) return teclas[f][3];
        }
    }
    return 0;
}

int main() {
    DDRD |= 0xF0; DDRB |= (1 << RS) | (1 << EN);
    DDRL = 0xFF; DDRK = 0x00; PORTK = 0x0F;
    
    _delay_ms(20);
    enviar4Bits(0x03); _delay_ms(5);
    enviar4Bits(0x03); _delay_ms(1);
    enviar4Bits(0x03); enviar4Bits(0x02);
    enviarComando(0x28); enviarComando(0x0C); enviarComando(0x01);

    while (1) {
        char t = leerTeclado();
        if (t != 0) {
            if (t >= '0' && t <= '9') {
                enviarLetra(t);
                if (!segundoNumero) num1 = (num1 * 10) + (t - '0');
                else num2 = (num2 * 10) + (t - '0');
            }
            else if (t == '+' || t == '-' || t == '*' || t == '/') {
                operacion = t;
                segundoNumero = true;
                enviarLetra(t);
            }
            else if (t == '=') {
                calcular();
            }
            else if (t == 'C') {
                num1 = num2 = operacion = 0;
                segundoNumero = false;
                enviarComando(0x01); 
            }
        }
    }
}