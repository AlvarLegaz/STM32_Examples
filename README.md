# EJEMPLOS BÁSICOS STM32F103T6

## Blink_Led_Digital_Analog_Input
Controla el parpadeo de un led (PIN PB12) de dos formas:
1.  Poniendo en alto el PIN PA6. Esto hace que parpadé con un periodo de 200 mseg.
2.  Si PA6 está a bajo el parpadeo se controla por entrada analógica en PIN PA7 (Entrada analógica 7 del ADC1). El tiempo oscila entre un T_minimo y un T_maximo.

## LCD Display I2C

https://naylampmechatronics.com/blog/35_tutorial-lcd-con-i2c-controla-un-lcd-con-solo-dos-pines.html