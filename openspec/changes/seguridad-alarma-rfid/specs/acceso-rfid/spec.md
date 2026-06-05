## ADDED Requirements

### Requirement: Inicializar módulo RFID via SPI
El sistema SHALL inicializar la comunicación SPI con el módulo RFID MFRC522 mediante `spi_master_init()` para leer UIDs de tarjetas.

#### Scenario: SPI inicializado correctamente
- **WHEN** se ejecuta `spi_master_init()` con los pines SS=PB0, SCK=PB1, MOSI=PB2, MISO=PB3
- **THEN** el módulo RFID queda listo para comunicación

### Requirement: Lectura de UID de tarjeta RFID
El sistema SHALL leer el UID (Unique Identifier) de una tarjeta RFID acercada al lector, utilizando el protocolo SPI para comunicarse con el MFRC522.

#### Scenario: Tarjeta detectada
- **WHEN** una tarjeta RFID se acerca al lector
- **THEN** el sistema lee el UID de 4 bytes y lo almacena temporalmente para validación

#### Scenario: Sin tarjeta presente
- **WHEN** no hay tarjeta RFID cerca del lector
- **THEN** el sistema retorna que no hay tarjeta disponible

### Requirement: Enrolamiento de nueva tarjeta RFID
El sistema SHALL permitir a un administrador autenticado (código de seguridad) enrolar una nueva tarjeta RFID, almacenando su UID en EEPROM.

#### Scenario: Enrolamiento exitoso
- **WHEN** el administrador ingresa el código correcto Y selecciona "Enrolar" Y acerca una tarjeta RFID nueva
- **THEN** el UID de la tarjeta se almacena en EEPROM Y el LCD muestra "Persona enrolada"

#### Scenario: Enrolamiento de tarjeta ya existente
- **WHEN** el administrador intenta enrolar una tarjeta cuyo UID ya está en EEPROM
- **THEN** el sistema muestra "Tarjeta ya enrolada" Y no duplica el registro

#### Scenario: Enrolamiento sin código de administrador
- **WHEN** se intenta enrolar sin autenticación de administrador
- **THEN** el sistema rechaza la operación Y muestra "Acceso denegado"

### Requirement: Borrado de tarjeta RFID existente
El sistema SHALL permitir a un administrador autenticado borrar una tarjeta RFID enrolada, eliminando su UID de EEPROM.

#### Scenario: Borrado exitoso
- **WHEN** el administrador autenticado selecciona una tarjeta de la lista Y confirma el borrado
- **THEN** el UID se elimina de EEPROM Y el LCD muestra "Persona eliminada"

#### Scenario: Borrado de tarjeta inexistente
- **WHEN** se intenta borrar un UID que no está en EEPROM
- **THEN** el sistema muestra "Tarjeta no encontrada"

### Requirement: Validación de tarjeta RFID para acceso
El sistema SHALL validar el UID de una tarjeta RFID contra la lista de UIDs almacenados en EEPROM para determinar si el acceso está autorizado.

#### Scenario: Acceso autorizado
- **WHEN** se acerca una tarjeta RFID con UID en la lista blanca de EEPROM
- **THEN** el sistema muestra "Acceso autorizado" en LCD Y procede a abrir la puerta

#### Scenario: Acceso denegado
- **WHEN** se acerca una tarjeta RFID con UID no registrado en EEPROM
- **THEN** el sistema muestra "Acceso denegado" en LCD Y no abre ninguna puerta
