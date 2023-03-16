/*
 * fir.h
 *
 *  Created on: 2022��7��25��
 *      Author: nongj
 */

#ifndef ALGOROTHM_FIR_H_
#define ALGOROTHM_FIR_H_
#include "base.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/*���¼�¼
 *FIR ��ͨ�˲���
*/

#define 	LENGTH_FIR		1024	//��������
#define	BLOCK_SIZE			1		//����arm_fir_f32����Ĳ�������
#define	NUM_TAPS			29		//�˲���ϵ������

uint32_t	blockSize_fir	=	BLOCK_SIZE;
uint32_t	numBlocks_fir	=	LENGTH_FIR/BLOCK_SIZE;	//��Ҫ����arm_fir_f32�Ĵ���

static	float32_t	firStateF32[BLOCK_SIZE + NUM_TAPS -1];	//״̬����


/*FIR��ͨ�˲���ϵ��*/
extern const float32_t firFilterHP[NUM_TAPS];
/*FIR��ͨ�˲�ת������*/
void HighPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR��ͨ�˲���ϵ��*/
extern const float32_t firFilterLP[NUM_TAPS];
/*FIR��ͨ�˲�ת������*/
void LowPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR��ͨ�˲���ϵ��*/
extern const float32_t firFilterBP[NUM_TAPS];
/*FIR��ͨ�˲�ת������*/
void BandPassFIR(float32_t *inPut,float32_t *outPut);

/*FIR��ͨ�˲���ϵ��*/
extern const float32_t firFilterBS[NUM_TAPS];
/*FIR��ͨ�˲�ת������*/
void BandStopFIR(float32_t *inPut,float32_t *outPut);


#endif /* ALGOROTHM_FIR_H_ */
