#include "myiic.h"

//累乘四次宏定义
#define Accumulate_4(value) (value * value * value * value)

//IIC句柄声明
IIC_TypeDef IIC;

//IIC初始化
void IIC_Init(void)
{	
    IIC_SDA_H;
    IIC_SCL_H;
}


/*!
 *  \brief  设置IIC的SDA为输入
 */
void IIC_SDA_SET_IN(void)
{
	if(IIC.SDA_Pin < GPIO_PIN_7)
	{
		IIC.SDA_Port->CRL &= ~(0xF * Accumulate_4(IIC.SDA_Pin       ));
		IIC.SDA_Port->CRL |=  (0x8 * Accumulate_4(IIC.SDA_Pin       ));
	}
	else
	{
		IIC.SDA_Port->CRH &= ~(0xF * Accumulate_4(IIC.SDA_Pin / 0x100));
		IIC.SDA_Port->CRH |=  (0x8 * Accumulate_4(IIC.SDA_Pin / 0x100));
	}
}


/*!
 *  \brief  设置IIC的SDA为输出
 */
void IIC_SDA_SET_OUT(void)
{
	if(IIC.SDA_Pin < GPIO_PIN_7)
	{
		IIC.SDA_Port->CRL &= ~(0xF * Accumulate_4(IIC.SDA_Pin       ));
		IIC.SDA_Port->CRL |=  (0x3 * Accumulate_4(IIC.SDA_Pin       ));
	}
	else
	{
		IIC.SDA_Port->CRH &= ~(0xF * Accumulate_4(IIC.SDA_Pin / 0x100));
		IIC.SDA_Port->CRH |=  (0x3 * Accumulate_4(IIC.SDA_Pin / 0x100));
	}
}


/*!
 *  \brief  产生 IIC 起始信号
 */
void IIC_Start(void)
{
	IIC_SDA_SET_OUT();     //sda线输出
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	delay_us(4);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_L;//钳住I2C总线，准备发送或接收数据 
}


/*!
 *  \brief  产生 IIC 停止信号
 */
void IIC_Stop(void)
{
	IIC_SDA_SET_OUT();//sda线输出
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_H; 
	IIC_SDA_H;//发送I2C总线结束信号
	delay_us(4);							   	
}


/*!
 *  \brief  等待IIC响应
 *  \retval 1为超时，0为成功
 */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_SET_IN();      //SDA设置为输入  
	IIC_SDA_H;delay_us(1);	   
	IIC_SCL_H;delay_us(1);	 
	while(IIC_SDA_In)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;//时钟输出0 	   
	return 0;  
}


/*!
 *  \brief  产生ACK应答
 */
void IIC_Ack(void)
{
	IIC_SCL_L;
	IIC_SDA_SET_OUT();
	IIC_SDA_L;
	delay_us(2);
	IIC_SCL_H;
	delay_us(2);
	IIC_SCL_L;
}


/*!
 *  \brief  不产生ACK应答
 */      
void IIC_NAck(void)
{
	IIC_SCL_L;
	IIC_SDA_SET_OUT();
	IIC_SDA_H;
	delay_us(2);
	IIC_SCL_H;
	delay_us(2);
	IIC_SCL_L;
}


/*!
 *  \brief  IIC发送一个字节
 *  \retval 1为有响应，0为无响应
 */ 		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC_SDA_SET_OUT(); 	    
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
		if((txd&0x80)>>7) IIC_SDA_H;
		else              IIC_SDA_L;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_H;
		delay_us(2); 
		IIC_SCL_L;	
		delay_us(2);
    }	 
}


/*!
 *  \brief  IIC读取一个字节
 *  \param 1为发送ACK，0为发送nACK
 */  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_SET_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L; 
        delay_us(2);
		IIC_SCL_H;
        receive<<=1;
        if(IIC_SDA_In)receive++;
		delay_us(1); 
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


