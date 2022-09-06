#include "24cxx.h"
#include "myiic.h"

/*!
 *  \brief  AT24CXX(EEPROMоƬ)��ʼ��
 */
void AT24CXX_Init(void)
{
	extern IIC_TypeDef IIC;
	IIC.SDA_Port = AT24CXX_SDA_GPIO_Port;
	IIC.SDA_Pin  = AT24CXX_SDA_Pin;
	IIC.SCL_Port = AT24CXX_SCL_GPIO_Port;
	IIC.SCL_Pin  = AT24CXX_SCL_Pin;
	IIC_Init();//IIC��ʼ��
}


/*!
 *  \brief  AT24CXX(EEPROMоƬ)��ȡһ���ֽ�
 *  \param  ReadAddr ��ȡ�ĵ�ַ
 *  \retval ��ȡ��������
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}


/*!
 *  \brief  AT24CXX(EEPROMоƬ)д��һ���ֽ�
 *  \param  WriteAddr   д��ĵ�ַ
 *  \param  DataToWrite д�������
 *  \retval ��
 */
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}


/*!
 *  \brief  AT24CXX����Ƿ�����
 *  \retval 1-���ʧ�ܣ�0-���ɹ�
 */
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


/*!
 *  \brief  AT24CXX(EEPROMоƬ)����ָ�������ֽ�
 *  \param  ReadAddr    ��ʼ�����ĵ�ַ
 *  \param  pBuffer     �����Ļ����ַ
 *  \param  NumToRead   Ҫ�������ݵĸ���
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
 *  \brief  AT24CXX(EEPROMоƬ)д��ָ�������ֽ�
 *  \param  WriteAddr   ��ʼд������ĵ�ַ
 *  \param  pBuffer     д������ݵ�ַ
 *  \param  NumToWrite  Ҫд�����ݵĸ���
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
