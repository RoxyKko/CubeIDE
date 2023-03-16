/*
 * FFTInc.c
 *
 *  Created on: 2022��7��25��
 *      Author: nongj
 */
#include "FFTInc.h"

struct compx s[MAX_FFT_N];

/*
*********************************************************************************************************
*	�� �� ��: PowerPhaseRadians_f32
*	����˵��: ����λ
*	��    �Σ�_usFFTPoints  ����������ÿ������������float32_t��ֵ
*             _uiCmpValue  �Ƚ�ֵ����Ҫ�����λ����ֵ
*	�� �� ֵ: ��
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
		lX= s[i].real; 	  /* ʵ�� */
		lY= s[i].imag;    /* �鲿 */

 		phase = atan2f(lY, lX);    		  				 /* atan2���Ľ����Χ��(-pi, pi], ������ */
		arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* ��ģ */

		if(_uiCmpValue > mag)
		{
			s[i].real = 0;
		}
		else
		{
			s[i].real= phase* 180.0f/3.1415926f;   /* �����Ľ���ɻ���ת��Ϊ�Ƕ� */
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: cfft_f32_mag
*	����˵��: �����Ƶ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void cfft_f32_mag(void)
{
	uint32_t i;

	/* ����һ��sin��cosϵ�� */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif

	for(i=0; i<MAX_FFT_N; i++)
	{
		/* ��������ֱ��������500Hz���Ҳ���ɣ����β�����MAX_FFT_N����ʼ��λ60�� */
		s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);
		s[i].imag = 0;
	}

	/* MAX_FFT_N�����FFT */
	cfft(s, MAX_FFT_N);

	/* �����Ƶ */
	for(i=0; i<MAX_FFT_N; i++)
	{
		arm_sqrt_f32((float32_t)(s[i].real *s[i].real+ s[i].imag*s[i].imag ), &s[i].real);
	}

	/* ���ڴ�ӡ���ķ�Ƶ */
	for(i=0; i<MAX_FFT_N; i++)
	{
		printf("%f\r\n", s[i].real);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: cfft_f32_phase
*	����˵��: ������Ƶ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void cfft_f32_phase(void)
{
	uint32_t i;


	/* ����һ��sin��cosϵ�� */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif

	for(i=0; i<MAX_FFT_N; i++)
	{
		/* ��������ֱ��������500Hz���Ҳ���ɣ����β�����MAX_FFT_N����ʼ��λ60�� */
		s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);
		s[i].imag = 0;
	}

	/* MAX_FFT_N�����FFT */
	cfft(s, MAX_FFT_N);

	/* ����Ƶ */
	PowerPhaseRadians_f32(MAX_FFT_N, 0.5f);

	/* ���ڴ�ӡ�����Ƶ */
	for(i=0; i<MAX_FFT_N; i++)
	{
		printf("%f\r\n", s[i].real);
	}

}

