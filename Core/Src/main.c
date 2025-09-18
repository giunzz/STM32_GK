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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
/* USER CODE BEGIN PV */
/* SysTick delay kiểu thanh ghi (1 ms) */
static void Delay_1_Ms(void);
static void Delay_Ms(uint32_t u32Delay);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* 1 ms delay với SysTick @ HCLK = 8 MHz (HSI) */
static void Delay_1_Ms(void)
{
  /* SysTick là counter 24-bit đếm xuống theo HCLK khi CLKSOURCE = 1 */
  SysTick->LOAD = 8U * 1000U - 1U;  /* 8000 tick = 1 ms @ 8 MHz */
  SysTick->VAL  = 0U;               /* reset current value */
  SysTick->CTRL = (1U << 2)         /* CLKSOURCE = HCLK */
                | (1U << 0);        /* ENABLE = 1, không bật interrupt */
  /* Đợi COUNTFLAG (bit 16) bật lên khi đếm xong */
  while ((SysTick->CTRL & (1U << 16)) == 0U) {
    /* wait */
  }
  /* Tắt SysTick sau mỗi lần delay (giữ “sạch”) */
  SysTick->CTRL = 0U;
}

/* Delay theo ms bằng cách lặp 1 ms */
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
  /* USER CODE BEGIN 2 */
  /* Mức khởi tạo:
     - PC13: set 1 (LED on-board active-low -> tắt)
     - PB1, PB2: kéo xuống 0 */
  GPIOC->ODR |=  (1U << 13);
  GPIOB->ODR &= ~(1U << 1);
  GPIOB->ODR &= ~(1U << 2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* --- PB1: toggle mỗi 50 ms --- */
    GPIOB->ODR ^= (1U << 1);
    Delay_Ms(50);

    /* --- PB2: toggle mỗi 5 ms --- */
    GPIOB->ODR ^= (1U << 2);
    Delay_Ms(5);


    GPIOC->ODR ^= (1U << 13);
    Delay_Ms(1000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  *        Giữ HSI 8 MHz như dự án gốc.
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator config: HSI ON, không dùng PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Bus clocks: SYSCLK=HSI(8MHz), HCLK=8MHz, PCLK1=8MHz, PCLK2=8MHz */
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
  *        Bật clock cho GPIOC, GPIOB và cấu hình:
  *        - PC13: Output push-pull (MODE=11 ~ 50MHz), CNF=00
  *        - PB1 : Output push-pull (MODE=11 ~ 50MHz), CNF=00
  *        - PB2 : Output push-pull (MODE=11 ~ 50MHz), CNF=00
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* -------- PC13 (CRH, pin 13 -> field index = (13-8)=5) -------- */
  {
    uint32_t tmp = GPIOC->CRH;
    tmp &= ~(0xFU << (5U * 4U));     /* clear 4 bit cấu hình pin13 */
    tmp |=  (0x3U << (5U * 4U));     /* MODE=11 (50MHz), CNF=00 (GP Push-Pull) */
    GPIOC->CRH = tmp;
  }

  /* -------- PB1 (CRL, pin 1 -> field index = 1) -------- */
  {
    uint32_t tmp = GPIOB->CRL;
    tmp &= ~(0xFU << (1U * 4U));     /* clear field pin1 */
    tmp |=  (0x3U << (1U * 4U));     /* MODE=11 (50MHz), CNF=00 */
    GPIOB->CRL = tmp;
  }

  /* -------- PB2 (CRL, pin 2 -> field index = 2) -------- */
  {
    uint32_t tmp = GPIOB->CRL;
    tmp &= ~(0xFU << (2U * 4U));     /* clear field pin2 */
    tmp |=  (0x3U << (2U * 4U));     /* MODE=11 (50MHz), CNF=00 */
    GPIOB->CRL = tmp;
  }
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
  (void)file; (void)line;
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
