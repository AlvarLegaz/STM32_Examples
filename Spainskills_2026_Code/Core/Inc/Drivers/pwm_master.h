/*
 * pwm_master.h
 *
 *  Created on: Dec 14, 2025
 *      Author: ALegaz
 */

#ifndef INC_DRIVERS_PWM_MASTER_H_
#define INC_DRIVERS_PWM_MASTER_H_

#include "stm32F1xx_hal.h"   // Cambia xxx por tu familia (f1, f4, g0, etc.)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Mide la duración de un pulso en nivel alto usando Input Capture.
 *
 * Importante:
 * - Las interrupciones del timer y del canal deben estar habilitadas.
 * - Necesario configurar timmer y input capture
 * - Por seguridad usar timer 2 a 4
 *
 * @param  htim        Puntero al timer previamente configurado.
 * @param  channel     Canal del timer (TIM_CHANNEL_1 .. TIM_CHANNEL_4).
 * @param  timeout_us  Timeout en microsegundos.
 *
 * @retval Duración del pulso en ticks del timer.
 *         0 si ocurre timeout.
 */
uint32_t pulseIn(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t timeout_us);


void pulseIn_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* PULSEIN_H */
