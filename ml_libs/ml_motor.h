#ifndef _motor_h
#define _motor_h
#include "headfile.h"

/*                                    
   PWMA->2.5  Channel_2
   AIN1->3.0
	 AIN2->5.7
	 PWMB->2.6  Channel_3
	 BIN1->6.1
	 BIN2->4.0
	 E1A->5.2
	 E1B->3.6
	 E2A->3.5
	 E2B->3.7
*/


void motor_init(void);
void motorA_duty(int duty);
void motorB_duty(int duty);
void encoder_init(void);

extern int Encoder_count1, Encoder_count2;
extern uint8_t motorA_dir, motorB_dir;

#endif

