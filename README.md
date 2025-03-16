# EJEMPLOS BÁSICOS STM32F

# Comparación de las series STM32F y STM32L

STMicroelectronics ofrece diversas familias de microcontroladores STM32, cada una optimizada para diferentes aplicaciones.  
Las series **STM32F** están enfocadas en rendimiento general, mientras que las **STM32L** están diseñadas para bajo consumo de energía.  

A continuación, se presentan tablas comparativas de ambas familias.

## Serie STM32F (Rendimiento general)

La serie **STM32F** está orientada a aplicaciones que requieren mayor potencia de procesamiento y un conjunto más amplio de periféricos.  

| Serie  | Núcleo      | Frecuencia | Flash / RAM        | Consumo    | Aplicaciones |
|--------|------------|------------|--------------------|------------|--------------|
| F0     | Cortex-M0  | Hasta 48MHz  | Hasta 256KB / 32KB  | Bajo       | IoT básico, controladores simples |
| F1     | Cortex-M3  | Hasta 72MHz  | Hasta 512KB / 96KB  | Medio      | Electrónica de consumo, automatización |
| F4     | Cortex-M4F | Hasta 180MHz | Hasta 2MB / 256KB   | Medio-Alto | Procesamiento de señales, control avanzado |

## Serie STM32L (Bajo consumo)

La serie **STM32L** está optimizada para dispositivos de bajo consumo, ideales para aplicaciones como **IoT, sensores y wearables**.  

| Serie  | Núcleo      | Frecuencia  | Flash / RAM        | Consumo     | Aplicaciones |
|--------|------------|-------------|--------------------|-------------|--------------|
| L0     | Cortex-M0+ | Hasta 32MHz  | Hasta 192KB / 20KB  | Ultra Bajo  | IoT, sensores, wearables simples |
| L1     | Cortex-M3  | Hasta 32MHz  | Hasta 512KB / 80KB  | Bajo        | Wearables avanzados, automatización eficiente |
| L4     | Cortex-M4F | Hasta 120MHz | Hasta 1MB / 320KB   | Bajo        | AI en bordes, dispositivos médicos |

## Blink_Led_Digital_Analog_Input
Controla el parpadeo de un led (PIN PB12) de dos formas:
1.  Poniendo en alto el PIN PA6. Esto hace que parpadé con un periodo de 200 mseg.
2.  Si PA6 está a bajo el parpadeo se controla por entrada analógica en PIN PA7 (Entrada analógica 7 del ADC1). El tiempo oscila entre un T_minimo y un T_maximo.

## LCD Display I2C

https://naylampmechatronics.com/blog/35_tutorial-lcd-con-i2c-controla-un-lcd-con-solo-dos-pines.html