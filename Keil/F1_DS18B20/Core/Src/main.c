/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"
#include "lcd.h"
#include "24cxx.h"
#include "touch.h"
#include "ds18b20.h"
#include "stdio.h"
#include "pid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}

#define xStart 30
#define yStart 230
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//清空屏幕并在右上角显??"RST"

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


double PIDOut, TempSetpoint;
short temperature;
u16 x=0,y=0;	//x是横坐标，描绘曲线，y为纵坐标，根据温度变化
u16 count=0;//计数值
char msg[100];
int ourPWM=0;
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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  delay_init(72);
  LCD_Init();
  LCD_Clear(WHITE);
  
  HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
  
  TempSetpoint = 30.0;
  
  
  char buf[128];
  POINT_COLOR=BLACK;
  LCD_ShowString(30,250,200,16,16,"KEY0:Set Temper +0.1");
  LCD_ShowString(30,280,200,16,16,"KEY1:Set Temper -0.1");
  POINT_COLOR=RED;
	
	
	//没有DS18B20测试时注释
  while(DS18B20_Init())	//DS18B20初始化	
  {
    LCD_ShowString(30,50,200,16,16,"DS18B20 ERROR");
  }	
  LCD_ShowString(30,50,200,16,16,"						"); //初始化完成，遮盖ERROR
  
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  POINT_COLOR=RED;
	//temperature=DS18B20_Get_Temp();	
//		temperature=count;		//测试用
		
	
	ourPWM=(int)Limiting_amplitude(PID_Calculation(temperature/10,TempSetpoint));
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,Limiting_amplitude(PID_Calculation(temperature,TempSetpoint)));//调节pwm占空比和限制pid和pid运算
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,ourPWM);//调节pwm占空比和限制pid和pid运算
	if(temperature<0)
	{
		LCD_ShowString(90,70,200,16,16,"-"); //显示负号
		temperature-=temperature; //转为正数
	}
	else
	{
		LCD_ShowString(90,70,200,16,16," "); //遮盖负号
	}
	
	LCD_ShowString(20,70,200,16,16,"Temper:");
	LCD_ShowString(20,90,200,16,16,"Set Temper:");
	
	sprintf(buf,"%3d",(int)temperature/10);
	LCD_ShowString(110,70,200,16,16,buf);	//显示整数
	
	
	LCD_ShowString(145,70,200,16,16,".");	//显示小数点
	sprintf(buf,"%1d",(int)(temperature-(temperature/10)*10));
	LCD_ShowString(155,70,200,16,16,buf);	//显示小数
	
	
	sprintf(buf,"%.1f",TempSetpoint);
	LCD_ShowString(130,90,200,16,16,buf);	//显示小数
	
	sprintf(buf,"%.1d",ourPWM);
	LCD_ShowString(130,50,200,16,16,buf);	//显示小数
	
	/************************曲线绘制***********************/
	
	LCD_DrawRectangle(xStart-1,yStart-100-1,xStart+180+1,yStart+1);//绘制矩形
	
	
	if(count==60)	{count=0;LCD_Fill(xStart,yStart-100,xStart+180,yStart,WHITE);}//局部清屏
			else{
				POINT_COLOR=BLACK;
				LCD_DrawLine(xStart+count*3,(yStart-(int)temperature/10),xStart+(count+1)*3,(yStart-(int)temperature/10));
			}
			count++;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(80);      	//消抖
    if(GPIO_Pin == KEY0_Pin)
	{
		LED0_T;
		LED1_T;
		TempSetpoint+=0.1;
	}
	else if(GPIO_Pin == KEY1_Pin)
	{
		LED0_T;
		TempSetpoint-=0.1;
	}
	else if(GPIO_Pin == WK_UP_Pin)
	{
		LED0_T;
	}
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==&htim3)
    {
			if(temperature<TempSetpoint*10)
			{
				temperature++;
			}
    }
}
/* USER CODE END 4 */

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
