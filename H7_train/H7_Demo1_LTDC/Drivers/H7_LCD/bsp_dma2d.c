/*
 * bsp_dma2d.c
 *
 *  Created on: 2022��8��20��
 *      Author: nongj
 */
#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: _DMA2D_Fill
*	����˵��: DMA2D��ɫ��书��
*	��    ��: pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         ɫ��X���С����ÿ��������
*             ySize         ɫ��Y���С��������
*             OffLine       ǰ����ͼ�����ƫ��
*             ColorIndex    ɫ����ɫֵ
*             PixelFormat   Ŀ������ɫ��ʽ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _DMA2D_Fill(void* 	  pDst,
	             uint32_t xSize,
                 uint32_t ySize,
                 uint32_t OffLine,
                 uint32_t ColorIndex,
                 uint32_t PixelFormat)
{

	/* DMA2D���üĴ������洢��ģʽ, ����ģʽ�ò���ǰ����ͱ����� */
	DMA2D->CR      = 0x00030000UL | (1 << 9);
	DMA2D->OCOLR   = ColorIndex;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->OOR     = OffLine;
	DMA2D->OPFCCR  = PixelFormat;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* �������� */
	DMA2D->CR   |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA2D_Copy
*	����˵��: ͨ��DMA2D��ǰ���㸴��ָ���������ɫ���ݵ�Ŀ������
*	��    ��: pSrc          ��ɫ����Դ��ַ
*             pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         Ŀ�������X���С����ÿ��������
*             ySize         Ŀ�������Y���С��������
*             OffLineSrc    ǰ����ͼ�����ƫ��
*             OffLineDst    �������ƫ��
*             PixelFormat   Ŀ������ɫ��ʽ
*	�� �� ֵ: ��
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

	/* DMA2D���ô洢�����洢��ģʽ, ����ģʽ��ǰ������ΪDMA2D���� */
	DMA2D->CR      = 0x00000000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pSrc;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->FGOR    = OffLineSrc;
	DMA2D->OOR     = OffLineDst;

	/* ǰ�����������򶼲��õ�RGB565��ɫ��ʽ */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;

	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* �������� */
	DMA2D->CR   |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA2D_MixColorsBulk
*	����˵��: ǰ�����Ŀ���������ɫ���
*	��    ��: pColorFG      ǰ��������Դ��ַ
*             OffLineSrcFG  ǰ����ͼ�����ƫ��
*             pColorDst     Ŀ�������ݵ�ַ
*             OffLineDst    Ŀ��������ƫ��
*             xSize         Ŀ�������X���С����ÿ��������
*             ySize         Ŀ�������Y���С��������
*             Intens        ����ǰ�����͸���ȣ�255��ʾ��ȫ��͸����0��ʾ��ȫ͸��
*	�� �� ֵ: ��
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
	/* DMA2D���ô洢�����洢��ģʽ, ����ģʽǰ����ͱ�������ΪDMA2D���룬��֧����ɫ��ʽת������ɫ��� */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pColorFG;
	DMA2D->BGMAR   = (uint32_t)pColorDst;
	DMA2D->OMAR    = (uint32_t)pColorDst;
	DMA2D->FGOR    = OffLineSrcFG;
	DMA2D->BGOR    = OffLineDst;
	DMA2D->OOR     = OffLineDst;

	/* ǰ���㣬�����������������õ�RGB565��ʽ */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565
				 | (1UL << 16)
				 | ((uint32_t)Intens << 24);
	DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;

	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* �������� */
	DMA2D->CR   |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA2D_AlphaBlendingBulk
*	����˵��: ǰ����ͱ��������ɫ���
*	��    ��: pColorFG      ǰ����Դ���ݵ�ַ
*             OffLineSrcFG  ǰ����Դ������ƫ��
*             pColorBG      ������Դ���ݵ�ַ
*             OffLineSrcBG  ������Դ������ƫ��
*             pColorDst     Ŀ������ַ
*             OffLineDst    Ŀ������ƫ��
*             xSize         Ŀ�������X���С����ÿ��������
*             ySize         Ŀ�������Y���С��������
*	�� �� ֵ: ��
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
	/* DMA2D���ô洢�����洢��ģʽ, ����ģʽǰ����ͱ�������ΪDMA2D���룬��֧����ɫ��ʽת������ɫ��� */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pColorFG;
	DMA2D->BGMAR   = (uint32_t)pColorBG;
	DMA2D->OMAR    = (uint32_t)pColorDst;
	DMA2D->FGOR    = OffLineSrcFG;
	DMA2D->BGOR    = OffLineSrcBG;
	DMA2D->OOR     = OffLineDst;

	/* ǰ���㣬���������ARGB8888��ʽ�����������RGB565��ʽ */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* �������� */
	DMA2D->CR   |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA2D_DrawAlphaBitmap
*	����˵��: ARGB8888��ʽλͼ��ʾ
*	��    ��: pDst        Ŀ������ַ
*             pSrc        Դ���ݵ�ַ����λͼ�׵�ַ
*             xSize       Ŀ�������X���С����ÿ��������
*             ySize       Ŀ�������Y���С��������
*             OffLineSrc  Դ������ƫ��
*             OffLineDst  Ŀ������ƫ��
*             PixelFormat Ŀ������ɫ��ʽ
*	�� �� ֵ: ��
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
	/* DMA2D���ô洢�����洢��ģʽ, ����ģʽǰ����ͱ�������ΪDMA2D���룬��֧����ɫ��ʽת������ɫ��� */
	DMA2D->CR      = 0x00020000UL | (1 << 9);
	DMA2D->FGMAR   = (uint32_t)pSrc;
	DMA2D->BGMAR   = (uint32_t)pDst;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->FGOR    = OffLineSrc;
	DMA2D->BGOR    = OffLineDst;
	DMA2D->OOR     = OffLineDst;

	/* ǰ������ɫ��ʽ��LTDC_PIXEL_FORMAT_ARGB8888����λͼ����ɫ��ʽ����������������ɫ��ʽ������ */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
	DMA2D->BGPFCCR = PixelFormat;
	DMA2D->OPFCCR  = PixelFormat;
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	/* �������� */
	DMA2D->CR   |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START) {}
}

