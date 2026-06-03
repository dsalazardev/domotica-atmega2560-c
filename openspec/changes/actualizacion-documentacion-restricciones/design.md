## Context

14 archivos .md conforman la documentación del proyecto. Tras las restricciones de la profesora (30% del proyecto cambió: estructura, timing, periféricos), 7 de esos archivos contienen información desactualizada o incorrecta. El AGENTS.md — punto de entrada del proyecto — todavía describe una estructura PlatformIO con clases C++ y delays bloqueantes.

## Goals / Non-Goals

**Goals:**
- Actualizar los 7 archivos que contienen información desactualizada
- Agregar sección de restricciones de la profesora en AGENTS.md (fuente central)
- Corregir el error histórico en hardware-map.md (pines LCD incorrectos)
- Marcar el conflicto PL3 como resuelto en pin-assignment.md
- Agregar advertencias en code-patterns.md sobre delays/librerías prohibidas

**Non-Goals:**
- No se cambia el contenido técnico correcto (business rules, arquitectura, protocolos SPI/I2C)
- No se crean nuevos archivos .md fuera de los 14 existentes
- No se toca código fuente
- No se modifican los códigos de clase de referencia

## Decisions

### D1: AGENTS.md como fuente de verdad central
Todas las restricciones de la profesora se documentan UNA vez en AGENTS.md (sección "Restricciones de la Profesora"). Los demás docs referencian esa sección en lugar de duplicar el texto. Esto evita divergencia si las restricciones cambian.

### D2: Banner en 13-code-patterns.md vs modificar cada patrón
En lugar de reescribir los patrones (que son códigos de clase de referencia que los estudiantes escribieron), se agrega un banner de advertencia al inicio y notas al pie en los 3 patrones afectados (LCD, teclado, PWM). Los patrones se conservan como referencia académica.

### D3: Corrección del error de pines LCD en 02-hardware-map.md
El documento dice "PB0-PB1 (RS, EN)" pero el código real usa PA0 (RS) y PA1 (EN). Es un error histórico que existía antes de las restricciones. Se corrige ahora porque forma parte de la misma ronda de actualizaciones.

Alternativa considerada: dejar el error y corregirlo después. Rechazado — si un futuro lector usa hardware-map.md para cablear el LCD, lo va a cablear mal.

### D4: pin-assignment.md pasa de "Conflictos Detectados" a "Conflictos Resueltos"
El conflicto TEC_FILA3 vs OC5A estaba documentado como pendiente desde el inicio del proyecto. Con la decisión de mover el servo a PB5/OC1A, el conflicto se resuelve. La documentación debe reflejarlo.

## Riesgos / Trade-offs

| Riesgo | Mitigación |
|---|---|
| AGENTS.md modificado en sesión anterior puede tener conflictos con esta actualización | Leer AGENTS.md actual antes de editar. La sección "Restricciones de la Profesora" ya existe parcialmente — unificar con la nueva versión |
| 5 archivos sin cambios pueden quedar "invisibles" para quien lea el diff | Incluir en proposal.md la lista explícita de "Archivos SIN cambios" |
| El banner en code-patterns.md puede pasar desapercibido | Ponerlo como bloque de cita (> ⚠️) al inicio del documento, visible sin scroll |
