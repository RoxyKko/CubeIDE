#include "myiic.h"

//�۳��Ĵκ궨��
#define Accumulate_4(value) (value * value * value * value)

//IIC�������
IIC_TypeDef IIC;

//IIC��ʼ��
void IIC_Init(void)
{	
    IIC_SDA_H;
    IIC_SCL_H;
}


/*!
 *  \brief  ����IIC��SDAΪ����
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
 *  \brief  ����IIC��SDAΪ���
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
 *  \brief  ���� IIC ��ʼ�ź�
 */
void IIC_Start(void)
{
	IIC_SDA_SET_OUT();     //sda�����
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	delay_us(4);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_L;//ǯסI2C���ߣ�׼�����ͻ�������� 
}


/*!
 *  \brief  ���� IIC ֹͣ�ź�
 */
void IIC_Stop(void)
{
	IIC_SDA_SET_OUT();//sda�����
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_H; 
	IIC_SDA_H;//����I2C���߽����ź�
	delay_us(4);							   	
}


/*!
 *  \brief  �ȴ�IIC��Ӧ
 *  \retval 1Ϊ��ʱ��0Ϊ�ɹ�
 */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_SET_IN();      //SDA����Ϊ����  
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
	IIC_SCL_L;//ʱ�����0 	   
	return 0;  
}


/*!
 *  \brief  ����ACKӦ��
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
 *  \brief  ������ACKӦ��
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
 *  \brief  IIC����һ���ֽ�
 *  \retval 1Ϊ����Ӧ��0Ϊ����Ӧ
 */ 		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC_SDA_SET_OUT(); 	    
    IIC_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
		if((txd&0x80)>>7) IIC_SDA_H;
		else              IIC_SDA_L;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_H;
		delay_us(2); 
		IIC_SCL_L;	
		delay_us(2);
    }	 
}


/*!
 *  \brief  IIC��ȡһ���ֽ�
 *  \param 1Ϊ����ACK��0Ϊ����nACK
 */  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_SET_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


