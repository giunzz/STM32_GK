/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* SysTick delay kiểu thanh ghi (1 ms) */
static void Delay_1_Ms(void);
static void Delay_Ms(uint32_t u32Delay);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN 0 */

/* 1 ms delay với SysTick @ HCLK = 8 MHz (HSI) */
static void Delay_1_Ms(void)
{
  SysTick->LOAD = 8000U - 1U;   /* 8000 tick = 1 ms @ 8 MHz */
  SysTick->VAL  = 0U;           /* reset current value */
  SysTick->CTRL = (1U << 2)     /* CLKSOURCE = HCLK */
                | (1U << 0);    /* ENABLE = 1, không bật interrupt */

  /* Chờ COUNTFLAG (bit 16) bật lên khi đếm xong */
  while ((SysTick->CTRL & (1U << 16)) == 0U) {
    /* wait */
  }

  /* Tắt SysTick sau mỗi lần delay */
  SysTick->CTRL = 0U;
}

/* Delay nhiều ms */
static void Delay_Ms(uint32_t u32Delay)
{
  while (u32Delay--) {
    Delay_1_Ms();
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* Đặt trạng thái ban đầu: PA3, PA4, PA5 = 0 */
  GPIOA->ODR &= ~((1U << 3) | (1U << 4) | (1U << 5));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* PA3: toggle mỗi 200 ms */
      GPIOA->ODR ^= (1U << 3);
      Delay_Ms(200);

      /* PA4: toggle mỗi 500 ms */
      GPIOA->ODR ^= (1U << 4);
      Delay_Ms(500);

      /* PA5: toggle mỗi 1000 ms */
      GPIOA->ODR ^= (1U << 5);
      Delay_Ms(1000);
  }
  /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration (HSI 8MHz, no PLL)
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Bật HSI 8 MHz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Cấu hình bus clock */
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
}

/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Bật clock GPIOA */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Đặt mức logic ban đầu */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /* Cấu hình PA3, PA4, PA5 làm output push-pull */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  Error Handler
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    /* stay here */
  }
}
