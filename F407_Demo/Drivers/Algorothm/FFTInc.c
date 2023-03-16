/*
 * FFTInc.c
 *
 *  Created on: 2022年7月25日
 *      Author: nongj
 */
#include "FFTInc.h"

struct compx s[MAX_FFT_N];

/*
*********************************************************************************************************
*	函 数 名: PowerPhaseRadians_f32
*	功能说明: 求相位
*	形    参：_usFFTPoints  复数个数，每个复数是两个float32_t数值
*             _uiCmpValue  比较值，需要求出相位的数值
*	返 回 值: 无
*********************************************************************************************************
*/
void PowerPhaseRadians_f32(uint16_t _usFFTPoints, float32_t _uiCmpValue)
{
	float32_t lX, lY;
	uint32_t i;
	float32_t phase;
	float32_t mag;


	for (i=0; i <_usFFTPoints; i++)
	{
		lX= s[i].real; 	  /* 实部 */
		lY= s[i].imag;    /* 虚部 */

 		phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
		arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */

		if(_uiCmpValue > mag)
		{
			s[i].real = 0;
		}
		else
		{
			s[i].real= phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: cfft_f32_mag
*	功能说明: 计算幅频
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void cfft_f32_mag(void)
{
	uint32_t i;

	/* 计算一批sin，cos系数 */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif

	for(i=0; i<MAX_FFT_N; i++)
	{
		/* 波形是由直流分量，500Hz正弦波组成，波形采样率MAX_FFT_N，初始相位60° */
		s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);
		s[i].imag = 0;
	}

	/* MAX_FFT_N点快速FFT */
	cfft(s, MAX_FFT_N);

	/* 计算幅频 */
	for(i=0; i<MAX_FFT_N; i++)
	{
		arm_sqrt_f32((float32_t)(s[i].real *s[i].real+ s[i].imag*s[i].imag ), &s[i].real);
	}

	/* 串口打印求解的幅频 */
	for(i=0; i<MAX_FFT_N; i++)
	{
		printf("%f\r\n", s[i].real);
	}
}

/*
*********************************************************************************************************
*	函 数 名: cfft_f32_phase
*	功能说明: 计算相频
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void cfft_f32_phase(void)
{
	uint32_t i;


	/* 计算一批sin，cos系数 */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif

	for(i=0; i<MAX_FFT_N; i++)
	{
		/* 波形是由直流分量，500Hz正弦波组成，波形采样率MAX_FFT_N，初始相位60° */
		s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);
		s[i].imag = 0;
	}

	/* MAX_FFT_N点快速FFT */
	cfft(s, MAX_FFT_N);

	/* 求相频 */
	PowerPhaseRadians_f32(MAX_FFT_N, 0.5f);

	/* 串口打印求解相频 */
	for(i=0; i<MAX_FFT_N; i++)
	{
		printf("%f\r\n", s[i].real);
	}

}

