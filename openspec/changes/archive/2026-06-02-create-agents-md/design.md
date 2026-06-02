## Context

El proyecto `domotica-atmega2560-c` requiere documentación de dominio para que agentes de IA puedan trabajar en él sin necesidad de leer el enunciado completo ni los 20,000+ líneas de datasheets. La exploración inicial identificó: 10 subsistemas, 18 reglas de negocio, 8 códigos de clase funcionales como referencia, y 9 ambigüedades de diseño. El diseño actual es esqueleto (~2% implementado).

El diseño sigue el principio de **divulgación progresiva** (Matt Pocock): el archivo raíz `AGENTS.md` es pequeño (~90 líneas) y contiene solo lo esencial para orientarse, con enlaces a archivos de dominio específicos. Un agente solo carga el archivo relevante para su tarea, manteniendo el contexto bajo.

## Goals / Non-Goals

**Goals:**
- Crear `/AGENTS.md` como punto de entrada único para agentes de IA (15 secciones, ~90 líneas)
- Crear 13 archivos de dominio en `/docs/*.md` con especificaciones detalladas (~60-150 líneas cada uno)
- Documentar las 18 reglas de negocio con formato estructurado (Actor, Condición, Acción, Restricciones, Prioridad)
- Documentar el mapa completo de hardware del ATmega2560
- Proveer diagramas de arquitectura (capas, flujo de datos)
- Catalogar patrones de código reutilizables de los 8 códigos de clase existentes

**Non-Goals:**
- No se implementa código fuente del sistema domótico
- No se modifican archivos existentes (`main.ino`, `Seguridad.h`, `Seguridad.cpp`, etc.)
- No se crean drivers, librerías ni lógica de aplicación
- No se definen asignaciones de pines definitivas (solo tentativas, a confirmar durante implementación)
- No se resuelven las ambigüedades de diseño — se documentan como pendientes

## Decisions

**Decisión 1: Divulgación progresiva (raíz pequeña + archivos de dominio)**
- Por qué: Un agente típico tiene ventana de contexto de 8K-128K tokens. Forzar a leer 1000+ líneas en cada interacción desperdicia contexto. Con divulgación progresiva, el agente lee solo `AGENTS.md` (~90 líneas) y luego carga el archivo específico que necesita.
- Alternativa considerada: Un solo archivo grande. Rechazado porque obliga a cargar toda la documentación aunque solo se necesite un subsistema.

**Decisión 2: AGENTS.md raíz con 15 secciones de referencia rápida**
- Incluye: Project Overview, Tech Stack, Repository Structure, Source Files, Setup Commands, Code Style, Business Rules Index, Hardware Reference, Architecture Reference, Subsystem Documentation, Protocol References, Pin Assignment, Code Patterns, Design Decisions Pending, Agent Guidelines.
- Cada sección es de 1-5 líneas + enlace al archivo de dominio correspondiente.

**Decisión 3: docs/01-business-rules.md con formato estructurado**
- Cada regla incluye: nombre, actor, condición, acción, restricciones, subsistema, prioridad, fuente textual del enunciado. Esto permite trazabilidad directa enunciado → documentación → implementación → pruebas.

**Decisión 4: Documentar ambigüedades como "Design Decisions Pending"**
- 9 ambigüedades identificadas del enunciado no se resuelven en esta fase. Se documentan para que cualquier agente sepa que son decisiones pendientes y no asuma valores por defecto.

**Decisión 5: Archivos de dominio separados por subsistema**
- Cada subsistema (alarma, RFID, juegos, iluminación, temperatura, horno, sonido, lista de mercado, LCD, USART) tiene su propio archivo. Esto permite carga granular: si la tarea es implementar RFID, solo se carga `docs/05-subsystem-rfid-access.md`.

**Decisión 6: Incluir referencias a códigos de clase**
- Los 8 códigos de clase en `DOCUMENTOS/CODIGO-CLASES/` contienen drivers funcionales (LCD, teclado, USART, SPI, I2C, PWM). Se referencian explícitamente para que el agente los use como base, no reinvente.

## Risks / Trade-offs

- **[Riesgo] Documentación estancada** → Si se implementa código que cambia las reglas de negocio, la documentación puede quedar desactualizada. Mitigación: incluir en AGENTS.md una sección "Agent Guidelines" que instruya a los agentes a mantener la documentación sincronizada.
- **[Riesgo] Ambigüedades no resueltas** → Si un agente no ve las "Design Decisions Pending", podría asumir valores incorrectos. Mitigación: la sección está en AGENTS.md raíz, visible siempre.
- **[Trade-off] 14 archivos vs 1 archivo** → Más archivos = más organización pero más enlaces rotos potenciales. Mitigación: tarea de verificación final que confirma todos los enlaces.
- **[Trade-off] Pines tentativos** → Los pines listados son probables (basados en códigos de clase) pero no vinculantes. Se marcan como "tentativos" para evitar commits incorrectos de hardware.
