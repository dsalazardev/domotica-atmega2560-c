## 1. Implementar iluminación dimerizada (R12)

- [ ] 1.1 Definir constantes: ILUM_PIN PE3, ILUM_DDR DDRE, ILUM_PORT PORTE, ILUM_ADC_CH 1
- [ ] 1.2 Implementar `iluminacion_iniciar()`: Timer3 Fast PWM, prescaler 64, TOP=2500, OCR3A=0, PE3 como salida
- [ ] 1.3 Implementar `iluminacion_dimerizar(uint8_t nivel)`: mapear 0–100 → 0–2500 en OCR3A
- [ ] 1.4 Integrar lectura ADC1 (potenciómetro) en `ambiente_actualizar()` cada 100ms, promediar 4 muestras, mapear a nivel 0–100

## 2. Implementar control de temperatura (R13)

- [ ] 2.1 Definir constantes: TEMP_ADC_CH 0, TEMP_SETPOINT 24, TEMP_HISTERESIS 2, CALEFACTOR_PIN PC5, VENTILADOR_PIN PC6, sus DDR/PORT
- [ ] 2.2 Implementar `temperatura_iniciar()`: ADC Vref=AVcc, prescaler 128, habilitar ADC
- [ ] 2.3 Implementar `temperatura_controlar()`: leer ADC0, convertir a °C (ADC/2.046), ejecutar histéresis on/off
- [ ] 2.4 Integrar en `ambiente_actualizar()` cada 500ms

## 3. Implementar horno remoto (R14)

- [ ] 3.1 Definir constantes: HORNO_PIN PC7, HORNO_DDR DDRC, HORNO_PORT PORTC
- [ ] 3.2 Variables static: `horno_activo`, `horno_temp`, `horno_ultimo_tick` (unsigned long para millis())
- [ ] 3.3 Implementar `horno_encender(uint16_t tiempo_seg, uint8_t temp)`: activar relé, guardar tiempo restante en ms
- [ ] 3.4 Implementar `horno_apagar()`: desactivar relé, limpiar flags
- [ ] 3.5 Integrar cuenta regresiva en `ambiente_actualizar()`: restar tiempo transcurrido desde última llamada, apagar al llegar a 0

## 4. Implementar sonido remoto (R15)

- [ ] 4.1 Definir constantes: SONIDO_PWM_PIN PH6, SONIDO_DDR DDRH, SONIDO_PORT PORTH, SONIDO_RELE_PIN PH7
- [ ] 4.2 Implementar `sonido_encender()`: Timer2 Fast PWM, no prescaler, WGM2=Fast, COM2B1=1 (OC2B clear on match), OCR2B=0
- [ ] 4.3 Implementar `sonido_apagar()`: desactivar Timer2 (TCCR2B=0), apagar relé
- [ ] 4.4 Implementar `sonido_volumen(uint8_t nivel)`: mapear 0–100 → 0–255 en OCR2B
- [ ] 4.5 No requiere integración en `ambiente_actualizar()` (solo comandos externos cambian estado)

## 5. Implementar ciclo principal

- [ ] 5.1 Implementar `ambiente_actualizar()` llamando a:
  - Lectura potenciómetro ADC1 (cada 100ms, con promedio de 4 muestras)
  - Lectura LM35 ADC0 (cada 500ms)
  - Cuenta regresiva horno (cada llamada, usando millis())
- [ ] 5.2 Agregar variables static de timing: `ultimo_poll_iluminacion`, `ultimo_poll_temperatura`

## 6. Integrar en Proyecto.ino

- [ ] 6.1 En `setup()`: agregar `iluminacion_iniciar()`, `temperatura_iniciar()` antes o después de `menu_iniciar()`
- [ ] 6.2 En `loop()` dentro del tick de 50ms: agregar `ambiente_actualizar()` después de `rfid_actualizar()`

## 7. Compilar y probar

- [ ] 7.1 Compilar con Arduino IDE o CLI — 0 errores, 0 warnings
- [ ] 7.2 Verificar que el código es plano (.ino, sin clases, sin `_delay_*()`)
- [ ] 7.3 Verificar que los 6 archivos .ino suman < 256KB Flash y < 8KB RAM
