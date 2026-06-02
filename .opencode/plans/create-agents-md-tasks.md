# Tasks: create-agents-md

## Tarea 1: Crear estructura de directorios
- Crear carpeta `docs/` en la raíz del proyecto

## Tarea 2: Crear AGENTS.md raíz
- Archivo: `/AGENTS.md`
- Contenido: Project Overview, Tech Stack, Repository Structure, Source Files, Setup Commands, Code Style & Conventions, Business Rules Index, Hardware Reference, Architecture Reference, Subsystem Documentation, Protocol References, Pin Assignment Reference, Code Pattern Reference, Design Decisions Pending, Agent Guidelines
- ~80-100 líneas

## Tarea 3: Crear docs/01-business-rules.md
- 18 reglas de negocio con formato completo (Actor, Condición, Acción, Restricciones, Subsistema, Prioridad, Fuente textual)
- ~150 líneas

## Tarea 4: Crear docs/02-hardware-map.md
- Tabla periféricos: nombre, tipo, protocolo, puertos probables, función
- Tabla pines PWM (Timer, pines OC, resolución)
- Tabla pines USART
- ~100 líneas

## Tarea 5: Crear docs/03-architecture-overview.md
- Diagrama ASCII 3 capas (Aplicación, Servicios, Hardware)
- Diagrama flujo de datos
- Descripción de cada capa
- ~80 líneas

## Tarea 6: Crear docs/04-subsystem-alarm.md
- Máquina de estados alarma
- Flujo notificación serial
- Código seguridad en EEPROM
- ~80 líneas

## Tarea 7: Crear docs/05-subsystem-rfid-access.md
- Flujos: enrolamiento, borrado, validación RFID
- Control puertas (imán relé, servomotor PWM)
- Habitación juegos: verificar saldo, descontar, recargar padres
- ~120 líneas

## Tarea 8: Crear docs/06-subsystem-environment.md
- PWM dimerización iluminación
- Control temperatura con histéresis
- ~60 líneas

## Tarea 9: Crear docs/07-subsystem-remote.md
- Comandos remotos USART para horno (tiempo + temperatura)
- Control sonido remoto + volumen analógico PWM+RC
- Lista de mercado en EEPROM: agregar, consultar
- ~80 líneas

## Tarea 10: Crear docs/08-subsystem-ui.md
- Árbol de menús completo (9 entradas)
- Mapeo teclado matricial 4x4 a acciones
- Secuencia init LCD modo 4 bits
- ~80 líneas

## Tarea 11: Crear docs/09-subsystem-comm.md
- Protocolo USART: 9600 8N1
- Formato comandos/respuestas/notificaciones
- Lista completa de comandos
- ~60 líneas

## Tarea 12: Crear docs/10-protocol-spi.md
- Código referencia SPI-Maestro.ino
- Registros SPCR, SPSR, SPDR
- Pines y función spi_init/spi_transfer
- ~40 líneas

## Tarea 13: Crear docs/11-protocol-i2c.md
- Código referencia I2C-Maestro.ino
- Registros TWBR, TWSR, TWCR, TWDR
- Pines y funciones init/transmit/request
- ~40 líneas

## Tarea 14: Crear docs/12-pin-assignment.md
- Tabla asignación definitiva pines ATmega2560
- Columnas: Pin, Señal, Periférico, Notas
- ~60 líneas

## Tarea 15: Crear docs/13-code-patterns.md
- 6 patrones reutilizables con referencias a códigos de clase
- ~60 líneas

## Tarea 16: Verificar estructura completa
- Confirmar todos los archivos existen
- Confirmar enlaces en AGENTS.md son válidos
- Confirmar no contradicciones entre archivos
