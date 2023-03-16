/*
 * max262.c
 *
 *  Created on: Mar 15, 2023
 *      Author: NJY_r
 */
#include "max262.h"

/***************************************************************
MAX262 IO口初始化
***************************************************************/
void MAX262_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 ;  //初始化具体IO口
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/***************************************************************
*写两比特的数据到某地址
*入口参数：A3到A0的地址add,D1和D0的2bit数据dat2bit 出口参数：无
*说明：D1和D0取dat2bit的最低两位。仅适用于PDL口的八位写操作。
***************************************************************/
void write(u8 add,u8 dat2bit)
{
	u16 data;
	u8 i;
	data = GPIO_ReadOutputData(GPIOD);
	data = (data&0xfff0)|(add&0x000f);           //取4位地址
	data = (data&0xffcf)|((dat2bit<<4)&0x0030); 	//取2位数据
	GPIO_Write(GPIOD,data);
	resetWr;
	for(i=0;i<255;i++);     //至少250ns
	setWr;
	for(i=0;i<100;i++);     //至少10ns
}

//设置A通道F值
void setAf(u8 datF)    //6位中心频率f0控制位F0-F5 对应十进制0-63
{
	write(1,datF);
	datF = datF>>2;
	write(2,datF);
	datF = datF>>2;
	write(3,datF);
}
//设置A通道Q值
void setAQ(u8 datQ)    //7位品质因数Q控制位Q0-Q6 对应十进制0-127
{
	write(4,datQ);
	datQ = datQ>>2;
	write(5,datQ);
	datQ = datQ>>2;
	write(6,datQ);
	datQ = (datQ>>2)&1;  //对地址7只取一位
	write(7,datQ);
}
//设置B通道F值
void setBf(u8 datF)
{
    write(9,datF);
	datF = datF>>2;
	write(10,datF);
	datF = datF>>2;
	write(11,datF);
}
//设置B通道Q值
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
//根据截止频率fc、品质因数Q和通过模式passMode计算滤波器的中心频率f0
//仅适用于低通和高通
//Q = 1 时
//低通：fc ： 1000-10000Hz 步进500Hz  f0:769.23-7692.31Hz
//高通：fc ： 1000-10000Hz 步进500Hz  f0:1300-13000Hz
float lh_Getf0(float fc,float Q,u8 passMode)
{
    float temp;
    temp = 1-0.5/pow(Q,2);   //pow(Q, 2) 计算Q的2次幂
	temp = sqrt(temp+sqrt(pow(temp,2)+1));
	if(passMode==lowPass)  //低通中心频率计算公式
	{
        return  fc/temp;
	}
	else  //高通中心频率计算公式
	{
        return  fc*temp;
	}
}
//根据时钟频率fclk、中心频率f0、和工作模式workMode求频率设定值FN
//uchar getFn(float fclk,float f0,u8 workMode)
//{
//       if (workMode==modeTwo)
//       return (u8)(fclk*2.8284271247/f0/PI-26);
//       else
//       return (u8)(fclk*2/f0/PI-26);
//}


//根据fc、Q、passMode、workMode和通道channel设置MAX262的低通和高通的工作数据
//返回时钟频率fclk
//低通：60384.6-603845.6Hz
//高通：102050-1020500Hz
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
		workMode=modeThree;  //高通只有工作模式3
	}

//	Fn = FnFin_config(f0, workMode);

   if(channel==channelA)
	{
    write(0,workMode);
		setAf(0);  //要想获得大范围可调，开关电容的采样频率参数取最大值.
		setAQ(Qn);
	}
	else
  {
    write(8,workMode);
		setBf(0);     //要想获得大范围可调，开关电容的采样频率参数取最大值.
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
//根据fh、fl、workMode和通道channel设置MAX262的带通工作数据
//返回时钟频率fclk

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
	setAf(0);  //要想获得大范围可调，开关电容的采样频率参数取最大值.
	setAQ(Qn);
	}
	else
	{
    write(8,workMode);
		setBf(0);     //要想获得大范围可调，开关电容的采样频率参数取最大值.
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
//根据f0、Q、workMode和通道channel设置MAX262的陷波工作数据
//返回时钟频率fclk
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
		setAf(63);  //要想获得大范围可调，开关电容的采样频率参数取最大值.
		setAQ(Qn);
	}
	else
	{
		write(8,workMode);
		setBf(63);     //要想获得大范围可调，开关电容的采样频率参数取最大值.
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
//根据f0、Q和通道channel设置MAX262的全通工作数据
//返回时钟频率fclk
float ap_WorkFclk(float f0,float Q,u8 channel)
{
	u8 Qn;
    Qn = (u8)(128-64/Q);

	if(channel==channelA)
   {
        write(0,modeFour);
		setAf(63);  //要想获得大范围可调，开关电容的采样频率参数取最大值.
		setAQ(Qn);
	}
	else
    {
        write(8,modeFour);
		setBf(63);     //要想获得大范围可调，开关电容的采样频率参数取最大值.
		setBQ(Qn);
	}
	return  44.5*PI*f0;
}

//u8 FnFin_config(float f0,u8 workMode)//根据f0调整fn
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
最小：26*1.11072 = 28.87872
      26*PI/2    = 40.8407



*/

