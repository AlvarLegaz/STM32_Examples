/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Drivers/LCD_i2c.h"
#include "Drivers/ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Select_ADC_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // 1. IMPORTANTE: Limpiar la selección de canales previa
    // Esto evita que se acumulen canales en el registro CHSELR
    hadc.Instance->CHSELR = 0;

    // 2. Configurar el nuevo canal
    sConfig.Channel = channel;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;

    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */

  // --- INICIALIZAR BUZZER ---//
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

  //---- Display LCD - I2C ---//
  LCD_Init();
  LCD_Clear();
  LCD_Set_Cursor(0,0);
  LCD_Write_String("*Menu 1: Display");

  LCD_Set_Cursor(1,0);
  LCD_Write_String(" Menu 2: Blink LEDs");

  LCD_Set_Cursor(2,0);
  LCD_Write_String(" Menu 3: ADC");

  LCD_Set_Cursor(3,0);
  LCD_Write_String(" Menu 4: Buzzer");

  //---- OLED SSD1306 - I2C ---//
  SSD1306_Init (); // initialise the display

  // El ADC debe estar encendido pero detenido para calibrar
  if (HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED) != HAL_OK)
  {
      Error_Handler(); // Error de calibración
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //--- BOTONES DE SERVICIO ---//

  uint8_t bot_B5_val_previo = SET; //Se pone a SET porque va con pull-up (lógica negada)
  uint8_t bot_B5_val;

  uint8_t bot_B3_val_previo = SET; //Se pone a SET porque va con pull-up (lógica negada)
  uint8_t bot_B3_val;


  uint8_t estado_actual_bot_central = 0;
  uint8_t estado_anterior_bot_central = 1;

  // Estados
  enum {menu_display, menu_leds, menu_adc, menu_buzzer, MENU_COUNT};
  int8_t estado=0; // tiene que tener signo para detectar num menores que cero

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// 1. Comprobamos botones
	  bot_B5_val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	  if(bot_B5_val_previo == SET && bot_B5_val == RESET)
	  {
		  //Actualizamos el display LCD que marca el menu
		  LCD_Set_Cursor(estado,0);
		  LCD_Write_String(" ");

		  estado--;
		  if(estado < 0)
		  estado = 0;

		 // Siempre que cambio de estado limpio display OLED y pongo nuevo menu en estado LCD
		 SSD1306_Clear();
		 LCD_Set_Cursor(estado,0);
		 LCD_Write_String("*");
	 }
	 bot_B5_val_previo = bot_B5_val;

	 bot_B3_val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	 	if(bot_B3_val_previo == SET && bot_B3_val == RESET){

	 		//Actualizamos el display LCD que marca el menu
	 		LCD_Set_Cursor(estado,0);
	 		LCD_Write_String(" ");

	 		estado++;
	 		if(estado > MENU_COUNT-1)
	 			estado = MENU_COUNT-1;

	 		// Siempre que cambio de estado limpio display OLED y pongo nuevo menu en estado LCD
	 		SSD1306_Clear();
	 		LCD_Set_Cursor(estado,0);
	 		LCD_Write_String("*");
	 	}
	 	bot_B3_val_previo = bot_B3_val;

	 switch(estado){

	 case menu_display:
		  int num = 123456;
		  float flt = 3.5;

		  int entero = (int)flt;
		  int decimal = (int)((flt - entero) * 100);
		  if (decimal < 0) decimal = -decimal;

		  char bufnum[7];
		  char bufflt[7];

		  sprintf (bufnum, "%d", num);
		  sprintf(bufflt, "%d.%02d", entero, decimal); // No está habiliatado el float...


		  SSD1306_GotoXY (10,0); // goto 10, 0
		  SSD1306_Puts ("Spainskills !!", &Font_11x18, 1);

		  SSD1306_GotoXY (10, 20); // va a columna 10 fila 30
		  SSD1306_Puts (bufnum, &Font_7x10, 1); // print num

		  SSD1306_GotoXY (10, 30); // va a columna 10 fila 30
		  SSD1306_Puts (bufflt, &Font_16x26, 1); // print num
		  SSD1306_UpdateScreen(); // update screen

		  //SSD1306_ScrollLeft(1,64);
	break;

	 case menu_leds:
		 HAL_Delay(250);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);

		 HAL_Delay(250);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0);
		 HAL_Delay(500);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	 break;

	 case menu_adc:
		 uint16_t valorPA0 = 0;
		 uint16_t valorPA1 = 0;
		 uint16_t valorPA4 = 0;

		 // --- Leer PA0 (Canal 0) ---
		 Select_ADC_Channel(ADC_CHANNEL_0);
		 HAL_ADC_Start(&hadc);
		 if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK) {
		     valorPA0 = HAL_ADC_GetValue(&hadc);
		 }
		 HAL_ADC_Stop(&hadc);
		 HAL_Delay(20); // Pequeña pausa para estabilizar

		 // --- Leer PA1 (Canal 1) ---
		 Select_ADC_Channel(ADC_CHANNEL_1);
		 HAL_ADC_Start(&hadc);
		 if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK) {
		     valorPA1 = HAL_ADC_GetValue(&hadc);
		 }
		 HAL_ADC_Stop(&hadc);
		 HAL_Delay(20); // Pequeña pausa para estabilizar

		 // --- Leer PA4 (Canal 4) ---
		 Select_ADC_Channel(ADC_CHANNEL_4);
		 HAL_ADC_Start(&hadc);
		 if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK) {
		     valorPA4 = HAL_ADC_GetValue(&hadc);
		 }
		 HAL_ADC_Stop(&hadc);
		 HAL_Delay(20); // Pequeña pausa para estabilizar

		 // Ahora puedes usar valorPA0, valorPA1 y valorPA4...
		 char str_PA0[16];
		 char str_PA1[16];
		 char str_PA4[16];

		 memset(str_PA0, 0, 16);
		 memset(str_PA1, 0, 16);
		 memset(str_PA4, 0, 16);

		 uint32_t mV = (uint32_t)valorPA0 * 3300 / 4095;
		 sprintf(str_PA0, "POT = %lu.%02lu V", mV/1000, (mV%1000)/10);

		 sprintf (str_PA1, "LDR = %d", valorPA1);

		 uint32_t mV_temp = (uint32_t)valorPA4 * 3300 / 4095;
		 mV_temp = mV_temp - 200; //Corrección por tmp36
		 uint32_t temperatura = mV_temp / 10.0;  // LM35 = 10mV por °C
		 sprintf(str_PA4, "TEMP = %d C", temperatura);

		 SSD1306_GotoXY (10,0); // goto 10, 0
		 SSD1306_Puts ("ADC !!", &Font_11x18, 1);

		 SSD1306_GotoXY (10, 20); // va a columna 10 fila 30
		 SSD1306_Puts ("               ", &Font_7x10, 1); // Borro antes de escribir
		 SSD1306_GotoXY (10, 20); // va a columna 10 fila 30
		 SSD1306_Puts (str_PA0, &Font_7x10, 1); // print num

		 SSD1306_GotoXY (10, 30); // va a columna 10 fila 30
		 SSD1306_Puts ("               ", &Font_7x10, 1); // Borro antes de escribir
		 SSD1306_GotoXY (10, 30); // va a columna 10 fila 30
		 SSD1306_Puts (str_PA1, &Font_7x10, 1); // print num

		 SSD1306_GotoXY (10, 40); // va a columna 10 fila 30
		 SSD1306_Puts ("               ", &Font_7x10, 1); // Borro antes de escribir
		 SSD1306_GotoXY (10, 40); // va a columna 10 fila 30
		 SSD1306_Puts (str_PA4, &Font_7x10, 1); // print num

		 SSD1306_UpdateScreen(); // update screen

	 break;

	 case menu_buzzer:
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10));
	 break;

	 default:
		 ;;
	 break;

	  }

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000000;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 PA8
                           PA9 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
