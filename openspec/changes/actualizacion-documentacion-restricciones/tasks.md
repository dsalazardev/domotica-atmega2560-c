## 1. AGENTS.md — Secciones principales

- [x] 1.1 Actualizar Tech Stack: agregar "Arduino IDE (obligatorio para revisión)" y "Lenguaje usado como C plano — sin clases"
- [x] 1.2 Actualizar Repository Structure: mostrar árbol destino (Proyecto.ino, LCD.ino, etc.) además del actual
- [x] 1.3 Actualizar Source Files: reemplazar tabla con la nueva lista de archivos .ino destino
- [x] 1.4 Actualizar Setup Commands: agregar instrucción de apertura en Arduino IDE
- [x] 1.5 Actualizar Code Style & Conventions: prohibir clases, .h/.cpp, include guards, delays. Permitir funciones con prefijo, NOPs, millis()
- [x] 1.6 Actualizar Agent Guidelines: agregar advertencia sobre códigos de clase + guideline #10 y #11
- [x] 1.7 Crear sección "Restricciones de la Profesora (Nubia Liliana)" con cita textual, tabla de implicaciones, y nota de librerías permitidas

## 2. AGENTS.md — Verificación

- [x] 2.1 Leer AGENTS.md completo para verificar que no haya texto residual de la estructura anterior
- [x] 2.2 Confirmar que la sección de restricciones aparece antes de Code Style

## 3. docs/02-hardware-map.md

- [x] 3.1 Corregir fila LCD: cambiar "PB0-PB1 (RS, EN)" por "PA0 (RS), PA1 (EN)"
- [x] 3.2 Cambiar fila Servomotor: "PL3 (OC5A)" → "PB5 (OC1A)"

## 4. docs/04-subsystem-alarm.md

- [x] 4.1 Cambiar "delay de 20-50ms" por "temporización no-bloqueante con millis() de 20-50ms"

## 5. docs/05-subsystem-rfid-access.md

- [x] 5.1 Cambiar "PWM (Timer1 o Timer5)" por "PWM (Timer1/OC1A en PB5)"

## 6. docs/08-subsystem-ui.md

- [x] 6.1 Agregar nota en Hardware: teclado sin Keypad.h, LCD non-blocking
- [x] 6.2 Agregar nota al pie en Inicialización LCD: esperas con millis()/NOPs
- [x] 6.3 Agregar nota en Mapeo Teclado: escaneo propio sin Keypad.h

## 7. docs/12-pin-assignment.md

- [x] 7.1 Agregar fila PB5: SERVO_PWM, servomotor, OC1A
- [x] 7.2 Modificar fila PL3: quitar "/ OC5A" y mención de conflicto
- [x] 7.3 Eliminar fila duplicada PL3 como OC5A
- [x] 7.4 Actualizar advertencia inicial: agregar "(T-02 resuelto)"
- [x] 7.5 Cambiar título "Conflictos Detectados" → "Conflictos Resueltos"
- [x] 7.6 Actualizar nota final sobre T-02: marcar como resuelto

## 8. docs/13-code-patterns.md

- [x] 8.1 Agregar banner de advertencia al inicio del documento
- [x] 8.2 Agregar nota al pie en Patrón LCD: NOPs + millis() + micros()
- [x] 8.3 Agregar nota al pie en Patrón Teclado: round-robin + debounce por muestreo
- [x] 8.4 Agregar nota al pie en Patrón PWM+ADC: servo usa Timer1/PB5, no Timer5

## 9. Verificación final

- [x] 9.1 Verificar que todos los cambios de AGENTS.md se guardaron correctamente
- [x] 9.2 Verificar que los 5 archivos SIN cambios (01, 03, 06, 07, 09, 10, 11) no fueron tocados
- [x] 9.3 Confirmar que no hay referencias a `_delay_*()`, `Servo.h`, `Keypad.h`, clases, o `include/` en ningún .md
- [x] 9.4 Git status para revisar diff completo antes de commit
