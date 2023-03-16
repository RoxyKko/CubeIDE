/*
 * iir.h
 *
 *  Created on: 2022年7月25日
 *      Author: nongj
 */

#ifndef ALGOROTHM_IIR_H_
#define ALGOROTHM_IIR_H_
#include "base.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#define numStages 		2		//2阶IIR滤波
#define LENGTH_SAMPLES	400		//采样点数
#define BLOCK_SIZE		1		//调用arm_biquad_cascade_df1_f32处理的采样点数

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = LENGTH_SAMPLES/BLOCK_SIZE;    /* 需要调用arm_biquad_cascade_df1_f32的次数 */

static float32_t IIRState[4*numStages];		/* 状态缓存 */

/* 巴特沃斯低通滤波器系数 80Hz*/
extern const float32_t IIRCoeffs32LP[5*numStages];
/*IIR低通滤波器*/
void LowPassIIR(float32_t *inPut,float32_t *outPut);


/* 巴特沃斯高通滤波器系数 140Hz*/
extern const float32_t IIRCoeffs32HP[5*numStages];
/*IIR高通滤波器*/
void HighPassIIR(float32_t *inPut,float32_t *outPut);


/* 巴特沃斯带通滤波器系数140Hz 400Hz*/
extern const float32_t IIRCoeffs32BP[5*numStages];
/*IIR带通滤波器*/
void BandPassIIR(float32_t *inPut,float32_t *outPut);


/* 巴特沃斯带阻滤波器系数100Hz 325Hz*/
extern const float32_t IIRCoeffs32BS[5*numStages];
/*IIR带阻滤波器*/
void BandStopIIR(float32_t *inPut,float32_t *outPut);


#endif /* ALGOROTHM_IIR_H_ */
