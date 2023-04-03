// base.c?��
#include "base.h"
static u8 fac_us = 0; //us��ʱ������
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK) {
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTickƵ��ΪHCLK
	fac_us = SYSCLK;
}
void delay_ns(u8 t) {
	do {
		;
	} while (--t);
}
void delay_us(u32 nus) {
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD��ֵ
	ticks = nus * fac_us;       //��Ҫ�Ľ�����
	told = SysTick->VAL;        //�ս�?ʱ�ļ�����ֵ
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //��?ע��?��SYSTICK��?���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms) {
	u32 i;
	for (i = 0; i < nms; i++)
		delay_us(1000);
}
//����ɨ�躯��
//��ʹ��ʱע��
//u8 KEY_Scan(u8 mode) {
//	static u8 key_up = 1; //�������ɿ���־
//	if (mode)
//		key_up = 1; //������
//	if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0)) {
//		HAL_Delay(10); //ȥ����
//		key_up = 0;
//		if (KEY0 == 0)
//			return KEY0_PRES;
//		else if (KEY1 == 0)
//			return KEY1_PRES;
//		else if (KEY2 == 0)
//			return KEY2_PRES;
//	} else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
//		key_up = 1;
//	return 0; // ��������
//}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR = PortVal;
}

/**
  * @brief  Reads the specified GPIO output data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  return ((uint16_t)GPIOx->ODR);
}

// ����TIM1 CH1����PA8����
void PWM_Set(u32 f0)
{
	u32 arr_t;

	if(f0<10000)
	{
		TIM1->PSC = 1680-1;
		arr_t = 100000/f0;
		arr_t--;
		TIM1->ARR = arr_t;
		TIM1->CCR1 = arr_t/2;
	}
	else
	{
		TIM1->PSC = 0;
		arr_t = 168000000/f0;
		arr_t--;
		TIM1->ARR = arr_t;
		TIM1->CCR1 = arr_t/2;
	}
}
