/*
 * iir.c
 *
 *  Created on: 2022��7��25��
 *      Author: nongj
 */
#include "iir.h"

/* ������˹��ͨ�˲���ϵ�� 80Hz*/
const float32_t IIRCoeffs32LP[5*numStages] = {
	1.0f,  2.0f,  1.0f,  1.479798894397216679763573665695730596781f,  -0.688676953053861784503908438637154176831f,
	1.0f,  2.0f,  1.0f,  1.212812092620218384908525877108331769705f,  -0.384004162286553540894828984164632856846f
};

/*IIR��ͨ�˲���*/
void LowPassIIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_biquad_casd_df1_inst_f32 S;
	float32_t ScaleValue;
	float32_t  *inputF32, *outputF32;

	/* ��ʼ�������������ָ�� */
	inputF32 = &inPut[0];
	outputF32 = &outPut[0];

	/*��ʼ��*/
	arm_biquad_cascade_df1_init_f32(&S,
									 numStages,
									 (float32_t *)&IIRCoeffs32LP[0],
									 (float32_t *)&IIRState[0]);

	/* ʵ��IIR�˲�������ÿ�δ���1���� */
	for(i=0 ; i<numBlocks ; i++)
	{
		arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}

	/*����ϵ�� */
		ScaleValue = 0.052219514664161220673932461977528873831f * 0.042798017416583809813257488485760404728f ;
}


/* ������˹��ͨ�˲���ϵ�� 140Hz */
const float32_t IIRCoeffs32HP[5*numStages] = {
	1.0f,  -2.0f,  1.0f,  0.98454301474115180070612041163258254528f,   -0.544565360850816415627662081533344462514f,
	1.0f,  -2.0f,  1.0f,  0.744714477864321211519893495278665795922f,  -0.168318873843973093595849377379636280239f
};

/*IIR��ͨ�˲���*/
void HighPassIIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_biquad_casd_df1_inst_f32 S;
	float32_t ScaleValue;
	float32_t  *inputF32, *outputF32;

	/* ��ʼ�������������ָ�� */
	inputF32 = &inPut[0];
	outputF32 = &outPut[0];

	/*��ʼ��*/
	arm_biquad_cascade_df1_init_f32(&S,
									 numStages,
									 (float32_t *)&IIRCoeffs32HP[0],
									 (float32_t *)&IIRState[0]);

	/* ʵ��IIR�˲�������ÿ�δ���1���� */
	for(i=0 ; i<numBlocks ; i++)
	{
		arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}

	/*����ϵ�� */
	ScaleValue = 0.632277093897992026327870007662568241358f * 0.478258337927073562401147910350118763745f;
}

/* ������˹��ͨ�˲���ϵ��140Hz 400Hz*/
const float32_t IIRCoeffs32BP[5*numStages] = {
	1.0f,  0.0f,  -1.0f,     -1.127651872054164616798743736580945551395f,  -0.470013145087532668853214090631809085608f,
	1.0f,  0.0f,  -1.0f,     0.774953058046049081397654845204669982195f,  -0.367077500556684199750634434167295694351f
};

/*IIR��ͨ�˲���*/
void BandPassIIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_biquad_casd_df1_inst_f32 S;
	float32_t ScaleValue;
	float32_t  *inputF32, *outputF32;

	/* ��ʼ�������������ָ�� */
	inputF32 = &inPut[0];
	outputF32 = &outPut[0];

	/*��ʼ��*/
	arm_biquad_cascade_df1_init_f32(&S,
									 numStages,
									 (float32_t *)&IIRCoeffs32BP[0],
									 (float32_t *)&IIRState[0]);

	/* ʵ��IIR�˲�������ÿ�δ���1���� */
	for(i=0 ; i<numBlocks ; i++)
	{
		arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}

	/*����ϵ�� */
	ScaleValue = 0.558156585760773649163013487850548699498f * 0.558156585760773649163013487850548699498f;
}

/* ������˹�����˲���ϵ��100Hz 325Hz*/
const float32_t IIRCoeffs32BS[5*numStages] = {
	1.0f,  -0.614001926383350049576392848393879830837f,  1.0f,  1.145142787949775309286337687808554619551f,   -0.502980071467214684410862446384271606803f,
	1.0f,  -0.614001926383350049576392848393879830837f,  1.0f,  -0.474587046588418992598690238082781434059f, -0.35305199748708809837083322236139792949f
};

/*IIR�����˲���*/
void BandStopIIR(float32_t *inPut,float32_t *outPut)
{
	uint32_t i;
	arm_biquad_casd_df1_inst_f32 S;
	float32_t ScaleValue;
	float32_t  *inputF32, *outputF32;

	/* ��ʼ�������������ָ�� */
	inputF32 = &inPut[0];
	outputF32 = &outPut[0];

	/*��ʼ��*/
	arm_biquad_cascade_df1_init_f32(&S,
									 numStages,
									 (float32_t *)&IIRCoeffs32BS[0],
									 (float32_t *)&IIRState[0]);

	/* ʵ��IIR�˲�������ÿ�δ���1���� */
	for(i=0 ; i<numBlocks ; i++)
	{
		arm_biquad_cascade_df1_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}

	/*����ϵ�� */
		ScaleValue = 0.558156585760773649163013487850548699498f * 0.558156585760773649163013487850548699498f;
}
