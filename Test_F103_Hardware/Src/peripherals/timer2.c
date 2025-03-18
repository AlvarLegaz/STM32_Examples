#include "peripherals/timer2.h"

void TIM2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Habilitar reloj para TIM2

    TIM2->PSC = 799; // Prescaler (8 MHz / (799 + 1) = 10 kHz)
    TIM2->ARR = 200 - 1; // Periodo (10 kHz / 200 = 50 Hz)

    // Configurar canal 1 (PA0) en modo PWM1
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // Modo PWM1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Preload habilitado

    // Configurar canal 2 (PA1) en modo PWM1
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // Modo PWM1
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE; // Preload habilitado

    // Habilitar salidas de los canales 1 y 2
    TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Habilitar auto-reload preload
    TIM2->CR1 |= TIM_CR1_ARPE;
}

void TIM2_Start(){
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_Stop(){
	TIM2->CR1 &= ~TIM_CR1_CEN;
}
