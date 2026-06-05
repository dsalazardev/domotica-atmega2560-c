# Ambiente Subsystem

## Overview

Control de iluminación dimerizada (R12), temperatura con calefactor y ventilador (R13), horno remoto con temporizador (R14) y equipo de sonido remoto con volumen analógico (R15).

## Capabilities

### `iluminacion-dimerizada` (R12)

**Interfaz pública:**
- `iluminacion_iniciar()` — Configurar Timer3 Fast PWM en PE3 (OC3A) para dimerización
- `iluminacion_dimerizar(uint8_t nivel)` — nivel 0–100, mapear a duty cycle OCR3A

**Hardware:**
- Timer3: Fast PWM, 16-bit, prescaler 64, TOP=2500 (~100Hz), OC3A/PE3
- Potenciómetro en ADC1 (PF1) como entrada analógica de nivel
- Lectura ADC cada 100ms en `ambiente_actualizar()`, mapear 0–1023 → 0–100%

### `temperatura-control` (R13)

**Interfaz pública:**
- `temperatura_iniciar()` — Configurar ADC (Vref=AVcc, prescaler 128)
- `temperatura_controlar()` — Leer LM35, calcular °C, histéresis on/off

**Hardware:**
- Sensor LM35 en ADC0 (PF0): 10mV/°C, 0°C = 0V, 25°C = 0.25V
- Calefactor (relé): PC5, activo en alto
- Ventilador (relé): PC6, activo en alto
- Setpoint: 24°C (fijo, `#define TEMP_SETPOINT 24`)
- Histéresis: ±2°C
- Polling: cada 500ms en `ambiente_actualizar()`

**Lógica:**
- Si T < 22°C → encender calefactor, apagar ventilador
- Si T > 26°C → encender ventilador, apagar calefactor
- Si 22°C ≤ T ≤ 26°C → mantener estado actual

### `horno-remoto` (R14)

**Interfaz pública:**
- `horno_encender(uint16_t tiempo_seg, uint8_t temp)` — Guardar parámetros, activar relé, iniciar cuenta regresiva
- `horno_apagar(void)` — Desactivar relé, resetear estado

**Hardware:**
- Relé horno: PC7, activo en alto
- Temporización: contador regresivo usando `timer_tick` (Timer5, 1ms tick)
- `temp` se almacena pero no se usa para control PID (solo on/off con timer)

**Estado interno:**
- `horno_activo` (bool)
- `horno_temp` (uint8_t)
- `horno_tiempo_restante_ms` (unsigned long)
- Al llegar a 0 → llamar a `horno_apagar()` automáticamente

### `sonido-remoto` (R15)

**Interfaz pública:**
- `sonido_encender(void)` — Activar relé + Timer2 Fast PWM, duty 0%
- `sonido_apagar(void)` — Desactivar relé + PWM
- `sonido_volumen(uint8_t nivel)` — nivel 0–100, mapear a OCR2B

**Hardware:**
- Timer2: Fast PWM, 8-bit, sin prescaler (~62.5kHz), OC2B/PH6
- Relé sonido: PH7, activo en alto
- Señal PWM + filtro RC → tensión DC 0–5V proporcional al volumen
- Frecuencia PWM > 10kHz (62.5kHz cumple)

### `ambiente-update` (ciclo principal)

**Interfaz pública:**
- `ambiente_actualizar(void)` — Llamar desde `loop()` en Proyecto.ino cada 50ms

**Responsabilidades:**
1. Leer potenciómetro iluminación (ADC1) cada 100ms, llamar `iluminacion_dimerizar()` si cambió
2. Leer LM35 (ADC0) cada 500ms, ejecutar `temperatura_controlar()`
3. Decrementar `horno_tiempo_restante_ms` según tiempo transcurrido, apagar horno si llegó a 0
