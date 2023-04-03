/*
 * @Author: RoxyKko
 * @Date: 2023-03-31 20:07:33
 * @LastEditors: RoxyKko
 * @LastEditTime: 2023-03-31 21:43:21
 * @Description: ���Ұ�˿ӡӡ���ˣ�����ס
 */

#include "ads8681.h"
#include "base.h"
#include "main.h"

volatile uint16_t ALARM; // �����Ĵ���

/**
 * @name: ADS8681_GPIO_Init(void)
 * @description: ADS8681 GPIO��ʼ����gpio�����Ѿ���CubeMX���ú��ˣ�����ֻ�ǳ�ʼ��һ������Ĭ�ϵ�ƽ{��ʵ��Cubemxһ�����Ҳ����}
 * @return {*}
 */
void ADS8681_GPIO_Init(void)
{
    nRESET_H;
    SCLK_L;
    nCS_H;
}

/**
 * @name: ADS8681_Reset(void)
 * @description: ASD8681��λ����λʱ��Ϊ��nRESET�͵�ƽ5us��nRESET�ߵ�ƽ5us
 * @return {*}
 */
void ADS8681_Reset(void)
{
    delay_us(5);
    nRESET_L;
    delay_us(5);
    nRESET_H;
    delay_us(5);
}

/**
 * @name: ADS8681_COM_Rreg(uint16_t Addr)
 * @description: ASD8681���Ĵ���������AddrΪ�Ĵ�����ַ������ֵΪ�Ĵ���ֵ
 * @param  {uint16_t} Addr  �Ĵ�����ַ
 * @return {uint16_t} Rdata �Ĵ���ֵ
 */
uint16_t ADS8681_COM_Rreg(uint16_t Addr)
{
    u8 s = 0;
    uint32_t DTemp = 0;
    uint16_t Rdata = 0;
    DTemp += Addr;
    DTemp <<= 16;
    DTemp |= 0xC8000000;

    while (!RVS)
        ;
    nCS_L;
    for (s = 0; s < 32; s++)
    {
        if (DTemp & 0x80000000)
        {
            SDI_H;
        }
        else
        {
            SDI_L;
        }
        SCLK_H;
        DTemp <<= 1;
        SCLK_L;
    }
    nCS_H;

    while (!RVS)
        ;
    nCS_L;
    for (s = 0; s < 16; s++)
    {
        Rdata <<= 1;
        SCLK_H;
        if (SDO_0)
        {
            Rdata++;
        }
        SCLK_L;
    }
    for (s = 0; s < 16; s++)
    {
        SCLK_H;
        SCLK_L;
    }
    nCS_H;
    return Rdata;
}

/**
 * @name: ADS8681_COM_Wreg(uint16_t Addr,uint16_t Com)
 * @description: ADS8681д�Ĵ���������AddrΪ�Ĵ�����ַ��ComΪ�Ĵ���ֵ
 * @param {uint16_t} Addr �Ĵ�����ַ
 * @param {uint16_t} Com  �Ĵ���ֵ
 * @return {*}
 */
void ADS8681_COM_Wreg(uint16_t Addr, uint16_t Com)
{
    u8 s = 0;
    uint32_t DTemp = 0;
    DTemp += Addr;
    DTemp <<= 16;
    DTemp += Com;
    DTemp |= 0xD0000000;

    while (!RVS)
        ;
    nCS_L;
    for (s = 0; s < 32; s++)
    {
        if (DTemp & 0x80000000)
        {
            SDI_H;
        }
        else
        {
            SDI_L;
        }
        SCLK_H;
        DTemp <<= 1;
        SCLK_L;
    }
    nCS_H;
}

/**
 * @name: ADS8681_Init_Mult(void)
 * @description: ASD8681��ʼ����������ʼ��ʱ��Ϊ����λ���������̣�����SDOģʽ����ȡ�����Ĵ���
 * @return {*}
 */
void ADS8681_Init_Mult(void)
{
    u8 a = 0;
    ADS8681_Reset();
    ADS8681_COM_Wreg(0x14, _ref3 | INTREF_External); // �������ã��ⲿ�ο���׼��ѹ
    ADS8681_COM_Wreg(0x0C, 0x0100);                  // SDOģʽ���ã�ʱ��Ϊ�ڲ�ʱ��

    for (a = 0; a < 2; a++)
    {
        nCS_L;
        delay_us(5);
        nCS_H;
        while (!RVS)
            ;
    }
    ALARM = ADS8681_COM_Rreg(0x0020);
}

/**
 * @name: ADS8681_GetData(void)
 * @description: ADS8681��ȡ���ݺ���������ֵΪ16λ����
 * @return {uint16_t} data_tmp 16λ����
 */
uint16_t ADS8681_GetData(void)
{
    uint16_t data_tmp = 0;
    nCS_H;  // �ͷ�Ƭѡ
    while(!RVS);  // �ȴ�ת�����
    nCS_L;  // ѡ��Ƭѡ
    int i = 0;
    for(i=0;i<16;i++)
    {
        data_tmp<<=1;
        SCLK_H;
        if(SDO_0)
        {
            data_tmp++;
        }
        SCLK_L;
    }
    return data_tmp;
}

/**
 * @name: ADS8681_GetVoltage(uint16_t data)
 * @description: ADS8681����ת��Ϊ��ѹֵ����������ֵΪ��ѹֵ
 * @param {uint16_t} data 16λ����
 * @return {float}voltage ��ѹֵ
 */
float ADS8681_GetVoltage(uint16_t data)
{
    float voltage,temp = 0;
    temp = data - 0x8000;
    voltage = (float)temp / 32768.0 * Vref * 3.0;  //��ѹת����ʽ��һ��Ҫ�����ڲ����õı��� ��У׼��׼��ѹֵ
    return voltage;
}
