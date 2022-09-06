#ifndef _MYIIC_H
#define _MYIIC_H
#include "base.h"

//串行数据线SDA——负责在设备间传输串行数据线
//串行时钟线SCL——负责产生同步时钟脉冲线
#define IIC_SCL_H   HAL_GPIO_WritePin(IIC.SCL_Port, IIC.SCL_Pin, GPIO_PIN_SET  )
#define IIC_SCL_L   HAL_GPIO_WritePin(IIC.SCL_Port, IIC.SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_H   HAL_GPIO_WritePin(IIC.SDA_Port, IIC.SDA_Pin, GPIO_PIN_SET  )
#define IIC_SDA_L   HAL_GPIO_WritePin(IIC.SDA_Port, IIC.SDA_Pin, GPIO_PIN_RESET)
#define IIC_SDA_In  HAL_GPIO_ReadPin (IIC.SDA_Port, IIC.SDA_Pin)


//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data); //IIC写数据
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	           //IIC读数据

//自定义IIC句柄结构体
typedef struct
{
	GPIO_TypeDef * SDA_Port;
	uint16_t       SDA_Pin;
	GPIO_TypeDef * SCL_Port;
	uint16_t       SCL_Pin;
}IIC_TypeDef;
#endif

