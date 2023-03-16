/*
 * fir.h
 *
 *  Created on: 2022年7月25日
 *      Author: nongj
 */

#ifndef ALGOROTHM_FIR_H_
#define ALGOROTHM_FIR_H_
#include "base.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/*更新记录
 *FIR 高通滤波器
*/

#define 	LENGTH_FIR		1024	//采样点数
#define	BLOCK_SIZE			1		//调用arm_fir_f32处理的采样点数
#define	NUM_TAPS			29		//滤波器系数个数

uint32_t	blockSize_fir	=	BLOCK_SIZE;
uint32_t	numBlocks_fir	=	LENGTH_FIR/BLOCK_SIZE;	//需要调用arm_fir_f32的次数

static	float32_t	firStateF32[BLOCK_SIZE + NUM_TAPS -1];	//状态缓冲


/*FIR高通滤波器系数*/
extern const float32_t firFilterHP[NUM_TAPS];
/*FIR高通滤波转换函数*/
void HighPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR低通滤波器系数*/
extern const float32_t firFilterLP[NUM_TAPS];
/*FIR低通滤波转换函数*/
void LowPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR带通滤波器系数*/
extern const float32_t firFilterBP[NUM_TAPS];
/*FIR带通滤波转换函数*/
void BandPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR带通滤波器系数*/
extern const float32_t firFilterBS[NUM_TAPS];
/*FIR带通滤波转换函数*/
void BandStopFIR(float32_t *inPut,float32_t *outPut);


#endif /* ALGOROTHM_FIR_H_ */
