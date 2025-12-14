/*
 * hc_sr04.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ALegaz
 *
 *  Descripción:
 *  Este archivo contiene las funciones para manejar el sensor ultrasónico
 *  HC-SR04 utilizando un microcontrolador STM32 (serie F1) con HAL.
 *  Permite inicializar el sensor, enviar pulsos, esperar el eco y calcular
 *  la distancia en centímetros.
 */

#ifndef INC_DRIVERS_HC_SR04_H_
#define INC_DRIVERS_HC_SR04_H_

#include "stm32f1xx_hal.h"

/* ========================================================================
 *                    PROTOTIPOS DE FUNCIONES
 * ======================================================================== */

/**
 * @brief Inicializa el HC-SR04 con un temporizador específico y canal.
 *
 * @param htim Puntero a la estructura del temporizador (TIM_HandleTypeDef)
 * @param channel Canal del temporizador a usar (TIM_CHANNEL_x)
 */
void HC_SR04_Init(TIM_HandleTypeDef *htim, uint32_t channel);

/**
 * @brief Obtiene la distancia medida por el HC-SR04 en centímetros.
 *
 * @return uint32_t Distancia en cm
 */
uint32_t HC_SR04_GetDistance_cm(void);

/**
 * @brief Envía un pulso de trigger de 10us al HC-SR04.
 */
void HC_SR04_SendPulse(void);

/**
 * @brief Espera el eco del sensor y devuelve el tiempo en microsegundos.
 *
 * @return int32_t Tiempo de respuesta en microsegundos.
 *         Devuelve 0 si no se recibe eco.
 */
uint32_t HC_SR04_WaitEcho(void);

#endif /* INC_DRIVERS_HC_SR04_H_ */
