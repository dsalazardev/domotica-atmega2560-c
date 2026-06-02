# Subsistema: Iluminación y Temperatura

## Reglas Asociadas
- **R12**: Control de iluminación dimerizada (PWM)
- **R13**: Control de temperatura con calefactor y ventilador

## Iluminación Dimerizada

### Control PWM
```
Valor ADC (potenciómetro) o teclado numérico
        │
        ▼
Mapear a rango 0-255 (8-bit) o 0-65535 (16-bit)
        │
        ▼
Ajustar OCRnx del timer correspondiente
        │
        ▼
Señal PWM → LED / carga
```

### Interfaz de Usuario
- Ajuste mediante teclado (subir/bajar) o potenciómetro (ADC)
- Barra de progreso en LCD: "███████░░░ 70%"
- Valor numérico opcional: "Iluminación: 70%"

## Control de Temperatura

### Control con Histéresis (On/Off)
```
Setpoint: T_deseada (configurable por usuario)
Histéresis: ±2°C

Leer sensor temperatura
        │
        ▼
¿T < T_deseada - 2°C? ──SÍ──▶ Encender calefactor, apagar ventilador
        │
        NO
        ▼
¿T > T_deseada + 2°C? ──SÍ──▶ Encender ventilador, apagar calefactor
        │
        NO
        ▼
¿T dentro de histéresis? ──SÍ──▶ Mantener estado actual
```

### Sensor de Temperatura
- Tipo: **PENDIENTE** — ver Design Decisions Pending en AGENTS.md
- Opciones: LM35 (ADC), DHT11/22 (digital 1-Wire/I2C), DS18B20 (1-Wire)
- Si LM35: lectura ADC → convertir a voltaje → calcular temperatura (10mV/°C)

### Control Proporcional (opcional)
Si se requiere precisión: usar PWM para controlar potencia de calefactor proporcionalmente a la diferencia de temperatura (control P o PID).
