/*
 * iir.h
 *
 *  Created on: Mar 16, 2023
 *      Author: NJY_r
 */

#ifndef IIR_IIR_H_
#define IIR_IIR_H_

#include "main.h"
#include "base.h"
#include "arm_math.h"
#define numStages 2             /* 2��IIR�˲��ĸ���,�ܽ���2*2=4 */
#define TEST_LENGTH_SAMPLES 1 /* �������� */
#define BLOCK_SIZE 1            /* ����һ��arm_biquad_cascade_df1_f32����Ĳ�������� */
void arm_iir_f32_lp_1k(void);


#endif /* IIR_IIR_H_ */
