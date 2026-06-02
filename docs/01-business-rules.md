# Reglas de Negocio — domotica-atmega2560-c

## Regla 1: Sistema de alarma dual (acceso + incendio)
- **Actor**: Sistema
- **Condición**: Condición que compromete la seguridad (intrusión o humo)
- **Acción**: Informar a través del puerto serial
- **Restricciones**: N/A
- **Subsistema**: Seguridad
- **Prioridad**: Alta
- **Fuente textual**: "Cuando se presente cualquier condición que comprometa la seguridad, deberá informarse a través del puerto serial"

## Regla 2: Activación/desactivación de alarmas solo con código
- **Actor**: Persona autorizada
- **Condición**: Poseer el código correcto
- **Acción**: Activar o desactivar las alarmas
- **Restricciones**: Solo personas con el código pueden hacerlo
- **Subsistema**: Seguridad
- **Prioridad**: Alta
- **Fuente textual**: "Estas alarmas deben poder ser activadas y desactivadas solo por personas que cuenten con el código para ello"

## Regla 3: Acceso a vivienda mediante RFID
- **Actor**: Persona
- **Condición**: Presentar tarjeta RFID autorizada
- **Acción**: Permitir acceso a la vivienda
- **Restricciones**: Solo tarjetas enroladas en el sistema
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "El acceso de las personas a la vivienda, se realizará mediante el uso de tarjetas RFID"

## Regla 4: Enrolamiento de nuevas personas
- **Actor**: Administrador
- **Condición**: Se inicia proceso de enrolamiento
- **Acción**: Agregar nueva tarjeta RFID al sistema como autorizada
- **Restricciones**: Requiere privilegios de administrador
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "el sistema debe permitir el enrolamiento de nuevas personas autorizadas"

## Regla 5: Borrado de personas existentes
- **Actor**: Administrador
- **Condición**: Se inicia proceso de borrado
- **Acción**: Eliminar tarjeta RFID del sistema
- **Restricciones**: Requiere privilegios de administrador
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "y el borrado de personas ya existentes"

## Regla 6: Dos puntos de acceso (principal + garaje)
- **Actor**: Persona autorizada
- **Condición**: Acceso por puerta principal o garaje
- **Acción**: Abrir la puerta correspondiente
- **Restricciones**: Cada puerta tiene su actuador específico
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "El acceso puede ser a través de la puerta principal o el garaje"

## Regla 7: Puerta principal controlada por imán
- **Actor**: Sistema
- **Condición**: Acceso autorizado por RFID
- **Acción**: Activar/desactivar imán para liberar puerta
- **Restricciones**: Actuador tipo imán (electroimán o solenoide)
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "La puerta principal se controla mediante un imán"

## Regla 8: Puerta garaje controlada por servomotor
- **Actor**: Sistema
- **Condición**: Acceso autorizado por RFID
- **Acción**: Activar servomotor para abrir/cerrar garaje
- **Restricciones**: PWM para control de servomotor
- **Subsistema**: Control de Acceso RFID
- **Prioridad**: Alta
- **Fuente textual**: "la puerta garaje se controla mediante un servomotor"

## Regla 9: Acceso a habitación de juegos con cuota de usos
- **Actor**: Hijo
- **Condición**: Presentar tarjeta RFID con saldo disponible
- **Acción**: Permitir acceso a la habitación de juegos
- **Restricciones**: La cantidad de usos está programada en la tarjeta RFID
- **Subsistema**: Habitación de Juegos
- **Prioridad**: Alta
- **Fuente textual**: "En la vivienda hay una habitación de juegos para los hijos a la que cada uno de ellos puede acceder una determinada cantidad de veces, la cual se programa en su tarjeta RFID"

## Regla 10: Descuento de acceso por ingreso
- **Actor**: Sistema
- **Condición**: Hijo ingresa a habitación de juegos
- **Acción**: Decrementar contador de usos en la tarjeta RFID
- **Restricciones**: El descuento ocurre después de cada ingreso
- **Subsistema**: Habitación de Juegos
- **Prioridad**: Alta
- **Fuente textual**: "Después de cada ingreso, se debe descontar de la cantidad total autorizada"

## Regla 11: Carga de accesos por los padres
- **Actor**: Padres
- **Condición**: Inician proceso de recarga
- **Acción**: Incrementar saldo de accesos en la tarjeta RFID del hijo
- **Restricciones**: Solo los padres pueden recargar
- **Subsistema**: Habitación de Juegos
- **Prioridad**: Alta
- **Fuente textual**: "Los padres deben poder realizar la carga de los accesos disponibles para sus hijos"

## Regla 12: Control de iluminación dimerizada
- **Actor**: Usuario
- **Condición**: Solicitud de cambio de intensidad lumínica
- **Acción**: Ajustar nivel de iluminación mediante dimerización (PWM)
- **Restricciones**: Señal PWM para control proporcional
- **Subsistema**: Iluminación
- **Prioridad**: Media
- **Fuente textual**: "El sistema también debe permitir el control de la iluminación de forma dimerizada"

## Regla 13: Control de temperatura con calefactor y ventilador
- **Actor**: Sistema / Usuario
- **Condición**: Temperatura fuera del rango deseado
- **Acción**: Activar calefactor (si frío) o ventilador (si calor)
- **Restricciones**: Dos actuadores distintos para calentar y enfriar
- **Subsistema**: Control de Temperatura
- **Prioridad**: Media
- **Fuente textual**: "Para el control de la temperatura, se cuenta con un calefactor y un ventilador"

## Regla 14: Encendido remoto del horno con tiempo y temperatura
- **Actor**: Usuario remoto
- **Condición**: Comando remoto recibido vía USART
- **Acción**: Encender horno durante el tiempo y temperatura especificados
- **Restricciones**: Debe especificar tiempo Y temperatura como parámetros
- **Subsistema**: Horno Remoto
- **Prioridad**: Media
- **Fuente textual**: "debe permitir encender el horno de forma remota, para lo cual se debe decir por cuánto tiempo y a qué temperatura"

## Regla 15: Encendido remoto del equipo de sonido con control de volumen
- **Actor**: Usuario remoto
- **Condición**: Comando remoto recibido vía USART
- **Acción**: Encender equipo de sonido y ajustar volumen
- **Restricciones**: Señal analógica proporcional al volumen solicitado
- **Subsistema**: Equipo de Sonido
- **Prioridad**: Media
- **Fuente textual**: "También se podrá encender el equipo de sonido de forma remota, con control para el volumen, a través de una señal analógica proporcional al volumen solicitado"

## Regla 16: Lista de mercado consultable remotamente
- **Actor**: Usuario remoto
- **Condición**: Solicitud de consulta vía USART
- **Acción**: Enviar contenido de la lista de mercado por serial
- **Restricciones**: La lista debe contener nombre y cantidad de cada producto
- **Subsistema**: Lista de Mercado
- **Prioridad**: Baja
- **Fuente textual**: "debe permitir realizar una lista de mercado, que pueda ser consultada de forma remota"

## Regla 17: Formato de la lista de mercado (producto + cantidad)
- **Actor**: Sistema
- **Condición**: Creación/consulta de ítem en la lista
- **Acción**: Almacenar/mostrar nombre del producto y cantidad requerida
- **Restricciones**: Ambos campos son obligatorios
- **Subsistema**: Lista de Mercado
- **Prioridad**: Baja
- **Fuente textual**: "Esta lista debe contener el nombre del producto y la cantidad requerida"

## Regla 18: LCD como interfaz de visualización única
- **Actor**: Sistema
- **Condición**: Cualquier información debe mostrarse al usuario
- **Acción**: Mostrar en el LCD 16x2
- **Restricciones**: Toda la información al usuario se muestra exclusivamente por LCD
- **Subsistema**: Interfaz LCD
- **Prioridad**: Alta
- **Fuente textual**: "Toda la información que deba mostrarse al usuario se hará por medio de un LCD"
