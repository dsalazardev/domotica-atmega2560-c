## Context

La exploración del proyecto domótica-atmega2560-c identificó 18 reglas de negocio, 7 hitos de desarrollo, 9 decisiones de diseño pendientes y ~26 unidades de trabajo. Actualmente no hay un sistema de seguimiento — solo archivos MD estáticos. Se necesita un tablero Kanban en Notion como herramienta de gestión visual que refleje la estrategia de implementación definida.

El MCP de Notion está disponible en OpenCode, permitiendo crear y gestionar el tablero programáticamente.

## Goals / Non-Goals

**Goals:**
- Crear un tablero Kanban en Notion como fuente de verdad para el seguimiento del proyecto
- Modelar propiedades: Estado, Hito, Prioridad, Subsistema, Reglas RN, Depende de, Archivos, Código clase, Tipo
- Crear ~26 tarjetas que representen cada HU, tarea técnica y decisión de diseño identificadas
- Establecer relaciones de dependencia entre tarjetas
- Crear vistas alternativas (por Hito, por Prioridad) además de la vista Kanban por Estado
- Asignar colores y emojis para mejorar legibilidad visual

**Non-Goals:**
- No se implementa integración con GitHub ni automatización de movimientos
- No se migran datos existentes (no hay datos previos que migrar)
- No se modifican archivos del proyecto de código (solo Notion)

## Decisions

| Decisión | Opción Elegida | Alternativas | Justificación |
|---|---|---|---|
| **Motor de BD** | Notion Database | Trello, Jira, GitHub Projects | MCP ya funcional, mismo ecosistema que el resto de la documentación del proyecto |
| **Estructura** | Single database con vistas | Múltiples databases vinculadas | Una sola database es más simple de mantener; las vistas filtran/agrupan |
| **Propiedad "Depende de"** | Relación (Relation type) a la misma database | Texto libre | Permite navegación directa entre tarjetas relacionadas y vista de dependencias |
| **Colores de Estado** | Sin estado=gris, En progreso=azul, En revisión=amarillo, Hecho=verde | Múltiples paletas | Sigue convención visual estándar Kanban |
| **Prefijo IDs** | HU-NN / T-NN / DD-NN | Solo nombre descriptivo | Facilita referencias cruzadas entre tarjetas y en discusiones |
| **Hitos vs Fases** | 7 fases (Fase 1-7) alineadas a los hitos de implementación | Hitos temporales con fechas | No tenemos fechas definidas; las fases representan orden lógico |
| **Emojis en Tipo** | HU 🧑‍💻, Tarea Técnica 🔧, Decisión 💡 | Solo texto | Mejora escaneabilidad visual del tablero |

## Riesgos / Trade-offs

| Riesgo | Mitigación |
|---|---|
| Notion MCP puede fallar al crear relaciones entre páginas de la misma database | Crear database primero con todas las propiedades, luego crear páginas, luego establecer relaciones con update_page |
| Timeout de Notion MCP al crear 26 tarjetas en secuencia | Las operaciones son independientes; si una falla, se reintenta individualmente |
| Duplicidad si el tablero ya tiene contenido | Paso explícito de limpieza (eliminar páginas existentes) antes de poblar |
| Los colores Select en Notion tienen nombres específicos | Mapear explícitamente: Alta=red, Media=yellow, Baja=green, HU=blue, Tarea Técnica=gray, Decisión=orange |

## Estructura del Tablero

```
Database: "Tablero de tareas Kanban"
├── Propiedades:
│   ├── Nombre (TITLE) — Nombre de la tarea
│   ├── Estado (SELECT) — Sin estado | En progreso | En revisión | Hecho
│   ├── Hito (SELECT) — Fase 1 - Infraestructura | ... | Fase 7 - Ambiente
│   ├── Prioridad (SELECT) — Alta(🔴 rojo) | Media(🟡 amarillo) | Baja(🟢 verde)
│   ├── Subsistema (SELECT) — 11 opciones
│   ├── Reglas RN (RICH_TEXT) — Ej: "R1, R2"
│   ├── Depende de (RELATION) — Auto-referencia a esta misma database
│   ├── Archivos (RICH_TEXT) — Archivos a crear/modificar
│   ├── Código clase (RICH_TEXT) — Referencia a código de clase
│   └── Tipo (SELECT) — 🧑‍💻 HU(blue) | 🔧 Tarea Técnica(gray) | 💡 Decisión de Diseño(orange)
│
├── Vistas:
│   ├── Kanban (agrupado por Estado)
│   ├── Por Hito (agrupado por Hito)
│   └── Por Prioridad (agrupado por Prioridad)
```

### Valores de Subsistema

| Subsistema | Reglas |
|---|---|
| Interfaz LCD | R18 |
| Comunicación USART | R1, R14-R17 |
| Seguridad | R1, R2 |
| Hardware | — (tareas técnicas de pines) |
| Control de Acceso RFID | R3-R8 |
| Habitación de Juegos | R9-R11 |
| Horno Remoto | R14 |
| Equipo de Sonido | R15 |
| Lista de Mercado | R16-R17 |
| Iluminación | R12 |
| Temperatura | R13 |
