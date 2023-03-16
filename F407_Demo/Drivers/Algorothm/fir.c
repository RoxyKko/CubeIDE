/*
 * fir.c
 *
 *  Created on: 2022年7月25日
 *      Author: nongj
 */
#include "fir.h"




/*FIR高通滤波器系数*/
const float32_t firFilterHP[NUM_TAPS] = {
		0.0018157335f, 0.001582013792f,
		-6.107207639e-18f, -0.003683975432f, -0.008045346476f, -0.008498443291f,
		-1.277260999e-17f, 0.01733288541f, 0.03401865438f, 0.0332348831f,
		-4.021742543e-17f, -0.06737889349f, -0.1516391635f, -0.2220942229f,
		0.7486887574f, -0.2220942229f, -0.1516391635f, -0.06737889349f,
		-4.021742543e-17f, 0.0332348831f, 0.03401865438f, 0.01733288541f,
		-1.277260999e-17f, -0.008498443291f, -0.008045346476f, -0.003683975432f,
		-6.107207639e-18f, 0.001582013792f, 0.0018157335f
};

/*FIR高通滤波*/
void HighPassFIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t  *inputF32, *outputF32;

		/* 初始化输入输出缓存指针 */
		inputF32 = &inPut[0];
		outputF32 = &outPut[0];

	/*初始化结构体S*/
	arm_fir_init_f32(&S,
					  NUM_TAPS,
					  (float32_t *)&firFilterHP[0],
					  &firStateF32[0],
					  blockSize_fir);

	/*实现FIR滤波，每次实现一个点*/
	for(i=0 ; i < numBlocks_fir ; i++)
	{
		arm_fir_f32(&S,inputF32 + (i * blockSize_fir),
				outputF32 + (i * blockSize_fir),blockSize_fir);
	}
}

/*FIR低通滤波器系数*/
const float32_t firFileterLP[NUM_TAPS] = {
		-0.001822523074f, -0.001587929321f,
		1.226008847e-18f, 0.003697750857f, 0.008075430058f, 0.008530221879f,
		-4.273456581e-18f, -0.01739769801f, -0.03414586186f, -0.03335915506f,
		8.073562366e-18f, 0.06763084233f, 0.1522061825f, 0.2229246944f,
		0.2504960895f, 0.2229246944f, 0.1522061825f, 0.06763084233f,
		8.073562366e-18f, -0.03335915506f, -0.03414586186f, -0.01739769801f,
		-4.273456581e-18f, 0.008530221879f, 0.008075430058f, 0.003697750857f,
		1.226008847e-18f, -0.001587929321f, -0.001822523074f

};

/*FIR低通滤波*/
void LowPassFIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t  *inputF32, *outputF32;

		/* 初始化输入输出缓存指针 */
		inputF32 = &inPut[0];
		outputF32 = &outPut[0];

	/*初始化结构体S*/
	arm_fir_init_f32(&S,
					NUM_TAPS,
					(float32_t*) &firFilterLP[0],
					&firStateF32[0],
					blockSize_fir);

	/*实现FIR滤波，每次实现一个点*/
	for(i=0 ; i < numBlocks_fir ; i++)
	{
		arm_fir_f32(&S,
				inputF32 + (i * blockSize_fir),
				outputF32 + (i * blockSize_fir),
					 blockSize_fir);
	}
}

/*FIR带通滤波器系数*/
const float32_t firFileterBP[NUM_TAPS] = {
		0.003531039227f, 0.0002660876198f,
		-0.001947779674f, 0.001266813371f, -0.008019094355f, -0.01986379735f,
		0.01018396299f, 0.03163734451f, 0.00165955862f, 0.03312643617f,
		0.0622616075f, -0.1229852438f, -0.2399847955f, 0.07637182623f,
		0.3482480049f, 0.07637182623f, -0.2399847955f, -0.1229852438f,
		0.0622616075f, 0.03312643617f, 0.00165955862f, 0.03163734451f,
		0.01018396299f, -0.01986379735f, -0.008019094355f, 0.001266813371f,
		-0.001947779674f, 0.0002660876198f, 0.003531039227f
};

/*FIR带通滤波*/
void BandPassFIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t  *inputF32, *outputF32;

		/* 初始化输入输出缓存指针 */
		inputF32 = &inPut[0];
		outputF32 = &outPut[0];

	/*初始化结构体S*/
	arm_fir_init_f32(&S,
					  NUM_TAPS,
					  (float32_t*) &firFilterBP[0],
					  &firStateF32[0],
					  blockSize_fir);

	/*实现FIR滤波，每次实现一个点*/
	for (i = 0; i < numBlocks_fir; i++) {
		arm_fir_f32(&S,
				inputF32 + (i * blockSize_fir),
				outputF32 + (i * blockSize_fir),
					blockSize_fir);
	}
}

/*FIR带通滤波器系数*/
const float32_t firFileterBS[NUM_TAPS] = {
		0.003531039227f, 0.0002660876198f,
		-0.001947779674f, 0.001266813371f, -0.008019094355f, -0.01986379735f,
		0.01018396299f, 0.03163734451f, 0.00165955862f, 0.03312643617f,
		0.0622616075f, -0.1229852438f, -0.2399847955f, 0.07637182623f,
		0.3482480049f, 0.07637182623f, -0.2399847955f, -0.1229852438f,
		0.0622616075f, 0.03312643617f, 0.00165955862f, 0.03163734451f,
		0.01018396299f, -0.01986379735f, -0.008019094355f, 0.001266813371f,
		-0.001947779674f, 0.0002660876198f, 0.003531039227f
};

/*FIR带通滤波*/
void BandStopFIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t  *inputF32, *outputF32;

		/* 初始化输入输出缓存指针 */
		inputF32 = &inPut[0];
		outputF32 = &outPut[0];

	/*初始化结构体S*/
	arm_fir_init_f32(&S,
					  NUM_TAPS,
					  (float32_t*) &firFilterBS[0],
					  &firStateF32[0],
					  blockSize_fir);

	/*实现FIR滤波，每次实现一个点*/
	for (i = 0; i < numBlocks_fir; i++) {
		arm_fir_f32(&S,
				inputF32 + (i * blockSize_fir),
				outputF32 + (i * blockSize_fir),
					blockSize_fir);
	}
}

