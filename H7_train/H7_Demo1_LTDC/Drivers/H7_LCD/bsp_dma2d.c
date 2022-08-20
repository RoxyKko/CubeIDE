/*
 * bsp_dma2d.c
 *
 *  Created on: 2022年8月20日
 *      Author: nongj
 */
#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: _DMA2D_Fill
*	功能说明: DMA2D颜色填充功能
*	形    参: pDst          颜色数据目的地址
*             xSize         色块X轴大小，即每行像素数
*             ySize         色块Y轴大小，即行数
*             OffLine       前景层图像的行偏移
*             ColorIndex    色块颜色值
*             PixelFormat   目标区颜色格式
*	返 回 值: 无
*********************************************************************************************************
*/
void _DMA2D_Fill(void* 	  pDst,
	             uint32_t xSize,
                 uint32_t ySize,
                 uint32_t OffLine,
                 uint32_t ColorIndex,
                 uint32_t PixelFormat)
{

	/* DMA2D采用寄存器到存储器模式, 这种模式用不到前景层和背景层 */
	DMA2D->CR      = 0x00030000UL | (1 << 9);
	DMA2D->OCOLR   = ColorIndex;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->OOR     = OffLine;
	DMA2D->OPFCCR  = PixelFormat;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;

	/* 等待DMA2D传输完成 */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	函 数 名: _DMA2D_Copy
*	功能说明: 通过DMA2D从前景层复制指定区域的颜色数据到目标区域
*	形    参: pSrc          颜色数据源地址
*             pDst          颜色数据目的地址
*             xSize         目的区域的X轴大小，即每行像素数
*             ySize         目的区域的Y轴大小，即行数
*             OffLineSrc    前景层图像的行偏移
*             OffLineDst    输出的行偏移
*             PixelFormat   目标区颜色格式
*	返 回 值: 无
*********************************************************************************************************
*/
static void _DMA2D_Copy(void * pSrc,
	                    void * pDst,
						uint32_t xSize,
						uint32_t ySize,
						uint32_t OffLineSrc,
						uint32_t OffLineDst,
						uint32_t PixelFormat)
{

	/* DMA2D采用存储器到存储器模式, 这种模式是前景层作为DMA2D输入 */
	DMA2D->CR      = 0x00000000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pSrc;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->FGOR    = OffLineSrc;
	DMA2D->OOR     = OffLineDst;

	/* 前景层和输出区域都采用的RGB565颜色格式 */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;

	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;

	/* 等待DMA2D传输完成 */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	函 数 名: _DMA2D_MixColorsBulk
*	功能说明: 前景层和目标区域的颜色混合
*	形    参: pColorFG      前景层数据源地址
*             OffLineSrcFG  前景层图像的行偏移
*             pColorDst     目标区数据地址
*             OffLineDst    目标区的行偏移
*             xSize         目的区域的X轴大小，即每行像素数
*             ySize         目的区域的Y轴大小，即行数
*             Intens        设置前景层的透明度，255表示完全不透明，0表示完全透明
*	返 回 值: 无
*********************************************************************************************************
*/
void _DMA2D_MixColorsBulk(uint32_t * pColorFG,
	                             uint32_t OffLineSrcFG,
                                 uint32_t * pColorDst,
                                 uint32_t OffLineDst,
							     uint32_t xSize,
                                 uint32_t ySize,
                                 uint8_t Intens)
{
	/* DMA2D采用存储器到存储器模式, 这种模式前景层和背景层作为DMA2D输入，且支持颜色格式转换和颜色混合 */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pColorFG;
	DMA2D->BGMAR   = (uint32_t)pColorDst;
	DMA2D->OMAR    = (uint32_t)pColorDst;
	DMA2D->FGOR    = OffLineSrcFG;
	DMA2D->BGOR    = OffLineDst;
	DMA2D->OOR     = OffLineDst;

	/* 前景层，背景层和输出区都是用的RGB565格式 */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565
				 | (1UL << 16)
				 | ((uint32_t)Intens << 24);
	DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;

	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;

	/* 等待DMA2D传输完成 */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	函 数 名: _DMA2D_AlphaBlendingBulk
*	功能说明: 前景层和背景层的颜色混合
*	形    参: pColorFG      前景层源数据地址
*             OffLineSrcFG  前景层源数据行偏移
*             pColorBG      背景层源数据地址
*             OffLineSrcBG  背景层源数据行偏移
*             pColorDst     目标区地址
*             OffLineDst    目标区行偏移
*             xSize         目标区域的X轴大小，即每行像素数
*             ySize         目标区域的Y轴大小，即行数
*	返 回 值: 无
*********************************************************************************************************
*/
void _DMA2D_AlphaBlendingBulk(uint32_t * pColorFG,
	                                 uint32_t OffLineSrcFG,
	                                 uint32_t * pColorBG,
                                     uint32_t OffLineSrcBG,
                                     uint32_t * pColorDst,
                                     uint32_t OffLineDst,
								     uint32_t xSize,
                                     uint32_t ySize)
{
	/* DMA2D采用存储器到存储器模式, 这种模式前景层和背景层作为DMA2D输入，且支持颜色格式转换和颜色混合 */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pColorFG;
	DMA2D->BGMAR   = (uint32_t)pColorBG;
	DMA2D->OMAR    = (uint32_t)pColorDst;
	DMA2D->FGOR    = OffLineSrcFG;
	DMA2D->BGOR    = OffLineSrcBG;
	DMA2D->OOR     = OffLineDst;

	/* 前景层，背景层采用ARGB8888格式，输出区采用RGB565格式 */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;

	/* 等待DMA2D传输完成 */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	函 数 名: _DMA2D_DrawAlphaBitmap
*	功能说明: ARGB8888格式位图显示
*	形    参: pDst        目标区地址
*             pSrc        源数据地址，即位图首地址
*             xSize       目标区域的X轴大小，即每行像素数
*             ySize       目标区域的Y轴大小，即行数
*             OffLineSrc  源数据行偏移
*             OffLineDst  目标区行偏移
*             PixelFormat 目标区颜色格式
*	返 回 值: 无
*********************************************************************************************************
*/
void _DMA2D_DrawAlphaBitmap(void  * pDst,
	                               void  * pSrc,
								   uint32_t xSize,
								   uint32_t ySize,
								   uint32_t OffLineSrc,
								   uint32_t OffLineDst,
								   uint32_t PixelFormat)
{
	/* DMA2D采用存储器到存储器模式, 这种模式前景层和背景层作为DMA2D输入，且支持颜色格式转换和颜色混合 */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pSrc;
	DMA2D->BGMAR   = (uint32_t)pDst;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->FGOR    = OffLineSrc;
	DMA2D->BGOR    = OffLineDst;
	DMA2D->OOR     = OffLineDst;

	/* 前景层颜色格式是LTDC_PIXEL_FORMAT_ARGB8888，即位图的颜色格式，背景层和输出区颜色格式可配置 */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->BGPFCCR = PixelFormat;
	DMA2D->OPFCCR  = PixelFormat;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* 启动传输 */
	DMA2D->CR   |= DMA2D_CR_START;

	/* 等待DMA2D传输完成 */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

