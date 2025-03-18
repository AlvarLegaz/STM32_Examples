# EJEMPLOS BÁSICOS STM32F

## Blink_Led_Digital_Analog_Input
Controla la frecuencia del parpadeo de un led (PIN PB12) de dos formas:
1.  Poniendo en alto el PIN PA6. Esto hace que parpadé con un periodo de 200 mseg.
2.  Si PA6 está a bajo el parpadeo se controla por entrada analógica en PIN PA7 (Entrada analógica 7 del ADC1). El tiempo oscila entre un T_minimo y un T_maximo.

## LCD Display I2C
Controla display LCD mediante I2C
https://naylampmechatronics.com/blog/35_tutorial-lcd-con-i2c-controla-un-lcd-con-solo-dos-pines.html

## Test_F103_Hardware
Prueba el hardware de la placa de control STM32F103 cuyo esquemático se encuentra adjunto en el repositorio.
Este programa contiene:
5 Menús por los que nos podemos desplazar mediante una pulsación en un botón de servicio. La implementación de los menú se realiza mediante una máquina de estados muy simple.
### Menú 1:### 
Muestra mensaje en display y realiza desplazamientos del texto de izquierda a derecha. 
Funciones implicadas: i2c y LCD_i2c.

### Menú 2:### 
Hace parpadear los leds Link y Status de la placa, asociados a PB12 y PB13.
Funciones implicadas: gpio, i2c y LCD_i2c

### Menú 3:### 
Registra estado de entrada del conector I1 y muestra su estado por pantalla.
Funciones implicadas: gpio, i2c y LCD_i2c

### Menú 4:### 
Le el valor analógico en las entradas de los conectores I2 e I3.
Funciones implicadas: adc, i2c y LCD_i2c

### Menú 5:### 
Asigna salida PWM a conector M0 y controla su ciclo de trabajo mediante el valor analógico de I
Funciones implicadas: pwm, timer2, i2c y LCD_i2c


