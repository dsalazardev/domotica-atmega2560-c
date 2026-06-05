## Why

El sistema domótico necesita control de seguridad (alarma dual, código), acceso RFID (enrolamiento, borrado, puerta principal con imán y garaje con servomotor) y habitación de juegos con cuota de usos. Actualmente `Seguridad.ino` es un esqueleto vacío. Este cambio implementa las reglas de negocio R1 a R11 completas.

## What Changes

- Implementar alarma dual (acceso + incendio) con sensores reed y humo, notificación por USART
- Implementar validación de código de seguridad para activar/desactivar alarma
- Implementar comunicación SPI con módulo RFID (MFRC522) para leer UID de tarjetas
- Implementar enrolamiento y borrado de tarjetas RFID autorizadas (almacenadas en EEPROM)
- Implementar control de puerta principal (imán via GPIO) y puerta garaje (servomotor via Timer1/OC1A PWM)
- Implementar habitación de juegos con cuota de usos almacenada en tarjeta RFID
- Implementar recarga de accesos por parte de los padres
- Integrar todo en el menú LCD + teclado existente

## Capabilities

### New Capabilities
- `alarma-seguridad`: Alarma dual con sensores reed/humo, activación/desactivación por código, notificación serial
- `acceso-rfid`: Comunicación SPI con módulo RFID, enrolamiento/borrado de tarjetas en EEPROM
- `control-puertas`: Accionamiento de imán (GPIO) y servomotor (Timer1 PWM) por RFID autorizado
- `juegos-infantiles`: Cuota de usos en tarjeta RFID, descuento por ingreso, recarga por padres

### Modified Capabilities
- `lcd-4bit-driver`: Integrar menú de navegación para acceder a funciones de seguridad y RFID

## Impact

- `Proyecto/Seguridad.ino`: Pasará de esqueleto a implementación completa (~250-300 líneas)
- `Proyecto/Drivers.ino`: Requiere agregar función `spi_master_init()` y `spi_master_transmit()` (desde SPI-Maestro.ino)
- `Proyecto/Timer.ino`: Sin cambios, Timer1 se usará para servomotor (ya documentado en AGENTS.md)
- `docs/`: Sin cambios, la documentación actual ya cubre estos subsistemas
- Pines: PL3 (TEC_FILA3) libre, PB5 (OC1A) para servomotor, PE3 (OC3A) libre para iluminación futura
