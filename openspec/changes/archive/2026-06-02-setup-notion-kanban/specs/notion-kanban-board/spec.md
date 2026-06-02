## ADDED Requirements

### Requirement: Tablero Kanban en Notion
The system SHALL provide a Kanban board in Notion to track the implementation progress of the domotica-atmega2560-c project. The board SHALL be a single Notion database with properties, views, and task cards.

#### Scenario: Board exists and is accessible
- **WHEN** the user opens the Notion page at the configured URL
- **THEN** the board SHALL display as a database with Kanban, Hito, and Prioridad views

#### Scenario: Board properties are complete
- **WHEN** inspecting the database schema
- **THEN** the following properties SHALL exist: Nombre (TITLE), Estado (SELECT), Hito (SELECT), Prioridad (SELECT), Subsistema (SELECT), Reglas RN (RICH_TEXT), Depende de (RELATION self-referencing), Archivos (RICH_TEXT), Código clase (RICH_TEXT), Tipo (SELECT)

### Requirement: Estado columnas predefinidas
The Estado property SHALL have exactly 4 options: Sin estado (gray), En progreso (blue), En revisión (yellow), Hecho (green). These SHALL be the columns of the Kanban view.

#### Scenario: Default state for new tasks
- **WHEN** a new task card is created
- **THEN** its Estado SHALL default to "Sin estado"

### Requirement: Hito property with 7 phases
The Hito property SHALL have exactly 7 options matching the implementation phases: Fase 1 - Infraestructura, Fase 2 - Comunicación, Fase 3 - Seguridad, Fase 4 - Acceso, Fase 5 - Juegos, Fase 6 - Remotos, Fase 7 - Ambiente.

#### Scenario: Each task belongs to exactly one phase
- **WHEN** a task is created
- **THEN** it SHALL be assigned to exactly one Hito value

### Requirement: Prioridad con colores
The Prioridad property SHALL have 3 options: Alta (red), Media (yellow), Baja (green).

#### Scenario: Priority color coding
- **WHEN** viewing the Kanban board
- **THEN** high priority tasks SHALL be visually distinguishable by red color, medium by yellow, low by green

### Requirement: Tipo de tarjeta con emojis y colores
The Tipo property SHALL have 3 options: 🧑‍💻 HU (blue), 🔧 Tarea Técnica (gray), 💡 Decisión de Diseño (orange).

#### Scenario: Type emoji is visible
- **WHEN** viewing a task card in any view
- **THEN** the Tipo property SHALL display the corresponding emoji prefix and color

### Requirement: Dependencias entre tarjetas
The Depende de property SHALL be a Relation type that references the same database (self-referencing), allowing bidirectional dependency links between task cards.

#### Scenario: Dependency link is navigable
- **WHEN** clicking a dependency link on a task card
- **THEN** the user SHALL navigate directly to the related task card

### Requirement: ~26 task cards created
The system SHALL contain approximately 26 task cards representing all planned work: 20 HU (Historias de Usuario), 2 Tareas Técnicas, 4 Decisiones de Diseño.

#### Scenario: All tasks from the implementation plan exist
- **WHEN** viewing the database in table view
- **THEN** all tasks listed in the implementation plan SHALL exist as cards with their properties populated

### Requirement: Existing tasks cleaned before population
Before creating new task cards, any existing cards in the database SHALL be removed.

#### Scenario: Board is clean before population
- **WHEN** the setup script runs
- **THEN** it SHALL first query and delete all existing pages in the database before creating new ones
