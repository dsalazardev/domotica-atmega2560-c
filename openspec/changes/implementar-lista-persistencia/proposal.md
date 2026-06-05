## Why

La lista de mercado actualmente solo existe en RAM volátil. Si el microcontrolador se reinicia (corte de luz, reset, reprogramación), todos los productos agregados se pierden. Con R16-R17 se requiere que la lista sea consultable remotamente y persista entre reinicios.

## What Changes

- Implementar `lista_guardar_eeprom()` (interna, static) que escribe RAM → EEPROM
- Implementar `lista_actualizar()` que carga EEPROM → RAM en primera llamada (once flag)
- Modificar `lista_agregar()`, `lista_eliminar()`, `lista_vaciar()` para persistir tras cada modificación
- Proyecto.ino: agregar `lista_actualizar()` en `setup()`
- No se modifican firmas públicas ni otros archivos

## Capabilities

### New Capabilities
- `lista-persistencia-eeprom`: Guardado/carga automática de la lista de mercado en EEPROM

### Modified Capabilities
- `lista-mercado`: Las 3 funciones de mutación ahora persisten automáticamente

## Impact

- `Proyecto/Lista.ino`: +30 líneas (guardar, cargar, modificaciones en 3 funciones)
- `Proyecto/Proyecto.ino`: +1 línea en setup
- Sin cambios en Drivers.ino, Seguridad.ino, Ambiente.ino, Timer.ino
