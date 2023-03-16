/*
 * ad7606.c
 *
 *  Created on: 2023年2月18日
 *      Author: NJY_r
 */

#include "ad7606.h"


AD7606_VAR_T g_tAD7606;		/* 定义1个全局变量，保存一些参数 */

AD7606_FIFO_T g_tAdcFifo;	/* 定义FIFO结构体变量 */


/* AD7606过采样标识符 */
static const char * AD7606_SampleStr[]=
{
	"无过采样, 最高支持采样率200Ksps",
	"2倍过采样, 最高支持采样率100Ksps",
	"4倍过采样, 最高支持采样率50Ksps",
	"8倍过采样, 最高支持采样率25Ksps",
	"16倍过采样, 最高支持采样率12.5Ksps",
	"32倍过采样, 最高支持采样率6.25Ksps",
	"64倍过采样, 最高支持采样率3.125Ksps",
};

/* AD7606不同过采样配置支持的最高采样速度 */
static const uint32_t AD7606_SampleFreq[]=
{
	200000,  /* 无过采样最高采样率200Ksps  */
	100000,  /* 2倍过采样最高采样率100Ksps */
	50000,   /* 4倍过采样最高采样率50Ksps  */
	25000,   /* 8倍过采样最高采样率25Ksps  */
	12500,   /* 16倍过采样最高采样率12.5Ksps  */
	6250,    /* 32倍过采样最高采样率6.25Ksps  */
	3125,    /* 64倍过采样最高采样率3.125Ksps */
};

/*
 * ****************************************************************************** *
 * 函 数 名: AD7606_Init
 * 功能说明: AD7606自定义初始化函数
 * 			1、设置OS012
 * 			2、复位
 * 			3、CS低电平使能
 * 			4、ST_A和ST_B输入一个时钟使AD7606开始AD转换
 * 			5、延时一段时间用FSMC读取
 * 形    参: 无
 * 返 回 值: 无
 * ****************************************************************************** *
 */
void AD7606_Init()
{
	AD7606_SetOS(AD_OS_NO);
	AD7606_RESET();
	AD7606_StartRecord();
	AD7606Cs_L;
}


/*
 * ****************************************************************************** *
 * 函 数 名: AD7606_RESET
 * 功能说明: AD7606复位程序
 * 形    参: 无
 * 返 回 值: 无
 * ****************************************************************************** *
 */
void AD7606_RESET()
{
	AD7606Rst_L;
	/* RESET复位高电平脉冲宽度最小50ns。 */
	AD7606Rst_H;
	// 主频168M  即1/168M = 0.00595us = 5.95ns
	for (int i =10; i>0; i--)
	{
		__NOP();
	}

	AD7606Rst_L;
}


/*
 * ****************************************************************************** *
 * 函 数 名: AD7606_SetOS
 * 功能说明: 配置AD7606数字滤波器，也就设置过采样倍率。
 * 			通过设置 AD7606_OS0、OS1、OS2口线的电平组合状态决定过采样倍率。
 * 			启动AD转换之后，AD7606内部自动实现剩余样本的采集，然后求平均值输出。
 *
 *			过采样倍率越高，转换时间越长。
 *			  0、无过采样时，AD转换时间 = 3.45us - 4.15us
 *			  1、2倍过采样时 = 7.87us - 9.1us
 *			  2、4倍过采样时 = 16.05us - 18.8us
 *			  3、8倍过采样时 = 33us - 39us
 *			  4、16倍过采样时 = 66us - 78us
 *			  5、32倍过采样时 = 133us - 158us
 *			  6、64倍过采样时 = 257us - 315us*
 *	形    参: _ucOS : 过采样倍率, 0 - 6
 *	返 回 值: 无
 * ****************************************************************************** *
 */
void AD7606_SetOS(uint8_t _usOS)
{
	g_tAD7606.ucOS = _usOS;
	switch (_usOS)
	{
		case AD_OS_X2:
			AD7606_Os0_0;
			AD7606_Os1_0;
			AD7606_Os2_1;
			break;
		case AD_OS_X4:
			AD7606_Os0_0;
			AD7606_Os1_1;
			AD7606_Os2_0;
			break;
		case AD_OS_X8:
			AD7606_Os0_0;
			AD7606_Os1_1;
			AD7606_Os2_1;
			break;
		case AD_OS_X16:
			AD7606_Os0_1;
			AD7606_Os1_0;
			AD7606_Os2_0;
			break;
		case AD_OS_X32:
			AD7606_Os0_1;
			AD7606_Os1_0;
			AD7606_Os2_1;
			break;
		case AD_OS_X64:
			AD7606_Os0_1;
			AD7606_Os1_1;
			AD7606_Os2_0;
			break;
		case AD_OS_NO:
		default:
			g_tAD7606.ucOS = AD_OS_NO;
			AD7606_Os0_0;
			AD7606_Os1_0;
			AD7606_Os2_0;
			break;
	}

}

/*
*********************************************************************************************************
*	函 数 名: AD7606_ReadNowAdc
*	功能说明: 读取8路采样结果。结果存储在全局变量 g_tAD7606
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_ReadNowAdc(void)
{
	g_tAD7606.sNowAdc[0] = AD7606_RESULT();	/* 读第1路样本 */
	g_tAD7606.sNowAdc[1] = AD7606_RESULT();	/* 读第2路样本 */
	g_tAD7606.sNowAdc[2] = AD7606_RESULT();	/* 读第3路样本 */
	g_tAD7606.sNowAdc[3] = AD7606_RESULT();	/* 读第4路样本 */
	g_tAD7606.sNowAdc[4] = AD7606_RESULT();	/* 读第5路样本 */
	g_tAD7606.sNowAdc[5] = AD7606_RESULT();	/* 读第6路样本 */
	g_tAD7606.sNowAdc[6] = AD7606_RESULT();	/* 读第7路样本 */
	g_tAD7606.sNowAdc[7] = AD7606_RESULT();	/* 读第8路样本 */

//	AD7606_SEGGER_RTTOUT();
}

/*
*********************************************************************************************************
*		下面的函数用于定时采集模式。 TIM8硬件定时中断中读取ADC结果，存在全局FIFO
*		若仅仅需要使用低速测电压用以上软件轮询查看即可
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	函 数 名: AD7606_HasNewData
*	功能说明: 判断FIFO中是否有新数据
*	形    参:  无
*	返 回 值: 1 表示有，0表示暂无数据
*********************************************************************************************************
*/
uint8_t AD7606_HasNewData()
{
	if (g_tAdcFifo.usCount > 0)
	{
		return 1;
	}

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_FifoFull
*	功能说明: 判断FIFO是否满
*	形    参: 无
*	返 回 值: 1 表示满，0表示未满
*********************************************************************************************************
*/
uint8_t AD7606_FifoFull(void)
{
	return g_tAdcFifo.ucFull;
}


/*
*********************************************************************************************************
*	函 数 名: AD7606_ReadFifo
*	功能说明: 从FIFO中读取一个ADC值
*	形    参:  _usReadAdc : 存放ADC结果的变量指针
*	返 回 值: 1 表示OK，0表示暂无数据
*********************************************************************************************************
*/
uint8_t AD7606_ReadFifo(uint16_t *_usReadAdc)
{
	if(AD7606_HasNewData())
	{
		*_usReadAdc = g_tAdcFifo.sBuf[g_tAdcFifo.usRead];
		if (++g_tAdcFifo.usRead >= ADC_FIFO_SIZE)
		{
			g_tAdcFifo.usRead = 0;
		}

		DISABLE_INT();

		if (g_tAdcFifo.usCount > 0)
		{
			g_tAdcFifo.usCount--;
		}

		ENABLE_INT();
		return 1;
	}

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_StartRecord
*	功能说明: 开始采集
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StartRecord()
{
	/* 必须在开启定时器之前清0 */
	g_tAdcFifo.usRead = 0;
	g_tAdcFifo.usWrite = 0;
	g_tAdcFifo.usCount = 0;
	g_tAdcFifo.ucFull = 0;
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_ISR
*	功能说明: 定时采集中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_ISR(void)
{
	uint8_t i;

	AD7606_ReadNowAdc();


		g_tAdcFifo.sBuf[g_tAdcFifo.usWrite] = g_tAD7606.sNowAdc[0];
		if (++g_tAdcFifo.usWrite >= ADC_FIFO_SIZE)
		{
			g_tAdcFifo.usWrite = 0;
		}
		if (g_tAdcFifo.usCount < ADC_FIFO_SIZE)
		{
			g_tAdcFifo.usCount++;
		}
		else
		{
			g_tAdcFifo.ucFull = 1;		/* FIFO 满，主程序来不及处理数据 */
		}

}

