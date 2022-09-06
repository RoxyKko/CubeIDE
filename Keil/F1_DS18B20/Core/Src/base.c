#include "base.h"

static u8 fac_us = 0; //us延时倍乘数


/*!
 *  \brief  初始化延迟函数
 *  \param  SYSCLK   系统时钟频率（如72Mhz，填写72）
 *  \note   SYSTICK的时钟固定为HCLK时钟的1/8
 */
void delay_init(u8 SYSCLK) {
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为HCLK
	fac_us = SYSCLK;
}


/*!
 *  \brief  延迟ns
 *  \param  t ns数
 *  \note   delay_init后使用
 */
void delay_ns(u8 t) {
	do {
		;
	} while (--t);
}


/*!
 *  \brief  延迟us
 *  \param  t us数
 *  \note   delay_init后使用
 */
void delay_us(u32 nus) {
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;       //需要的节拍数
	told = SysTick->VAL;        //刚进⼊时的计数器值
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //这⾥注意⼀下SYSTICK是⼀个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
}


/*!
 *  \brief  延迟ms
 *  \param  t ms数
 *  \note   delay_init后使用
 */
void delay_ms(u16 nms) {
	delay_us(1000*nms);
}

/*!
 *  \brief  按键扫描函数
 *  \param  mode 0不支持连按，1支持连按
 *  \retval 按键值
 *  \note   此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP
 */
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
        else if(WK_UP==1) return WKUP_PRES;
    }else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
    return 0;   //无按键按下
}
