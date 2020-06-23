/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
typedef StaticQueue_t osStaticMessageQDef_t;
osThreadId_t defaultTaskHandle;
osThreadId_t taskUartSendHandle;
osThreadId_t taskFeedWDHandle;
osMessageQueueId_t uartRxQueueHandle;
uint8_t uartRxQueueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t uartRxQueueControlBlock;
osMessageQueueId_t uartTxQueueHandle;
uint8_t uartTxQueueBuffer[ 512 * sizeof( uint8_t ) ];
osStaticMessageQDef_t uartTxQueueControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskUartSend(void *argument);
void StartTaskFeedWD(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
osKernelInitialize();

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
  /* definition and creation of uartRxQueue */
  const osMessageQueueAttr_t uartRxQueue_attributes = {
    .name = "uartRxQueue",
    .cb_mem = &uartRxQueueControlBlock,
    .cb_size = sizeof(uartRxQueueControlBlock),
    .mq_mem = &uartRxQueueBuffer,
    .mq_size = sizeof(uartRxQueueBuffer)
  };
  uartRxQueueHandle = osMessageQueueNew (512, sizeof(uint8_t), &uartRxQueue_attributes);

  /* definition and creation of uartTxQueue */
  const osMessageQueueAttr_t uartTxQueue_attributes = {
    .name = "uartTxQueue",
    .cb_mem = &uartTxQueueControlBlock,
    .cb_size = sizeof(uartTxQueueControlBlock),
    .mq_mem = &uartTxQueueBuffer,
    .mq_size = sizeof(uartTxQueueBuffer)
  };
  uartTxQueueHandle = osMessageQueueNew (512, sizeof(uint8_t), &uartTxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* definition and creation of taskUartSend */
  const osThreadAttr_t taskUartSend_attributes = {
    .name = "taskUartSend",
    .priority = (osPriority_t) osPriorityHigh,
    .stack_size = 128
  };
  taskUartSendHandle = osThreadNew(StartTaskUartSend, NULL, &taskUartSend_attributes);

  /* definition and creation of taskFeedWD */
  const osThreadAttr_t taskFeedWD_attributes = {
    .name = "taskFeedWD",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  taskFeedWDHandle = osThreadNew(StartTaskFeedWD, NULL, &taskFeedWD_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskUartSend */
/**
* @brief Function implementing the taskUartSend thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUartSend */
void StartTaskUartSend(void *argument)
{
  /* USER CODE BEGIN StartTaskUartSend */
  /* Infinite loop */
	uint8_t data;
	osStatus_t status;
  for(;;)
  {
		status = osMessageQueueGet(uartRxQueueHandle, &data, NULL, 0U);
		if (status == osOK)
		{
			osMessageQueuePut(uartTxQueueHandle,&data,NULL,0U);
			LL_USART_EnableIT_TXE(USART1);
		}
    osDelay(1);
  }
  /* USER CODE END StartTaskUartSend */
}

/* USER CODE BEGIN Header_StartTaskFeedWD */
/**
* @brief Function implementing the taskFeedWD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskFeedWD */
void StartTaskFeedWD(void *argument)
{
  /* USER CODE BEGIN StartTaskFeedWD */
  /* Infinite loop */
  for(;;)
  {
		FeddWDG();
    osDelay(1);
  }
  /* USER CODE END StartTaskFeedWD */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
