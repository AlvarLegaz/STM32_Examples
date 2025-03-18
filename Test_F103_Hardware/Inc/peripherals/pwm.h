/*
 * pwm.h
 *
 *  Created on: Mar 18, 2025
 *      Author: ALegaz
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

void PWM_Init(void);
void PWM_Start(void);
void PWM_SetDuty(uint8_t percentaje);


#ifdef __cplusplus
}
#endif

#endif /* INC_PWM_H_ */
