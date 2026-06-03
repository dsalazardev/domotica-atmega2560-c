# Subsistema: Alarma de Seguridad

## Reglas Asociadas
- **R1**: Alarma dual (acceso + incendio) con notificación serial
- **R2**: Activación/desactivación solo con código

## Máquina de Estados

```
        ┌─────────┐   código correcto   ┌─────────┐
        │         │────────────────────▶│         │
        │Desactiv.│                     │ Activada│
        │         │◀────────────────────│         │
        └─────────┘   código correcto   └────┬────┘
              ▲                              │
              │                    sensor disparado
              │                              ▼
              │                      ┌────────────┐
              │       código         │           │
              └──────────────────────│ Disparada  │
                      correcto       │ (notifica) │
                                     └────────────┘
```

## Flujo de Notificación

```
Sensor disparado (INT/PCINT o polling)
    │
    ▼
¿Alarma activada? ──NO──▶ Ignorar
    │
    SÍ
    ▼
Enviar por USART: "ALERTA: intrusión [sensor]" o "ALERTA: humo [sensor]"
    │
    ▼
LCD muestra mensaje de alarma
    │
    ▼
Esperar código de desactivación
```

## Código de Seguridad

- Almacenado en EEPROM del ATmega2560 (4KB disponibles)
- Ingresado mediante teclado matricial 4x4
- Validado contra valor almacenado
- Cambiable desde menú de configuración (requiere código actual + código nuevo dos veces)
- Formato: pendiente de definición (ver Design Decisions Pending en AGENTS.md)

## Interrupciones

- Sensores de puerta/ventana: usar INT0-7 o PCINT para detección inmediata
- Sensores de humo: polling periódico o PCINT según diseño
- Debounce por software (temporización no-bloqueante con millis() de 20-50ms) para sensores reed
