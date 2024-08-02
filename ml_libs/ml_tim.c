#include "ml_tim.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief	  ��ʼ����ʱ���ж� 
// @param	  timgn	  ѡ��ʱ��	  ��ѡ��ʱ�� TIMG0��TIMG6��TIMG7��TIMG8��TIMG12
// @param	  time_ms   �ж�ʱ����(��λms ��Ҫ����1300ms ����Ƕ�ʱ��TIMG12���ܳ���6ms)
// @param	  pri       �ж����ȼ�
// @return		void  
// Sample usage:		tim_interrupt_ms_init(TIMG0, 10, 1);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_ms_init(GPTIMER_Regs *timgn, uint32_t time_ms, uint8_t pri)
{
		uint8_t psc, irq;
		uint16_t arr;
		if(timgn == TIMG0)
		{
			psc = 99;
			arr = 2 * time_ms - 1;
			irq = TIMG0_INT_IRQn;
		}
		else if(timgn == TIMG6)
		{
			psc = 199;
			arr = 10 * time_ms - 1;
			irq = TIMG6_INT_IRQn;
		}
		else if(timgn == TIMG7)
		{
			psc = 199;
			arr = 50 * time_ms - 1;
			irq = TIMG7_INT_IRQn;
		}
		else if(timgn == TIMG8)
		{
			psc = 99;
			arr = 50 * time_ms - 1;
			irq = TIMG8_INT_IRQn;
		}
		else if(timgn == TIMG12)
		{
			psc = 0;
			arr = 10000 * time_ms - 1;
			irq = TIMG12_INT_IRQn;
		}
		
		DL_TimerG_ClockConfig gTIMERClockConfig = {
			.clockSel    = DL_TIMER_CLOCK_BUSCLK,
			.divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
			.prescale    = psc,
		};

		DL_TimerG_TimerConfig gTIMERTimerConfig = {
			.period     = arr,
			.timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
			.startTimer = DL_TIMER_START,
		};
	
    DL_TimerG_reset(timgn);
	  DL_TimerG_enablePower(timgn);

    DL_TimerG_setClockConfig(timgn,
        (DL_TimerG_ClockConfig *) &gTIMERClockConfig);
		
		DL_TimerG_initTimerMode(timgn,
				(DL_TimerG_TimerConfig *) &gTIMERTimerConfig);
		
    DL_TimerG_enableInterrupt(timgn , DL_TIMERG_INTERRUPT_LOAD_EVENT);
    DL_TimerG_enableClock(timgn);
		
	  NVIC_ClearPendingIRQ(irq);
		NVIC_SetPriority(irq, pri);
    //ʹ�ܶ�ʱ���ж�
    NVIC_EnableIRQ(irq);
}






