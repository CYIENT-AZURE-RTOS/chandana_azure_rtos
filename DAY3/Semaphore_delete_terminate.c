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
int RedFlag=0;
int GreenFlag=0;
int LD2_Pin=0;
int LD3_Pin=0;
extern UART_HandleTypeDef huart1;
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define T1_REPS 20
#define T2_REPS 9
#define T3_REPS 6
#define T4_REPS 2


#define T1_RATE 10
#define T2_RATE 100
#define T3_RATE 300
#define T4_RATE 600
uint16_t Thread_Count1;
uint16_t Thread_Count2;
uint16_t Thread_Count3;
uint16_t Thread_Count4;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD ThreadThree;
TX_THREAD ThreadFour;
TX_SEMAPHORE semaphore_ptr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void ThreadThree_Entry(ULONG thread_input);
void ThreadFour_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);

//uint16_t Pin;
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
     if (tx_thread_create(&ThreadOne, "Thread One", ThreadTwo_Entry, 0,
                          pointer, APP_STACK_SIZE,
 						 THREAD_ONE_PRIO , THREAD_ONE_PREEMPTION_THRESHOLD,
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
                         THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                         100, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }



    /* Allocate the stack for ThreadTHREE.  */
        if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                             APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
        {
          ret = TX_POOL_ERROR;
        }

        /* Create ThreadThree.  */
        if (tx_thread_create(&ThreadThree, "Thread Three", ThreadThree_Entry, 0,
                             pointer, APP_STACK_SIZE,
                             THREAD_THREE_PRIO, THREAD_THREE_PREEMPTION_THRESHOLD,
                             100, TX_AUTO_START) != TX_SUCCESS)
        {
          ret = TX_THREAD_ERROR;
        }



        /* Allocate the stack for ThreadFour.  */
                if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                                     APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
                {
                  ret = TX_POOL_ERROR;
                }

                /* Create ThreadThree.  */
                if (tx_thread_create(&ThreadFour, "Thread Four", ThreadFour_Entry, 0,
                                     pointer, APP_STACK_SIZE,
                                     THREAD_FOUR_PRIO, THREAD_FOUR_PREEMPTION_THRESHOLD,
                                     100, TX_AUTO_START) != TX_SUCCESS)
                {
                  ret = TX_THREAD_ERROR;
                }


   tx_semaphore_create(&semaphore_ptr,"semaphore_ptr",4);


  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
	UINT status;
    uint8_t i;
    uint16_t pin;
  while(1)									/* Infinite loop */
  {

	  status=tx_semaphore_get(&semaphore_ptr,TX_WAIT_FOREVER);
	  Thread_Count1++;
      if(status==TX_SUCCESS)
      {
    	  HAL_UART_Transmit(&huart1,"\nthread 1 acquired successfully\r",34,100);
    	  //App_Delay(100);
    	  if(0==RedFlag)
    	  {
    		  RedFlag=1;
    		  pin=GPIO_PIN_6;
    	  }
    	  else if(0==GreenFlag)
    	  {
    		  GreenFlag=1;
    		  pin=GPIO_PIN_7;
    	  }
    	 for(i=0;i<T1_REPS;i++)
    	 {
    		 HAL_GPIO_TogglePin(GPIOH,pin);
    		 App_Delay(T1_RATE);
    	 }
    	  if(pin==LD2_Pin)
    	  {
    		  RedFlag=0;
    	  }
    	  else  if(pin==LD3_Pin)
    	  {
    		  GreenFlag=0;
    	  }
    	  HAL_UART_Transmit(&huart1,"\nthread 1 released successfully\r",34,100);
    	       tx_semaphore_put(&semaphore_ptr);
      }

	 // HAL_UART_Transmit(&huart1,"\nhello bangalore\r",19, 100);
	//HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6);	/* Toggle Green LED */


  }
}
void ThreadTwo_Entry(ULONG thread_input)
{
	UINT status;
    uint8_t i;
    uint16_t pin;
  while(1)									/* Infinite loop */
  {

	  status=tx_semaphore_get(&semaphore_ptr,TX_WAIT_FOREVER);
	  Thread_Count2++;
      if(status==TX_SUCCESS)
      {
    	  HAL_UART_Transmit(&huart1,"\nthread 2 acquired successfully\r",34,100);
    	  //App_Delay(100);
    	  if(0==RedFlag)
    	  {
    		  RedFlag=1;
    		  pin=LD2_Pin;
    	  }
    	  else if(0==GreenFlag)
    	  {
    		  GreenFlag=1;
    		  pin=LD3_Pin;
    	  }
    	 for(i=0;i<T2_REPS;i++)
    	 {
    		 HAL_GPIO_TogglePin(GPIOH,pin);
    		 App_Delay(100);
    	 }
    	  if(pin==LD2_Pin)
    	  {
    		  RedFlag=0;
    	  }
    	  else  if(pin==LD3_Pin)
    	  {
    		  GreenFlag=0;
    	  }
    	  HAL_UART_Transmit(&huart1,"\nthread 2 released successfully\r",34,100);
    	       tx_semaphore_put(&semaphore_ptr);
      }

	 // HAL_UART_Transmit(&huart1,"\nhello bangalore\r",19, 100);
	//HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6);	/* Toggle Green LED */


  }
}
void ThreadThree_Entry(ULONG thread_input)
{
	UINT status;
    uint8_t i;
    uint16_t pin;
  while(1)									/* Infinite loop */
  {

	  status=tx_semaphore_get(&semaphore_ptr,TX_WAIT_FOREVER);
	  Thread_Count3++;
      if(status==TX_SUCCESS)
      {
    	  HAL_UART_Transmit(&huart1,"\nthread 3 acquired successfully\r",34,100);
    	  //App_Delay(100);
    	  if(0==RedFlag)
    	  {
    		  RedFlag=1;
    		  pin=LD2_Pin;
    	  }
    	  else if(0==GreenFlag)
    	  {
    		  GreenFlag=1;
    		  pin=LD3_Pin;
    	  }
    	 for(i=0;i<T3_REPS;i++)
    	 {
    		 HAL_GPIO_TogglePin(GPIOH,pin);
    		 App_Delay(300);
    	 }
    	  if(pin==LD2_Pin)
    	  {
    		  RedFlag=0;
    	  }
    	  else  if(pin==LD3_Pin)
    	  {
    		  GreenFlag=0;
    	  }
    	  HAL_UART_Transmit(&huart1,"\nthread 3 released successfully\r",34,100);
    	       tx_semaphore_put(&semaphore_ptr);
      }

	 // HAL_UART_Transmit(&huart1,"\nhello bangalore\r",19, 100);
	//HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6);	/* Toggle Green LED */


  }
}
void ThreadFour_Entry(ULONG thread_input)
{
	UINT status;
    uint8_t i;
    uint16_t pin;

  while(1)									/* Infinite loop */
  {

	  status=tx_semaphore_get(&semaphore_ptr,TX_WAIT_FOREVER);
	  Thread_Count4++;
      if(status==TX_SUCCESS)
      {
    	  if(2<=Thread_Count1)
    	  {
    		  status=tx_thread_terminate(&ThreadOne);
    		  if(status==TX_SUCCESS)
    		  {
    			  HAL_UART_Transmit(&huart1,"\nTHREAD1 TERMINATED SUCCESSFULLY\r",35,100);
    			  status=tx_thread_delete(&ThreadOne);
    			  if(status==TX_SUCCESS)
    			  {
    				  HAL_UART_Transmit(&huart1,"\nTHREAD1 DELETED SUCCESSFULLY\r",32,100);
    			  }
    		  }
    	  }
    	  HAL_UART_Transmit(&huart1,"\nthread 4 acquired successfully",32,100);
    	  //App_Delay(100);
    	  if(0==RedFlag)
    	  {
    		  RedFlag=1;
    		  pin=LD2_Pin;
    	  }
    	  else if(0==GreenFlag)
    	  {
    		  GreenFlag=1;
    		  pin=LD3_Pin;
    	  }
    	 for(i=0;i<T4_REPS;i++)
    	 {
    		 HAL_GPIO_TogglePin(GPIOH,pin);
    		 App_Delay(600);
    	 }
    	  if(pin==LD2_Pin)
    	  {
    		  RedFlag=0;
    	  }
    	  else  if(pin==LD3_Pin)
    	  {
    		  GreenFlag=0;
    	  }
    	  HAL_UART_Transmit(&huart1,"\nthread 4 released successfully\r",34,100);
    	       tx_semaphore_put(&semaphore_ptr);
      }

	 // HAL_UART_Transmit(&huart1,"\nhello bangalore\r",19, 100);
	//HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6);	/* Toggle Green LED */


  }
}
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
