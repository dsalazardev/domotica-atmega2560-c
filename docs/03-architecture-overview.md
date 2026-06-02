# Arquitectura del Sistema

## Diagrama de Capas

```
                  ┌─────────────────────────────────────────────┐
                  │          CAPA DE APLICACIÓN                  │
                  │                                              │
                  │  ┌─────────┐ ┌─────────┐ ┌───────────────┐  │
                  │  │ Alarma  │ │  RFID   │ │ Hab. Juegos   │  │
                  │  │ Segurid │ │ Acceso  │ │ (cuota usos)  │  │
                  │  └────┬────┘ └────┬────┘ └───────┬───────┘  │
                  │       │           │               │          │
                  │  ┌────┴───────────┴───────────────┴───────┐  │
                  │  │           Menú Principal               │  │
                  │  │  Iluminación · Temperatura · Horno    │  │
                  │  │  Sonido · Lista Mercado · Config      │  │
                  │  └────────────────┬──────────────────────┘  │
                  └───────────────────┼─────────────────────────┘
                                      │
                  ┌───────────────────┼─────────────────────────┐
                  │    CAPA DE SERVICIOS                        │
                  │                                              │
                  │  ┌──────────┐ ┌──────────┐ ┌────────────┐  │
                  │  │  Código  │ │  EEPROM  │ │   Timers   │  │
                  │  │ Segurid  │ │  Manager │ │   + PWM    │  │
                  │  └──────────┘ └──────────┘ └────────────┘  │
                  │  ┌──────────┐ ┌──────────┐ ┌────────────┐  │
                  │  │  USART   │ │   SPI    │ │    I2C     │  │
                  │  │  Driver  │ │  Driver  │ │   Driver   │  │
                  │  └──────────┘ └──────────┘ └────────────┘  │
                  └───────────────────┼─────────────────────────┘
                                      │
                  ┌───────────────────┼─────────────────────────┐
                  │    CAPA DE HARDWARE (Periféricos)           │
                  │                                              │
                  │  LCD · Teclado · RFID · Sensores · Relés    │
                  │  Servomotor · Calefactor · Ventilador       │
                  │  USART PC                                    │
                  └──────────────────────────────────────────────┘
```

## Flujo de Datos

```
  PC (Proteus/Monitor Serial)
        ↕ USART (9600 baud)
┌─────────────────────┐
│     ATmega2560      │
│                      │
│  Teclado ──▶ Menú ──▶ Subsistema activo ──▶ LCD
│                      │
│  RFID ──▶ Acceso ──▶ Relé imán / Servomotor
│  RFID ──▶ Juegos ──▶ Escribe saldo en tarjeta
│                      │
│  Sensor T° ──▶ Control ──▶ PWM calefactor/ventilador
│  ADC pot ──▶ Volumen ──▶ PWM+RC → señal sonido
│                      │
│  EEPROM ◀──▶ Lista de Mercado / Configuración
│  USART RX ──▶ Parser ──▶ Comando → Subsistema
│  Subsistema ──▶ USART TX ──▶ Notificación/Respuesta
└─────────────────────┘
```

## Descripción de Capas

**Capa de Aplicación**: Lógica de negocio de cada subsistema. Procesa entradas (teclado, RFID, USART), ejecuta acciones y actualiza la interfaz. Incluye el sistema de menús que orquesta la navegación del usuario.

**Capa de Servicios**: Drivers de bajo nivel y módulos compartidos. Aquí residen los controladores USART, SPI, I2C, el sistema de timers/PWM, la gestión de EEPROM y la validación de código de seguridad.

**Capa de Hardware**: Pines físicos, registros de periféricos y actuadores del ATmega2560. Es la interfaz directa con sensores (reed, humo, temperatura), actuadores (relés, servomotor, calefactor, ventilador) y dispositivos de E/S (LCD, teclado, RFID, USART).
