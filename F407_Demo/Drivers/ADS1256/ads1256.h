#ifndef _ADS1256_H_
#define _ADS1256_H_
#include "base.h"
// (2)通过改变缓冲区的启动和禁?，可以改变ADS1256_ADC模块的电压输?范围。
// 缓冲区修改位置：在ads1256.c?件（ADS1256_CfgADC函数中）
// 禁?buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
// 启动buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
// 当缓冲区启?时，输?阻抗为80MΩ,ADS1256_ADC模块的电压输?范围为0～3V.
// 当缓冲区禁?时，输?阻抗为(150/PGA增益)KΩ，ADS1256_ADC模块的电压输?范围0～5V.
// (3)通过改变第90?ADS1256_CfgADC()参数，可以改变ADC的增益和采样速率。
// PGA_1 放?1倍 每个通道允许输?电压范围0-5V
// PGA_2 放?2倍 每个通道允许输?电压范围0-2.5V
// PGA_4 放?4倍 每个通道允许输?电压范围0-1.25V
// PGA_8 放?8倍 每个通道允许输?电压范围0-0.625V
// PGA_16 放?16倍 每个通道允许输?电压范围0-312.5mV
// PGA_32 放?32倍 每个通道允许输?电压范围0-156.25mV
// PGA_64 放?64倍 每个通道允许输?电压范围0-78.125mV
//
// DATARATE_30K 采样速率30KHz
// DATARATE_15K 采样速率15KHz
// DATARATE_7_5K 采样速率7.5KHz
// DATARATE_3_7_5K 采样速率3.75KHz
// DATARATE_2K 采样速率2KHz
// DATARATE_1K 采样速率1KHz
// DATARATE_500 采样速率500Hz
// DATARATE_100 采样速率100Hz
// DATARATE_60 采样速率60Hz
// DATARATE_50 采样速率50Hz
// DATARATE_30 采样速率30Hz
// DATARATE_25 采样速率25Hz
// DATARATE_15 采样速率15Hz
// DATARATE_10 采样速率10Hz
// DATARATE_5 采样速率5Hz
// DATARATE_2_5 采样速率2.5

#define AD24_mV_Scale	5000/2^24

#define SCLK_H PEout(2)=1
#define SCLK_L PEout(2)=0

#define DIN_H PEout(4)=1
#define DIN_L PEout(4)=0

#define DO_IS_H PEin(6)

#define DRDY_IS_H PFin(0)

#define CS_H PFout(2)=1
#define CS_L PFout(2)=0

#define RST_H PFout(4)=1
#define RST_L PFout(4)=0

#define AIN0 POSITIVE_AIN0|NEGTIVE_AINCOM
#define AIN1 POSITIVE_AIN1|NEGTIVE_AINCOM
#define AIN2 POSITIVE_AIN2|NEGTIVE_AINCOM
#define AIN3 POSITIVE_AIN3|NEGTIVE_AINCOM
#define AIN4 POSITIVE_AIN4|NEGTIVE_AINCOM
#define AIN5 POSITIVE_AIN5|NEGTIVE_AINCOM
#define AIN6 POSITIVE_AIN6|NEGTIVE_AINCOM
#define AIN7 POSITIVE_AIN7|NEGTIVE_AINCOM
#define DAA0 POSITIVE_AIN0 | NEGTIVE_AIN1
#define DAA2 POSITIVE_AIN2 | NEGTIVE_AIN3
#define DAA4 POSITIVE_AIN4 | NEGTIVE_AIN5
#define DAA6 POSITIVE_AIN6 | NEGTIVE_AIN7

/* 寄存器地址， 后?是复位后缺省值 */
#define REG_STATUS (0) // x1H 状态寄存器
#define REG_MUX (1) // 01H 输?多路复?器控制寄存器
#define REG_ADCON (2) // 20H A/D控制寄存器
#define REG_DRATE (3) // F0H A/D数据速率
#define REG_IO (4) // E0H GPIO控制寄存器
#define REG_OFC0 (5) // xxH 偏移校准字节0，最低有效字节
#define REG_OFC1 (6) // xxH 偏移校准字节1
#define REG_OFC2 (7) // xxH 偏移校准字节2，最?有效字节
#define REG_FSC0 (8) // xxH 满量程校准字节0，最低有效字节
#define REG_FSC1 (9) // xxH 满量程校准字节1
#define REG_FSC2 (10)// xxH 满量程校准字节2，最?有效字节

/* 命令定义： TTable 24. Command Definitions --- ADS1256数据?册第34页 */
#define CMD_WAKEUP (0x00) // Completes SYNC and Exits Standby Mode0000 0000 (00h) 完成SYNC并退出待机状态
#define CMD_RDATA (0x01) // Read Data 0000 0001 (01h) 读取数据
#define CMD_RDATAC (0x03) // Read Data Continuously 0000 0011(03h) 连续读取数据
#define CMD_SDATAC (0x0F) // Stop Read Data Continuously 00001111 (0Fh) 停?连续读取数据
#define CMD_RREG (0x10) // Read from REG rrr 0001 rrrr (1xh) 从REG读取
#define CMD_WREG (0x50) // Write to REG rrr 0101 rrrr (5xh) 写到REG
#define CMD_SELFCAL (0xF0) // Offset and Gain Self-Calibration 11110000 (F0h) 偏移和增益?校准
#define CMD_SELFOCAL (0xF1) // Offset Self-Calibration 1111 0001(F1h) 偏移?校准
#define CMD_SELFGCAL (0xF2) // Gain Self-Calibration 1111 0010(F2h) 增益?校准
#define CMD_SYSOCAL (0xF3) // System Offset Calibration 1111 0011(F3h) 系统偏移校准
#define CMD_SYSGCAL (0xF4) // System Gain Calibration 1111 0100(F4h) 系统增益校准
#define CMD_SYNC (0xFC) // Synchronize the A/D Conversion 11111100 (FCh) 同步A/D转换
#define CMD_STANDBY (0xFD) // Begin Standby Mode 1111 1101 (FDh)开始待机模式
#define CMD_RESET (0xFE) // Reset to Power-Up Values 1111 1110(FEh) 重置为开机值
#define PGA_1 0x00
#define PGA_2 0x01
#define PGA_4 0x02
#define PGA_8 0x03
#define PGA_16 0x04
#define PGA_32 0x05
#define PGA_64 0x06
#define POSITIVE_AIN0 (0X00)
#define POSITIVE_AIN1 (0X10)
#define POSITIVE_AIN2 (0X20)
#define POSITIVE_AIN3 (0X30)
#define POSITIVE_AIN4 (0X40)
#define POSITIVE_AIN5 (0X50)
#define POSITIVE_AIN6 (0X60)
#define POSITIVE_AIN7 (0X70)
#define POSITIVE_AINCOM (0X80)
#define NEGTIVE_AIN0 0X00
#define NEGTIVE_AIN1 0X01
#define NEGTIVE_AIN2 0X02
#define NEGTIVE_AIN3 0X03
#define NEGTIVE_AIN4 0X04
#define NEGTIVE_AIN5 0X05
#define NEGTIVE_AIN6 0X06
#define NEGTIVE_AIN7 0X07
#define NEGTIVE_AINCOM 0X08
#define DATARATE_30K 0xF0
#define DATARATE_15K 0xE0
#define DATARATE_7_5K 0xD0
#define DATARATE_3_7_5K 0xC0
#define DATARATE_2K 0xB0
#define DATARATE_1K 0xA0
#define DATARATE_500 0x92
#define DATARATE_100 0x82
#define DATARATE_60 0x72
#define DATARATE_50 0x63
#define DATARATE_30 0x53
#define DATARATE_25 0x43
#define DATARATE_15 0x33
#define DATARATE_10 0x23
#define DATARATE_5 0x13
#define DATARATE_2_5 0x02
///*STATUS REGISTER*/
//#define MSB_FRIST (0x00<<3)
//#define LSB_FRIST (0x01<<3)
//#define ACAL_OFF (0x00<<2)
//#define ACAL_ON (0x01<<2)
//#define BUFEN_OFF (0x00<<1)
//#define BUFEN_ON (0x01<<1)
///*ADCON REGISTER*/
//#define CLKOUT_OFF (0x00<<5)
//#define CLKOUT_CLKIN (0x01<<5)
//#define DETECT_OFF (0x00<<3)
//#define DETECT_ON_2UA (0x02<<3)
void ADS1256_CfgADC(uint8_t gain,uint8_t drate);
uint32_t ADS1256_GetAdc(uint8_t channel);
#endif
