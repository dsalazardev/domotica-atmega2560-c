# Design: create-agents-md

## Estructura de Archivos

```
/AGENTS.md                          ← Raíz: punto de entrada único para agentes (~80-100 líneas)
/docs/
├── 01-business-rules.md            ← 18 reglas de negocio (~150 líneas)
├── 02-hardware-map.md              ← Mapa periféricos, pines, protocolos (~100 líneas)
├── 03-architecture-overview.md     ← Diagramas de arquitectura y flujo de datos (~80 líneas)
├── 04-subsystem-alarm.md           ← Alarma de seguridad (R1-R2) (~80 líneas)
├── 05-subsystem-rfid-access.md     ← Control acceso RFID + habitación juegos (R3-R11) (~120 líneas)
├── 06-subsystem-environment.md     ← Iluminación dimerizada + temperatura (R12-R13) (~60 líneas)
├── 07-subsystem-remote.md          ← Horno, sonido, lista de mercado (R14-R17) (~80 líneas)
├── 08-subsystem-ui.md              ← LCD + teclado matricial (R18) (~80 líneas)
├── 09-subsystem-comm.md            ← USART protocolo de comunicación (~60 líneas)
├── 10-protocol-spi.md              ← Referencia driver SPI (~40 líneas)
├── 11-protocol-i2c.md              ← Referencia driver I2C (~40 líneas)
├── 12-pin-assignment.md            ← Asignación definitiva de pines ATmega2560 (~60 líneas)
└── 13-code-patterns.md             ← Patrones reutilizables de códigos de clase (~60 líneas)
```

## Contenido del AGENTS.md raíz

Secciones exactas:

1. **Project Overview** — Descripción de 1 línea del sistema
2. **Tech Stack** — ATmega2560, C++11, Arduino Framework, PlatformIO, CLion, Proteus
3. **Repository Structure** — Mapa de directorios con propósito de cada uno
4. **Source Files** — Lista de archivos fuente y su propósito
5. **Setup Commands** — Cómo compilar, limpiar, subir a Proteus
6. **Code Style & Conventions** — Convenciones de código del proyecto
7. **Business Rules Index** — Enlace a `docs/01-business-rules.md`
8. **Hardware Reference** — Enlace a `docs/02-hardware-map.md`
9. **Architecture Reference** — Enlace a `docs/03-architecture-overview.md`
10. **Subsystem Documentation** — Lista de subsistemas con enlaces a `docs/04-09`
11. **Protocol References** — Enlaces a `docs/10-11`
12. **Pin Assignment Reference** — Enlace a `docs/12-pin-assignment.md`
13. **Code Pattern Reference** — Enlace a `docs/13-code-patterns.md`
14. **Design Decisions Pending** — 9 ambigüedades del enunciado que requieren decisión
15. **Agent Guidelines** — Instrucciones para agentes de IA

## Contenido de cada archivo de dominio

### docs/01-business-rules.md
Las 18 reglas de negocio con formato:
```
Regla N: [Nombre]
- Actor: Quién ejecuta
- Condición: Cuándo se aplica
- Acción: Qué sucede
- Restricciones: Límites y validaciones
- Subsistema: A qué pertenece
- Prioridad: Alta/Media/Baja
- Fuente textual: Cita del enunciado
```

### docs/02-hardware-map.md
- Tabla de periféricos: nombre, tipo, protocolo, puertos probables, función
- Tabla de pines PWM disponibles con timers y resolución
- Tabla de pines USART disponibles

### docs/03-architecture-overview.md
- Diagrama ASCII de 3 capas (Aplicación → Servicios → Hardware)
- Diagrama de flujo de datos (entradas → procesamiento → salidas)
- Descripción de cada capa

### docs/04-subsystem-alarm.md
- Reglas R1-R2 expandidas
- Máquina de estados: Desactivada → Activada → Disparada → Notificando
- Flujo de notificación por serial
- Código de seguridad en EEPROM

### docs/05-subsystem-rfid-access.md
- Reglas R3-R11 expandidas
- Flujo de enrolamiento, borrado, validación
- Control de puertas (imán + servomotor)
- Habitación juegos: verificar saldo, descontar, recargar

### docs/06-subsystem-environment.md
- Reglas R12-R13 expandidas
- PWM dimerización
- Control temperatura con histéresis (calefactor abajo, ventilador arriba)

### docs/07-subsystem-remote.md
- Reglas R14-R17 expandidas
- Horno: comando remoto con tiempo y temperatura
- Sonido: comando remoto con volumen (señal analógica PWM+RC)
- Lista de mercado: agregar/consultar por serial, almacenar en EEPROM

### docs/08-subsystem-ui.md
- Regla R18 expandida
- Árbol de menús completo (9 entradas principales)
- Mapeo de teclado a acciones
- Secuencia de inicialización LCD modo 4 bits

### docs/09-subsystem-comm.md
- Protocolo USART: 9600 8N1
- Formato comandos: `COMANDO [param1] [param2]\n`
- Formato respuestas: `OK: mensaje\n` o `ERROR: descripción\n`
- Lista de comandos: ALARMA, HORNO, SONIDO, TEMP?, LISTA?, ESTADO?

### docs/10-protocol-spi.md
- Código de referencia: SPI-Maestro.ino
- Registros: SPCR, SPSR, SPDR
- Pines: PB0-PB3
- Función: spi_init(), spi_transfer()

### docs/11-protocol-i2c.md
- Código de referencia: I2C-Maestro.ino
- Registros: TWBR, TWSR, TWCR, TWDR
- Pines: PD0 (SDA), PD1 (SCL)
- Funciones: i2c_master_init(), transmit(), request_data()

### docs/12-pin-assignment.md
- Tabla de asignación definitiva de pines
- Columnas: Pin ATmega2560, Señal, Periférico, Notas

### docs/13-code-patterns.md
- Patrón LCD 4-bit (6 referencias)
- Patrón Teclado Matricial (3 referencias)
- Patrón USART TX+RX (1 referencia)
- Patrón SPI Maestro (1 referencia)
- Patrón I2C Maestro (1 referencia)
- Patrón PWM con ADC (1 referencia)

## Design Decisions Pending (9 ambigüedades)

1. **Sensor de temperatura** — LM35 (ADC) vs DHT11/22 (digital) vs DS18B20 (1-Wire)
2. **Formato código seguridad** — 4 dígitos? 6? Alfanumérico?
3. **Protocolo módulo RFID** — SPI (MFRC522/RC522) vs otro
4. **Datos en tarjeta RFID** — Solo saldo? También UID? Formato?
5. **Número exacto de sensores** — ¿Cuántos de humo? ¿Cuántos de puerta/ventana?
6. **Tipo de dimerización** — PWM LED directo vs triac para AC
7. **Señal analógica sonido** — PWM+RC vs DAC externo
8. **Capacidad máxima lista mercado** — Número máximo de productos
9. **Cuota máxima habitación juegos** — 255 (1 byte) vs 65535 (2 bytes)
