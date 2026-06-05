## 1. Definir constantes EEPROM para lista

- [ ] 1.1 Agregar `#define EEPROM_LISTA_CNT  0x60`
- [ ] 1.2 Agregar `#define EEPROM_LISTA_DATA 0x61`

## 2. Implementar guardado EEPROM

- [ ] 2.1 Crear función static `lista_guardar_eeprom()` que escribe `total_productos` en EEPROM_LISTA_CNT
- [ ] 2.2 Iterar i=0..total_productos: escribir nombre[20] + cantidad[1] en EEPROM_LISTA_DATA + i*21

## 3. Implementar carga EEPROM

- [ ] 3.1 En `lista_actualizar()`: si `lista_cargada==false`, leer `total_productos` desde EEPROM_LISTA_CNT
- [ ] 3.2 Para cada producto: leer nombre[20] + cantidad[1] desde EEPROM_LISTA_DATA + i*21
- [ ] 3.3 Marcar `lista_cargada = true` al finalizar

## 4. Integrar en funciones de mutación

- [ ] 4.1 En `lista_agregar()`: agregar llamada a `lista_guardar_eeprom()` al final
- [ ] 4.2 En `lista_eliminar()`: agregar llamada a `lista_guardar_eeprom()` al final
- [ ] 4.3 En `lista_vaciar()`: agregar llamada a `lista_guardar_eeprom()` al final

## 5. Integrar en Proyecto.ino

- [ ] 5.1 Agregar `lista_actualizar();` en `setup()` después de `rfid_init()`

## 6. Compilar y probar

- [ ] 6.1 Compilar con Arduino CLI — 0 errores, 0 warnings
- [ ] 6.2 Verificar que el código es plano (.ino, sin clases, sin `_delay_*()`)
- [ ] 6.3 Verificar que los 6 archivos .ino suman < 256KB Flash y < 8KB RAM
