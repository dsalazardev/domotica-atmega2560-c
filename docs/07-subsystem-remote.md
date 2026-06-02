# Subsistema: Control Remoto — Horno, Sonido y Lista de Mercado

## Reglas Asociadas
- **R14**: Encendido remoto del horno con tiempo y temperatura
- **R15**: Encendido remoto del equipo de sonido con control de volumen
- **R16**: Lista de mercado consultable remotamente
- **R17**: Formato de la lista: nombre del producto + cantidad

## Horno Remoto

### Comando Remoto
```
PC ──USART──▶ ATmega2560: "HORNO ON 180 30"
                                   │       │
                                  temp    tiempo
                                  (°C)   (minutos)
```

### Flujo de Ejecución
```
1. Recibir comando "HORNO ON <temp> <tiempo>"
2. Validar parámetros (temp > 0, tiempo > 0)
3. Activar relé del horno
4. Iniciar timer de cuenta regresiva
5. Al alcanzar el tiempo: desactivar relé
6. Enviar confirmación por USART: "OK: horno apagado"
```

### Notas
- El horno funciona a la temperatura configurada durante el tiempo especificado
- No se especifica control PID de temperatura del horno (solo on/off con timer)
- Comando: "HORNO OFF" para apagado manual

## Equipo de Sonido

### Comando Remoto
```
PC ──USART──▶ ATmega2560: "SONIDO ON 50"
                               │
                             volumen
                               (0-100%)
```

### Flujo de Ejecución
```
1. Recibir comando "SONIDO ON <volumen>"
2. Validar volumen (0-100)
3. Activar relé del equipo de sonido
4. Generar señal analógica proporcional al volumen:
   - PWM (OCnx) + filtro RC = tensión DC proporcional
   - O: DAC externo controlado por I2C/SPI
5. Comando "SONIDO OFF": desactivar relé y señal
```

### Señal Analógica
- **Opción A**: PWM + filtro RC paso bajo → voltaje DC de 0-Vcc
  - Frecuencia PWM > 10kHz para evitar ripple audible
- **Opción B**: DAC externo (ej: MCP4921 via SPI)

## Lista de Mercado

### Estructura de Datos (EEPROM)
```
Producto[0]:  nombre[16 bytes] + cantidad[4 bytes] = 20 bytes
Producto[1]:  nombre[16 bytes] + cantidad[4 bytes] = 20 bytes
...
Producto[N-1]: ...

EEPROM disponible: 4096 bytes
Capacidad máxima: ~200 productos (PENDIENTE de definir)
```

### Comandos Remotos
```
Comando: "LISTA?"  →  Respuesta: "OK: [item1: cant1] [item2: cant2] ..."
Comando: "LISTA+"  →  Agregar ítem (siguiente línea: nombre + cantidad)
Comando: "LISTA-"  →  Borrar ítem
```

### Interacción Local (LCD + Teclado)
- Menú: Lista de Mercado → Agregar Item / Ver Lista / Limpiar Lista
- Ingreso de nombre por teclado (mapping alfabético en teclado numérico)
- Cantidad: numérico 0-9999
- Confirmación: tecla 'D' o '#' según mapeo
