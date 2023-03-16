/*
 * max262.c
 *
 *  Created on: Mar 15, 2023
 *      Author: NJY_r
 */
#include "max262.h"

/***************************************************************
MAX262 IO�ڳ�ʼ��
***************************************************************/
void MAX262_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 ;  //��ʼ������IO��
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/***************************************************************
*д�����ص����ݵ�ĳ��ַ
*��ڲ�����A3��A0�ĵ�ַadd,D1��D0��2bit����dat2bit ���ڲ�������
*˵����D1��D0ȡdat2bit�������λ����������PDL�ڵİ�λд������
***************************************************************/
void write(u8 add,u8 dat2bit)
{
	u16 data;
	u8 i;
	data = GPIO_ReadOutputData(GPIOD);
	data = (data&0xfff0)|(add&0x000f);           //ȡ4λ��ַ
	data = (data&0xffcf)|((dat2bit<<4)&0x0030); 	//ȡ2λ����
	GPIO_Write(GPIOD,data);
	resetWr;
	for(i=0;i<255;i++);     //����250ns
	setWr;
	for(i=0;i<100;i++);     //����10ns
}

//����Aͨ��Fֵ
void setAf(u8 datF)    //6λ����Ƶ��f0����λF0-F5 ��Ӧʮ����0-63
{
	write(1,datF);
	datF = datF>>2;
	write(2,datF);
	datF = datF>>2;
	write(3,datF);
}
//����Aͨ��Qֵ
void setAQ(u8 datQ)    //7λƷ������Q����λQ0-Q6 ��Ӧʮ����0-127
{
	write(4,datQ);
	datQ = datQ>>2;
	write(5,datQ);
	datQ = datQ>>2;
	write(6,datQ);
	datQ = (datQ>>2)&1;  //�Ե�ַ7ֻȡһλ
	write(7,datQ);
}
//����Bͨ��Fֵ
void setBf(u8 datF)
{
    write(9,datF);
	datF = datF>>2;
	write(10,datF);
	datF = datF>>2;
	write(11,datF);
}
//����Bͨ��Qֵ
void setBQ(u8 datQ)
{
    write(12,datQ);
	datQ = datQ>>2;
	write(13,datQ);
	datQ = datQ>>2;
	write(14,datQ);
    datQ = (datQ>>2)&1;
	write(15,datQ);
}
//***************************************************************
//���ݽ�ֹƵ��fc��Ʒ������Q��ͨ��ģʽpassMode�����˲���������Ƶ��f0
//�������ڵ�ͨ�͸�ͨ
//Q = 1 ʱ
//��ͨ��fc �� 1000-10000Hz ����500Hz  f0:769.23-7692.31Hz
//��ͨ��fc �� 1000-10000Hz ����500Hz  f0:1300-13000Hz
float lh_Getf0(float fc,float Q,u8 passMode)
{
    float temp;
    temp = 1-0.5/pow(Q,2);   //pow(Q, 2) ����Q��2����
	temp = sqrt(temp+sqrt(pow(temp,2)+1));
	if(passMode==lowPass)  //��ͨ����Ƶ�ʼ��㹫ʽ
	{
        return  fc/temp;
	}
	else  //��ͨ����Ƶ�ʼ��㹫ʽ
	{
        return  fc*temp;
	}
}
//����ʱ��Ƶ��fclk������Ƶ��f0���͹���ģʽworkMode��Ƶ���趨ֵFN
//uchar getFn(float fclk,float f0,u8 workMode)
//{
//       if (workMode==modeTwo)
//       return (u8)(fclk*2.8284271247/f0/PI-26);
//       else
//       return (u8)(fclk*2/f0/PI-26);
//}


//����fc��Q��passMode��workMode��ͨ��channel����MAX262�ĵ�ͨ�͸�ͨ�Ĺ�������
//����ʱ��Ƶ��fclk
//��ͨ��60384.6-603845.6Hz
//��ͨ��102050-1020500Hz
float lhp_WorkFclk(float fc,float Q,u8 passMode,u8 workMode,u8 channel)
{
	float f0;
	u8 Qn, Fn;
  f0 = lh_Getf0(fc, Q, passMode);
  if (workMode==modeTwo)
	{
		Qn = (u8)(128-90.51/Q);
	}
	else
	{
		Qn = (u8)(128-64/Q);
	}
	if(passMode==highPass)
	{
		workMode=modeThree;  //��ֻͨ�й���ģʽ3
	}

//	Fn = FnFin_config(f0, workMode);

   if(channel==channelA)
	{
    write(0,workMode);
		setAf(0);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setAQ(Qn);
	}
	else
  {
    write(8,workMode);
		setBf(0);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setBQ(Qn);
	}
  if(workMode==modeTwo)
	{
		return  28.87872*f0;//(Fn+26)*1.11072*f0;//31.46625*PI*f0;//31.46625-63   16.26361-20
	}
	else
	{
    return  40.84071*f0;//(26+Fn)*PI/2*f0;//44.5*PI*f0;//44.5-63  23-20
	}
}
//����fh��fl��workMode��ͨ��channel����MAX262�Ĵ�ͨ��������
//����ʱ��Ƶ��fclk

float bp_WorkFclk(float fh,float fl,u8 workMode,u8 channel)
{
  float f0,Q;
	u8 Qn, Fn;
  f0 = sqrt(fh*fl);
	Q = f0/(fh-fl);

	if (workMode==modeTwo)
	{
          Qn = (u8)(128-90.51/Q);
	}
	else
	{
          Qn = (u8)(128-64/Q);
	}

//	Fn = FnFin_config(f0, workMode);

	if(channel==channelA)
	{
	write(0,workMode);
	setAf(0);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
	setAQ(Qn);
	}
	else
	{
    write(8,workMode);
		setBf(0);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setBQ(Qn);
	}
	if(workMode==modeTwo)
	{
		return 28.87872*f0;//(Fn+26)*1.11072*f0;//31.46625*PI*f0;
	}
	else
	{
    return 40.84071*f0;//(26+Fn)*PI/2*f0;//44.5*PI*f0;
	}
}
//����f0��Q��workMode��ͨ��channel����MAX262���ݲ���������
//����ʱ��Ƶ��fclk
float bs_WorkFclk(float f0,float Q,u8 workMode,u8 channel)
{
	u8 Qn;
    if (workMode==modeTwo)
	{
		Qn = (u8)(128-90.51/Q);
	}
	else
	{
    Qn = (u8)(128-64/Q);
	}
	if(channel==channelA)
	{
        write(0,workMode);
		setAf(63);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setAQ(Qn);
	}
	else
	{
		write(8,workMode);
		setBf(63);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setBQ(Qn);
	}
	if(workMode==modeTwo)
	{
		return  31.46625*PI*f0;
	}
	else
	{
       return  44.5*PI*f0;
	}
}
//����f0��Q��ͨ��channel����MAX262��ȫͨ��������
//����ʱ��Ƶ��fclk
float ap_WorkFclk(float f0,float Q,u8 channel)
{
	u8 Qn;
    Qn = (u8)(128-64/Q);

	if(channel==channelA)
   {
        write(0,modeFour);
		setAf(63);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setAQ(Qn);
	}
	else
    {
        write(8,modeFour);
		setBf(63);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
		setBQ(Qn);
	}
	return  44.5*PI*f0;
}

//u8 FnFin_config(float f0,u8 workMode)//����f0����fn
//{
//	u8 i;
//
//	for (i = 0; i < 64; i++)
//	{
//		if(workMode==modeTwo)
//		{
//			//if( (i+26)*1.11072*f0 )
//		}
//		else
//		{
//			//if( (26+i)*PI/2*f0 )
//		}
//	}
//
//}
/*
��С��26*1.11072 = 28.87872
      26*PI/2    = 40.8407



*/

