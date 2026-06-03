# Subsistema: Control de Acceso RFID + Habitación de Juegos

## Reglas Asociadas
- **R3**: Acceso a vivienda mediante RFID
- **R4**: Enrolamiento de nuevas personas
- **R5**: Borrado de personas existentes
- **R6**: Dos puntos de acceso (principal + garaje)
- **R7**: Puerta principal controlada por imán
- **R8**: Puerta garaje controlada por servomotor
- **R9**: Acceso habitación juegos con cuota de usos
- **R10**: Descuento de acceso por ingreso
- **R11**: Carga de accesos por los padres

## Flujo de Enrolamiento

```
1. Admin autenticado (ingresa código de seguridad)
2. Menú: RFID → Enrolar Persona
3. LCD muestra: "Acerca tarjeta..."
4. Leer UID de tarjeta RFID via SPI
5. Almacenar UID en EEPROM + posición en lista
6. LCD muestra: "Persona enrolada: [UID]"
```

## Flujo de Borrado

```
1. Admin autenticado (ingresa código de seguridad)
2. Menú: RFID → Borrar Persona
3. LCD muestra lista de personas enroladas
4. Admin selecciona persona a borrar
5. Eliminar UID de EEPROM
6. LCD muestra: "Persona eliminada"
```

## Flujo de Acceso (puerta principal o garaje)

```
1. Persona acerca tarjeta RFID al lector
2. Leer UID via SPI
3. Buscar UID en EEPROM
   ├── No encontrado: LCD muestra "Acceso denegado"
   └── Encontrado:
       ├── ¿Puerta principal? → Activar relé imán por 3 segundos
       └── ¿Puerta garaje? → Activar servomotor (PWM ángulo 90°)
```

## Habitación de Juegos

```
1. Niño acerca tarjeta RFID
2. Leer UID + datos de tarjeta (saldo de usos)
3. Verificar:
   ├── UID en lista blanca (persona autorizada)?
   ├── Saldo > 0?
   │   Sí: 
   │      ├── LCD: "Bienvenido! Te quedan N usos"
   │      ├── Decrementar saldo en tarjeta (escribir RFID)
   │      └── Abrir puerta juegos
   │   No:
   │      └── LCD: "Sin accesos. Pide a tus padres"
   └── No autorizado: LCD: "Acceso denegado"
```

## Carga de Accesos (Padres)

```
1. Admin autenticado (padre ingresa código)
2. Menú: Hab. Juegos → Recargar Accesos
3. LCD: "Acerca tarjeta del hijo"
4. Leer tarjeta → mostrar saldo actual
5. Ingresar nuevo saldo via teclado
6. Escribir nuevo saldo en tarjeta RFID
7. LCD: "Recarga exitosa. Saldo: N usos"
```

## Control de Puertas

| Puerta | Actuador | Control | Temporización |
|---|---|---|---|
| Principal | Imán (relé) | GPIO HIGH/LOW | 3 segundos (luego liberar) |
| Garaje | Servomotor | PWM (Timer1/OC1A en PB5) | Ángulo 0°→90°, esperar, 90°→0° |
