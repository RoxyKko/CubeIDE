/*
 * iir.c
 *
 *  Created on: Mar 16, 2023
 *      Author: NJY_r
 */
#include "iir.h"

float32_t testInput[TEST_LENGTH_SAMPLES];
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* ������ */

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES / BLOCK_SIZE; /* ��Ҫ����arm_biquad_cascade_df1_f32�Ĵ��� */

// float32_t testInput[TEST_LENGTH_SAMPLES]; /* ������ */
// float32_t testOutput[TEST_LENGTH_SAMPLES];               /* �˲������� */
static float32_t IIRStateF32[4 * numStages];                    /* ״̬���� */

/* ʵ��ʹ�� ARM �ٷ��� IIR �������õ�ʱ��Ҫ�� a1 �� a2 ȡ�� */
/* ��Ӧ�ð������µ������������
{b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...} */

/* ������˹��ͨ�˲���ϵ�� ����Ƶ��40k��ֹƵ��1kHz*/
const float32_t IIRCoeffs32LP[5 * numStages] = {
    1.0f, 2.0f, 1.0f, 1.863800492075235348821138359198812395334f, -0.887032999652694775782890701520955190063f,
    1.0f, 2.0f, 1.0f, 1.725933395036940654065915623505134135485f, -0.747447371907790980571917316410690546036f};

/*
*********************************************************************************************************
* �� �� ��: arm_iir_f32_lp
* ����˵��: ���ú��� arm_iir_f32_lp ʵ�ֵ�ͨ�˲���
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void arm_iir_f32_lp_1k(void)
{
    uint32_t i;
    arm_biquad_casd_df1_inst_f32 S;
    float32_t ScaleValue; //����ϵ��
    float32_t *inputF32, *outputF32;
    /* ��ʼ�������������ָ�� */
    inputF32 = &testInput[0];
    outputF32 = &testOutput[0];
    /* ��ʼ�� */
    arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32LP[0],
                                    (float32_t *)&IIRStateF32[0]);
    /* ʵ�� IIR �˲�������ÿ�δ��� 1 ���� */
    for (i = 0; i < numBlocks; i++)
    {
        arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize),
                                   blockSize);
    }

    /*����ϵ�� */
    ScaleValue = 0.005808126894364891434907605116677586921f * 0.005378494217712603310543872936477782787f;
    for (i = 0; i < TEST_LENGTH_SAMPLES; i++)
    {
        testOutput[i] *= ScaleValue;
    }
}
