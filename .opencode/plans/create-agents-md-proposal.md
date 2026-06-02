# Proposal: create-agents-md

## Why

El proyecto `domotica-atmega2560-c` está en estado esqueleto (~2% implementado): `main.ino` con loop vacío, clase `Seguridad` sin implementar. Cualquier agente de IA que llegue al repositorio no tiene contexto del negocio: qué hace el sistema, cuáles son las 18 reglas de negocio, qué periféricos se usan, cómo está organizada la arquitectura, ni qué hardware se requiere. Sin este contexto, cada interacción empieza desde cero.

Se necesita una fuente única de verdad (`AGENTS.md` + documentos de dominio en `docs/`) que cualquier agente —OpenCode, GitHub Copilot, Cursor, Claude— pueda leer para entender el proyecto completo en segundos, sin depender de la memoria de conversación.

## What Changes

- Creación de `/AGENTS.md` en la raíz del proyecto como punto de entrada único para agentes de IA (~80-100 líneas)
- Creación de 13 archivos de dominio en `/docs/*.md` con divulgación progresiva:
  - Reglas de negocio (18 reglas extraídas del enunciado)
  - Mapa de hardware (periféricos, pines, protocolos)
  - Arquitectura general (diagramas ASCII, capas, flujo de datos)
  - Especificación detallada de cada subsistema (alarma, RFID, juegos, iluminación, temperatura, horno, sonido, lista de mercado, LCD, USART)
  - Referencias de protocolos (SPI, I2C)
  - Asignación de pines del ATmega2560
  - Patrones de código reutilizables de los códigos de clase

No se modifica código fuente existente. No se implementan drivers ni lógica de negocio en esta fase.

## Capabilities

### New Capabilities
- `agents-md-root`: Archivo AGENTS.md raíz como punto de entrada para agentes
- `docs-business-rules`: Documentación de las 18 reglas de negocio del sistema domótico
- `docs-hardware-map`: Mapa completo de periféricos, protocolos y pines del ATmega2560
- `docs-architecture`: Diagramas de arquitectura y flujo de datos del sistema
- `docs-subsystem-alarm`: Especificación del subsistema de alarma de seguridad
- `docs-subsystem-rfid-access`: Especificación del control de acceso RFID y habitación de juegos
- `docs-subsystem-environment`: Especificación de control de iluminación y temperatura
- `docs-subsystem-remote`: Especificación de horno remoto, sonido y lista de mercado
- `docs-subsystem-ui`: Especificación de interfaz LCD y teclado matricial
- `docs-subsystem-comm`: Especificación del protocolo USART de comunicación
- `docs-protocol-spi`: Referencia del driver SPI con ejemplos de clase
- `docs-protocol-i2c`: Referencia del driver I2C con ejemplos de clase
- `docs-pin-assignment`: Asignación definitiva de pines del ATmega2560
- `docs-code-patterns`: Patrones de código reutilizables de los códigos de clase existentes

### Modified Capabilities
Ninguna — no existen specs previas en `openspec/specs/`.

## Impact

- Creación de 14 nuevos archivos (1 AGENTS.md + 13 docs/*.md)
- Sin cambios en código fuente (`Proyecto/`, `DOCUMENTOS/`)
- Sin cambios en configuración de build (`platformio.ini`, `CMakeLists.txt`)
- Sin cambios en herramientas (`openspec/`, `.opencode/`)
- Cero impacto en funcionalidad existente (no hay funcionalidad aún)
