/*
 * iir.h
 *
 *  Created on: 2022��7��25��
 *      Author: nongj
 */

#ifndef ALGOROTHM_IIR_H_
#define ALGOROTHM_IIR_H_
#include "base.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#define numStages 		2		//2��IIR�˲�
#define LENGTH_SAMPLES	400		//��������
#define BLOCK_SIZE		1		//����arm_biquad_cascade_df1_f32����Ĳ�������

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = LENGTH_SAMPLES/BLOCK_SIZE;    /* ��Ҫ����arm_biquad_cascade_df1_f32�Ĵ��� */

static float32_t IIRState[4*numStages];		/* ״̬���� */

/* ������˹��ͨ�˲���ϵ�� 80Hz*/
extern const float32_t IIRCoeffs32LP[5*numStages];
/*IIR��ͨ�˲���*/
void LowPassIIR(float32_t *inPut,float32_t *outPut);


/* ������˹��ͨ�˲���ϵ�� 140Hz*/
extern const float32_t IIRCoeffs32HP[5*numStages];
/*IIR��ͨ�˲���*/
void HighPassIIR(float32_t *inPut,float32_t *outPut);


/* ������˹��ͨ�˲���ϵ��140Hz 400Hz*/
extern const float32_t IIRCoeffs32BP[5*numStages];
/*IIR��ͨ�˲���*/
void BandPassIIR(float32_t *inPut,float32_t *outPut);


/* ������˹�����˲���ϵ��100Hz 325Hz*/
extern const float32_t IIRCoeffs32BS[5*numStages];
/*IIR�����˲���*/
void BandStopIIR(float32_t *inPut,float32_t *outPut);


#endif /* ALGOROTHM_IIR_H_ */
