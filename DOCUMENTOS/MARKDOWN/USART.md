
## Slide 1

Comunicación serial


## Slide 2

Tipos de comunicación
Paralela
Serial
Tx
Tx
Rx
Rx


## Slide 3

Protocolos de comunicación serial
Un protocolo de comunicación, es un conjunto de reglas establecidas para permitir que dos o más dispositivos puedan compartir información entre ellos. Estas reglas incluyen cómo se inicia y termina la comunicación, cómo se envía el dato y cómo se detectan errores.
Protocolos de comunicación serial
  - USART
  - I2C
  - SPI


## Slide 4

USART (universal synchronous/asynchronous receiver/transmitter)
Síncrona (Ambos dispositivos usan el mismo reloj)
Asíncrona (Cada dispositivo tiene su propio reloj)
Full-dúplex (Ambos dispositivos pueden transmitir al mismo tiempo)
Maestro/esclavo síncrona (Un maestro, varios esclavos)
  - El maestro controla la comunicación
  - Envía la dirección del periférico con el que se desea comunicar y sólo éste responde.


## Slide 5

Formato de la trama


## Slide 6

inicialización
Configurar en receptor y transmisor
  - La velocidad de comunicación en baudios
  - El formato de la trama
  - Habilitar recepción y transmisión
  - En caso de usar interrupciones, habilitarlas (Nota: Deben estar inhabilitadas las interrupciones globales durante la inicialización.)


## Slide 7

Cálculo para la velocidad


## Slide 8

Detección de errores
Se cuenta con 3 banderas de error
Error de trama FE0: Indica si el bit de parada fue leído adecuadamente (uno)
Sobreflujo de datos DOR0: Ocurre cuando el buffer está lleno, el registro de desplazamiento está lleno y se detecta un nuevo bit de inicio.
Error de paridad UPE0
Estas banderas están ubicadas en el registro UCSR0A


## Slide 9

Notas importantes
Al usar comunicación con 9 bits, el 9 bit debe ser leído/enviado antes de leer/enviar los restantes 8 bits almacenados en el registro UDR0.
Si se está realizando detección de errores, el registro UCSR0A debe ser leído antes del registro UDR0.
Las banderas de error no generan interrupción.
Si se inhabilita el transmisor durante una transmisión, no se hará efectiva hasta que esa transmisión termine.
Si se inhabilita el receptor durante una recepción, se hará efectiva inmediatamente, lo que significa, que ese dato se perderá.
El buffer de transmisión solo puede ser cargado si el bit UDRE0 es uno, ya que indica si el buffer está vacío.
La transmisión inicia una vez que el buffer es cargado, siempre y cuando el transmisor esté habilitado.


## Slide 10

Registros


## Slide 11

Configuración de modos y trama


## Slide 12

Configuración de modos y trama (Cont.)
