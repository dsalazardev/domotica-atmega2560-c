## ADDED Requirements

### Requirement: AGENTS.md raíz como punto de entrada

El sistema SHALL proveer un archivo `AGENTS.md` en la raíz del proyecto que sirva como punto de entrada único para agentes de IA.

#### Scenario: Agente lee AGENTS.md por primera vez
- **WHEN** un agente de IA llega al repositorio
- **THEN** el agente encuentra `/AGENTS.md` con 15 secciones que describen: Project Overview, Tech Stack, Repository Structure, Source Files, Setup Commands, Code Style, Business Rules Index, Hardware Reference, Architecture Reference, Subsystem Documentation, Protocol References, Pin Assignment, Code Patterns, Design Decisions Pending, y Agent Guidelines

### Requirement: Divulgación progresiva hacia docs/

El sistema SHALL proveer archivos de dominio separados en `docs/` para cada aspecto del sistema, de modo que un agente pueda cargar solo el archivo relevante para su tarea.

#### Scenario: Agente consulta reglas de negocio
- **WHEN** un agente necesita conocer las reglas de negocio del sistema
- **THEN** el agente sigue el enlace en AGENTS.md a `docs/01-business-rules.md` y encuentra las 18 reglas con formato estructurado

#### Scenario: Agente consulta hardware
- **WHEN** un agente necesita conocer la asignación de periféricos
- **THEN** el agente sigue el enlace en AGENTS.md a `docs/02-hardware-map.md`

#### Scenario: Agente consulta arquitectura
- **WHEN** un agente necesita entender la arquitectura del sistema
- **THEN** el agente sigue el enlace en AGENTS.md a `docs/03-architecture-overview.md`

### Requirement: Documentación de cada subsistema

El sistema SHALL proveer un archivo de especificación por cada subsistema del sistema domótico.

#### Scenario: Agente implementa alarma de seguridad
- **WHEN** un agente necesita implementar el subsistema de alarma
- **THEN** el agente lee `docs/04-subsystem-alarm.md` y encuentra: reglas R1-R2, máquina de estados, flujo de notificación serial, manejo de código de seguridad

#### Scenario: Agente implementa control RFID
- **WHEN** un agente necesita implementar el control de acceso RFID
- **THEN** el agente lee `docs/05-subsystem-rfid-access.md` y encuentra: reglas R3-R11, flujos de enrolamiento, borrado, validación, control de puertas, habitación de juegos

#### Scenario: Agente implementa control ambiental
- **WHEN** un agente necesita implementar iluminación o temperatura
- **THEN** el agente lee `docs/06-subsystem-environment.md` y encuentra: reglas R12-R13, PWM dimerización, control de temperatura con histéresis

#### Scenario: Agente implementa control remoto
- **WHEN** un agente necesita implementar horno, sonido o lista de mercado
- **THEN** el agente lee `docs/07-subsystem-remote.md` y encuentra: reglas R14-R17, comandos USART, formato de datos

#### Scenario: Agente implementa interfaz LCD
- **WHEN** un agente necesita implementar la interfaz de usuario
- **THEN** el agente lee `docs/08-subsystem-ui.md` y encuentra: regla R18, árbol de menús, mapeo de teclado, secuencia de inicialización LCD

#### Scenario: Agente implementa comunicación serial
- **WHEN** un agente necesita implementar la comunicación USART con PC
- **THEN** el agente lee `docs/09-subsystem-comm.md` y encuentra: protocolo 9600 8N1, formato de comandos y respuestas, lista completa de comandos

### Requirement: Referencias técnicas de protocolos

El sistema SHALL proveer referencias técnicas de los protocolos SPI e I2C con ejemplos de códigos de clase.

#### Scenario: Agente necesita driver SPI
- **WHEN** un agente necesita implementar comunicación SPI (ej: con módulo RFID)
- **THEN** el agente lee `docs/10-protocol-spi.md` y encuentra: código de referencia SPI-Maestro.ino, registros SPCR/SPSR/SPDR, pines PB0-PB3

#### Scenario: Agente necesita driver I2C
- **WHEN** un agente necesita implementar comunicación I2C
- **THEN** el agente lee `docs/11-protocol-i2c.md` y encuentra: código de referencia I2C-Maestro.ino, registros TWBR/TWSR/TWCR/TWDR, pines PD0-PD1

### Requirement: Asignación de pines

El sistema SHALL proveer una tabla de asignación de pines del ATmega2560.

#### Scenario: Agente necesita conectar un periférico
- **WHEN** un agente necesita saber qué pin usar para un periférico
- **THEN** el agente lee `docs/12-pin-assignment.md` y encuentra la asignación definitiva

### Requirement: Patrones de código reutilizables

El sistema SHALL catalogar los patrones de código reutilizables de los códigos de clase existentes.

#### Scenario: Agente necesita implementar LCD
- **WHEN** un agente necesita escribir el driver LCD
- **THEN** el agente lee `docs/13-code-patterns.md` y encuentra 6 referencias de implementaciones LCD funcionales en los códigos de clase
