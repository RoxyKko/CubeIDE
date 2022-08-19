/*
*********************************************************************************************************
*
*	模块名称 : STM32H7驱动液晶
*	文件名称 : bsp_tft_h7.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TFT_H7_H
#define _BSP_TFT_H7_H

#define LCD_LAYER_1     0x0000		/* 顶层 */
#define LCD_LAYER_2		0x0001		/* 第2层 */

/* 可供外部模块调用的函数 */

/* 初始化LCD */
void LCDH7_InitHard(void);

/* 读取LCD驱动芯片的描述符号，用于显示 */
void LCDH7_GetChipDescribe(char *_str);

/* 切换层。只是更改程序变量，以便于后面的代码更改相关寄存器。硬件支持2层。 */
void LCDH7_SetLayer(uint8_t _ucLayer);

/* 配置当前层的透明属性 */
void LCDH7_SetTransparency(uint8_t transparency);

/* 配置当前层的像素格式 */
void LCDH7_SetPixelFormat(uint32_t PixelFormat);

/* 设置显示窗口，进入窗口显示模式。 */
void LCDH7_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);

/* 退出窗口显示模式，变为全屏显示模式 */
void LCDH7_QuitWinMode(void);

/* 打开显示 */
void LCDH7_DispOn(void);

/* 关闭显示 */
void LCDH7_DispOff(void);

/* 根据输入的颜色值清屏 */
void LCDH7_ClrScr(uint16_t _usColor);

/* 画1个像素 */
void LCDH7_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);

/* 读取1个像素 */
uint16_t LCDH7_GetPixel(uint16_t _usX, uint16_t _usY);

/* 采用 Bresenham 算法，在2点间画一条直线。 */
void LCDH7_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);

/* 绘制一条水平线. 使用DMA2D硬件绘制 */
void LCDH7_DrawHLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor);

/* 绘制一条垂直线。 使用DMA2D硬件绘制. */
void LCDH7_DrawVLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor);

/* 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。 */
void LCDH7_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);

/* 绘制水平放置的矩形。 */
void LCDH7_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* 用一个颜色值填充一个矩形。使用内部DMA2D硬件绘制。 */
void LCDH7_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* 绘制一个圆，笔宽为1个像素 */
void LCDH7_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);

/* 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下 */
void LCDH7_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);

/* 设置显示屏显示方向（横屏 竖屏） */
void LCDH7_SetDirection(uint8_t _dir);

/* 通过DMA2D从前景层复制指定区域的颜色数据到目标区域 */
void DMA2D_CopyBuffer(uint32_t LayerIndex, void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);

/* 16bpp位图绘制，专用于摄像头 */
void _LCD_DrawCamera16bpp(int x, int y, uint16_t * p, int xSize, int ySize, int SrcOffLine);
#endif


