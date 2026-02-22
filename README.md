# EJEMPLOS BÁSICOS STM32F

## Placas desarrollo STM32
<img width="531" height="455" alt="image" src="https://github.com/user-attachments/assets/ece3ee35-3f99-48de-aa26-3943d3013089" />

<img width="505" height="439" alt="image" src="https://github.com/user-attachments/assets/181c78a9-6bd8-437d-938c-412a261b2a10" />

## Pulsadores entrada
Para shield Spainskills 2026 debe establecerse pull-up interno y logica negada.
<img width="900" height="513" alt="image" src="https://github.com/user-attachments/assets/e47d3523-e6be-4345-8c83-7c9cbb2d4ca1" />


## LCD Display I2C
Controla display LCD mediante I2C
https://naylampmechatronics.com/blog/35_tutorial-lcd-con-i2c-controla-un-lcd-con-solo-dos-pines.html

## Pantalla OLED con controlador SSD1306 e I2C

https://controllerstech.com/oled-display-using-i2c-stm32/

Importante configurar I2C en Fast Mode o no funciona...

<img width="537" height="671" alt="image" src="https://github.com/user-attachments/assets/b9c2d4c6-6c46-4730-bb86-20571676db7c" />

https://controllerstech.com/oled-display-using-i2c-stm32/


## Test_F103_Hardware
Prueba el hardware de la placa de control STM32F103 cuyo esquemático se encuentra adjunto en el repositorio.
Este programa contiene:
5 Menús por los que nos podemos desplazar mediante una pulsación en un botón de servicio. La implementación de los menú se realiza mediante una máquina de estados muy simple.
### Menú 1 
Muestra mensaje en display y realiza desplazamientos del texto de izquierda a derecha, si se habilita.
Funciones implicadas: i2c y LCD_i2c.

### Menú 2 
Hace parpadear los leds del shield.
Funciones implicadas: gpio, i2c y LCD_i2c

### Menú 3 
Usa conversor ADC para leer valor potenciometro, LDR y sensor de temperatura.

### Menú 4 
Hace sonar el buzzer. Al estar controlado con pnp usa logica negada.

### Menú 5 
Sin implementar, deberia haber sido ultrasonico que controla un servo en función de la distancia...

### Menú 6 
Juego del laberinto.


