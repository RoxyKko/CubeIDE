/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_tft_lcd.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TFT_LCD_H
#define _BSP_TFT_LCD_H

#define BUTTON_BEEP()	BEEP_KeyTone();	/* 按键提示音 */
//#define BUTTON_BEEP()	/* 无按键提示音 */

/* 定义LCD显示区域的分辨率 */
#define LCD_30_HEIGHT	240		/* 3.0寸宽屏 高度，单位：像素 */
#define LCD_30_WIDTH	400		/* 3.0寸宽屏 宽度，单位：像素 */

#define LCD_43_HEIGHT	272		/* 4.3寸宽屏 高度，单位：像素 */
#define LCD_43_WIDTH	480		/* 4.3寸宽屏 宽度，单位：像素 */

#define LCD_70_HEIGHT	480		/* 7.0寸宽屏 高度，单位：像素 */
#define LCD_70_WIDTH	800		/* 7.0寸宽屏 宽度，单位：像素 */

/* 支持的驱动芯片ID */
enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075,	
	IC_9488 	= 0x9488
};

#define CHIP_STR_5420	"SPFD5420A"
#define CHIP_STR_4001	"OTM4001A"
#define CHIP_STR_61509	"R61509V"
#define CHIP_STR_8875	"RA8875"
#define CHIP_STR_9488	"ILI9488"

#define CHIP_STR_F429	"STM32F429"

/*
	LCD 颜色代码，CL_是Color的简写
	16Bit由高位至低位， RRRR RGGG GGGB BBBB

	下面的RGB 宏将24位的RGB值转换为16位格式。
	启动windows的画笔程序，点击编辑颜色，选择自定义颜色，可以获得的RGB值。

	推荐使用迷你取色器软件获得你看到的界面颜色。
*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* 将8位R,G,B转化为 16位RGB565格式 */

/* 解码出 R=8bit G=8bit B=8bit */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

/* 解码出 R=5bit G=6bit B=5bit */
#define RGB565_R2(x)  ((x >> 11) & 0x1F)
#define RGB565_G2(x)  ((x >> 5) & 0x3F)
#define RGB565_B2(x)  ((x >> 0) & 0x1F)


enum
{
	CL_WHITE        = RGB(255,255,255),	/* 白色 */
	CL_BLACK        = RGB(  0,  0,  0),	/* 黑色 */
	CL_RED          = RGB(255,	0,  0),	/* 红色 */
	CL_GREEN        = RGB(  0,255,  0),	/* 绿色 */
	CL_BLUE         = RGB(  0,	0,255),	/* 蓝色 */
	CL_YELLOW       = RGB(255,255,  0),	/* 黄色 */

	CL_GREY			= RGB( 98, 98, 98), 	/* 深灰色 */
	CL_GREY1		= RGB( 150, 150, 150), 	/* 浅灰色 */
	CL_GREY2		= RGB( 180, 180, 180), 	/* 浅灰色 */
	CL_GREY3		= RGB( 200, 200, 200), 	/* 最浅灰色 */
	CL_GREY4		= RGB( 230, 230, 230), 	/* 最浅灰色 */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS 按钮表面灰色 */

	CL_MAGENTA      = 0xF81F,	/* 红紫色，洋红色 */
	CL_CYAN         = 0x7FFF,	/* 蓝绿色，青色 */

	CL_BLUE1        = RGB(  0,  0, 240),		/* 深蓝色 */
	CL_BLUE2        = RGB(  0,  0, 128),		/* 深蓝色 */
	CL_BLUE3        = RGB(  68, 68, 255),		/* 浅蓝色1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* 浅蓝色1 */

	/* UI 界面 Windows控件常用色 */
	CL_BTN_FACE		= RGB(236, 233, 216),	/* 按钮表面颜色(灰) */
	
	CL_BTN_FONT		= CL_BLACK,				/* 按钮字体颜色（黑） */
	
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* 分组框主线颜色 */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* 分组框阴影线颜色 */


	/* Roxy的调色板 */



	
	CL_MASK			= 0x9999	/* 颜色掩码，用于文字背景透明 */
};

/* 文字对齐方式 */
enum
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

/* 编辑框风格 */
enum
{
	EDIT_BORDER_COLOR		= CL_BLUE2,		/* 编辑框四个边的颜色 */
	EDIT_BACK_COLOR			= CL_WHITE,			/* 编辑框背景 */
};

/* 按钮风格 */
enum
{
	BUTTON_BORDER_COLOR		= CL_BLUE2,			/* 按钮四个边的颜色 */
	BUTTON_BORDER1_COLOR	= CL_WHITE,			/* 按钮四个边内部2列的颜色 */
	BUTTON_BORDER2_COLOR	= CL_GREY1,			/* 按钮四个边内部2列的颜色 */
	BUTTON_BACK_COLOR		= CL_GREY3,			/* 按钮背景 */
	BUTTON_ACTIVE_COLOR		= CL_CYAN,			/* 激活的按钮背景 */
};

/* 窗口风格 */
enum
{
	WIN_BORDER_COLOR	= CL_BLUE4,		/* 窗口边框 */
	WIN_TITLE_COLOR		= CL_BLUE3,		/* 窗口标题蓝背景颜色 */
	WIN_CAPTION_COLOR	= CL_WHITE,		/* 窗口标题栏文字颜色 */
	WIN_BODY_COLOR		= CL_GREY2,		/* 窗体颜色 */
};

/* 检查框风格 */
enum
{
	CHECK_BOX_BORDER_COLOR	= CL_BLUE2,		/* 检查框四个边的颜色 */
	CHECK_BOX_BACK_COLOR	= CL_GREY3,		/* 检查框背景 */
	CHECK_BOX_CHECKED_COLOR	= CL_RED,		/* 检查框打勾的颜色 */

	CHECK_BOX_H			= 24,				/* 检查框高度 */
	CHECK_BOX_W			= 24,				/* 检查框高度 */
};

/* 字体代码 */
typedef enum
{
	FC_ST_12 = 0,		/* 宋体12x12点阵 （宽x高） */
	FC_ST_16,			/* 宋体15x16点阵 （宽x高） */
	FC_ST_24,			/* 宋体24x24点阵 （宽x高） -- 暂时未支持 */
	FC_ST_32,			/* 宋体32x32点阵 （宽x高） -- 暂时未支持 */
	
	FC_RA8875_16,		/* RA8875 内置字体 16点阵 */
	FC_RA8875_24,		/* RA8875 内置字体 24点阵 */
	FC_RA8875_32		/* RA8875 内置字体 32点阵 */
}FONT_CODE_E;

/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	FONT_CODE_E FontCode;	/* 字体代码 FONT_CODE_E  */
	uint16_t FrontColor;/* 字体颜色 */
	uint16_t BackColor;	/* 文字背景颜色，透明 */
	uint16_t Space;		/* 文字间距，单位 = 像素 */
}FONT_T;

/* 控件ID */
typedef enum
{
	ID_ICON		= 1,
	ID_WIN		= 2,
	ID_LABEL	= 3,
	ID_BUTTON	= 4,
	ID_CHECK 	= 5,
	ID_EDIT 	= 6,
	ID_GROUP 	= 7,
}CONTROL_ID_T;

/* 图标结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;		/* 左上角X坐标 */
	uint16_t Top;		/* 左上角Y坐标 */
	uint16_t Height;	/* 图标高度 */
	uint16_t Width;		/* 图标宽度 */
	uint16_t *pBmp;		/* 指向图标图片数据 */
	char  Text[16];	/* 图标文本, 最多显示5个汉字16点阵 */
}ICON_T;

/* 窗体结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;
	char *pCaption;
}WIN_T;

/* 文本标签结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t MaxLen;		/* 字符串长度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}LABEL_T;

/* 按钮结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	/* 按钮的颜色，由底层自动管理 */
	FONT_T *Font;			/* 字体 */
	char *pCaption;
	uint8_t Focus;			/* 焦点 */
}BUTTON_T;

/* 编辑框结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;			/* 字体 */
	char   *pCaption;
	char Text[32];			/* 用于存放编辑内容 */
}EDIT_T;

/* 检查框 CHECK BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t Color;			/* 颜色 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
	uint8_t Checked;		/* 1表示打勾 */
}CHECK_T;

/* 分组框GROUP BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}GROUP_T;

/* 背景光控制 */
#define BRIGHT_MAX		255
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5

/* 可供外部模块调用的函数 */
/* 初始化LCD */
void LCD_InitHard(void);

/* 读取LCD驱动芯片的描述符号，用于显示 */
void LCD_GetChipDescribe(char *_str);

/* 读取LCD分辨率之高度 */
uint16_t LCD_GetHeight(void);

/* 读取LCD分辨率之宽度 */
uint16_t LCD_GetWidth(void);

/* 打开显示 */
void LCD_DispOn(void);

/* 关闭显示 */
void LCD_DispOff(void);

/* 根据输入的颜色值清屏 */
void LCD_ClrScr(uint16_t _usColor);

/* 在LCD指定坐标（左上角）显示一个字符串 */
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);

/* 读取字体的宽度（像素单位) */
uint16_t LCD_GetFontWidth(FONT_T *_tFont);

/* 读取字体的高度（像素单位) */
uint16_t LCD_GetFontHeight(FONT_T *_tFont);

/* 计算字符串宽度(像素单位) */
uint16_t LCD_GetStrWidth(char *_ptr, FONT_T *_tFont);

/* 在LCD指定坐标（左上角）显示一个字符串。 增强型函数。支持左\中\右对齐，支持定长清屏。 */
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont, uint16_t _Width,
        uint8_t _Align);

/* 画1个像素 */
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);

/* 读取1个像素 */
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY);

/* 采用 Bresenham 算法，在2点间画一条直线。 */
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);

/* 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。 */
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);

/* 绘制水平放置的矩形。 */
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* 用一个颜色值填充一个矩形。【emWin 中有同名函数 LCD_FillRect，因此加了下划线区分】 */
void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

/* 绘制一个圆，笔宽为1个像素 */
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);

/* 在LCD上显示一个BMP位图，位图点阵扫描次序: 从左到右，从上到下 */
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);

/* 在LCD上绘制一个窗口 */
void LCD_DrawWin(WIN_T *_pWin);

/* 在LCD上绘制一个图标，四角自动切为弧脚 */
void LCD_DrawIcon(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);

/* 对像素透明化 颜色混合 */
uint16_t LCD_Blend565(uint16_t src, uint16_t dst, uint8_t alpha);

/* 在LCD上绘制一个图标, 带有透明信息的位图(32位， RGBA). 图标下带文字 */
void LCD_DrawIcon32(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);

/* 在LCD上绘制一个32位的BMP图, 带有透明信息的位图(32位， RGBA) */
void LCD_DrawBmp32(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_pBmp);

/* 绘制一个文本标签 */
void LCD_DrawLabel(LABEL_T *_pLabel);

/* 绘制一个检查框 */
void LCD_DrawCheckBox(CHECK_T *_pCheckBox);

/* 在LCD上绘制一个编辑框 */
void LCD_DrawEdit(EDIT_T *_pEdit);

/* 在LCD上绘制一个按钮 */
void LCD_DrawButton(BUTTON_T *_pBtn);

/* 在LCD上绘制一个分组框 */
void LCD_DrawGroupBox(GROUP_T *_pBox);

/* 绘制控件 */
void LCD_DispControl(void *_pControl);

/* 设置显示屏显示方向（横屏 竖屏） */
void LCD_SetDirection(uint8_t _dir);

/* 判断按钮是否被按下. 检查触摸坐标是否在按钮的范围之内。并重绘按钮。 */
uint8_t LCD_ButtonTouchDown(BUTTON_T *_btn, uint16_t _usX, uint16_t _usY);

/* 判断按钮是否被触摸释放. 并重绘按钮。在触摸释放事件中被调用。 */
uint8_t LCD_ButtonTouchRelease(BUTTON_T *_btn, uint16_t _usX, uint16_t _usY);

/* 初始化按钮结构体成员。 */
void LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, char *_pCaption, FONT_T *_pFont);

/* 初始化字体结构体成员。 */
void LCD_InitFonts(FONT_T *_pFont ,FONT_CODE_E _FontCode, uint16_t _fontCol, uint16_t _backCol, uint16_t _space);

/*初始化icon结构体成员。*/
void LCD_InitIcon(ICON_T *_pIcon, uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, uint16_t *_pBmp, char _text);

/*初始化窗口结构体成员。*/
void LCD_InitWin(WIN_T *_pWin, uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, uint16_t _col, FONT_T *_pFont ,char *_Cap);

/*初始化标签结构体成员。*/
void LCD_InitLabel(LABEL_T *_pLabel, uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, uint16_t _maxLen, FONT_T *_pFont ,char *_Cap);

/*初始化编辑框结构体成员。*/
void LCD_InitEdit(EDIT_T *_pEdit, uint8_t _id, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, uint16_t _col, FONT_T *_pFont ,char *_Cap);

/*控制背光*/
void LCD_SetBackLight(uint8_t _bright);

/*控制背光*/
uint8_t LCD_GetBackLight(void);

/* 下面3个变量，主要用于使程序同时支持不同的屏 */
extern uint16_t g_LcdHeight;		/* 显示屏分辨率-高度 */
extern uint16_t g_LcdWidth;			/* 显示屏分辨率-宽度 */
extern uint8_t g_LcdDirection;		/* 显示方向.0，1，2，3 */

#endif


