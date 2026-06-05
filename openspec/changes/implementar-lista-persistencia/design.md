## Context

Lista.ino ya tiene funciones de lista en RAM y funciones de EEPROM para RFID/código. La extensión es natural: agregar un área EEPROM dedicada a la lista y sincronizarla con las funciones de mutación existentes. El diseño sigue el mismo patrón de EEPROM plana (sin estructura compleja, solo bytes secuenciales).

## Goals / Non-Goals

**Goals:**
- Persistencia de la lista de mercado en EEPROM al agregar, eliminar o vaciar
- Carga automática al iniciar el sistema
- Sin cambios en la API pública
- Sin cambios en el formato de datos en RAM

**Non-Goals:**
- Parser de comandos USART (LISTA?, LISTA+, LISTA-) — se implementa en Proyecto.ino en otro cambio
- Menú LCD para lista de mercado — ya existe en Proyecto.ino
- Capacidad > 20 productos — LISTA_MAX=20 es suficiente, se puede ajustar en #define

## Decisions

| Decisión | Opción elegida | Alternativas | Razón |
|---|---|---|---|
| Formato EEPROM por producto | nombre[20] + cantidad[1] = 21 bytes | nombre[16] + cantidad[4] = 20 bytes (docs/07) | Coincide con RAM actual; cantidad uint8_t suficiente (0-255) |
| Cuándo guardar | En cada mutación (agregar/eliminar/vaciar) | Guardado periódico en loop, guardado explícito | Simple, consistente, las mutaciones son poco frecuentes |
| Carga inicial | `lista_actualizar()` en setup con flag once | Carga implícita en primera lectura | Explícito, fácil de entender, sin side effects |
| Dirección EEPROM lista | 0x60+ | 0x00 (reubicar) | No interfiere con áreas existentes (0x00-0x59), espacio suficiente |

## Risks / Trade-offs

- [Riesgo] EEPROM tiene vida útil de ~100k escrituras → Mitigación: las mutaciones de lista son poco frecuentes (decenas por día, no por segundo). 100k escrituras = décadas de uso normal.
- [Riesgo] Escritura EEPROM es lenta (~3.3ms por byte) → Mitigación: solo se escribe cuando el usuario modifica la lista, no en cada ciclo de loop. 20 productos × 21 bytes = ~70ms, imperceptible para el usuario.
