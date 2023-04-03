/*
 * @Author: RoxyKko
 * @Date: 2023-03-31 20:07:33
 * @LastEditors: RoxyKko
 * @LastEditTime: 2023-03-31 21:43:21
 * @Description: 这店家把丝印印错了，绷不住
 */

#include "ads8681.h"
#include "base.h"
#include "main.h"

volatile uint16_t ALARM; // 报警寄存器

/**
 * @name: ADS8681_GPIO_Init(void)
 * @description: ADS8681 GPIO初始化，gpio设置已经用CubeMX配置好了，这里只是初始化一下引脚默认电平{其实用Cubemx一起配好也可以}
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
 * @description: ASD8681复位，复位时序为：nRESET低电平5us，nRESET高电平5us
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
 * @description: ASD8681读寄存器函数，Addr为寄存器地址，返回值为寄存器值
 * @param  {uint16_t} Addr  寄存器地址
 * @return {uint16_t} Rdata 寄存器值
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
 * @description: ADS8681写寄存器函数，Addr为寄存器地址，Com为寄存器值
 * @param {uint16_t} Addr 寄存器地址
 * @param {uint16_t} Com  寄存器值
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
 * @description: ASD8681初始化函数，初始化时序为：复位，设置量程，设置SDO模式，读取报警寄存器
 * @return {*}
 */
void ADS8681_Init_Mult(void)
{
    u8 a = 0;
    ADS8681_Reset();
    ADS8681_COM_Wreg(0x14, _ref3 | INTREF_External); // 量程设置，外部参考基准电压
    ADS8681_COM_Wreg(0x0C, 0x0100);                  // SDO模式设置，时钟为内部时钟

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
 * @description: ADS8681读取数据函数，返回值为16位数据
 * @return {uint16_t} data_tmp 16位数据
 */
uint16_t ADS8681_GetData(void)
{
    uint16_t data_tmp = 0;
    nCS_H;  // 释放片选
    while(!RVS);  // 等待转换完成
    nCS_L;  // 选中片选
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
 * @description: ADS8681数据转换为电压值函数，返回值为电压值
 * @param {uint16_t} data 16位数据
 * @return {float}voltage 电压值
 */
float ADS8681_GetVoltage(uint16_t data)
{
    float voltage,temp = 0;
    temp = data - 0x8000;
    voltage = (float)temp / 32768.0 * Vref * 3.0;  //电压转换公式，一定要乘以内部设置的倍数 和校准基准电压值
    return voltage;
}
