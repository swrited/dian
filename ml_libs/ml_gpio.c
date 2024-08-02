#include "ml_gpio.h"
//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO��ʼ��
// @param	  gpio	  ѡ��GPIO�˿�	
// @param	  pins    ѡ�����ź�
// @param	  gpion   ѡ���Ӧ���� 
//                  ע������������ͬ ��ʵ���Ƕ��һ������ ����Ҫ��ʼ��PA1���� �������������PA1
// @return		void  
// Sample usage:		gpio_init(GPIOA, DL_GPIO_PIN_1, PA1, OUT);		
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(GPIO_Regs* gpio, uint32_t pins, GPIOn_enum gpion, GPIO_Mode_enum mode)
{
		if(mode == OUT)
		{
			DL_GPIO_initDigitalOutput(gpion);
      gpio_set(gpio, pins, 0);
      DL_GPIO_enableOutput(gpio, pins);
		}
		else if(mode == IN_DOWN)
		{
			DL_GPIO_initDigitalInputFeatures(gpion, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
		}
		else if(mode == IN_UP)
		{
			DL_GPIO_initDigitalInputFeatures(gpion, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
		}
}


//-------------------------------------------------------------------------------------------------------------------
// @brief	  ����GPIO��ƽ
// @param	  gpio	  ѡ��GPIO�˿�	
// @param	  pins    ѡ�����ź�
// @param	  status   �������ŵ�ƽ(0/1)
// @return		void  
// Sample usage:		gpio_set(GPIOB, DL_GPIO_PIN_3, 1);
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(GPIO_Regs* gpio, uint32_t pins, uint8_t status)
{
	if(status)
		gpio->DOUTSET31_0 = pins;
	else
		gpio->DOUTCLR31_0 = pins;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief	  ��תGPIO��ƽ
// @param	  gpiox	  ѡ��GPIO�˿�	
// @param	  pinx    ѡ�����ź�
// Sample usage:		gpio_toggle(GPIOB, DL_GPIO_PIN_3);
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle(GPIO_Regs* gpio, uint32_t pins)
{
	gpio->DOUTTGL31_0 = pins;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief	  ��ȡGPIO��ƽ
// @param	  gpio	  ѡ��GPIO�˿�	
// @param	  pins    ѡ�����ź�
// @return	  ���ŵ�ƽ 
// Sample usage:		uint8_t state = gpio_get(GPIOB, DL_GPIO_PIN_3);
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get(GPIO_Regs* gpio, uint32_t pins)
{
	uint32_t temp;
	temp = gpio->DIN31_0 & pins;
	if(temp > 0)
		return 1;
	else 
		return 0;
}

