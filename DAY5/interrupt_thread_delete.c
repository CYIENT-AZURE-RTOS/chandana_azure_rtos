/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "app_threadx.h"
extern UART_HandleTypeDef huart1;
extern volatile uint8_t Exit1_Flag;
//TX_MUTEX *mutex_ptr2;
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
TX_THREAD ThreadOne;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;
  /* Allocate the stack for ThreadOne.  */
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                        APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
   {
     ret = TX_POOL_ERROR;
   }

  /* Create ThreadOne.  */
  if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,
                       pointer, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       100, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  // tx_mutex_create(&mutex_ptr,"my_mutex_name",TX_NO_INHERIT);


  /* USER CODE END App_ThreadX_Init */

  return ret;
}


/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}


/**
  * @brief  Function implementing the ThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
	UINT status;

  while(1)									/* Infinite loop */
  {
     if(Exit1_Flag==1)
      {
    	  HAL_UART_Transmit(&huart1,"\nuart even\r",13,100);
    	  App_Delay(100);

      }
     else
     {
    	 HAL_UART_Transmit(&huart1,"\nuart odd\r",12,100);
    	     	  App_Delay(100);
     }
     tx_thread_terminate(&ThreadOne);
       tx_thread_delete(&ThreadOne);
  }


}


/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/* USER CODE END 1 */
