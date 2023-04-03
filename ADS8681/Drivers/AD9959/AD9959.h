#ifndef _AD9959_H_
#define _AD9959_H_
#include "base.h"
#include "stdint.h"

#define kHz 1000*0.9987013790118112670830499041494
//������0.9986705

//AD9959�Ĵ�����ַ����
#define CSR_ADD   0x00   //CSR ͨ��ѡ��Ĵ���
#define FR1_ADD   0x01   //FR1 ���ܼĴ���1
#define FR2_ADD   0x02   //FR2 ���ܼĴ���2
#define CFR_ADD   0x03   //CFR ͨ�����ܼĴ���
#define CFTW0_ADD 0x04   //CTW0 ͨ��Ƶ��ת���ּĴ���
#define CPOW0_ADD 0x05   //CPW0 ͨ����λת���ּĴ���
#define ACR_ADD   0x06   //ACR ���ȿ��ƼĴ���
#define LSRR_ADD  0x07   //LSR ͨ������ɨ��Ĵ���
#define RDW_ADD   0x08   //RDW ͨ����������ɨ��Ĵ���
#define FDW_ADD   0x09   //FDW ͨ����������ɨ��Ĵ���\

/*gpiO��������*/
//#define SD2_Pin GPIO_PIN_13
//#define SD2_GPIO_Port GPIOB
//
//#define SD1_Pin GPIO_PIN_11
//#define SD1_GPIO_Port GPIOB
//
//#define SD0_Pin GPIO_PIN_15
//#define SD0_GPIO_Port GPIOE
//
//#define PS3_Pin GPIO_PIN_13
//#define PS3_GPIO_Port GPIOE
//
//#define PS2_Pin GPIO_PIN_11
//#define PS2_GPIO_Port GPIOE
//
//#define PS1_Pin GPIO_PIN_9
//#define PS1_GPIO_Port GPIOE
//
//#define PS0_Pin GPIO_PIN_7
//#define PS0_GPIO_Port GPIOE
//
//#define SD3_Pin GPIO_PIN_8
//#define SD3_GPIO_Port GPIOE
//
//#define SCK_Pin GPIO_PIN_10
//#define SCK_GPIO_Port GPIOE
//
//#define CS_Pin GPIO_PIN_12
//#define CS_GPIO_Port GPIOE
//
//#define UP_Pin GPIO_PIN_14
//#define UP_GPIO_Port GPIOE
//
//#define RST_Pin GPIO_PIN_10
//#define RST_GPIO_Port GPIOB
//
//#define PDC_Pin GPIO_PIN_12
//#define PDC_GPIO_Port GPIOB


//AD9959�ܽź궨��
#define AD9959_PWR	PCout(2)
#define Reset		PCout(0)
#define UPDATE		PFout(9)
#define CS			PFout(7)
#define SCLK		PFout(5)
#define SDIO3		PFout(3)

#define SDIO2		PFout(2)
#define SDIO1		PFout(4)
#define SDIO0		PFout(6)
#define PS3			PFout(8)
#define PS2			PFout(10)
#define PS1			PCout(1)
#define PS0			PCout(3)


void delay1 (u32 length);
void IntReset(void);	  //AD9959��λ
void IO_Update(void);   //AD9959��������
void Intserve(void);		   //IO�ڳ�ʼ��
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData,u8 temp);
void Init_AD9959(void);
void Write_frequence(u8 Channel,u32 Freq);
void Write_Amplitude(u8 Channel, u16 Ampli);
void Write_Phase(u8 Channel,u16 Phase);



#endif
