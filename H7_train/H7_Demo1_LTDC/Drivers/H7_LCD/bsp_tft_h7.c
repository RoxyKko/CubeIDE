/*
*********************************************************************************************************
*
*	ģ������ : STM32H7�ڲ�LCD��������
*	�ļ����� : bsp_tft_h7.c
*	��    �� : V1.0
*	˵    �� : STM32F429 �ڲ�LCD�ӿڵ�Ӳ�����ó���
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		V1.0    2014-05-05 armfly ���� STM32F429 �ڲ�LCD�ӿڣ� ����ST�����Ӹ��ģ���Ҫ�������ǰ���㶨�壬ֱ��
*							      �� LTDC_Layer1 �� LTDC_Layer2, ����2���ṹ��ָ��
*		V1.1	2015-11-19 armfly
*						1. ��ͼ�����滻ΪDMA2DӲ����������߻�ͼЧ��
*						2. ͳһ�����������ú������Զ�ʶ���������
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "fonts.h"

typedef uint32_t LCD_COLOR;

/* ƫ�Ƶ�ַ���㹫ʽ:
   Maximum width x Maximum Length x Maximum Pixel size (RGB565)����λ�ֽ�
   => 800 x 480 x 2 =  768000 */
#define BUFFER_OFFSET SDRAM_LCD_SIZE // (uint32_t)(g_LcdHeight * g_LcdWidth * 2)

/* �궨�� */
#define LCDH7_FRAME_BUFFER SDRAM_LCD_BUF1

/* ���� */
extern LTDC_HandleTypeDef hltdc;
extern DMA2D_HandleTypeDef hdma2d;
uint32_t s_CurrentFrameBuffer;
uint8_t s_CurrentLayer;

/* ���� */
static void LCDH7_ConfigLTDC(void);

void LCDH7_LayerInit(void);
void LCDH7_SetPixelFormat(uint32_t PixelFormat);

static void LCDH7_InitDMA2D(void);
static inline uint32_t LCD_LL_GetPixelformat(uint32_t LayerIndex);
void DMA2D_CopyBuffer(uint32_t LayerIndex, void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);
static void DMA2D_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_InitHard
*	����˵��: ��ʼ��LCD
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_InitHard(void)
{
	LCDH7_ConfigLTDC();			 /* ����429 CPU�ڲ�LTDC */
	LCDH7_InitDMA2D();			 /* ʹ��DMA2D */
	LCDH7_SetLayer(LCD_LAYER_1); /* ʹ�õ�ͼ��1 */
	LCDH7_QuitWinMode();
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_ConfigLTDC
*	����˵��: ����LTDC
*	��    ��: ��
*	�� �� ֵ: ��
*   ��    ��:
*       LCD_TFT ͬ��ʱ�����ã������Թٷ�����һ����ͼ���Լ����ࣩ��
*       ----------------------------------------------------------------------------
*
*                                                 Total Width
*                             <--------------------------------------------------->
*                       Hsync width HBP             Active Width                HFP
*                             <---><--><--------------------------------------><-->
*                         ____    ____|_______________________________________|____
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |Width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  VBP |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  VFP |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*
*
*     ÿ��LCD�豸�����Լ���ͬ��ʱ��ֵ��
*     Horizontal Synchronization (Hsync)
*     Horizontal Back Porch (HBP)
*     Active Width
*     Horizontal Front Porch (HFP)
*
*     Vertical Synchronization (Vsync)
*     Vertical Back Porch (VBP)
*     Active Heigh
*     Vertical Front Porch (VFP)
*
*     LCD_TFT ����ˮƽ�ʹ�ֱ����ʼ�Լ�����λ�� :
*     ----------------------------------------------------------------
*
*     HorizontalStart = (Offset_X + Hsync + HBP);
*     HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1);
*     VarticalStart   = (Offset_Y + Vsync + VBP);
*     VerticalStop    = (Offset_Y + Vsync + VBP + Window_Heigh - 1);
*
*********************************************************************************************************
*/
static void LCDH7_ConfigLTDC(void)
{
	/* ����LCD��ص�GPIO */
	{
		/* GPIOs Configuration */
		/**LTDC GPIO Configuration
		PI9     ------> LTDC_VSYNC
		PI10     ------> LTDC_HSYNC
		PF10     ------> LTDC_DE
		PH9     ------> LTDC_R3
		PH10     ------> LTDC_R4
		PH11     ------> LTDC_R5
		PH12     ------> LTDC_R6
		PG6     ------> LTDC_R7
		PG7     ------> LTDC_CLK
		PH13     ------> LTDC_G2
		PH14     ------> LTDC_G3
		PH15     ------> LTDC_G4
		PI0     ------> LTDC_G5
		PI1     ------> LTDC_G6
		PI2     ------> LTDC_G7
		PG11     ------> LTDC_B3
		PI4     ------> LTDC_B4
		PI5     ------> LTDC_B5
		PI6     ------> LTDC_B6
		PI7     ------> LTDC_B7
		*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* ʹ��LTDCʱ�� */
		__HAL_RCC_LTDC_CLK_ENABLE();
		/* ʹ��GPIOʱ�� */
		__HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();

		/* GPIOI ���� */
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

		/* GPIOF ���� */
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		/* GPIOH ���� */
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		/* GPIOG ���� */
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	}

	{
		LTDC_LayerCfgTypeDef pLayerCfg = {0};
		/* �����źż��� */
		hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL; /* HSYNC �͵�ƽ��Ч */
		hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; /* VSYNC �͵�ƽ��Ч */
		hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL; /* DE �͵�ƽ��Ч */
		hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

		/* ʱ������ */
		hltdc.Init.HorizontalSync = 19;
		hltdc.Init.VerticalSync = 2;
		hltdc.Init.AccumulatedHBP = 1426;
		hltdc.Init.AccumulatedVBP = 22;
		hltdc.Init.AccumulatedActiveH = 622;
		hltdc.Init.AccumulatedActiveW = 2450;
		hltdc.Init.TotalHeigh = 644;
		hltdc.Init.TotalWidth = 2610;

		/* ���ñ�������ɫ */
		hltdc.Init.Backcolor.Blue = 0;
		hltdc.Init.Backcolor.Green = 0;
		hltdc.Init.Backcolor.Red = 0;

		hltdc.Instance = LTDC;

		/* ��ʼ����ͼ�� ------------------------------------------------------*/
		/* ������ʾ������ */
		pLayerCfg.WindowX0 = 0;
 		pLayerCfg.WindowX1 = 1024;
  		pLayerCfg.WindowY0 = 0;
  		pLayerCfg.WindowY1 = 600;

		/* ������ɫ��ʽ */
		pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

		/* �Դ��ַ */
		pLayerCfg.FBStartAdress = LCDH7_FRAME_BUFFER;

		/* Alpha���� (255 ��ʾ��ȫ��͸��) */
		pLayerCfg.Alpha = 255;

		/* �ޱ���ɫ */
		pLayerCfg.Alpha0 = 0; /* ��ȫ͸�� */
		pLayerCfg.Backcolor.Blue = 0;
		pLayerCfg.Backcolor.Green = 0;
		pLayerCfg.Backcolor.Red = 0;

		/* ����ͼ�������� */
		pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR1_PAxCA;

		/* �������д�С */
		pLayerCfg.ImageWidth = 1024;
		pLayerCfg.ImageHeight = 600;

		/* ����LTDC  */
		if (HAL_LTDC_Init(&hltdc) != HAL_OK)
		{
			/* ��ʼ������ */
			Error_Handler();
		}

		/* ����ͼ��1 */
		if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LTDC_LAYER_1) != HAL_OK)
		{
			/* ��ʼ������ */
			Error_Handler();
		}

		/* ����ͼ��2 */
		if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LTDC_LAYER_2) != HAL_OK)
		{
			/* ��ʼ������ */
			Error_Handler();
		}

	}

#if 1
	HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_GetChipDescribe
*	����˵��: ��ȡLCD����оƬ���������ţ�������ʾ
*	��    ��: char *_str : �������ַ�������˻�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_GetChipDescribe(char *_str)
{
	strcpy(_str, "STM32H743");
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_SetLayer
*	����˵��: �л��㡣ֻ�Ǹ��ĳ���������Ա��ں���Ĵ��������ؼĴ�����Ӳ��֧��2�㡣
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_SetLayer(uint8_t _ucLayer)
{
	if (_ucLayer == LCD_LAYER_1)
	{
		s_CurrentFrameBuffer = LCDH7_FRAME_BUFFER;
		s_CurrentLayer = LCD_LAYER_1;
	}
	else if (_ucLayer == LCD_LAYER_2)
	{
		s_CurrentFrameBuffer = LCDH7_FRAME_BUFFER + BUFFER_OFFSET;
		s_CurrentLayer = LCD_LAYER_2;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_SetTransparency
*	����˵��: ���õ�ǰ���͸������
*	��    ��: ͸���ȣ� ֵ�� 0x00 - 0xFF
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_SetTransparency(uint8_t transparency)
{
	HAL_LTDC_SetAlpha(&hltdc, transparency, s_CurrentLayer); /* ����ˢ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_SetPixelFormat
*	����˵��: ���õ�ǰ������ظ�ʽ
*	��    ��: ���ظ�ʽ:
*                      LTDC_PIXEL_FORMAT_ARGB8888
*                      LTDC_PIXEL_FORMAT_ARGB8888_RGB888
*                      LTDC_PIXEL_FORMAT_ARGB8888_RGB565
*                      LTDC_PIXEL_FORMAT_ARGB8888_ARGB1555
*                      LTDC_PIXEL_FORMAT_ARGB8888_ARGB4444
*                      LTDC_PIXEL_FORMAT_ARGB8888_L8
*                      LTDC_PIXEL_FORMAT_ARGB8888_AL44
*                      LTDC_PIXEL_FORMAT_ARGB8888_AL88
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_SetPixelFormat(uint32_t PixelFormat)
{
	HAL_LTDC_SetPixelFormat(&hltdc, PixelFormat, s_CurrentLayer);
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_SetDispWin
*	����˵��: ������ʾ���ڣ����봰����ʾģʽ��
*	��    ��:
*		_usX : ˮƽ����
*		_usY : ��ֱ����
*		_usHeight: ���ڸ߶�
*		_usWidth : ���ڿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{
	HAL_LTDC_SetWindowSize_NoReload(&hltdc, _usWidth, _usHeight, s_CurrentLayer); /* ���������� */
	HAL_LTDC_SetWindowPosition(&hltdc, _usX, _usY, s_CurrentLayer);				  /* �������� */
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_QuitWinMode
*	����˵��: �˳�������ʾģʽ����Ϊȫ����ʾģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_QuitWinMode(void)
{
	HAL_LTDC_SetWindowSize_NoReload(&hltdc, g_LcdWidth, g_LcdHeight, s_CurrentLayer); /* ���������� */
	HAL_LTDC_SetWindowPosition(&hltdc, 0, 0, s_CurrentLayer);						  /* �������� */
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DispOn
*	����˵��: ����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DispOn(void)
{
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DispOff
*	����˵��: �ر���ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DispOff(void)
{
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_ClrScr
*	����˵��: �����������ɫֵ����
*	��    ��: _usColor : ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_ClrScr(uint16_t _usColor)
{
	LCDH7_FillRect(0, 0, g_LcdHeight, g_LcdWidth, _usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  : ������ɫ ( RGB = 565 ��ʽ)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	uint16_t *p;
	uint32_t index = 0;

	p = (uint16_t *)s_CurrentFrameBuffer;

	if (g_LcdDirection == 0) /* ���� */
	{
		index = (uint32_t)_usY * g_LcdWidth + _usX;
	}
	else if (g_LcdDirection == 1) /* ����180��*/
	{
		index = (uint32_t)(g_LcdHeight - _usY - 1) * g_LcdWidth + (g_LcdWidth - _usX - 1);
	}
	else if (g_LcdDirection == 2) /* ���� */
	{
		index = (uint32_t)(g_LcdWidth - _usX - 1) * g_LcdHeight + _usY;
	}
	else if (g_LcdDirection == 3) /* ����180�� */
	{
		index = (uint32_t)_usX * g_LcdHeight + g_LcdHeight - _usY - 1;
	}

	if (index < g_LcdHeight * g_LcdWidth)
	{
		p[index] = _usColor;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  : ������ɫ
*	�� �� ֵ: RGB��ɫֵ
*********************************************************************************************************
*/
uint16_t LCDH7_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;
	uint16_t *p;
	uint32_t index = 0;

	p = (uint16_t *)s_CurrentFrameBuffer;

	if (g_LcdDirection == 0) /* ���� */
	{
		index = (uint32_t)_usY * g_LcdWidth + _usX;
	}
	else if (g_LcdDirection == 1) /* ����180��*/
	{
		index = (uint32_t)(g_LcdHeight - _usY - 1) * g_LcdWidth + (g_LcdWidth - _usX - 1);
	}
	else if (g_LcdDirection == 2) /* ���� */
	{
		index = (uint32_t)(g_LcdWidth - _usX - 1) * g_LcdHeight + _usY;
	}
	else if (g_LcdDirection == 3) /* ����180�� */
	{
		index = (uint32_t)_usX * g_LcdHeight + g_LcdHeight - _usY - 1;
	}

	usRGB = p[index];

	return usRGB;
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawLine
*	����˵��: ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 : ��ʼ������
*			_usX2, _usY2 : ��ֹ��Y����
*			_usColor     : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawLine(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2, uint16_t _usColor)
{
	int32_t dx, dy;
	int32_t tx, ty;
	int32_t inc1, inc2;
	int32_t d, iTag;
	int32_t x, y;

	/* ���� Bresenham �㷨����2��仭һ��ֱ�� */

	LCDH7_PutPixel(_usX1, _usY1, _usColor);

	/* ��������غϣ���������Ķ�����*/
	if (_usX1 == _usX2 && _usY1 == _usY2)
	{
		return;
	}

	iTag = 0;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if (dx < dy) /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1;
		temp = _usX1;
		_usX1 = _usY1;
		_usY1 = temp;
		temp = _usX2;
		_usX2 = _usY2;
		_usY2 = temp;
		temp = dx;
		dx = dy;
		dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1; /* ȷ������1���Ǽ�1 */
	ty = _usY2 > _usY1 ? 1 : -1;
	x = _usX1;
	y = _usY1;
	inc1 = 2 * dy;
	inc2 = 2 * (dy - dx);
	d = inc1 - dx;
	while (x != _usX2) /* ѭ������ */
	{
		if (d < 0)
		{
			d += inc1;
		}
		else
		{
			y += ty;
			d += inc2;
		}
		if (iTag)
		{
			LCDH7_PutPixel(y, x, _usColor);
		}
		else
		{
			LCDH7_PutPixel(x, y, _usColor);
		}
		x += tx;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawHLine
*	����˵��: ����һ��ˮƽ��. ʹ��STM32F429 DMA2DӲ������.
*	��    ��:
*			_usX1, _usY1 : ��ʼ������
*			_usLen       : �ߵĳ���
*			_usColor     : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawHLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor)
{
#if 0
	LCDH7_FillRect(_usX, _usY, 1, _usLen, _usColor);
#else
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		LCDH7_PutPixel(_usX + i, _usY, _usColor);
	}
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawVLine
*	����˵��: ����һ����ֱ�ߡ� ʹ��STM32F429 DMA2DӲ������.
*	��    ��:
*			_usX, _usY : ��ʼ������
*			_usLen       : �ߵĳ���
*			_usColor     : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawVLine(uint16_t _usX, uint16_t _usY, uint16_t _usLen, uint16_t _usColor)
{
#if 0
	LCDH7_FillRect(_usX, _usY, _usLen, 1, _usColor);
#else
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		LCDH7_PutPixel(_usX, _usY + i, _usColor);
	}
#endif
}
/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawPoints
*	����˵��: ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    ��:
*			x, y     : ��������
*			_usColor : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor)
{
	uint16_t i;

	for (i = 0; i < _usSize - 1; i++)
	{
		LCDH7_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _usColor);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawRect
*	����˵��: ����ˮƽ���õľ��Ρ�
*	��    ��:
*			_usX,_usY: �������Ͻǵ�����
*			_usHeight : ���εĸ߶�
*			_usWidth  : ���εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/
	LCDH7_DrawHLine(_usX, _usY, _usWidth, _usColor);
	LCDH7_DrawVLine(_usX + _usWidth - 1, _usY, _usHeight, _usColor);
	LCDH7_DrawHLine(_usX, _usY + _usHeight - 1, _usWidth, _usColor);
	LCDH7_DrawVLine(_usX, _usY, _usHeight, _usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_FillRect
*	����˵��: ��һ����ɫֵ���һ�����Ρ�ʹ��STM32F429�ڲ�DMA2DӲ�����ơ�
*	��    ��:
*			_usX,_usY: �������Ͻǵ�����
*			_usHeight : ���εĸ߶�
*			_usWidth  : ���εĿ��
*			_usColor  : ��ɫ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/* ʹ��DMA2DӲ�������� */
	uint32_t Xaddress = 0;
	uint16_t OutputOffset;
	uint16_t NumberOfLine;
	uint16_t PixelPerLine;

	if (g_LcdDirection == 0) /* ���� */
	{
		Xaddress = s_CurrentFrameBuffer + 2 * (g_LcdWidth * _usY + _usX);
		OutputOffset = g_LcdWidth - _usWidth;
		NumberOfLine = _usHeight;
		PixelPerLine = _usWidth;
	}
	else if (g_LcdDirection == 1) /* ����180��*/
	{
		Xaddress = s_CurrentFrameBuffer + 2 * (g_LcdWidth * (g_LcdHeight - _usHeight - _usY) + g_LcdWidth - _usX - _usWidth);
		OutputOffset = g_LcdWidth - _usWidth;
		NumberOfLine = _usHeight;
		PixelPerLine = _usWidth;
	}
	else if (g_LcdDirection == 2) /* ���� */
	{
		Xaddress = s_CurrentFrameBuffer + 2 * (g_LcdHeight * (g_LcdWidth - _usX - _usWidth) + _usY);
		OutputOffset = g_LcdHeight - _usHeight;
		NumberOfLine = _usWidth;
		PixelPerLine = _usHeight;
	}
	else if (g_LcdDirection == 3) /* ����180�� */
	{
		Xaddress = s_CurrentFrameBuffer + 2 * (g_LcdHeight * _usX + g_LcdHeight - _usHeight - _usY);
		OutputOffset = g_LcdHeight - _usHeight;
		NumberOfLine = _usWidth;
		PixelPerLine = _usHeight;
	}

	// DMA2D_FillBuffer(uint32_t LayerIndex, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
	DMA2D_FillBuffer(s_CurrentLayer, (void *)Xaddress, PixelPerLine, NumberOfLine, OutputOffset, _usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			_usX,_usY  : Բ�ĵ�����
*			_usRadius  : Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	int32_t D;	   /* Decision Variable */
	uint32_t CurX; /* ��ǰ X ֵ */
	uint32_t CurY; /* ��ǰ Y ֵ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		LCDH7_PutPixel(_usX + CurX, _usY + CurY, _usColor);
		LCDH7_PutPixel(_usX + CurX, _usY - CurY, _usColor);
		LCDH7_PutPixel(_usX - CurX, _usY + CurY, _usColor);
		LCDH7_PutPixel(_usX - CurX, _usY - CurY, _usColor);
		LCDH7_PutPixel(_usX + CurY, _usY + CurX, _usColor);
		LCDH7_PutPixel(_usX + CurY, _usY - CurX, _usColor);
		LCDH7_PutPixel(_usX - CurY, _usY + CurX, _usColor);
		LCDH7_PutPixel(_usX - CurY, _usY - CurX, _usColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ��ͼƬ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint16_t i, k, y;
	const uint16_t *p;

	p = _ptr;
	y = _usY;
	for (i = 0; i < _usHeight; i++)
	{
		for (k = 0; k < _usWidth; k++)
		{
			LCDH7_PutPixel(_usX + k, y, *p++);
		}

		y++;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_SetDirection
*	����˵��: ������ʾ����ʾ���򣨺��� ������
*	��    ��: ��ʾ������� 0 ��������, 1=����180�ȷ�ת, 2=����, 3=����180�ȷ�ת
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_SetDirection(uint8_t _dir)
{
	uint16_t temp;

	if (_dir == 0 || _dir == 1) /* ������ ����180�� */
	{
		if (g_LcdWidth < g_LcdHeight)
		{
			temp = g_LcdWidth;
			g_LcdWidth = g_LcdHeight;
			g_LcdHeight = temp;
		}
	}
	else if (_dir == 2 || _dir == 3) /* ����, ����180��*/
	{
		if (g_LcdWidth > g_LcdHeight)
		{
			temp = g_LcdWidth;
			g_LcdWidth = g_LcdHeight;
			g_LcdHeight = temp;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_BeginDraw
*	����˵��: ˫����������ģʽ����ʼ��ͼ������ǰ��ʾ�������������������Ƶ�����һ����������
*			 ����� LCDH7_EndDraw�����ɶ�ʹ�á� ʵ��Ч�������á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_BeginDraw(void)
{
	//	uint16_t *src;
	//	uint16_t *dst;
	//
	//	if (s_CurrentFrameBuffer == LCDH7_FRAME_BUFFER)
	//	{
	//		src = (uint16_t *)LCDH7_FRAME_BUFFER;
	//		dst =  (uint16_t *)(LCDH7_FRAME_BUFFER + BUFFER_OFFSET);
	//
	//		s_CurrentFrameBuffer = LCDH7_FRAME_BUFFER + BUFFER_OFFSET;
	//	}
	//	else
	//	{
	//		src = (uint16_t *)(LCDH7_FRAME_BUFFER + BUFFER_OFFSET);
	//		dst = (uint16_t *)LCDH7_FRAME_BUFFER;
	//
	//		s_CurrentFrameBuffer = LCDH7_FRAME_BUFFER;
	//	}
	//
	//	_DMA_Copy(src, dst, g_LcdHeight, g_LcdWidth, 0, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_EndDraw
*	����˵��: APP�����˻�������ͼ�������л�Ӳ����ʾ��
*			 ����� LCDH7_BeginDraw�����ɶ�ʹ�á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCDH7_EndDraw(void)
{
	// LTDC_LayerAddress(LTDC_Layer1, s_CurrentFrameBuffer);
}

/*
*********************************************************************************************************
*	�� �� ��: _GetBufferSize
*	����˵��: ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint32_t _GetBufferSize(uint8_t LayerIndex)
{
	return g_LcdWidth * g_LcdHeight;
}

/*
*********************************************************************************************************
*	�� �� ��: LCDH7_InitDMA2D
*	����˵��: ����DMA2D
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCDH7_InitDMA2D(void)
{
	/* ʹ��DMA2Dʱ�� */
	__HAL_RCC_DMA2D_CLK_ENABLE();

	/* ����Ĭ��ģʽ */
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.ColorMode = DMA2D_INPUT_RGB565;
	hdma2d.Init.OutputOffset = 0x0;

	hdma2d.Instance = DMA2D;

	if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_LL_GetPixelformat
*	����˵��: ��ȡͼ�����ɫ��ʽ
*	��    ��: ��
*	�� �� ֵ: ������Ӧͼ�����ɫ��ʽ
*********************************************************************************************************
*/
static inline uint32_t LCD_LL_GetPixelformat(uint32_t LayerIndex)
{
	if (LayerIndex == 0)
	{
		return LTDC_PIXEL_FORMAT_RGB565;
	}
	else
	{
		return LTDC_PIXEL_FORMAT_RGB565; // LTDC_PIXEL_FORMAT_ARGB1555;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DMA2D_CopyBuffer
*	����˵��: ͨ��DMA2D��ǰ���㸴��ָ���������ɫ���ݵ�Ŀ������
*	��    ��: LayerIndex    ͼ��
*             pSrc          ��ɫ����Դ��ַ
*             pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         Ҫ���������X���С����ÿ��������
*             ySize         Ҫ���������Y���С��������
*             OffLineSrc    ǰ����ͼ�����ƫ��
*             OffLineDst    �������ƫ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DMA2D_CopyBuffer(uint32_t LayerIndex, void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst)
{
	uint32_t PixelFormat;

	PixelFormat = LCD_LL_GetPixelformat(LayerIndex);
	DMA2D->CR = 0x00000000UL | (1 << 9);

	/* ���û������� */
	DMA2D->FGMAR = (uint32_t)pSrc;
	DMA2D->OMAR = (uint32_t)pDst;
	DMA2D->FGOR = OffLineSrc;
	DMA2D->OOR = OffLineDst;

	/* ������ɫ��ʽ */
	DMA2D->FGPFCCR = PixelFormat;

	/*  ���ô����С */
	DMA2D->NLR = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	DMA2D->CR |= DMA2D_CR_START;

	/* �ȴ�������� */
	while (DMA2D->CR & DMA2D_CR_START)
	{
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _LCD_DrawBitmap16bpp
*	����˵��: 16bppλͼ���ƣ�ר��������ͷ
*	��    ��: --
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _LCD_DrawCamera16bpp(int x, int y, uint16_t *p, int xSize, int ySize, int SrcOffLine)
{
	uint32_t AddrDst;
	int OffLineSrc, OffLineDst;

	AddrDst = s_CurrentFrameBuffer + (y * g_LcdWidth + x) * 2;
	OffLineSrc = SrcOffLine;		 /* Դͼ�ε�ƫ�ƣ�������ȫ���������� */
	OffLineDst = g_LcdWidth - xSize; /* Ŀ��ͼ�εı��� */
	DMA2D_CopyBuffer(LCD_LAYER_1, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_Fill
*	����˵��: ͨ��DMA2D����ָ�����������ɫ���
*	��    ��: LayerIndex    ͼ��
*             pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         Ҫ���������X���С����ÿ��������
*             ySize         Ҫ���������Y���С��������
*             OffLine       ǰ����ͼ�����ƫ��
*             ColorIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
	uint32_t PixelFormat;

	PixelFormat = LCD_LL_GetPixelformat(LayerIndex);

	/* ��ɫ��� */
	DMA2D->CR = 0x00030000UL | (1 << 9);
	DMA2D->OCOLR = ColorIndex;

	/* ����������ɫĿ�ĵ�ַ */
	DMA2D->OMAR = (uint32_t)pDst;

	/* Ŀ����ƫ�Ƶ�ַ */
	DMA2D->OOR = OffLine;

	/* ������ɫ��ʽ */
	DMA2D->OPFCCR = PixelFormat;

	/* ��������С */
	DMA2D->NLR = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	DMA2D->CR |= DMA2D_CR_START;

	/* �ȴ�DMA2D������� */
	while (DMA2D->CR & DMA2D_CR_START)
	{
	}
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
