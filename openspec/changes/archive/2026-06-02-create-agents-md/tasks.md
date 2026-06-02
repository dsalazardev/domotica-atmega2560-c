## 1. Setup

- [x] 1.1 Crear carpeta `docs/` en la raíz del proyecto

## 2. Archivo raíz AGENTS.md

- [x] 2.1 Crear `/AGENTS.md` con las 15 secciones de referencia: Project Overview, Tech Stack, Repository Structure, Source Files, Setup Commands, Code Style, Business Rules Index, Hardware Reference, Architecture Reference, Subsystem Documentation, Protocol References, Pin Assignment, Code Patterns, Design Decisions Pending, Agent Guidelines

## 3. Documentación de reglas de negocio

- [x] 3.1 Crear `docs/01-business-rules.md` con las 18 reglas de negocio en formato estructurado (Actor, Condición, Acción, Restricciones, Subsistema, Prioridad, Fuente textual)

## 4. Documentación de hardware

- [x] 4.1 Crear `docs/02-hardware-map.md` con: tabla de periféricos, tabla de pines PWM, tabla de pines USART

## 5. Documentación de arquitectura

- [x] 5.1 Crear `docs/03-architecture-overview.md` con: diagrama de 3 capas (Aplicación/Servicios/Hardware), diagrama de flujo de datos, descripción de cada capa

## 6. Especificación de subsistemas

- [x] 6.1 Crear `docs/04-subsystem-alarm.md` (reglas R1-R2, máquina de estados, notificación serial, código seguridad)
- [x] 6.2 Crear `docs/05-subsystem-rfid-access.md` (reglas R3-R11, flujos enrolamiento/borrado/validación, puertas, habitación juegos)
- [x] 6.3 Crear `docs/06-subsystem-environment.md` (reglas R12-R13, PWM dimerización, control temperatura con histéresis)
- [x] 6.4 Crear `docs/07-subsystem-remote.md` (reglas R14-R17, horno, sonido, lista de mercado)
- [x] 6.5 Crear `docs/08-subsystem-ui.md` (regla R18, árbol de menús, mapeo teclado, init LCD)
- [x] 6.6 Crear `docs/09-subsystem-comm.md` (protocolo USART: 9600 8N1, comandos, respuestas, notificaciones)

## 7. Referencias técnicas

- [x] 7.1 Crear `docs/10-protocol-spi.md` (código referencia SPI-Maestro.ino, registros SPCR/SPSR/SPDR, pines PB0-PB3)
- [x] 7.2 Crear `docs/11-protocol-i2c.md` (código referencia I2C-Maestro.ino, registros TWBR/TWSR/TWCR/TWDR, pines PD0-PD1)

## 8. Asignación de pines y patrones

- [x] 8.1 Crear `docs/12-pin-assignment.md` (tabla de asignación definitiva de pines del ATmega2560)
- [x] 8.2 Crear `docs/13-code-patterns.md` (6 patrones reutilizables: LCD, teclado, USART, SPI, I2C, PWM)

## 9. Verificación

- [x] 9.1 Confirmar que los 14 archivos existen (1 AGENTS.md + 13 docs/*.md)
- [x] 9.2 Confirmar que todos los enlaces en AGENTS.md apuntan a archivos existentes
- [x] 9.3 Confirmar que no hay contradicciones entre archivos de dominio
