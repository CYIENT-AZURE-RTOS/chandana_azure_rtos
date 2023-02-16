/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "hts221.h"
#include "custom_bus.h"
#include <stdio.h>
#include <string.h>
#include "ism330dhcx.h"
#include "app_FreeFallDetection.h"
#include "custom_mems_conf.h"
//#include "b_u585i_iot02a_bus.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD ThreadThree;
TX_QUEUE Myqueue;
TX_TIMER timer;
TX_TIMER rtc;

extern ISM330DHCX_Object_t pobj;
extern HTS221_Object_t HTS221_Obj;

uint8_t          HTS221_Id;
float            HTS221_Odr;
uint8_t          HTS221_Status;
float            HTS221_HUM_Value;
float            HTS221_HUM_Odr;
float            HTS221_TEMP_Value;
float            HTS221_TEMP_Odr;


uint8_t          ISM330DHCX_Id;
//float            ISM330DHCX_Odr;
uint8_t          ISM330DHCX_Status;

ISM330DHCX_Axes_t accl_val;
uint8_t tx_buf[100];
uint8_t len;

uint8_t temp[50];

volatile uint8_t TEMP_Data;
volatile uint8_t Acce_Data[4];

char time[30];
char date[30];


char msg[50];
//uint8_t tf_buf[20];
uint8_t  rx_buf[10], str_buf[20];
uint8_t length;

  uint8_t t1_count_hour=0;
uint8_t t1_count_min=0;
uint8_t t1_count_sec=0;
char array[30];


//uint8_t task1[]="im in task one lkzslisnlknalksf aslkfnaksnfklasnf  ASDASDASFASFASFASFASF\r\n";
uint8_t task2[]="\r\n im in task two \n\r";

extern volatile uint8_t Ext1_Flag;
extern volatile uint8_t fall_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void threadReadTemp(ULONG thread_input);
void threadSwitchPress(ULONG thread_input);
void printTime(ULONG thread_input);
void App_Delay(uint32_t Delay);

void printSensorData(void);
void printRTC(void);


 static int32_t HTS221_Probe(HTS221_Object_t *pObj);
 static int32_t ISM330DHCX_Probe(ISM330DHCX_Object_t *pObj);

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
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer1,*pointer2,*pointer3, *Qpointer;

 // ret = tx_byte_allocate(byte_pool, (VOID **) &pointer1, APP_STACK_SIZE, TX_NO_WAIT);


   /* Allocate the stack for ThreadOne.  */
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer1,
                        APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
   {
     ret = TX_POOL_ERROR;
   }

   /* Create ThreadOne.  */
   if (tx_thread_create(&ThreadOne, "Thread One", threadReadTemp, 0,
                        pointer1, APP_STACK_SIZE,
                        THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
						TX_8_ULONG, TX_AUTO_START) != TX_SUCCESS)
   {
     ret = TX_THREAD_ERROR;
   }

   //ret = tx_byte_allocate(byte_pool, (VOID **) &pointer2,APP_STACK_SIZE, TX_NO_WAIT);


   /* Allocate the stack for ThreadTwo.  */
    if (tx_byte_allocate(byte_pool, (VOID **) &pointer2,
                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      ret = TX_POOL_ERROR;
    }

    /* Create ThreadTwo.  */
    if (tx_thread_create(&ThreadTwo, "Thread Two", threadSwitchPress, 0,
                         pointer2, APP_STACK_SIZE,
                         THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
						 TX_8_ULONG, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }

    /* Allocate the stack for ThreadThree.  */
    char *three;
        if (tx_byte_allocate(byte_pool, (VOID **) &three,
                             APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
        {
          ret = TX_POOL_ERROR;
        }

        /* Create ThreadThree.  */
        if (tx_thread_create(&ThreadThree, "Thread Three", printTime, 0,
                             three, APP_STACK_SIZE,
    						 THREAD_THREE_PRIO, THREAD_THREE_PREEMPTION_THRESHOLD,
     						TX_8_ULONG, TX_AUTO_START) != TX_SUCCESS)
        {
          ret = TX_THREAD_ERROR;
        }



    /*Allocate the byte for message queue*/
    if (tx_byte_allocate(byte_pool, (VOID **) &Qpointer,
                           1024, TX_NO_WAIT) != TX_SUCCESS)
      {
        ret = TX_POOL_ERROR;
      }


    if(tx_queue_create(&Myqueue, "queue", 16, (VOID *) Qpointer, 1000) != TX_SUCCESS )
    {
          ret = TX_THREAD_ERROR;
    }

	/*Create the Timer for print sensor's data for every 1 min*/

	if(tx_timer_create(&timer, "tx_timer1", printSensorData, 0, 200, 6000, TX_AUTO_ACTIVATE) != TX_SUCCESS)
	{
		ret = TX_THREAD_ERROR;

	}

	/*Create the Timer for print RTC every time*/

		if(tx_timer_create(&rtc, "tx_timer2", printRTC, 0, 200, 100, TX_AUTO_ACTIVATE) != TX_SUCCESS)
		{
			ret = TX_THREAD_ERROR;

		}

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

/* USER CODE BEGIN 1 */

void printSensorData(void)
{
	sprintf(temp, "Temp data : %d\r\n", TEMP_Data );
	HAL_UART_Transmit(&huart1, temp, sizeof(temp), 100);
	len = sprintf(tx_buf,"Acce_data: x = %d\ty = %d\tz = %d\n\r",accl_val.x,accl_val.y,accl_val.z);
	HAL_UART_Transmit(&huart1, tx_buf, len, 100);

}

void printRTC(void)
{

	t1_count_sec++;
	  sprintf(array,"TIME : %d:%d:%d\r\n",t1_count_hour, t1_count_min, t1_count_sec);
	  HAL_UART_Transmit(&huart1,array,strlen(array), 100);
	if(t1_count_sec==60)
	{
		t1_count_min++; t1_count_sec=0;
	}
	if(t1_count_min==60)
	{
		t1_count_hour++;
		t1_count_min=0,t1_count_sec=0;
	}

}

void threadReadTemp(ULONG thread_input)
{
  uint8_t status;
  uint8_t msg[20] = "Hello India";

  HTS221_Probe(&HTS221_Obj);
  HTS221_Init(&HTS221_Obj);
  HTS221_TEMP_Enable(&HTS221_Obj);
	while(1)
	{


		  HTS221_TEMP_GetTemperature(&HTS221_Obj, &HTS221_TEMP_Value);
//		  len = sprintf(tx_buf,"temp:%f \n\r",HTS221_TEMP_Value);
//		  HAL_UART_Transmit(&huart1, tx_buf, len, 100);

		  TEMP_Data = (int)HTS221_TEMP_Value;
		  if(1 == Ext1_Flag)
		  {
				sprintf(temp, "Present Temp value : %d\r\n", TEMP_Data );
				HAL_UART_Transmit(&huart1, temp, sizeof(temp), 100);
				len = sprintf(tx_buf,"acc_data: x = %d\ty = %d\tz = %d\n\r",accl_val.x,accl_val.y,accl_val.z);
				HAL_UART_Transmit(&huart1, tx_buf, len, 100);
										/* Delay for 100=1000ms */
				Ext1_Flag =0;

		  }

		  App_Delay(100);
	}

}

void threadSwitchPress(ULONG thread_input)
{
  uint8_t status=0;

  while(1)									/* Infinite loop */
  {


	  FreeFallDetection_Init();
	  if(fall_flag ==1)
	  	{
		  //FreeFallDetection_Handler();
		  HAL_UART_Transmit(&huart1, "*******************************\r\n", 50, 100);
		  HAL_UART_Transmit(&huart1, "!!!!!!!!!!!Fall Detected!!!!!!!!!!\r\n", 50, 100);
		  HAL_UART_Transmit(&huart1, "*******************************\r\n", 50, 100);
//	  		FreeFallDetection_SetState(0);
		  fall_flag= 0;
	  	}

	  //HAL_UART_Transmit(&huart1, "\r\n Thread2 \n\r", 15, 200);

	  App_Delay(100);


 }
}

void printTime(ULONG thread_input)
{
	if(ISM330DHCX_Probe(&pobj) != ISM330DHCX_OK)
					      {
					        __BKPT(0);
					      }

					      if(ISM330DHCX_Init(&pobj) != ISM330DHCX_OK)
					      {
					        __BKPT(0);
					      }

					      if(ISM330DHCX_ACC_Enable(&pobj) != ISM330DHCX_OK)
					    		         {
					    		           __BKPT(0);
					    		         }

					    		         if(ISM330DHCX_GYRO_Enable(&pobj) != ISM330DHCX_OK)
					    		         {
					    		           __BKPT(0);
					    		         }
	while (1)
	  {
	    /* USER CODE END WHILE */


				    		         if(ISM330DHCX_ACC_GetAxes(&pobj, &accl_val) != ISM330DHCX_OK)
				    		       	    {
				    		       	      __BKPT(0);
				    		       	    }
//				    		       	    len = sprintf(tx_buf,"acc_data: x = %d\ty = %d\tz = %d\n\r",accl_val.x,accl_val.y,accl_val.z);
//				    		       	    HAL_UART_Transmit(&huart1, tx_buf, len, 100);
//				    		       	   len = sprintf(tx_buf,"gyro_data: x = %d\ty = %d\tz = %d\n\r",accl_val.x,accl_val.y,accl_val.z);
//				    		       		    HAL_UART_Transmit(&huart1, tx_buf, len, 100);

		App_Delay(30);
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

static int32_t HTS221_Probe(HTS221_Object_t *pObj)
{
  HTS221_IO_t         io_ctx = {0};

  /* Configure the accelerometer driver */
  io_ctx.BusType     = HTS221_I2C_BUS; /* I2C */
  io_ctx.Address     = HTS221_I2C_ADDRESS;
  io_ctx.Init        = BSP_I2C2_Init;
  io_ctx.DeInit      = BSP_I2C2_DeInit;
  io_ctx.ReadReg     = BSP_I2C2_ReadReg;
  io_ctx.WriteReg    = BSP_I2C2_WriteReg;

  return HTS221_RegisterBusIO(pObj, &io_ctx);
}

static int32_t ISM330DHCX_Probe(ISM330DHCX_Object_t *pObj)
{
	ISM330DHCX_IO_t         io_ctx = {0};

  /* Configure the accelerometer driver */
  io_ctx.BusType     = ISM330DHCX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DHCX_I2C_ADD_H;
  io_ctx.Init        = BSP_I2C2_Init;
  io_ctx.DeInit      = BSP_I2C2_DeInit;
  io_ctx.ReadReg     = BSP_I2C2_ReadReg;
  io_ctx.WriteReg    = BSP_I2C2_WriteReg;

  return ISM330DHCX_RegisterBusIO(pObj, &io_ctx);
}

/* USER CODE END 1 */
