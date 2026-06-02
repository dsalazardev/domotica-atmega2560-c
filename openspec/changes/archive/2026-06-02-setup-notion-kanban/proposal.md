## Why

El proyecto domótica-atmega2560-c tiene 18 reglas de negocio, 10 subsistemas, y ~26 tareas de implementación planificadas, pero actualmente solo existe documentación en MD y un esqueleto de código. No hay un sistema centralizado para visualizar el progreso, asignar prioridades, gestionar dependencias entre tareas, o hacer seguimiento visual. Sin un gestor de tareas, el desarrollo avanza a ciegas: no se sabe qué está bloqueado, qué sigue, ni quién depende de qué.

Un tablero Kanban en Notion resuelve esto: permite ver el flujo de trabajo completo (Sin estado → En progreso → En revisión → Hecho), organizar tareas por hito y prioridad, y gestionar dependencias entre Historias de Usuario de forma nativa.

## What Changes

- Crear un tablero Kanban en Notion con 7 hitos (Fase 1-7) alineados con la estrategia de implementación definida en la exploración
- Crear ~26 tarjetas (HU, Tareas Técnicas, Decisiones de Diseño) con propiedades completas: prioridad, subsistema, reglas de negocio, dependencias, archivos, código de clase de referencia
- Establecer relaciones de dependencia entre tarjetas (quién necesita qué para arrancar)
- Crear vistas alternativas: por Hito (agrupado), por Prioridad (agrupado)
- Eliminar tareas existentes en el tablero si las hay (limpiar antes de poblar)
- No hay cambios en código fuente, documentación del proyecto, ni infraestructura de build

## Capabilities

### New Capabilities
- `notion-kanban-board`: Tablero Kanban en Notion con propiedades, columnas, vistas y relaciones entre tareas para gestionar el desarrollo del proyecto domótica

### Modified Capabilities

Ninguna — no existen specs previas en este proyecto.

## Impact

- **Sistema externo**: Se crea/actualiza contenido en Notion (tablero Kanban). No afecta al código embebido ni al build de PlatformIO.
- **Flujo de trabajo**: El equipo/desarrollador usará Notion como fuente de verdad para saber qué hacer a continuación. Las tareas se mueven entre columnas manualmente.
- **Documentación**: Las decisiones de diseño capturadas como tarjetas "Decisión de Diseño" en el Kanban reemplazan la necesidad de documentos separados para pendientes.
