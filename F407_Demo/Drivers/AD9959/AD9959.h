#ifndef _AD9959_H_
#define _AD9959_H_
#include "base.h"
#include "stdint.h"

#define kHz 1000*0.9987013790118112670830499041494
//误差比例0.9986705

//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器\

/*gpiO引脚设置*/
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


//AD9959管脚宏定义
#define AD9959_PWR	PBout(12)
#define Reset		PBout(10)
#define UPDATE		PEout(14)
#define CS			PEout(12)
#define SCLK		PEout(10)
#define SDIO3		PEout(8)

#define SDIO2		PBout(13)
#define SDIO1		PBout(11)
#define SDIO0		PEout(15)
#define PS3		PEout(13)
#define PS2		PEout(11)
#define PS1		PEout(9)
#define PS0		PEout(7)


void delay1 (u32 length);
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData,u8 temp);
void Init_AD9959(void);
void Write_frequence(u8 Channel,u32 Freq);
void Write_Amplitude(u8 Channel, u16 Ampli);
void Write_Phase(u8 Channel,u16 Phase);



#endif
