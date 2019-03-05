/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    25-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stm32f1xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId LEDThread1Handle, LEDThread2Handle;

/* Private function prototypes -----------------------------------------------*/
static void LED_Thread1(void const *argument);
static void LED_Thread2(void const *argument);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

static UART_HandleTypeDef s_UARTHandle;// = UART_HandleTypeDef();

int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
	HAL_Init();  
	//__GPIOA_CLK_ENABLE();
	
	 RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	

	/**Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
	
	
	__GPIOC_CLK_ENABLE();
	__USART1_CLK_ENABLE();
	
	__AFIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_14;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		//GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	
	//GPIO_InitTypeDef GPIO_InitStructure;
// 
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	s_UARTHandle.Instance        = USART1;
	s_UARTHandle.Init.BaudRate   = 115200;
	s_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
	s_UARTHandle.Init.StopBits   = UART_STOPBITS_1;
	s_UARTHandle.Init.Parity     = UART_PARITY_NONE;
	s_UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	s_UARTHandle.Init.Mode       = UART_MODE_TX_RX;
	s_UARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&s_UARTHandle) != HAL_OK)
		asm("bkpt 255");
	
	


	/* Thread 1 definition */
	osThreadDef(LED1, LED_Thread1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	 /*  Thread 2 definition */
	osThreadDef(LED2, LED_Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	/* Start thread 1 */
	LEDThread1Handle = osThreadCreate(osThread(LED1), NULL);
  
	/* Start thread 2 */
	LEDThread2Handle = osThreadCreate(osThread(LED2), NULL);
  
	/* Start scheduler */
	osKernelStart();

	  /* We should never get here as control is now taken by the scheduler */
	for (;;)
		;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}

/**
  * @brief  Toggle LED1
  * @param  thread not used
  * @retval None
  */
static void LED_Thread1(void const *argument)
{
	(void) argument;
	uint8_t t2enable = 0;
	for (;;)
	{
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		//osDelay(2000);
		
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
//		osDelay(2000);
//		if (t2enable == 0)
//		{
//			osThreadResume(LEDThread2Handle);
//			t2enable = 1;
//		}
//		else
//		{
//			osThreadSuspend(LEDThread2Handle);	
//			t2enable = 0;
//		}
		vTaskDelay(1000);
		
		if (t2enable == 0)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
			osThreadResume(LEDThread2Handle);
			t2enable = 1;
		}
		else
		{
			osThreadSuspend(LEDThread2Handle);	
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
			t2enable = 0;
		}
		
		
	}
}

/**
  * @brief  Toggle LED2 thread
  * @param  argument not used
  * @retval None
  */
static void LED_Thread2(void const *argument)
{
	uint32_t count;
	(void) argument;
	uint8_t c_collback[] = "CollBack  \r";
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//osDelay(50);
		vTaskDelay(50);
		HAL_UART_Transmit(&s_UARTHandle,c_collback,sizeof(c_collback)-1, HAL_MAX_DELAY);
	}
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
