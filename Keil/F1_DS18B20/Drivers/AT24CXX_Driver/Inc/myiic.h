#ifndef _MYIIC_H
#define _MYIIC_H
#include "base.h"

//����������SDA�����������豸�䴫�䴮��������
//����ʱ����SCL�����������ͬ��ʱ��������
#define IIC_SCL_H   HAL_GPIO_WritePin(IIC.SCL_Port, IIC.SCL_Pin, GPIO_PIN_SET  )
#define IIC_SCL_L   HAL_GPIO_WritePin(IIC.SCL_Port, IIC.SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_H   HAL_GPIO_WritePin(IIC.SDA_Port, IIC.SDA_Pin, GPIO_PIN_SET  )
#define IIC_SDA_L   HAL_GPIO_WritePin(IIC.SDA_Port, IIC.SDA_Pin, GPIO_PIN_RESET)
#define IIC_SDA_In  HAL_GPIO_ReadPin (IIC.SDA_Port, IIC.SDA_Pin)


//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data); //IICд����
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	           //IIC������

//�Զ���IIC����ṹ��
typedef struct
{
	GPIO_TypeDef * SDA_Port;
	uint16_t       SDA_Pin;
	GPIO_TypeDef * SCL_Port;
	uint16_t       SCL_Pin;
}IIC_TypeDef;
#endif

