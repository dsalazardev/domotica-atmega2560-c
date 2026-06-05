# Lista de Mercado Subsystem

## Overview

Persistencia en EEPROM de la lista de mercado con nombre del producto y cantidad (R16-R17). La lista vive en RAM para acceso rápido durante la sesión y se sincroniza a EEPROM en cada modificación para sobrevivir a reinicios.

## Capabilities

### `lista-persistencia-eeprom` (R16-R17)

**EEPROM Layout:**

| Dirección | Tamaño | Campo |
|---|---|---|
| 0x00-0x01 | 2 bytes | CODIGO (existente) |
| 0x02 | 1 byte | RFID_CNT (existente) |
| 0x10-0x37 | 40 bytes | RFID_LIST (existente) |
| 0x50-0x59 | 10 bytes | SALDO_LIST (existente) |
| 0x60 | 1 byte | LISTA_CNT — número de productos |
| 0x61+ | n × 21 bytes | LISTA_DATA — productos almacenados secuencialmente |

Cada producto en EEPROM (21 bytes):
- Bytes 0-19: nombre (20 bytes, null-terminated, max 15 chars visibles)
- Byte 20: cantidad (uint8_t, 0-255)

Capacidad: 20 productos (definido por `LISTA_MAX`), total 420 bytes EEPROM.

**Interfaz pública:**

| Función | Comportamiento actual | Cambio |
|---|---|---|
| `lista_agregar(nombre, cantidad)` | Solo RAM | + guarda EEPROM al final |
| `lista_eliminar(indice)` | Solo RAM | + guarda EEPROM al final |
| `lista_vaciar()` | Solo RAM | + guarda EEPROM al final |
| `lista_actualizar()` | Stub vacío | Carga EEPROM → RAM en primera llamada |
| `lista_total()` | Retorna contador RAM | Sin cambios |
| `lista_obtener_nombre(idx)` | Lee RAM | Sin cambios |
| `lista_obtener_cantidad(idx)` | Lee RAM | Sin cambios |

**Flujo de datos:**

```
EEPROM ──lista_actualizar()──▶ RAM (al iniciar)
RAM    ──lista_guardar()─────▶ EEPROM (tras cada modificación)
```

**Integración:**
- `lista_actualizar()` se llama desde `setup()` en Proyecto.ino para cargar datos guardados
- No requiere cambios en el menú LCD ni en comandos USART existentes
