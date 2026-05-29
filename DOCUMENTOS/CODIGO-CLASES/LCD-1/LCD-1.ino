int RS = 8;
int EN = 9;
int D4 = 4;
int D5 = 5;
int D6 = 6;
int D7 = 7;

void enviar4Bits(byte valor) {
  digitalWrite(D4, bitRead(valor, 0));
  digitalWrite(D5, bitRead(valor, 1));
  digitalWrite(D6, bitRead(valor, 2));
  digitalWrite(D7, bitRead(valor, 3));
  digitalWrite(EN, HIGH);
  delay(1); 
  digitalWrite(EN, LOW);
  delay(1);
}

void enviarComando(byte comando) {
  digitalWrite(RS, LOW);
  enviar4Bits(comando >> 4);
  enviar4Bits(comando);
  delay(2);
}

void enviarLetra(char letra) {
  digitalWrite(RS, HIGH);
  enviar4Bits(letra >> 4);
  enviar4Bits(letra);
  delay(1);
}

void setup() {
  pinMode(RS, OUTPUT); 
  pinMode(EN, OUTPUT);
  pinMode(D4, OUTPUT); 
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT); 
  pinMode(D7, OUTPUT);
  delay(20);
  enviar4Bits(0x03); delay(5);
  enviar4Bits(0x03); delay(1);
  enviar4Bits(0x03);
  enviar4Bits(0x02);
  enviarComando(0x28);
  enviarComando(0x0C);
  enviarComando(0x01);
  delay(2);
}

void loop() {
  enviarComando(0x80); 

  enviarLetra('A');
  enviarLetra('l');
  enviarLetra('e');
  enviarLetra('j');
  enviarLetra('a');
  enviarLetra('n');
  enviarLetra('d');
  enviarLetra('r');
  enviarLetra('o');

  while(1);
}