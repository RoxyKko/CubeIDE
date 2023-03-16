#ifndef _ADS1256_H_
#define _ADS1256_H_
#include "base.h"
// (2)ͨ���ı仺�����������ͽ�?�����Ըı�ADS1256_ADCģ��ĵ�ѹ��?��Χ��
// �������޸�λ�ã���ads1256.c?����ADS1256_CfgADC�����У�
// ��?buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
// ����buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
// ����������?ʱ����?�迹Ϊ80M��,ADS1256_ADCģ��ĵ�ѹ��?��ΧΪ0��3V.
// ����������?ʱ����?�迹Ϊ(150/PGA����)K����ADS1256_ADCģ��ĵ�ѹ��?��Χ0��5V.
// (3)ͨ���ı��90?ADS1256_CfgADC()���������Ըı�ADC������Ͳ������ʡ�
// PGA_1 ��?1�� ÿ��ͨ��������?��ѹ��Χ0-5V
// PGA_2 ��?2�� ÿ��ͨ��������?��ѹ��Χ0-2.5V
// PGA_4 ��?4�� ÿ��ͨ��������?��ѹ��Χ0-1.25V
// PGA_8 ��?8�� ÿ��ͨ��������?��ѹ��Χ0-0.625V
// PGA_16 ��?16�� ÿ��ͨ��������?��ѹ��Χ0-312.5mV
// PGA_32 ��?32�� ÿ��ͨ��������?��ѹ��Χ0-156.25mV
// PGA_64 ��?64�� ÿ��ͨ��������?��ѹ��Χ0-78.125mV
//
// DATARATE_30K ��������30KHz
// DATARATE_15K ��������15KHz
// DATARATE_7_5K ��������7.5KHz
// DATARATE_3_7_5K ��������3.75KHz
// DATARATE_2K ��������2KHz
// DATARATE_1K ��������1KHz
// DATARATE_500 ��������500Hz
// DATARATE_100 ��������100Hz
// DATARATE_60 ��������60Hz
// DATARATE_50 ��������50Hz
// DATARATE_30 ��������30Hz
// DATARATE_25 ��������25Hz
// DATARATE_15 ��������15Hz
// DATARATE_10 ��������10Hz
// DATARATE_5 ��������5Hz
// DATARATE_2_5 ��������2.5

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

/* �Ĵ�����ַ�� ��?�Ǹ�λ��ȱʡֵ */
#define REG_STATUS (0) // x1H ״̬�Ĵ���
#define REG_MUX (1) // 01H ��?��·��?�����ƼĴ���
#define REG_ADCON (2) // 20H A/D���ƼĴ���
#define REG_DRATE (3) // F0H A/D��������
#define REG_IO (4) // E0H GPIO���ƼĴ���
#define REG_OFC0 (5) // xxH ƫ��У׼�ֽ�0�������Ч�ֽ�
#define REG_OFC1 (6) // xxH ƫ��У׼�ֽ�1
#define REG_OFC2 (7) // xxH ƫ��У׼�ֽ�2����?��Ч�ֽ�
#define REG_FSC0 (8) // xxH ������У׼�ֽ�0�������Ч�ֽ�
#define REG_FSC1 (9) // xxH ������У׼�ֽ�1
#define REG_FSC2 (10)// xxH ������У׼�ֽ�2����?��Ч�ֽ�

/* ����壺 TTable 24. Command Definitions --- ADS1256����?���34ҳ */
#define CMD_WAKEUP (0x00) // Completes SYNC and Exits Standby Mode0000 0000 (00h) ���SYNC���˳�����״̬
#define CMD_RDATA (0x01) // Read Data 0000 0001 (01h) ��ȡ����
#define CMD_RDATAC (0x03) // Read Data Continuously 0000 0011(03h) ������ȡ����
#define CMD_SDATAC (0x0F) // Stop Read Data Continuously 00001111 (0Fh) ͣ?������ȡ����
#define CMD_RREG (0x10) // Read from REG rrr 0001 rrrr (1xh) ��REG��ȡ
#define CMD_WREG (0x50) // Write to REG rrr 0101 rrrr (5xh) д��REG
#define CMD_SELFCAL (0xF0) // Offset and Gain Self-Calibration 11110000 (F0h) ƫ�ƺ�����?У׼
#define CMD_SELFOCAL (0xF1) // Offset Self-Calibration 1111 0001(F1h) ƫ��?У׼
#define CMD_SELFGCAL (0xF2) // Gain Self-Calibration 1111 0010(F2h) ����?У׼
#define CMD_SYSOCAL (0xF3) // System Offset Calibration 1111 0011(F3h) ϵͳƫ��У׼
#define CMD_SYSGCAL (0xF4) // System Gain Calibration 1111 0100(F4h) ϵͳ����У׼
#define CMD_SYNC (0xFC) // Synchronize the A/D Conversion 11111100 (FCh) ͬ��A/Dת��
#define CMD_STANDBY (0xFD) // Begin Standby Mode 1111 1101 (FDh)��ʼ����ģʽ
#define CMD_RESET (0xFE) // Reset to Power-Up Values 1111 1110(FEh) ����Ϊ����ֵ
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
