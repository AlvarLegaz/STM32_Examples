/*
 * hc_sr04.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ALegaz
 */

#include "drivers/hc_sr04.h"
#include "Drivers/pwm_master.h"

// Timer tick has to be 1 us
static TIM_HandleTypeDef *hc_tim = NULL;
static uint32_t hc_channel;

void HC_SR04_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
    hc_tim = htim;
    hc_channel = channel;
}

uint32_t HC_SR04_GetDistance_cm(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_Delay(1);

	HC_SR04_SendPulse();

	int32_t ancho_pulso_us = HC_SR04_WaitEcho();
	if (ancho_pulso_us <= 0)
	   return 0;

	return (ancho_pulso_us) / 58;
}

void HC_SR04_SendPulse(void)
{
    // Aseguramos que el pin está en bajo
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_TIM_Base_Start(hc_tim);
	__HAL_TIM_SET_COUNTER(hc_tim, 0);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	while (__HAL_TIM_GET_COUNTER(hc_tim) < 10);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_TIM_Base_Stop(hc_tim);
}

uint32_t HC_SR04_WaitEcho(void)
{
	int ancho_pulso;
	ancho_pulso = pulseIn(hc_tim, TIM_CHANNEL_4, 60000); // timeout 60 ms

	if (ancho_pulso > 0)
	{
	    // ancho_pulso está en microsegundos
		return ancho_pulso;
	}

    return 0;
}

