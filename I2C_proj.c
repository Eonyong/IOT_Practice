/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <math.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define BMP180_ADDRESS			(0x77	<<	1)
#define BMP180_WHO_AM_I			 0xD0
#define BMP180_RESET				 0xE0
#define BMP180_CONTROL			 0xF4
#define BMP180_OUT_MSB			 0xF6
#define BMP180_OUT_LSB			 0xF7
#define BMP180_OUT_XLSB			 0xF8
#define BMP180_CALIBRATION	 0xAA
#define BMP180_GET_UT        0x2E
#define BMP180_GET_UP				 0x34
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
PUTCHAR_PROTOTYPE{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	
	return ch;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int16_t AC1, AC2, AC3, B1, B2, B5, MB, MC, MD;
uint16_t AC4, AC5, AC6;
long UT, UP, X1, X2, X3, B3, B6, T, P;
double Al;
unsigned long B4, B7;
short oss = 0;
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
	
	uint8_t wData[4];
	uint8_t rData[32];
	/* MSB의 주소를 변수에 저장 */
	uint8_t regAddr = BMP180_OUT_MSB;
	int32_t P0 = 101325;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	wData[0] = BMP180_WHO_AM_I;
		if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, wData, 1, 0xFFFFFFFF)){
			if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP180_ADDRESS, rData, 1, 0xFFFFFFFF)){
				if (rData[0] == 0x55){
					printf("BMP-180 is online\r\n");
					wData[0] = BMP180_CALIBRATION;
					if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, wData, 1, 0xFFFFFFFF)){
						if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP180_ADDRESS, rData, 22, 0xFFFFFFFF)){
							AC1 = (rData[0] << 8) + rData[1];
							AC2 = (rData[2] << 8) + rData[3];
							AC3 = (rData[4] << 8) + rData[5];
							AC4 = (rData[6] << 8) + rData[7];
							AC5 = (rData[8] << 8) + rData[9];
							AC6 = (rData[10] << 8) + rData[11];
							B1 = (rData[12] << 8) + rData[13];
							B2 = (rData[14] << 8) + rData[15];
							MB = (rData[16] << 8) + rData[17];
							MC = (rData[18] << 8) + rData[19];
							MD = (rData[20] << 8) + rData[21];
						}
					}
				}	
			}
		}
	
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	/* CONTROL 주소와 온도 측정 주소를 배열에 저장 */
		wData[0] = BMP180_CONTROL;
		wData[1] = BMP180_GET_UT;
	/* CONTROL 주소와 온도 측정 주소를 측정 기기에 입력 */
		HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, wData, 2, 0xFFFFFFFF);
		HAL_Delay(450);
	/* MSB의 주소를 측정 기기에 입력 */
		HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, &regAddr, 1, 0xFFFFFFFF);
	/* 측정 값을 rData에 저장 */
	  HAL_I2C_Master_Receive(&hi2c1, BMP180_ADDRESS, rData, 2, 0xFFFFFFFF);
	  UT = (rData[0] << 8) | rData[1];
	/* 압력 측정 주소로 변경 */
	  wData[1] = BMP180_GET_UP | (oss << 6);
	/* CONTROL 주소와 압력 측정 주소를 측정 기기에 입력 */
	  HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, wData, 2, 0xFFFFFFFF);
	  HAL_Delay(450);
	/* MSB의 주소를 측정 기기에 입력 */
	  HAL_I2C_Master_Transmit(&hi2c1, BMP180_ADDRESS, &regAddr, 1, 0xFFFFFFFF);
	/* 측정 값을 rData에 저장 */
	  HAL_I2C_Master_Receive(&hi2c1, BMP180_ADDRESS, rData, 3, 0xFFFFFFFF);
	  UP = ((rData[0] << 16) | (rData[1] << 8) | rData[2]) >> (8 - oss);
	/* Temperature calculation */
	  X1 = ((UT - (long)AC6) * (long)AC5) >> 15;
	  X2 = ((long)MC << 11) / (X1 + MD);
	  B5 = X1 + X2;
	  T = (B5 + 8) >> 4;
	  T /= 10;
	/* Pressure calculation */ 		
	  B6 = B5 - 4000;
	  X1 = (B2 * (B6 * B6) >> 12) >> 11;
	  X2 = (AC2 * B6) >> 11;
	  X3 = X1 + X2;
	  B3 = ((((long)AC1 * 4 + X3) << oss) + 2) >> 2;
	 	
	  X1 = (AC3 * B6) >> 13;
	  X2 = (B1 * (B6 * B6) >> 12) >> 16;
	  X3 = ((X1 + X2) + 2) >> 2;
	  B4 = (AC4 * (unsigned long)(X3 + 32768)) >> 15;
	  B7 = (unsigned long)(UP - B3) * (50000 >> oss);
	  if(B7 < 0x80000000){
	  	P = (B7 * 2) / B4;
	  }
	  else{
	  	P = (B7 / B4) * 2;
	  }
	  X1 = (P >> 8) * (P >> 8);
	  X1 = (X1 * 3038) >> 16;
	  X2 = (-7357 * P) >> 16;
	  P += (X1 + X2 + 3791) >> 4;
	/* Altitude calculationn */	
		Al = pow((P / P0), (1 / 5.255));
 	  Al = 44330 * (1 - Al);

	  printf(" Temperature : %ld degree, Pressure : %ld Pa, Altitude : %.2f m \r\n", T, P, Al);
    /* USER CODE BEGIN 3 */
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
