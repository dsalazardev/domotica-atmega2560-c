## 1. Fetch existing tablero and clean

- [x] 1.1 Fetch the Notion page "Tablero de tareas Kanban" to discover current content and data source ID
- [x] 1.2 Query all existing pages in the database
- [x] 1.3 Delete all existing pages to ensure a clean slate

## 2. Create database schema

- [x] 2.1 Create database with all properties: Nombre (TITLE), Estado (SELECT), Hito (SELECT), Prioridad (SELECT), Subsistema (SELECT), Reglas RN (RICH_TEXT), Depende de (RELATION self), Archivos (RICH_TEXT), Código clase (RICH_TEXT), Tipo (SELECT)
- [x] 2.2 Configure Estado SELECT options: Sin estado (gray), En progreso (blue), En revisión (yellow), Hecho (green)
- [x] 2.3 Configure Hito SELECT options: Fase 1 - Infraestructura through Fase 7 - Ambiente
- [x] 2.4 Configure Prioridad SELECT options: Alta (red), Media (yellow), Baja (green)
- [x] 2.5 Configure Subsistema SELECT options: all 11 subsystems (Interfaz LCD, Comunicación USART, Seguridad, Hardware, Control Acceso RFID, Habitación Juegos, Horno Remoto, Equipo Sonido, Lista Mercado, Iluminación, Temperatura)
- [x] 2.6 Configure Tipo SELECT options: 🧑‍💻 HU (blue), 🔧 Tarea Técnica (gray), 💡 Decisión de Diseño (orange)
- [x] 2.7 Add self-relation for Depende de property (two-way with synced properties)

## 3. Create Hito 1 cards — Infraestructura

- [x] 3.1 Create HU-01: Driver LCD 4-bit (Alta, Interfaz LCD, R18, Fase 1)
- [x] 3.2 Create HU-02: Driver Teclado Matricial 4x4 (Alta, Interfaz LCD, R18, Fase 1)
- [x] 3.3 Create HU-03: Sistema de Menús Navegable (Alta, Interfaz LCD, R18, Fase 1)
- [x] 3.4 Create T-01: Resolver conflicto pines LCD_EN vs SPI_SCK (Alta, Hardware, Fase 1)
- [x] 3.5 Set dependency: HU-03 depends on HU-01, HU-02
- [x] 3.6 Set dependency: HU-01 depends on T-01

## 4. Create Hito 2 cards — Comunicación

- [x] 4.1 Create HU-04: Driver USART TX/RX por Interrupción (Alta, Comunicación USART, R1/R14-R17, Fase 2)
- [x] 4.2 Create HU-05: Parser de Comandos USART (Alta, Comunicación USART, R1/R14-R17, Fase 2)
- [x] 4.3 Set dependency: HU-05 depends on HU-04

## 5. Create Hito 3 cards — Seguridad

- [x] 5.1 Create HU-06: Módulo EEPROM Manager (Alta, Seguridad, R2, Fase 3)
- [x] 5.2 Create HU-07: Código de Seguridad (Alta, Seguridad, R2, Fase 3)
- [x] 5.3 Create HU-08: Sistema de Alarma Dual (Alta, Seguridad, R1/R2, Fase 3)
- [x] 5.4 Set dependency: HU-07 depends on HU-06, HU-03
- [x] 5.5 Set dependency: HU-08 depends on HU-07, HU-04, HU-03

## 6. Create Hito 4 cards — Acceso RFID

- [x] 6.1 Create HU-09: Driver SPI Maestro (Alta, Control de Acceso RFID, R3-R11, Fase 4)
- [x] 6.2 Create HU-10: Lector RFID MFRC522 (Alta, Control de Acceso RFID, R3-R8, Fase 4)
- [x] 6.3 Create HU-11: Enrolamiento y Borrado de Personas (Alta, Control de Acceso RFID, R4/R5, Fase 4)
- [x] 6.4 Create HU-12: Control de Puertas Imán + Servomotor (Alta, Control de Acceso RFID, R6-R8, Fase 4)
- [x] 6.5 Create T-02: Resolver conflicto pines TEC_FILA3 vs OC5A (Alta, Hardware, Fase 4)
- [x] 6.6 Set dependency: HU-09 depends on T-01
- [x] 6.7 Set dependency: HU-10 depends on HU-09
- [x] 6.8 Set dependency: HU-11 depends on HU-10, HU-07, HU-06, HU-03
- [x] 6.9 Set dependency: HU-12 depends on HU-11, T-02

## 7. Create Hito 5 cards — Juegos

- [x] 7.1 Create HU-13: Control Acceso Habitación de Juegos (Alta, Habitación de Juegos, R9/R10, Fase 5)
- [x] 7.2 Create HU-14: Recarga de Accesos por Padres (Alta, Habitación de Juegos, R11, Fase 5)
- [x] 7.3 Set dependency: HU-13 depends on HU-10, HU-03
- [x] 7.4 Set dependency: HU-14 depends on HU-13, HU-07, HU-03

## 8. Create Hito 6 cards — Control Remoto

- [x] 8.1 Create HU-15: Horno Remoto (Media, Horno Remoto, R14, Fase 6)
- [x] 8.2 Create HU-16: Equipo de Sonido Remoto (Media, Equipo de Sonido, R15, Fase 6)
- [x] 8.3 Create HU-17: Lista de Mercado (Baja, Lista de Mercado, R16/R17, Fase 6)
- [x] 8.4 Set dependency: HU-15 depends on HU-05, HU-03
- [x] 8.5 Set dependency: HU-16 depends on HU-05, HU-03
- [x] 8.6 Set dependency: HU-17 depends on HU-06, HU-05, HU-03

## 9. Create Hito 7 cards — Ambiente

- [x] 9.1 Create HU-18: Driver PWM + ADC (Media, Hardware, R12/R13, Fase 7)
- [x] 9.2 Create HU-19: Control Iluminación Dimerizada (Media, Iluminación, R12, Fase 7)
- [x] 9.3 Create HU-20: Control de Temperatura (Media, Temperatura, R13, Fase 7)
- [x] 9.4 Set dependency: HU-19 depends on HU-18, HU-03
- [x] 9.5 Set dependency: HU-20 depends on HU-18, HU-03

## 10. Create Decisiones de Diseño cards

- [x] 10.1 Create DD-01: Definir formato código seguridad (Alta, Seguridad, Fase 3)
- [x] 10.2 Create DD-02: Definir protocolo módulo RFID (Alta, Control de Acceso RFID, Fase 4)
- [x] 10.3 Create DD-03: Definir datos en tarjeta RFID (Alta, Habitación de Juegos, Fase 5)
- [x] 10.4 Create DD-04: Definir sensor de temperatura (Media, Temperatura, Fase 7)

## 11. Create views

- [x] 11.1 Create Kanban view grouped by Estado
- [x] 11.2 Create "Por Hito" view grouped by Hito
- [x] 11.3 Create "Por Prioridad" view grouped by Prioridad
- [x] 11.4 Create "Tabla completa" table view for data overview

## 12. Verify

- [x] 12.1 Fetch the database and verify all 26 cards exist with correct properties
- [x] 12.2 Verify dependency relations are correctly linked
- [x] 12.3 Verify all 4 views display correctly
