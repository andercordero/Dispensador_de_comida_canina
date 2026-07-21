# 🐾 Dispensador de Comida Canina (Arduino)

Dispensador automático de comida para perros, controlado con **Arduino**. Permite programar hasta **3 horarios de alimentación** al día mediante un teclado matricial, muestra la hora y los horarios configurados en una pantalla LCD, y guarda la configuración de forma permanente en la memoria **EEPROM**, para que no se pierda si el dispositivo se apaga o se reinicia.

## 📋 Descripción del proyecto

El sistema utiliza un **módulo RTC (reloj de tiempo real)** para llevar el control exacto de la hora, incluso sin conexión a internet. Cuando la hora actual coincide con alguno de los horarios programados (A, B o C), un **servomotor** se activa para abrir y cerrar la compuerta del dispensador, liberando la comida.

También cuenta con un **botón físico** para dispensar comida de forma manual en cualquier momento, sin necesidad de esperar a un horario programado.

## ⚙️ Componentes utilizados (hardware)

- Arduino (Uno/Nano o compatible)
- Módulo RTC DS3231 (reloj en tiempo real)
- Servomotor (mecanismo de apertura/cierre)
- Pantalla LCD 16x2 con módulo I2C
- Teclado matricial 4x4
- Botón pulsador (dispensado manual)
- Memoria EEPROM interna del Arduino (para guardar los horarios)

## 🧠 ¿Cómo funciona el código?

1. **Al iniciar (`setup()`):** se cargan desde la EEPROM los últimos horarios guardados para las comidas A, B y C. Si la memoria está vacía, se asignan valores por defecto.
2. **En el bucle principal (`loop()`):**
   - Se lee la hora actual desde el RTC y se muestra en el LCD.
   - Se compara la hora actual con los 3 horarios programados; si coincide, se dispensa comida automáticamente.
   - Se revisa si el botón manual fue presionado para dispensar comida al instante.
   - Se escucha el teclado: al presionar **A**, **B** o **C**, se entra al modo de configuración de horario para esa comida.
3. **Configuración de horarios (`setFeedingTime()`):** el usuario ingresa la hora en formato `HHMM` desde el teclado. Si es válida, se guarda tanto en la variable en memoria como en la EEPROM (para persistencia).
4. **Dispensado (`dispenseFood()`):** mueve el servomotor para abrir y cerrar la compuerta, liberando una porción de comida.

## 🎮 Uso del teclado

| Tecla | Función |
|-------|---------|
| A | Configurar horario de comida A |
| B | Configurar horario de comida B |
| C | Configurar horario de comida C |
| 0-9 | Ingresar hora en formato HHMM |

## 🚀 Requisitos (librerías de Arduino)

- `Wire.h`
- `RTClib.h`
- `Servo.h`
- `LiquidCrystal_I2C.h`
- `Keypad.h`
- `EEPROM.h`

Estas librerías se pueden instalar desde el **Gestor de Librerías** del IDE de Arduino.

## 📌 Notas

- Los horarios configurados se conservan aunque el Arduino se apague o se reinicie, gracias al uso de la EEPROM.
- El sistema evita dispensar comida más de una vez en el mismo minuto programado.

---
**U.E Colegio Santa Teresita del Niño Jesus**
