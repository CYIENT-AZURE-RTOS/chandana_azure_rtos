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
TX_QUEUE queue_0;
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
TX_THREAD ThreadTwo;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
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
  /* Allocate the stack for ThreadTwo.  */
     if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                          APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
     {
       ret = TX_POOL_ERROR;
     }

    /* Create ThreadTwo.  */
    if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0,
                         pointer, APP_STACK_SIZE,
						 THREAD_TWO_PRIO , THREAD_TWO_PREEMPTION_THRESHOLD ,
                         100, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }

    if( tx_queue_create(&queue_0, "queue_0",
        16, (VOID *)&pointer,2000)!=TX_SUCCESS)
    {
    	ret=TX_QUEUE_ERROR;
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
uint8_t msg1[100]="\r this is message queue\n";

  while(1)									/* Infinite loop */
  {
	  HAL_UART_Transmit(&huart1,"THREAD1 SEND MESSAGE INTO QUEUE",31,100);
	  status=tx_queue_send(&queue_0,msg1,TX_WAIT_FOREVER);
     if(TX_SUCCESS==status)
      {
    	 HAL_UART_Transmit(&huart1,"\rTHREAD1 SENT MESSAGE INTO QUEUE\n",35,100);
    	 tx_thread_sleep(100);
    	 // App_Delay(100);

      }
     else
     {
    	 HAL_UART_Transmit(&huart1,"THREAD1 FAILED SEND MESSAGE INTO QUEUE\n",40,100);
    	     	  //App_Delay(100);
     }
       }


}

void ThreadTwo_Entry(ULONG thread_input)
{
	UINT status;
uint8_t msg_rx[100]="\r thread for receiving \n";

  while(1)									/* Infinite loop */
  {
	  HAL_UART_Transmit(&huart1,"\rTHREAD1 receive MESSAGE INTO QUEUE\n",38,100);
	  status=tx_queue_receive(&queue_0,msg_rx,TX_WAIT_FOREVER);
     if(TX_SUCCESS==status)
      {
    	 HAL_UART_Transmit(&huart1,"\rTHREAD1 recived MESSAGE SUCCESSFULLYE\n",41,100);
    	 tx_thread_sleep(100);
    	 // App_Delay(100);

      }
     else
     {
    	 HAL_UART_Transmit(&huart1,"THREAD1 FAILED SEND MESSAGE INTO QUEUE\n",40,100);
    	 HAL_UART_Transmit(&huart1,msg_rx,strlen(msg_rx),100);
    	     	  //App_Delay(100);
     }
       }


}
/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */

/* USER CODE END 1 */
