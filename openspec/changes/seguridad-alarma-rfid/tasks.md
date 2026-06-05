## 1. Agregar SPI a Drivers.ino

- [ ] 1.1 Agregar `spi_master_init()` con SS=PB0, SCK=PB1, MOSI=PB2, MISO=PB3 (desde SPI-Maestro.ino)
- [ ] 1.2 Agregar `spi_master_transmit(char data)` (desde SPI-Maestro.ino)
- [ ] 1.3 Agregar `spi_master_receive()` para lectura de datos

## 2. Configurar Timer1 para servomotor

- [ ] 2.1 Configurar Timer1/OC1A en PB5 en modo Fast PWM con ICR1 como TOP
- [ ] 2.2 Calcular valores OCR1A para ángulos 0° (~1ms) y 90° (~1.5ms) a 50Hz
- [ ] 2.3 Implementar `servo_abrir()` y `servo_cerrar()` con temporización no-bloqueante (contador tick)

## 3. Implementar lógica de alarma (R1-R2)

- [ ] 3.1 Implementar constantes de pines para sensores reed (puertas/ventanas) y humo
- [ ] 3.2 Implementar máquina de estados de alarma: Desactivada → Activada → Disparada
- [ ] 3.3 Implementar `alarma_verificar_codigo()` con código 4 dígitos en EEPROM
- [ ] 3.4 Implementar `alarma_activar()` y `alarma_desactivar()` con validación de código
- [ ] 3.5 Implementar `alarma_actualizar()` con polling de sensores cada 100ms
- [ ] 3.6 Implementar notificación por USART: "ALERTA: Intrusion/Humo en [sensor]"
- [ ] 3.7 Implementar cambio de código de seguridad en menú

## 4. Implementar comunicación RFID (R3-R5)

- [ ] 4.1 Implementar rutina de lectura de UID via SPI (comandos MFRC522)
- [ ] 4.2 Implementar `rfid_enrolar()` con almacenamiento de UID en EEPROM
- [ ] 4.3 Implementar `rfid_borrar()` con eliminación de UID de EEPROM
- [ ] 4.4 Implementar `rfid_validar_uid()` contra lista en EEPROM
- [ ] 4.5 Implementar listado de tarjetas enroladas para selección en borrado

## 5. Implementar control de puertas (R6-R8, R10-R11)

- [ ] 5.1 Implementar apertura de puerta principal (imán GPIO, 3s no-bloqueante)
- [ ] 5.2 Implementar apertura de puerta garaje (servomotor Timer1)
- [ ] 5.3 Integrar validación RFID con selección de puerta (principal/garaje)

## 6. Implementar habitación de juegos (R9-R11)

- [ ] 6.1 Implementar estructura en EEPROM: tabla UID → saldo de usos
- [ ] 6.2 Implementar descuento de 1 uso por ingreso (actualizar EEPROM para el UID)
- [ ] 6.3 Implementar `juegos_cargar_accesos()` con autenticación de padres
- [ ] 6.4 Implementar `juegos_consultar_saldo()` leyendo desde EEPROM sin modificar contador

## 7. Integrar menú LCD + teclado

- [ ] 7.1 Implementar menú principal: 1.Seguridad 2.Acceso 3.Juegos
- [ ] 7.2 Implementar submenú de seguridad: Activar / Desactivar / Cambiar código
- [ ] 7.3 Implementar submenú de acceso: Enrolar / Borrar / Abrir puerta
- [ ] 7.4 Implementar submenú de juegos: Ingresar / Recargar / Consultar saldo
- [ ] 7.5 Integrar todo en `Seguridad.ino` con las funciones prefijadas (`alarma_`, `rfid_`, `juegos_`)

## 8. Compilar y probar

- [ ] 8.1 Compilar con `pio run` — 0 errores, 0 warnings
- [ ] 8.2 Verificar LCD muestra menú correctamente
- [ ] 8.3 Verificar teclado navega menú y selecciona opciones
- [ ] 8.4 Verificar flujo completo: enrolar tarjeta → cerrar puerta → acercar tarjeta → abrir puerta
- [ ] 8.5 Verificar flujo juegos: recargar accesos → ingresar → descontar → denegar al llegar a 0
- [ ] 8.6 Verificar notificación USART en condición de alarma
