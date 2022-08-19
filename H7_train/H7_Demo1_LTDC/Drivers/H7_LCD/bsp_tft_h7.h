/*
*********************************************************************************************************
*
*	ģ������ : STM32H7����Һ��
*	�ļ����� : bsp_tft_h7.h
*	��    �� : V2.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TFT_H7_H
#define _BSP_TFT_H7_H

#define LCD_LAYER_1     0x0000		/* ���� */
#define LCD_LAYER_2		0x0001		/* ��2�� */

/* �ɹ��ⲿģ����õĺ��� */

/* ��ʼ��LCD */
void LCDH7_InitHard(void);

/* ��ȡLCD����оƬ���������ţ�������ʾ */
void LCDH7_GetChipDescribe(char *_str);

/* �л��㡣ֻ�Ǹ��ĳ���������Ա��ں���Ĵ��������ؼĴ�����Ӳ��֧��2�㡣 */
void LCDH7_SetLayer(uint8_t _ucLayer);

/* ���õ�ǰ���͸������ */
void LCDH7_SetTransparency(uint8_t transparency);

/* ���õ�ǰ������ظ�ʽ */
void LCDH7_SetPixelFormat(uint32_t PixelFormat);

/* ������ʾ���ڣ����봰����ʾģʽ�� */
void LCDH7_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);

/* �˳�������ʾģʽ����Ϊȫ����ʾģʽ */
void LCDH7_QuitWinMode(void);

/* ����ʾ */
void LCDH7_DispOn(void);

/* �ر���ʾ */
void LCDH7_DispOff(void);

/* �����������ɫֵ���� */
void LCDH7_ClrScr(uint16_t _usColor);

/* ��1������ */
void LCDH7_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);

/* ��ȡ1������ */
uint16_t LCDH7_GetPixel(uint16_t _usX, uint16_t _usY);

/* ���� Bresenham �㷨����2��仭һ��ֱ�ߡ� */
void LCDH7_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);

/* ����һ��ˮƽ��. ʹ��DMA2DӲ������ */
void LCDH7_DrawHLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor);

/* ����һ����ֱ�ߡ� ʹ��DMA2DӲ������. */
void LCDH7_DrawVLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor);

/* ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ�� */
void LCDH7_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);

/* ����ˮƽ���õľ��Ρ� */
void LCDH7_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* ��һ����ɫֵ���һ�����Ρ�ʹ���ڲ�DMA2DӲ�����ơ� */
void LCDH7_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* ����һ��Բ���ʿ�Ϊ1������ */
void LCDH7_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);

/* ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ��� */
void LCDH7_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);

/* ������ʾ����ʾ���򣨺��� ������ */
void LCDH7_SetDirection(uint8_t _dir);

/* ͨ��DMA2D��ǰ���㸴��ָ���������ɫ���ݵ�Ŀ������ */
void DMA2D_CopyBuffer(uint32_t LayerIndex, void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);

/* 16bppλͼ���ƣ�ר��������ͷ */
void _LCD_DrawCamera16bpp(int x, int y, uint16_t * p, int xSize, int ySize, int SrcOffLine);
#endif


