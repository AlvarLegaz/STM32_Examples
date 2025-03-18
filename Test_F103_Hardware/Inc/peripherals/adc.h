
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void);
uint32_t ADC_GetValue(uint8_t channel);


#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

