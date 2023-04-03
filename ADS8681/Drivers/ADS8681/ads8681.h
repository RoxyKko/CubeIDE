/*** 
 * @Author: RoxyKko
 * @Date: 2023-03-31 20:07:22
 * @LastEditors: RoxyKko
 * @LastEditTime: 2023-03-31 21:45:18
 * @Description: 这店家把丝印印错了，绷不住
 */

#ifndef _ADS8681_H_
#define _ADS8681_H_
#include "main.h"
// 基准电压
#define Vref 4.096

/*******ADS8681引脚初始化*******
	*	 RVS    -->  PB15    input
	*	 SCLK   -->  PD9     output
	*    nCS    -->  PD10    output
	*    RST    -->  PD13    output
	*	 SDI    -->  PD11    output
	*    SDO1   -->  PB14    input
    *    SDO0   -->  PD8     input
********************************/
#define RVS             HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) 

#define SCLK_H          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define SCLK_L          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

#define nCS_H           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET)
#define nCS_L           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET)

#define nRESET_H        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define nRESET_L        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)

#define SDI_H           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET)
#define SDI_L           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET)

#define SDO_1           HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)

#define SDO_0           HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)

#define _ref3      0x0000 	
#define _ref25     0x0001
#define _ref15     0x0002 
#define _ref125    0x0003  
#define _ref0625   0x0004

#define ref3       0x0008 
#define ref25      0x0009
#define ref15      0x000A
#define ref125     0x000B

#define INTREF_Internal  0x0000      //内部参考输入
#define INTREF_External  0x0100    //外部参考输入


void ADS8681_Init_Mult(void);
void ADS8681_GPIO_Init(void) ;
void Delay(__IO uint32_t nCount);
void ADS8681_COM_Wreg(uint16_t Addr,uint16_t Com);
uint16_t ADS8681_COM_Rreg(uint16_t Addr);
void ADS8681_Reset(void);

// 读取ADS8681数据
uint16_t ADS8681_GetData(void);
float ADS8681_GetVoltage(uint16_t data);




#endif /* ADS8681_ADS8681_H_ */
