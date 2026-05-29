
## Slide 1

Interrupciones externas


## Slide 2

Qué es una interrupción
Una interrupción, es una situación que debe ser atendida por el microcontrolador de forma inmediata. Es decir, el microcontrolador debe dejar de hacer lo que esté realizando en el momento en que ocurre el evento desencadenante de la interrupción, atender el evento, y retornar a su ejecución normal.
Pueden ser generadas por los periféricos del microcontrolador como memorias, temporizadores, conversores A/D, puertos de comunicación, etc. O por dispositivos externos, a través de interrupciones externas.


## Slide 3

Vector de interrupción
Es la posición de memoria a la que acude el microcontrolador, en el momento de la ocurrencia del evento, para ejecutar la(s) instrucción(es) que se encuentren allí.
Dependiendo del microcontrolador, se puede contar con vector de interrupción único o específico.
Único: Se deben consultar las banderas de interrupciones para saber cuál interrupción fue generada y por lo tanto saber lo que se debe ejecutar para atenderla.
Específico: Cada interrupción tiene su propio vector de interrupción establecido. No es necesario verificar banderas de interrupción.


## Slide 4

Vectores de interrupción ATmega 2560


## Slide 5

Interrupciones externas
INT0 a INT7
El evento generador de la interrupción es configurable:
  - Nivel 0
  - Cualquier cambio (flanco)
  - Flanco de subida
  - Flanco de bajada
La configuración se realiza con el registro EICRA(INT3:0) y EICRB(INT7:4).
Para utilizar estas interrupciones, se debe habilitar el bit de interrupciones globales (SREG.7) y el bit correspondiente de la interrupción a utilizar en EIMSK.


## Slide 6

Interrupciones externas


## Slide 7

Interrupciones por cambio (PCINTx)
Cualquier pin de los puertos B, J* y K puede ser usado como una interrupción por cambio.
Estos pines se encuentran divididos en tres grupos:
Grupo 0: PCINT 0 a 7
Grupo 1: PCINT 8 a 15
Grupo 2: PCINT 16 a 23
Para utilizar estas interrupciones, se debe habilitar el bit de interrupciones globales (SREG.7) y el bit del grupo de interrupción a utilizar PCICR.0, PCICR.1 o PCICR.2 respectivamente. Además del bit correspondiente al PCINT específico en el registro PCMSKx.
