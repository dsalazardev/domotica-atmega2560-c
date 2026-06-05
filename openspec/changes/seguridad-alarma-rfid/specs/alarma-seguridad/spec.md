## ADDED Requirements

### Requirement: Detección de sensores reed (puertas/ventanas)
El sistema SHALL leer el estado de los sensores reed en puertas y ventanas mediante GPIO digital (PINK, PINE, etc. según asignación de pines).

#### Scenario: Puerta abierta detectada
- **WHEN** un sensor reed cambia a estado abierto
- **THEN** el sistema marca una condición de intrusión pendiente

#### Scenario: Ventana abierta detectada
- **WHEN** un sensor reed de ventana cambia a estado abierto
- **THEN** el sistema marca una condición de intrusión pendiente

### Requirement: Detección de sensores de humo
El sistema SHALL leer el estado de los sensores de humo mediante GPIO digital (polling periódico cada 100ms).

#### Scenario: Humo detectado
- **WHEN** un sensor de humo cambia a estado activo
- **THEN** el sistema marca una condición de incendio pendiente

#### Scenario: Sensor de humo en estado normal
- **WHEN** el sensor de humo está en estado inactivo
- **THEN** el sistema no toma ninguna acción

### Requirement: Notificación por USART en condición de seguridad
El sistema SHALL enviar un mensaje por USART cuando se detecte una condición que comprometa la seguridad Y la alarma esté activada.

#### Scenario: Intrusión notificada
- **WHEN** la alarma está activada Y un sensor reed se dispara
- **THEN** el sistema envía "ALERTA: Intrusion en [sensor]" por USART

#### Scenario: Incendio notificado
- **WHEN** la alarma está activada Y un sensor de humo se dispara
- **THEN** el sistema envía "ALERTA: Humo en [sensor]" por USART

#### Scenario: Alarma desactivada sin notificación
- **WHEN** la alarma está desactivada Y un sensor se dispara
- **THEN** el sistema NO envía ninguna notificación

### Requirement: Alarma dual con máquina de estados
El sistema SHALL mantener una máquina de estados para la alarma con tres estados: Desactivada, Activada, Disparada.

#### Scenario: Activación exitosa
- **WHEN** se ingresa el código correcto en estado Desactivada
- **THEN** la alarma pasa a estado Activada

#### Scenario: Desactivación exitosa
- **WHEN** se ingresa el código correcto en estado Activada o Disparada
- **THEN** la alarma pasa a estado Desactivada

#### Scenario: Alarma se dispara
- **WHEN** la alarma está Activada Y un sensor se dispara
- **THEN** la alarma pasa a estado Disparada Y se notifica por USART

### Requirement: Activación/desactivación solo con código de seguridad
El sistema SHALL solicitar un código numérico de 4 dígitos para activar o desactivar la alarma. Solo se activará/desactivará si el código ingresado coincide con el almacenado.

#### Scenario: Código correcto para activar
- **WHEN** se ingresa el código correcto de 4 dígitos
- **THEN** la alarma se activa/desactiva según corresponda

#### Scenario: Código incorrecto
- **WHEN** se ingresa un código incorrecto
- **THEN** la alarma NO cambia de estado Y el LCD muestra "Codigo incorrecto"

### Requirement: Cambio de código de seguridad
El sistema SHALL permitir cambiar el código de seguridad desde el menú de configuración, solicitando el código actual, luego el nuevo código dos veces.

#### Scenario: Cambio de código exitoso
- **WHEN** se ingresa el código actual correcto Y el nuevo código coincide en ambas ocasiones
- **THEN** el nuevo código se almacena en EEPROM Y el LCD confirma el cambio

#### Scenario: Cambio de código fallido por código actual incorrecto
- **WHEN** se ingresa un código actual incorrecto
- **THEN** el sistema muestra "Codigo actual incorrecto" Y no modifica el código

#### Scenario: Cambio de código fallido por nueva confirmación
- **WHEN** el nuevo código y su confirmación no coinciden
- **THEN** el sistema muestra "Confirmacion fallida" Y no modifica el código
