## Why

El proyecto `domotica-atmega2560-c` está en estado esqueleto (~2% implementado): `main.ino` con loop vacío, clase `Seguridad` sin implementar. Cualquier agente de IA que llegue al repositorio no tiene contexto del negocio: qué hace el sistema, cuáles son las reglas de negocio, qué periféricos se usan, cómo está organizada la arquitectura, ni qué hardware se requiere. Sin este contexto, cada interacción empieza desde cero.

Se necesita una fuente única de verdad (`AGENTS.md` + documentos de dominio en `docs/`) que cualquier agente —OpenCode, GitHub Copilot, Cursor, Claude— pueda leer para entender el proyecto completo en segundos, sin depender de la memoria de conversación.

## What Changes

- Creación de `/AGENTS.md` en la raíz del proyecto como punto de entrada único para agentes de IA (~80-100 líneas)
- Creación de 13 archivos de dominio en `/docs/*.md` con divulgación progresiva:
  - Reglas de negocio (18 reglas extraídas del enunciado del proyecto)
  - Mapa de hardware (periféricos, pines, protocolos)
  - Arquitectura general (diagramas ASCII, capas, flujo de datos)
  - Especificación detallada de cada subsistema (alarma, RFID, juegos, iluminación, temperatura, horno, sonido, lista de mercado, LCD, USART)
  - Referencias de protocolos (SPI, I2C)
  - Asignación de pines del ATmega2560
  - Patrones de código reutilizables de los códigos de clase existentes
- Sin modificación de código fuente existente
- Sin creación de drivers ni lógica de aplicación

## Capabilities

### New Capabilities
- `agents-md-root`: Archivo AGENTS.md raíz como punto de entrada para agentes de IA, con 15 secciones de referencia rápida
- `docs-business-rules`: Documentación de las 18 reglas de negocio del sistema domótico extraídas del enunciado
- `docs-hardware-map`: Mapa completo de periféricos, protocolos, pines probables y función de cada uno
- `docs-architecture`: Diagramas de arquitectura de 3 capas y flujo de datos del sistema completo
- `docs-subsystem-alarm`: Especificación del subsistema de alarma de seguridad (acceso + incendio)
- `docs-subsystem-rfid-access`: Especificación del control de acceso RFID y habitación de juegos
- `docs-subsystem-environment`: Especificación del control de iluminación dimerizada y temperatura
- `docs-subsystem-remote`: Especificación de horno remoto, equipo de sonido y lista de mercado
- `docs-subsystem-ui`: Especificación de la interfaz LCD 16x2 y teclado matricial 4x4
- `docs-subsystem-comm`: Especificación del protocolo USART de comunicación con PC
- `docs-protocol-spi`: Referencia del driver SPI con ejemplos de códigos de clase
- `docs-protocol-i2c`: Referencia del driver I2C con ejemplos de códigos de clase
- `docs-pin-assignment`: Asignación definitiva de pines del ATmega2560
- `docs-code-patterns`: Patrones de código reutilizables extraídos de los 8 códigos de clase existentes

### Modified Capabilities
Ninguna — no existen specs previas en `openspec/specs/`.

## Impact

- Creación de 14 nuevos archivos (1 AGENTS.md raíz + 13 docs/*.md)
- Sin cambios en código fuente (`Proyecto/src/`, `Proyecto/include/`)
- Sin cambios en configuración de build (`platformio.ini`, `CMakeLists.txt`)
- Sin cambios en herramientas del proyecto (`openspec/`, `.opencode/`)
- Sin cambios en documentación existente (`DOCUMENTOS/`)
- Cero impacto en funcionalidad (no hay funcionalidad implementada aún)
