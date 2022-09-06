#include "24cxx.h"
#include "myiic.h"

/*!
 *  \brief  AT24CXX(EEPROM芯片)初始化
 */
void AT24CXX_Init(void)
{
	extern IIC_TypeDef IIC;
	IIC.SDA_Port = AT24CXX_SDA_GPIO_Port;
	IIC.SDA_Pin  = AT24CXX_SDA_Pin;
	IIC.SCL_Port = AT24CXX_SCL_GPIO_Port;
	IIC.SCL_Pin  = AT24CXX_SCL_Pin;
	IIC_Init();//IIC初始化
}


/*!
 *  \brief  AT24CXX(EEPROM芯片)读取一个字节
 *  \param  ReadAddr 读取的地址
 *  \retval 读取到的数据
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}


/*!
 *  \brief  AT24CXX(EEPROM芯片)写入一个字节
 *  \param  WriteAddr   写入的地址
 *  \param  DataToWrite 写入的数据
 *  \retval 无
 */
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}


/*!
 *  \brief  AT24CXX检测是否正常
 *  \retval 1-检测失败，0-检测成功
 */
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


/*!
 *  \brief  AT24CXX(EEPROM芯片)读出指定个数字节
 *  \param  ReadAddr    开始读出的地址
 *  \param  pBuffer     读出的缓存地址
 *  \param  NumToRead   要读出数据的个数
 */
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}


/*!
 *  \brief  AT24CXX(EEPROM芯片)写入指定个数字节
 *  \param  WriteAddr   开始写入读出的地址
 *  \param  pBuffer     写入的数据地址
 *  \param  NumToWrite  要写入数据的个数
 */
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
