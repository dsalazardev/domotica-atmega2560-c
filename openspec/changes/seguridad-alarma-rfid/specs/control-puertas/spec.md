## ADDED Requirements

### Requirement: Apertura de puerta principal con imán
El sistema SHALL activar un imán (relé) mediante GPIO para liberar la puerta principal durante 3 segundos cuando se autoriza el acceso RFID.

#### Scenario: Puerta principal abierta
- **WHEN** una tarjeta RFID autorizada es detectada Y se selecciona puerta principal
- **THEN** el sistema activa el GPIO del imán por 3 segundos Y muestra "Puerta abierta" en LCD

#### Scenario: Puerta principal se cierra automáticamente
- **WHEN** pasan 3 segundos desde la activación del imán
- **THEN** el sistema desactiva el GPIO del imán

### Requirement: Apertura de puerta garaje con servomotor
El sistema SHALL activar un servomotor mediante PWM (Timer1/OC1A en PB5) para abrir la puerta del garaje cuando se autoriza el acceso RFID.

#### Scenario: Garaje abierto
- **WHEN** una tarjeta RFID autorizada es detectada Y se selecciona puerta garaje
- **THEN** el servomotor gira a 90° (pulso ~1.5ms), espera, y retorna a 0°

#### Scenario: Servomotor configurado correctamente
- **WHEN** se inicializa el sistema
- **THEN** Timer1/OC1A está configurado en Fast PWM con TOP en ICR1 para generar pulsos de 1ms a 2ms (ángulo 0° a 180°)

### Requirement: Temporización no-bloqueante para puertas
El sistema SHALL usar el contador tick del Timer5 para temporizar la activación de puertas sin usar `_delay_ms()`.

#### Scenario: Temporizador de imán no bloquea el sistema
- **WHEN** el imán está activo por 3 segundos
- **THEN** el sistema continúa escaneando teclado y sensores durante la espera

#### Scenario: Temporizador de servomotor no bloquea el sistema
- **WHEN** el servomotor está girando
- **THEN** el sistema continúa respondiendo a otros eventos durante el movimiento
