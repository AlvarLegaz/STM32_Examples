/*
 * pwm_master.c
 *
 *  Created on: Dec 14, 2025
 *      Author: ALegaz
 */

#include "drivers/pwm_master.h"

volatile uint32_t t_rise = 0, t_fall = 0;
volatile uint8_t got_rise = 0, got_fall = 0;

uint32_t pulseIn(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t timeout_us)
{
    got_rise = 0;
    got_fall = 0;

    // Empezar capturando flanco de subida
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, channel, TIM_INPUTCHANNELPOLARITY_RISING);

    HAL_TIM_IC_Start_IT(htim, channel);

    uint32_t t0 = __HAL_TIM_GET_COUNTER(htim);

    while (!got_fall)
    {
        uint32_t now = __HAL_TIM_GET_COUNTER(htim);
        uint32_t elapsed = (now >= t0) ?
                           (now - t0) :
                           (0xFFFFFFFFu - t0 + now + 1u);

        if (elapsed >= timeout_us)
        {
            HAL_TIM_IC_Stop_IT(htim, channel);
            return 0;   // timeout
        }
    }

    HAL_TIM_IC_Stop_IT(htim, channel);

    // Calcular duración del pulso (con overflow)
    return (t_fall >= t_rise) ?
           (t_fall - t_rise) :
           (0xFFFFFFFFu - t_rise + t_fall + 1u);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    uint32_t t;
    //HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 ||
        htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 ||
        htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 ||
        htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        uint32_t channel;

        switch (htim->Channel)
        {
            case HAL_TIM_ACTIVE_CHANNEL_1: channel = TIM_CHANNEL_1; break;
            case HAL_TIM_ACTIVE_CHANNEL_2: channel = TIM_CHANNEL_2; break;
            case HAL_TIM_ACTIVE_CHANNEL_3: channel = TIM_CHANNEL_3; break;
            case HAL_TIM_ACTIVE_CHANNEL_4: channel = TIM_CHANNEL_4; break;
            default: return;
        }

        t = HAL_TIM_ReadCapturedValue(htim, channel);

        if (!got_rise)
        {
            t_rise = t;
            got_rise = 1;

            // Cambiar a flanco de bajada
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, channel,
                                          TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if (!got_fall)
        {
            t_fall = t;
            got_fall = 1;

            // Preparar siguiente medición
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, channel,
                                          TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}
