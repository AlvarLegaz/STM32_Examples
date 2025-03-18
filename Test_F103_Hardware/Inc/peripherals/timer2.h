/*
 * timers.h
 *
 *  Created on: Mar 18, 2025
 *      Author: ALegaz
 */
#ifndef INC_TIMER2_H_
#define INC_TIMER2_H_


#ifdef __cplusplus
extern "C" {
#endif

	#include "main.h"

	void TIM2_Init(void);
	void TIM2_Start(void);
	void TIM2_Stop(void);

#ifdef __cplusplus
}
#endif
#endif /* INC_TIMER2_H_ */
