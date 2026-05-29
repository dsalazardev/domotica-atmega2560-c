
## Slide 1

temporizadores


## Slide 2

Qué es un temporizador
Es un periférico que se utiliza para medir tiempo. La medición de tiempo se realiza a través del conteo de pulsos de un periodo establecido. Es decir, es un contador. Debido a esto, algunos de ellos se pueden usar también como contadores. Para que un temporizador pueda funcionar como contador, requiere tener una conexión con algún pin externo del microcontrolador.
Los temporizadores de manera general producen una interrupción en el momento en que se reciclan (pasan de su último estado al primero).
En algunos casos, como ocurre con el microcontrolador ATmega2560, también pueden generar interrupción en el momento en que llegan a un valor, el cual puede ser programado.
Este microcontrolador cuenta con seis temporizadores, dos de 8 bits y cuatro de 16 bits. Todos pueden generar PWM (Pulse Width Modulation)


## Slide 3

pwm
Control del ancho del pulso de una señal cuadrada, para controlar la cantidad de potencia que se entrega a una carga.
Ciclo útil: Porcentaje de tiempo en que la señal permanece en uno.
Tomada de https://en.wikipedia.org/wiki/Pulse-width_modulation


## Slide 4

TIMER 0


## Slide 5

Modos de operación timer 0
Los modos de operación se definen por los bits (WGM02:0 y COM0x1:0)
WGM02:0 controlan la secuencia de conteo
COM0x1:0 controlan la forma de onda en las salidas OC0x
Modo Normal: Cuenta de forma ascendente hasta 255 y vuelve a empezar. Cada vez que pasa de 255 a 0, activa la interrupción TOV0.
Clear Timer on Compare Match (CTC): El temporizador se hace cero, en el momento que la cuenta alcanza el valor programado en el comparador A (valor almacenado en el registro OCR0A). Se puede generar una forma de onda en el pin OC0A, configurando ese pin para que se complemente cada vez que ocurra la interrupción. El pin debe estar configurado como salida para que se pueda ver la forma de onda.


## Slide 6

Modos de operación timer 0 (cont.)
Modo PWM Rápido: El temporizador cuenta de forma ascendente de 0 hasta el valor programado. En el modo no-inverso El pin OC0x se pone en 0 cuando alcanza el valor programado y se pone en 1 cuando se recicla. En el modo inverso, es 1 cuando alcanza el valor programado y 0 cuando se recicla. TOV0 se activa cada vez que el timer alcanza el valor programado. El pin OC0A puede también ser configurado para que se complemente cada que se alcance el valor programado.
Phase Correct PWM Mode: Modo preferido para control de motores, debido a su simetría (centrado en el periodo). El contador cuenta de 0 hasta el valor programado y luego de este último hasta 0 otra vez. En el modo no-inverso, el pin OC0x es puesto a 0 cuando se alcanza el valor programado en cuenta ascendente y puesto en 1 cuando se alcanza el valor programado en cuenta descendente. El modo inverso es lo contrario.


## Slide 7

Registros timer 0


## Slide 8

Configuración de la fuente de reloj


## Slide 9

Configuración del modo de operación


## Slide 10

Configuración del modo de operación


## Slide 11

Timers 1, 3, 4, 5
Son de 16 bits.
Tres comparadores, pero adicional, una unidad para captura de entrada (cuando se genera un flanco, se almacena el valor del contador en ese momento).
Tienen los mismos modos de operación de Timer 0 y adicional: Phase and frequency correct PWM mode.
Dado que este microcontrolador es de 8 bits, todos los registros tienen este tamaño. Por lo tanto, el dato de estos timers se almacena en dos registros TCNTnH y TCNTnL. En una operación de escritura se debe escribir primero el byte alto y en una operación de lectura se debe leer primero el byte bajo.


## Slide 12

preescaladores
Preescalador: Circuito para disminuir la frecuencia de reloj.
Excepto timer 2, todos los demás usan el mismo conjunto de preescaladores, pero pueden tener diferentes configuraciones.
Cuatro preescaladores: Por 8, 64, 256 o 1024.
Una fuente de reloj externa no puede ser preescalada.


## Slide 13

Timer 2
Mismo funcionamiento que Timer 0
Dos comparadores
Adicional, tiene capacidad de funcionamiento asíncrono, lo que quiere decir que puede tener un reloj externo, y por lo tanto, no deja de funcionar cuando se utiliza el Power Save Mode de la CPU.
