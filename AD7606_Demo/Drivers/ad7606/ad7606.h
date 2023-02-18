/*
 * ad7606.h
 *
 *  Created on: 2023年2月18日
 *      Author: NJY_r
 */

#ifndef AD7606_AD7606_H_
#define AD7606_AD7606_H_

#include "gpio.h"
#include "tim.h"

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

// 引脚定义
#define AD7606Rst_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define AD7606Rst_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define AD7606Cs_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define AD7606Cs_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)

#define AD7606Busy HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)

#define AD7606_Os0_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define AD7606_Os0_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define AD7606_Os1_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define AD7606_Os1_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define AD7606_Os2_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define AD7606_Os2_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)

/* AD7606 FSMC总线地址，只能读，无需写 */
#define AD7606_RESULT()	*(__IO uint16_t *)0x6C000000

// 应用层函数
void AD7606_Init();
void AD7606_RESET();
void AD7606_SetOS(uint8_t _usOS);
void AD7606_ReadNowAdc();

// 过采样速率枚举结构体
typedef enum
{
	AD_OS_NO = 0,
	AD_OS_X2,
	AD_OS_X4,
	AD_OS_X8,
	AD_OS_X16,
	AD_OS_X32,
	AD_OS_X64,
}AD7606_OS_E;

// AD7606参数保存结构体
typedef struct
{
	uint8_t ucOS;			/* 过采样倍率，0 - 6. 0表示无过采样 */
	int16_t sNowAdc[8];		/* 当前ADC值, 有符号数 */
}AD7606_VAR_T;

/* *****************************************************
 *
 * 以下为FIFO模式所使用
 *
 * *****************************************************/
/* AD数据采集缓冲区 FIFO */
#define ADC_FIFO_SIZE	(2*1024)	/* 总体样本数 */

typedef struct
{
	/* FIFO 结构 */
	uint16_t usRead;		/* 读指针 */
	uint16_t usWrite;		/* 写指针 */

	uint16_t usCount;		/* 新数据个数 */
	uint8_t ucFull;			/* FIFO满标志 */

	int16_t  sBuf[ADC_FIFO_SIZE];
}AD7606_FIFO_T;

uint8_t AD7606_HasNewData();
uint8_t AD7606_ReadFifo(uint16_t *_usReadAdc);
void AD7606_StartRecord();
void AD7606_ISR();


#endif /* AD7606_AD7606_H_ */
