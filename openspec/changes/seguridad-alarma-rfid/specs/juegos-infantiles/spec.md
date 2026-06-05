## ADDED Requirements

### Requirement: Acceso a habitación de juegos con cuota de usos
El sistema SHALL permitir el acceso a la habitación de juegos solo si el UID de la tarjeta RFID del niño tiene saldo de usos disponible (mayor a 0), almacenado en EEPROM local indexada por UID.

#### Scenario: Acceso permitido con saldo disponible
- **WHEN** un niño acerca su tarjeta RFID Y el saldo de usos asociado a su UID es mayor a 0
- **THEN** el sistema muestra "Bienvenido! Te quedan N usos" en LCD Y permite el acceso

#### Scenario: Acceso denegado por saldo insuficiente
- **WHEN** un niño acerca su tarjeta RFID Y el saldo de usos asociado a su UID es 0
- **THEN** el sistema muestra "Sin accesos. Pide a tus padres" en LCD Y no permite el acceso

#### Scenario: Tarjeta no enrolada
- **WHEN** un niño acerca una tarjeta RFID cuyo UID no está enrolado en el sistema
- **THEN** el sistema muestra "Acceso denegado" en LCD

### Requirement: Descuento de acceso por ingreso
El sistema SHALL decrementar el contador de usos almacenado en EEPROM local (indexado por UID) después de cada ingreso exitoso a la habitación de juegos.

#### Scenario: Descuento aplicado correctamente
- **WHEN** el niño ingresa a la habitación de juegos
- **THEN** el saldo de usos en EEPROM para ese UID se decrementa en 1 Y el nuevo saldo se muestra en LCD

#### Scenario: Descuento hasta llegar a cero
- **WHEN** el niño ingresa con saldo de 1 uso
- **THEN** después del ingreso el saldo queda en 0 Y en el próximo intento se deniega el acceso

### Requirement: Recarga de accesos por los padres
El sistema SHALL permitir a los padres (autenticados con código de seguridad) recargar el saldo de usos asociado al UID de la tarjeta RFID del hijo, actualizando el valor en EEPROM local.

#### Scenario: Recarga exitosa
- **WHEN** un padre autenticado acerca la tarjeta del hijo Y selecciona un nuevo saldo
- **THEN** el sistema actualiza el saldo en EEPROM para ese UID Y muestra "Recarga exitosa. Saldo: N usos" en LCD

#### Scenario: Recarga con saldo máximo
- **WHEN** un padre intenta recargar más de 255 usos
- **THEN** el sistema limita el saldo a 255 Y muestra el valor máximo en LCD

### Requirement: Consulta de saldo de usos
El sistema SHALL permitir consultar el saldo actual de usos de una tarjeta RFID leyendo el valor desde EEPROM local según su UID, sin descontar accesos.

#### Scenario: Consulta de saldo
- **WHEN** se selecciona "Consultar saldo" en el menú Y se acerca una tarjeta RFID
- **THEN** el sistema muestra "Saldo: N usos" en LCD sin modificar el contador en EEPROM
