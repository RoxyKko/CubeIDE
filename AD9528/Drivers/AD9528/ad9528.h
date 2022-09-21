#ifndef __AD9528_H
#define __AD9528_H
/* include standard types and definitions */
#include "main.h"

/* define gpio operation */
#define ext_sysref_req_high() HAL_GPIO_WritePin(AD9528_EXT_SYSREF_REQ_GPIO_Port, AD9528_EXT_SYSREF_REQ_Pin,  GPIO_PIN_SET  )
#define ext_sysref_req_low()  HAL_GPIO_WritePin(AD9528_EXT_SYSREF_REQ_GPIO_Port, AD9528_EXT_SYSREF_REQ_Pin,  GPIO_PIN_RESET)
#define sclk_high()           HAL_GPIO_WritePin(AD9528_SCLK_GPIO_Port          , AD9528_SCLK_Pin          ,  GPIO_PIN_SET  )
#define sclk_low()            HAL_GPIO_WritePin(AD9528_SCLK_GPIO_Port          , AD9528_SCLK_Pin          ,  GPIO_PIN_RESET)
#define sdio_high()           HAL_GPIO_WritePin(AD9528_SDIO_GPIO_Port          , AD9528_SDIO_Pin          ,  GPIO_PIN_SET  )
#define sdio_low()            HAL_GPIO_WritePin(AD9528_SDIO_GPIO_Port          , AD9528_SDIO_Pin          ,  GPIO_PIN_RESET)
#define csb_high()            HAL_GPIO_WritePin(AD9528_CSB_GPIO_Port           , AD9528_CSB_Pin           ,  GPIO_PIN_SET  )
#define csb_low()             HAL_GPIO_WritePin(AD9528_CSB_GPIO_Port           , AD9528_CSB_Pin           ,  GPIO_PIN_RESET)
#define status0_high()        HAL_GPIO_WritePin(AD9528_STATUS0_GPIO_Port       , AD9528_STATUS0_Pin       ,  GPIO_PIN_SET  )
#define status0_low()         HAL_GPIO_WritePin(AD9528_STATUS0_GPIO_Port       , AD9528_STATUS0_Pin       ,  GPIO_PIN_RESET)
#define status1_high()        HAL_GPIO_WritePin(AD9528_STATUS1_GPIO_Port       , AD9528_STATUS1_Pin       ,  GPIO_PIN_SET  )
#define status1_low()         HAL_GPIO_WritePin(AD9528_STATUS1_GPIO_Port       , AD9528_STATUS1_Pin       ,  GPIO_PIN_RESET)
#define reset_b_high()        HAL_GPIO_WritePin(AD9528_RESET_B_GPIO_Port       , AD9528_RESET_B_Pin       ,  GPIO_PIN_SET  )
#define reset_b_low()         HAL_GPIO_WritePin(AD9528_RESET_B_GPIO_Port       , AD9528_RESET_B_Pin       ,  GPIO_PIN_RESET)
#define sdo_read()            HAL_GPIO_ReadPin (AD9528_SDO_GPIO_Port           , AD9528_SDO_Pin)

HAL_StatusTypeDef AD9528_spiWriteByte (uint16_t addr, uint8_t  data    ); // Write data to a register
HAL_StatusTypeDef AD9528_spiReadByte  (uint16_t addr, uint8_t* readdata); // Read data from a register
HAL_StatusTypeDef AD9528_spiWriteField(uint16_t addr, uint8_t fieldVal,   // Modify a field of bits from a register
                                        uint8_t mask, uint8_t startBit);
HAL_StatusTypeDef AD9528_resetDevice();                                   // Performs a hard reset on the AD9528 DUT
//HAL_StatusTypeDef AD9528_setSpiSettings();                                // Sets the AD9528 device SPI settings (3wire/4wire, MSBFirst, etc).
HAL_StatusTypeDef AD9528_initialize();                                    // Initializes the AD9528 by writing all SPI registers
HAL_StatusTypeDef AD9528_requestSysref(uint8_t enableSYSREF);             // Requests a SYSREF from the AD9528.
HAL_StatusTypeDef AD9528_setupSYSREF(uint16_t divideFromPll1Out);         // Allow changing the 9528 SYSREF frequency and pattern mode(CONTINUOUS, PRBS vs NSHOT mode)
HAL_StatusTypeDef AD9528_sysrefReq(uint8_t mode);                         // Change sysref mode


// Initializes the AD9528 by writing all SPI registers

#endif
