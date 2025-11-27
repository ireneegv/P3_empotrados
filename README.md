---

# Conexiones de Componentes
---
A continuación se describen todas las conexiones realizadas para los distintos sensores y actuadores.
Se han reservado los **pines de interrupción (2 y 3)** para los botones.

## LCD 16x2 (modo 4 bits)

| LCD | Arduino UNO                 |
| --- | --------------------------- |
| RS  | 12                          |
| EN  | 11                          |
| D4  | 7                           |
| D5  | 8                           |
| D6  | 9                           |
| D7  | 10                          |
| VSS | GND                         |
| VDD | 5V                          |
| VO  | Potenciómetro               |
| RW  | GND                         |
| A/K | 5V / GND (retroiluminación) |

---

##  Joystick analógico con botón

| Joystick   | Arduino UNO          |
| ---------- | -------------------- |
| VRx        | A1                   |
| VRy        | A0                   |
| SW (botón) | 3 (**interrupción**) |
| +5V        | 5V                   |
| GND        | GND                  |

---

##  Sensor de Temperatura y Humedad – DHT11

| DHT11 | Arduino UNO |
| ----- | ----------- |
| DATA  | 4           |
| VCC   | 5V          |
| GND   | GND         |

---

##  Sensor Ultrasónico HC-SR04


| HC-SR04 | Arduino UNO |
| ------- | ----------- |
| TRIG    | 5           |
| ECHO    | A4          |
| VCC     | 5V          |
| GND     | GND         |

---

## Botón externo (interrupción)


| Botón                                             | Arduino UNO          |
| ------------------------------------------------- | -------------------- |
| Pulsador                                          | 2 (**interrupción**) |
| Otro extremo                                      | GND                  |
| *(opcional: resistor pull-up o interno activado)* |                      |

---

## LEDs normales

**Conexiones:**

| LED             | Arduino UNO     |
| --------------- | --------------- |
| LED1 (Azul)     | A5              |
| LED2 (Amarillo) | 6               |
| Resistencias    | Entre LED y GND |

---
# Imagen de las conexiones

<img width="2529" height="1875" alt="p3_bb" src="https://github.com/user-attachments/assets/e3be66b1-674d-4573-a0f2-06638d1472ed" />

[archivo de las conexiones](p3.fzz)

---
#  video
---
[![WhatsApp Image 2025-11-27 at 12 49 03](https://github.com/user-attachments/assets/0eafb455-ae20-4ebf-888a-503e34d60107)](https://youtu.be/NGJ5uwuH57I)

---
#  código
---
Para la práctica 3 se emplea un switch dentro del loop principal para gestionar los distintos estados del programa. Cada estado ejecuta una funcionalidad concreta, lo que permite organizar mejor el flujo y evitar estructuras complejas.

* **Interrupciones:**
En el botón principal cada vez que cambia su estado se llamará a la función. Cuando el botón se pulsa, se activa un contador y se reinicia la variable que mide el tiempo de pulsación. Cuando se libera, el contador se detiene y se marca que el botón fue liberado. Para medir con precisión cuánto tiempo se mantiene pulsado, se utiliza Timer1, configurado para generar una interrupción cada 1 ms. Cada vez que Timer1 interrumpe y el contador está activo, se incrementa el tiempo de pulsación. Esto permite activar distintas acciones según la duración de la pulsación, sin bloquear el resto del programa.

En el caso del joystick, su botón genera frecuentemente falsos positivos debido al rebote mecánico. Para solucionarlo, también se utiliza una interrupción de tipo FALLING, que simplemente marca que se ha pulsado el botón. Luego, en el loop principal, se comprueba esta marca y se aplica un umbral de 200 ms para filtrar pulsaciones consecutivas muy rápidas. De este modo, solo se considera una pulsación válida si ha pasado suficiente tiempo desde la anterior.

* **watchdog:**
  
  Se implementó un watchdog dentro del bucle while para garantizar que, si el programa se queda bloqueado, el sistema se reinicie automáticamente, asegurando mayor estabilidad.

 * **Threads:**
 * En este caso no se usan threads, en su lugar se usan condiciones para no bloquear, permitiendonos enseñar los distintos menús de la práctica.

---
#  Dificultades y datos tenidos en cuenta
---

* **Botón:**
  
  Se presentaron algunos problemas al intentar medir el tiempo cuando el botón estaba pulsado. Para solucionarlo,
  se optó por utilizar la interrupción configurada con el modo **CHANGE**, ya que fue la opción que mejor se adaptó
  al comportamiento necesario para detectar tanto la pulsación como la liberación del botón.

* **LED PWM:**
  
  El LED2, que debía permitir el ajuste progresivo de la intensidad luminosa mediante PWM, únicamente se encendía y se apagaba sin mostrar variación en el brillo.
  El problema se debía a que estaba conectado a un pin que **no soportaba PWM**, por lo que no era posible modificar su intensidad.
  Al reconectar el LED a un pin compatible con PWM, el control de brillo comenzó a funcionar correctamente.
  
* **Tiempo en interrupciones:**
  
  Al principio utilizaba el Timer 0 para las interrupciones, pero como esto podía generar errores ya que el tiempo que estas en la ISR no se contabiliza, se cambió al Timer 1.

* **Mínimo uso de delay:**
  
  Como la función delay() bloquea la ejecución del programa, se reemplazó por condiciones basadas en el tiempo transcurrido, permitiendo que el código siga ejecutándose sin interrupciones innecesarias.

  
---
#  Páginas usadas y librerías usadas
---

- [Timer 1](https://docs.arduino.cc/libraries/timerone/)
- [Sensores DHT11](https://github.com/adafruit/DHT-sensor-library)
- [Crear símbolos para el LCD](https://chareditor.com/)
- [Conexión LCD](https://docs.arduino.cc/learn/electronics/lcd-displays/)
- [Conexión LEDs](https://docs.arduino.cc/built-in-examples/basics/Blink/)
- [Conexión ultrasonidos](https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1)
- [Conexión joystick](https://projecthub.arduino.cc/hibit/using-joystick-module-with-arduino-0ffdd4)
- [Conexión botón](https://programarfacil.com/blog/utilizar-pulsadores-en-arduino/)



