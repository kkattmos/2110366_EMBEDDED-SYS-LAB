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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
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
UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osMessageQId uartQueueHandle;
osMutexId UartMutexHandle;
/* USER CODE BEGIN PV */
osThreadId thread1Handle;
osThreadId thread2Handle;
osThreadId thread3Handle;
osThreadId thread4Handle;
osThreadId uartTx0Handle;
osThreadId uartTx1Handle;
osThreadId uartConsumerHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
void Thread1_50ms(void const *argument);
void Thread2_18ms(void const *argument);
void Thread3_128ms(void const *argument);
void Thread4_64ms(void const *argument);

// UART 
void Task_UART_Tx0(void const *argument);
void Task_UART_Tx1(void const *argument);

// Function prototypes
void Task_UART_Consumer(void const *argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
  
  // Thread 1 (50 ms)
  osThreadDef(Task1, Thread1_50ms, osPriorityNormal, 0, 128);
  thread1Handle = osThreadCreate(osThread(Task1), NULL);

  // Thread 2 (18 ms)
  osThreadDef(Task2, Thread2_18ms, osPriorityNormal, 0, 128);
  thread2Handle = osThreadCreate(osThread(Task2), NULL);

  // Thread 3 (128 ms)
  osThreadDef(Task3, Thread3_128ms, osPriorityNormal, 0, 128);
  thread3Handle = osThreadCreate(osThread(Task3), NULL);

  // Thread 4 (64 ms)
  osThreadDef(Task4, Thread4_64ms, osPriorityNormal, 0, 128);
  thread4Handle = osThreadCreate(osThread(Task4), NULL);
  
  // Producer Thread 0  
  osThreadDef(UartTx0, Task_UART_Tx0, osPriorityNormal, 0, 256);
  uartTx0Handle = osThreadCreate(osThread(UartTx0), NULL);
  
  // Producer Thread 1 
  osThreadDef(UartTx1, Task_UART_Tx1, osPriorityNormal, 0, 256);
  uartTx1Handle = osThreadCreate(osThread(UartTx1), NULL);

  // Consumer Threads 
  osThreadDef(UartConsumer, Task_UART_Consumer, osPriorityNormal, 0, 256);
  uartConsumerHandle = osThreadCreate(osThread(UartConsumer), NULL);



  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of UartMutex */
  osMutexDef(UartMutex);
  UartMutexHandle = osMutexCreate(osMutex(UartMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of uartQueue */
  osMessageQDef(uartQueue, 16, uint32_t);
  uartQueueHandle = osMessageCreate(osMessageQ(uartQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Thread1_50ms
void Thread1_50ms(void const *argument) {
  while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
    osDelay(50);
  }
}

//Thread2_18ms
void Thread2_18ms(void const *argument) {
  while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
    osDelay(18);
  }
}

//Thread3_128ms 
void Thread3_128ms(void const *argument) {
  while (1) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
    osDelay(128);
  }
}

//Thread4_64ms 
void Thread4_64ms(void const *argument) {
  while (1) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    osDelay(64);
  }
}

// UART Consumer Thread 
void Task_UART_Consumer(void const *argument) {
  osEvent event;
  char *msgPtr;

  while (1) {
    event = osMessageGet(uartQueueHandle, osWaitForever);
    if (event.status == osEventMessage) {
      msgPtr = (char*)event.value.p;
      HAL_UART_Transmit(&huart2, (uint8_t*)msgPtr, strlen(msgPtr), 1000);
      vPortFree(msgPtr);
    }
  }
}

// UART Producer Thread 0 
void Task_UART_Tx0(void const *argument) {
  int threadID = 0;
  int idx = 0;

  while (1) {
    char *buffer = (char*)pvPortMalloc(32 * sizeof(char));
    if (buffer == NULL) {
      osDelay(1);
      continue;
    }
    
    snprintf(buffer, 32, "TID: %d %d\r\n", threadID, idx++);

    if (osMessagePut(uartQueueHandle, (uint32_t)buffer, 100) != osOK) {
      vPortFree(buffer);
    }
    osDelay(1);
  }
}

// UART Producer Thread 1 
void Task_UART_Tx1(void const *argument) {
  int threadID = 1;
  int idx = 0;

  while (1) {
    char *buffer = (char*)pvPortMalloc(32 * sizeof(char));
    if (buffer == NULL) {
      osDelay(1);
      continue;
    }  
    
    snprintf(buffer, 32, "TID: %d %d\r\n", threadID, idx++);
    
    if (osMessagePut(uartQueueHandle, (uint32_t)buffer, 100) != osOK) {
      vPortFree(buffer);
    }
    osDelay(1);
  }
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
