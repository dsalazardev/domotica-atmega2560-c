# Subsistema: Comunicación USART

## Configuración
- **Velocidad**: 9600 baud
- **Formato trama**: 8N1 (8 bits, sin paridad, 1 stop bit)
- **Full-dúplex**: Transmisión y recepción simultáneas
- **Recepción**: Por interrupción (USART0_RX_vect) para no perder datos
- **Transmisión**: Por polling (esperar UDRE0 antes de cargar UDR0)

## Registros del ATmega2560 (USART0)

| Registro | Función |
|---|---|
| UBRR0H/L | Baud rate (UBRR0 = 104 para 9600 @ 16MHz) |
| UCSR0A | Flags: RXC0, TXC0, UDRE0, FE0, DOR0, UPE0 |
| UCSR0B | Enable: RXCIE0, TXCIE0, UDRIE0, RXEN0, TXEN0 |
| UCSR0C | Config: UMSEL, UPM, USBS, UCSZ |
| UDR0 | Dato transmitido/recibido |

## Formato de Comandos (PC → ATmega2560)

```
COMANDO [param1] [param2]\n
```

### Comandos Disponibles

| Comando | Parámetros | Descripción |
|---|---|---|
| `ALARMA ON` | código | Activar alarma de seguridad |
| `ALARMA OFF` | código | Desactivar alarma |
| `HORNO ON` | temperatura, tiempo | Encender horno |
| `HORNO OFF` | — | Apagar horno |
| `SONIDO ON` | volumen (0-100) | Encender equipo de sonido |
| `SONIDO OFF` | — | Apagar sonido |
| `TEMP?` | — | Consultar temperatura actual |
| `LISTA?` | — | Consultar lista de mercado |
| `LISTA+` | nombre, cantidad | Agregar item a la lista |
| `LISTA-` | índice | Borrar item de la lista |
| `ESTADO?` | — | Estado general del sistema |

## Formato de Respuestas (ATmega2560 → PC)

### Respuesta Exitosa
```
OK: <mensaje>\n
```

### Respuesta de Error
```
ERROR: <descripción>\n
```

### Notificación de Alarma (evento asíncrono)
```
ALERTA: <tipo> <descripción>\n
```

### Ejemplos
```
PC → "ALARMA ON 1234\n"
ATm → "OK: alarma activada\n"

PC → "TEMP?\n"
ATm → "OK: 24.5°C\n"

Sensor dispara → "ALERTA: intrusión sensor puerta principal\n"

PC → "ALARMA OFF 0000\n"
ATm → "ERROR: código incorrecto\n"
```
