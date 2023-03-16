/*
 * FFTInc.h
 *
 *  Created on: 2022年7月25日
 *      Author: nongj
 */

#ifndef ALGOROTHM_FFTINC_H_
#define ALGOROTHM_FFTINC_H_
#include "base.h"
#include "math.h"
#include "arm_math.h"

#define MAX_FFT_N 8192

/*复数结构体*/
struct compx
{
	float32_t real,imag;
};

/*设置非8192或16384个fft点时初始化波表来进行fft运算，
 *
 * 使用8192或16384个点时不需初始化波表
 *
 * */
void InitTableFFT(uint32_t n);

/*对输入的复数组进行快速傅里叶变换（FFT）*/
void cfft(struct compx *_ptr , uint32_t FFT_N);

/*FFT求相位*/
void PowerPhaseRadians_f32(uint16_t _usFFTPoints, float32_t _uiCmpValue);
#endif /* ALGOROTHM_FFTINC_H_ */
