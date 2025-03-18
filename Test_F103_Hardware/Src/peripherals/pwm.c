/*
 * pwm.c
 *
 *  Created on: Mar 18, 2025
 *      Author: ALegaz
 */

#include "peripherals/timer2.h"

void PWM_Init(){
	TIM2_Init();
}

void PWM_Start(void){
	TIM2_Start();
}

void PWM_Stop(void){
	TIM2_Start();
}

void PWM_SetDuty(uint8_t percentaje){

	TIM2->CCR1 =  (200*(percentaje/100.0)); // Canal 1:
}
