/*
 * iir.c
 *
 *  Created on: Mar 16, 2023
 *      Author: NJY_r
 */
#include "iir.h"

float32_t testInput[TEST_LENGTH_SAMPLES];
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 采样点 */

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES / BLOCK_SIZE; /* 需要调用arm_biquad_cascade_df1_f32的次数 */

// float32_t testInput[TEST_LENGTH_SAMPLES]; /* 采样点 */
// float32_t testOutput[TEST_LENGTH_SAMPLES];               /* 滤波后的输出 */
static float32_t IIRStateF32[4 * numStages];                    /* 状态缓存 */

/* 实际使用 ARM 官方的 IIR 函数调用的时候要将 a1 和 a2 取反 */
/* 数应该按照如下的逆序进行排列
{b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...} */

/* 巴特沃斯低通滤波器系数 采样频率40k截止频率1kHz*/
const float32_t IIRCoeffs32LP[5 * numStages] = {
    1.0f, 2.0f, 1.0f, 1.863800492075235348821138359198812395334f, -0.887032999652694775782890701520955190063f,
    1.0f, 2.0f, 1.0f, 1.725933395036940654065915623505134135485f, -0.747447371907790980571917316410690546036f};

/*
*********************************************************************************************************
* 函 数 名: arm_iir_f32_lp
* 功能说明: 调用函数 arm_iir_f32_lp 实现低通滤波器
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
void arm_iir_f32_lp_1k(void)
{
    uint32_t i;
    arm_biquad_casd_df1_inst_f32 S;
    float32_t ScaleValue; //放缩系数
    float32_t *inputF32, *outputF32;
    /* 初始化输入输出缓存指针 */
    inputF32 = &testInput[0];
    outputF32 = &testOutput[0];
    /* 初始化 */
    arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32LP[0],
                                    (float32_t *)&IIRStateF32[0]);
    /* 实现 IIR 滤波，这里每次处理 1 个点 */
    for (i = 0; i < numBlocks; i++)
    {
        arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize),
                                   blockSize);
    }

    /*放缩系数 */
    ScaleValue = 0.005808126894364891434907605116677586921f * 0.005378494217712603310543872936477782787f;
    for (i = 0; i < TEST_LENGTH_SAMPLES; i++)
    {
        testOutput[i] *= ScaleValue;
    }
}
