/*
 * FFTInc.h
 *
 *  Created on: 2022��7��25��
 *      Author: nongj
 */

#ifndef ALGOROTHM_FFTINC_H_
#define ALGOROTHM_FFTINC_H_
#include "base.h"
#include "math.h"
#include "arm_math.h"

#define MAX_FFT_N 8192

/*�����ṹ��*/
struct compx
{
	float32_t real,imag;
};

/*���÷�8192��16384��fft��ʱ��ʼ������������fft���㣬
 *
 * ʹ��8192��16384����ʱ�����ʼ������
 *
 * */
void InitTableFFT(uint32_t n);

/*������ĸ�������п��ٸ���Ҷ�任��FFT��*/
void cfft(struct compx *_ptr , uint32_t FFT_N);

/*FFT����λ*/
void PowerPhaseRadians_f32(uint16_t _usFFTPoints, float32_t _uiCmpValue);
#endif /* ALGOROTHM_FFTINC_H_ */
