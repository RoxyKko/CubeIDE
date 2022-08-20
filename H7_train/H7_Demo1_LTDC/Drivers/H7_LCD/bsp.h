/*
 * bsp.h
 *
 *  Created on: 2022年8月19日
 *      Author: nongj
 */

#ifndef _BSP_H_
#define _BSP_H_
#include "stm32h7xx_hal.h"
#include "ltdc.h"
#include "fmc.h"
#include "gpio.h"
#include "dma2d.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

#include "bsp_fmc_sdram.h"
#include "bsp_tft_h7.h"
#include "bsp_tft_lcd.h"
#include "bsp_dma2d.h"


#endif /* _BSP_H_ */
