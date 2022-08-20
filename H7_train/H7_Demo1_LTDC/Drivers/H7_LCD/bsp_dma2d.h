/*
 * bsp_dma2d.h
 *
 *  Created on: 2022年8月20日
 *      Author: nongj
 */

#ifndef _BSP_DMA2D_H_
#define _BSP_DMA2D_H_
#include "bsp.h"

/*DMA2D颜色填充功能*/
void _DMA2D_Fill(void * pDst,
	                    uint32_t xSize,
                        uint32_t ySize,
                        uint32_t OffLine,
                        uint32_t ColorIndex,
                        uint32_t PixelFormat);




#endif /* H7_LCD_BSP_DMA2D_H_ */
