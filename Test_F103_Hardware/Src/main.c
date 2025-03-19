
#include "main.h"
#include "peripherals/i2c.h"
#include "peripherals/gpio.h"
#include "peripherals/adc.h"
#include "peripherals/pwm.h"
#include "peripherals/timer2.h"
#include "drivers/LCD_i2c.h"


#define MENU_DELAY 150
#define SET 1
#define RESET 0

void SystemClock_Config(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();

  /* Initialize display */
  LCD_Init();
  PWM_Init();
  PWM_Start();

  /* Service button*/
  uint8_t blue_button_previous = SET;
  uint8_t blue_button_val;

  /* Menu states */
  enum {start_menu, led_menu, press_button_menu, adc_menu, pwm_menu, final_menu};
  uint8_t state=0;

  uint8_t display_posicion = 0;

  while (1)
  {
	  /* Check service buttons */
	  blue_button_val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
	  if(blue_button_previous == SET && blue_button_val == RESET){
		  state++;
	  }
	  blue_button_previous = blue_button_val;

	  /* State machine based in menus*/
	  switch(state){

	  case start_menu:
		  LCD_Clear();
		  LCD_Set_Cursor(0,0);
		  LCD_Write_String("Start menu");
		  LCD_Set_Cursor(1,0);
		  LCD_Write_String("Display test");

		  // Shift display left/right.
		  for(int i =0; i<display_posicion; i++){
			  if(i < 16)
				  LCD_SL();
			  else
				  LCD_SR();
		  }

		  display_posicion++;
		  if(display_posicion>32)
			  display_posicion = 0;

		  HAL_Delay(MENU_DELAY*3);

		  break;

	  case led_menu:
	  	  LCD_Clear();
	  	  LCD_Set_Cursor(0,0);
	  	  LCD_Write_String("LED menu");
	  	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
	  	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	  	  HAL_Delay(MENU_DELAY);

	  	  break;

	  case press_button_menu:
		  LCD_Clear();
		  LCD_Set_Cursor(0,0);
		  LCD_Write_String("Press button menu");
		  LCD_Set_Cursor(1,0);
		  LCD_Write_String("BOT0=");

		  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == RESET)
		  {
			  LCD_Set_Cursor(1,5);
			  LCD_Write_String("FALSE");
		  }
		  else
		  {
			  LCD_Set_Cursor(1,5);
			  LCD_Write_String("TRUE");
		  }

		  HAL_Delay(MENU_DELAY);

		  break;

	  case adc_menu:
		  // PB0 - channel=8
		  // PB1 - channel=9
		  uint32_t value_adc0 = ADC_GetValue(8);
		  uint32_t value_adc1 = ADC_GetValue(9);

		  // Convert uint32_t to string
		  char adcValueStr0[16];
		  sprintf(adcValueStr0, "%lu", value_adc0);

		  char adcValueStr1[16];
		  sprintf(adcValueStr1, "%lu", value_adc1);

		  LCD_Clear();
		  LCD_Set_Cursor(0,0);
		  LCD_Write_String("ADC0 val=");
		  LCD_Set_Cursor(0,10);
		  LCD_Write_String(adcValueStr0);

		  LCD_Set_Cursor(1,0);
		  LCD_Write_String("ADC1 val=");
		  LCD_Set_Cursor(1,10);
		  LCD_Write_String(adcValueStr1);

		  HAL_Delay(MENU_DELAY);

		  break;

	  case pwm_menu:
		  LCD_Clear();
		  LCD_Set_Cursor(0,0);
		  LCD_Write_String("PWM menu");
		  HAL_Delay(MENU_DELAY);

		  value_adc0 = ADC_GetValue(8);
		  // Duty Cycle from 12% to 3%.
		  uint8_t MAX= 12;
		  uint8_t MIN= 3;
		  uint16_t RANGO = 4096.0;

		  uint8_t posicion = (MAX-MIN)*value_adc0/RANGO + MIN;
		  PWM_SetDuty(posicion);

		  break;

	  default:
		  state = 0;
		  break;

	  };
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
