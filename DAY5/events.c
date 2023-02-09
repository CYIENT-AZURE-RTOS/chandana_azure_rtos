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
TX_EVENT_FLAGS_GROUP Event_grp1;
//TX_QUEUE queue_0;
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
TX_THREAD ThreadThree;
TX_EVENT_FLAGS_GROUP Event_Grp1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void ThreadThree_Entry(ULONG thread_input);
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
    /* Allocate the stack for ThreadThree.  */
         if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                              APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
         {
           ret = TX_POOL_ERROR;
         }

        /* Create ThreadTwo.  */
        if (tx_thread_create(&ThreadThree, "Thread Three", ThreadThree_Entry, 0,
                             pointer, APP_STACK_SIZE,
    						 THREAD_THREE_PRIO , THREAD_THREE_PREEMPTION_THRESHOLD ,
                             100, TX_AUTO_START) != TX_SUCCESS)
        {
          ret = TX_THREAD_ERROR;
        }
//    if( tx_queue_create(&queue_0, "queue_0",
//        16, (VOID *)&pointer,2000)!=TX_SUCCESS)
//    {
//    	ret=TX_QUEUE_ERROR;
//    }
    ret=tx_event_flags_create(&Event_grp1,"Event_grp1");
    if(ret==TX_SUCCESS)
    {
    	HAL_UART_Transmit(&huart1,"\r could not create the event\n",30, 100);
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
	ULONG actual_events;
//status=tx_event_flags_get(&Event_Grp1,0x111,TX_AND_CLEAR,&actual_events,20);
  while(1)									/* Infinite loop */
  {
	 // HAL_UART_Transmit(&huart1,"THREAD1 SEND MESSAGE INTO QUEUE",31,100);
	 // status=tx_queue_send(&queue_0,msg1,TX_WAIT_FOREVER);
	  status=tx_event_flags_get(&Event_Grp1,0x111,TX_AND_CLEAR,&actual_events,20);
     if(TX_SUCCESS==status)
      {
    	 HAL_UART_Transmit(&huart1,"EVENT DONE SUCCESSFULLY\n",25,100);
    	 tx_thread_sleep(100);
    	 // App_Delay(100);

      }
     else
     {
    	 HAL_UART_Transmit(&huart1,"\rEVENT NOT DONE\n",16,100);
     }

    }


}

void ThreadTwo_Entry(ULONG thread_input)
{
	UINT status;

  while(1)									/* Infinite loop */
  {
	  HAL_UART_Transmit(&huart1,"\rTHREAD2 executing\n",21,100);
	  status=tx_event_flags_set(&Event_Grp1,0x11,TX_OR);
     if(TX_SUCCESS==status)
      {
    	 HAL_UART_Transmit(&huart1,"\r events 0 &4\n",16,100);
    	 tx_thread_sleep(100);
    	 // App_Delay(100);

      }
     else
     {
    	 HAL_UART_Transmit(&huart1,"\rEVENT NOT DONE\n",16,100);
     }
       }

}
void ThreadThree_Entry(ULONG thread_input)
{
	UINT status;

  while(1)									/* Infinite loop */
  {
	  HAL_UART_Transmit(&huart1,"\rTHREAD3 executing\n",21,100);
	  status=tx_event_flags_set(&Event_Grp1,0x100,TX_OR);
     if(TX_SUCCESS==status)
      {
    	 HAL_UART_Transmit(&huart1,"\r event 8 set\n",16,100);
    	 tx_thread_sleep(100);
    	 // App_Delay(100);

      }
     else
         {
        	 HAL_UART_Transmit(&huart1,"\rEVENT NOT DONE\n",16,100);
         }
       }

}
/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */

/* USER CODE END 1 */
