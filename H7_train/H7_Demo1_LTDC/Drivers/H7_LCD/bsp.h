/*
 * bsp.h
 *
 *  Created on: 2022��8��19��
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

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

#include "bsp_fmc_sdram.h"
#include "bsp_tft_h7.h"
#include "bsp_tft_lcd.h"
#include "bsp_dma2d.h"


#endif /* _BSP_H_ */
