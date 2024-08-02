#include "headfile.h"

extern int state[5];

// ��ʱ���ж�
int pwm_time0=0;
int pwm_time1=0;
int pwm_max = 200;
void TIMG0_IRQHandler()
{	
	if(DL_TimerG_getPendingInterrupt(TIMG0) == DL_TIMER_IIDX_LOAD)
	{
						// �˴���д�жϺ���
		//pid_control();  // ���PID����
		if(pwm_motor1==0)
			gpio_set(GPIOA, DL_GPIO_PIN_27,0);
		else
		{
			if(pwm_time0<pwm_motor1)
			{
				gpio_set(GPIOA, DL_GPIO_PIN_27,1);
				pwm_time0++;
			}
			else
			{
				gpio_set(GPIOA, DL_GPIO_PIN_27,0);
				pwm_time0++;
			}
			if (pwm_time0 == pwm_max)
			{
				pwm_time0 = 0;
			}
		}
		if(pwm_motor2==0)
			gpio_set(GPIOA, DL_GPIO_PIN_24,0);
		else
		{
			if(pwm_time1< pwm_motor2 )
			{
				gpio_set(GPIOA, DL_GPIO_PIN_24,1);
				pwm_time1++;
			}
			else
			{
				gpio_set(GPIOA, DL_GPIO_PIN_24,0);
				pwm_time1++;
			}
			if (pwm_time1 == pwm_max)
			{
				pwm_time1 = 0;
			}
		}

	}
}

int count_6=0;
void TIMG6_IRQHandler()
{
	if(DL_TimerG_getPendingInterrupt(TIMG6) == DL_TIMER_IIDX_LOAD)
	{
						// �˴���д�жϺ���
		if(count_6>100)
		{
			if(my_z[2]<3)
				my_z[2]=0;
			my_z[3]=my_z[2];
			count_6=0;
			my_z[0]=0;
			my_z[1]=0;
			my_z[2]=0;
			
			if(my_y[2]<3)
				my_y[2]=0;
			my_y[3]=my_y[2];
			my_y[0]=0;
			my_y[1]=0;
			my_y[2]=0;
			
		}
		count_6++;
		my_z[0]=gpio_get(GPIOB, DL_GPIO_PIN_2);
			if(my_z[0]!=my_z[1])
				my_z[2]++;
			my_z[1]=my_z[0];
			
		my_y[0]=gpio_get(GPIOB, DL_GPIO_PIN_7);
		if(my_y[0]!=my_y[1])
			my_y[2]++;
		my_y[1]=my_y[0];
	}
}
//int count7=0;
int abs_sin(int x)
{
	if(x<0)
		return -x;
	else
		return x;

}

int time8_flag=0;
int time8_flag1=0;
int speed=17;//ѭ�����ٶ�
int speed_base=22;//��·�ٶ�
int huang_time=0;
int huang_flag=0;
int ji_time=0;
int jixu_flag=0;
int jixu_time=0;
//int time_my=0;
int time_dajiao=80;
int zi_flag=0;
int zi_time=0;

void TIMG7_IRQHandler()
{
	if(DL_TimerG_getPendingInterrupt(TIMG7) == DL_TIMER_IIDX_LOAD)
	{
						// �˴���д�жϺ���

  
		if(stop_flag==0&&zi_flag==0&&pian_num==-1)
		{
			zi_flag=1;
			
		}
		if(zi_flag==1||zi_flag==2)
		{
			if(zi_time<10)
			{
				if(zi_time<5)
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
				else
					gpio_set(GPIOA, DL_GPIO_PIN_15,1);//������
				
			}
			else
			{
				zi_time=0;
				if(zi_flag==1)
					zi_flag=2;
				else if(zi_flag==2)
				{
					zi_flag=3;
					gpio_set(GPIOA, DL_GPIO_PIN_15,1);
				}
				printf("%d",zi_flag);
			}
		}
			
		//MPU6050_GetData();
		if(task==0&&stop_flag==1)
		{
			if(pian_num!=-1)
			{
				pwm_cha=pid_cha(pian_num);
			}
			
//				if(pian_num==-1)
//				{
//					pwm_cha=0;
//				}
//			if(pwm_cha>0) pwm_cha=-pwm_cha;//���
//			if(pwm_cha>0) pwm_cha=-pwm_cha;//���
		    pwm_motor2=pid_left(my_z[3],speed*(1+pwm_cha/40.0));
				pwm_motor1=pid_right(my_y[3],speed*(1-pwm_cha/40.0));
//				if(pian_num==-1&&time_kai==0&&abs_sin(yaw_my)>180-5)//�뿪����
//				{
////					zhuang=7;
//					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
//					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
//					time_kai=1;
//					sum_flag=0;
//					time8_flag1=1;
//					
//				}
		}
		
		if(task==1&&stop_flag==1)
		{
			pwm_cha=pid_angle(yaw_my,yaw_set[0]);
			pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
			pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
			if(pian_num!=-1&&time_kai==0)//������������ʱͣ�������ҷ���������
			{
				stop_flag=0;
				OLED_Clear();
				gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
				gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
				pian_num=-1;
				time_kai=1;
				
			}
		}
		
		if(task==2&&stop_flag==1)
		{
			if(zhuang==0)
			{
//
				pwm_cha=pid_angle(yaw_my,yaw_set[0]);
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(pian_num!=-1&&time_kai==0)//�������������ҷ���������
				{
					zhuang=1;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
				}
			}
			if(zhuang==1)
			{
				if(pian_num!=-1)
				{
					pwm_cha=pid_cha(pian_num);
				}
				if(pwm_cha<-2) pwm_cha=-2;
//				pwm_cha=pid_cha(pian_num);
//				if(pian_num==-1)
//				{
//					pwm_cha=0;
//				}
//			if(pwm_cha<0) pwm_cha=-pwm_cha;//�ҹ�
		    pwm_motor2=pid_left(my_z[3],speed*(1+pwm_cha/40.0));
				pwm_motor1=pid_right(my_y[3],speed*(1-pwm_cha/40.0));
				if(pian_num==-1&&time_kai==0&&abs_sin(yaw_my)>180)//�뿪����
				{
					zhuang=5;
					
					sum_flag=0;
					ji_flag=1;
					time8_flag=1;
					
				}
			}
			if(zhuang==5)//����
			{
				pwm_cha=pid_angle(yaw_my,yaw_set[1]-30);
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag==0)//�������������ҷ���������
				{
					zhuang=2;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
				}
			}
			if(zhuang==2)//��·
			{
				if(ji_time<50)
				{
					pwm_cha=pid_angle(yaw_my,-yaw_set[1]+10);
				}
				else
				{
					pwm_cha=pid_angle(yaw_my,yaw_set[1]-20);
				}
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(pian_num!=-1&&time_kai==0)//�������������ҷ���������
				{
					zhuang=3;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
					ji_flag=0;
					ji_time=0;
				}
			
			}
			
			if(zhuang==3)//ѭ��
			{
//				pwm_cha=pid_cha(pian_num);
//				if(pian_num==-1)
//			{
//				pwm_cha=0;
//			}
//			if(pwm_cha<0) pwm_cha=-pwm_cha;//���
								if(pian_num!=-1)
				{
					pwm_cha=pid_cha(pian_num);
				}
				if(pwm_cha<-2) pwm_cha=-2;
		    pwm_motor2=pid_left(my_z[3],speed*(1+pwm_cha/40.0));
				pwm_motor1=pid_right(my_y[3],speed*(1-pwm_cha/40.0));
				if(pian_num==-1&&time_kai==0&&abs_sin(yaw_my)<30)//�뿪����
				{
					zhuang=4;
					time8_flag=1;
				}
			}
			if(zhuang==4)//����
			{
				pwm_cha=pid_angle(yaw_my,yaw_set[0]);
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag==0)//�������������ҷ���������
				{
					stop_flag=0;
					zhuang=0;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
//					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
//					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
//					time_kai=1;
				}
			}
		
		}
		if(((task==3)||task==4)&&stop_flag==1)
		{
			if(zhuang==0)//8��б��1
			{
				if(ji_time<time_dajiao)
				{
					pwm_cha=pid_angle(yaw_my,yaw_set[2]-10);
				}
				else
				{
					pwm_cha=pid_angle(yaw_my,yaw_set[0]+20);
				}
				
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);

				if(pian_num!=-1&&time_kai==0)//�������������ҷ���������
				{
					zhuang=1;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
					time8_flag=1;
					ji_flag=0;
//					huang_flag=0;
				}
			}
			if(zhuang==1)//����
			{
//				if(ji_time<time_dajiao)
//				{
					pwm_cha=pid_angle(yaw_my,yaw_set[0]);
					ji_time=0;
//				}
//				else
//				{
//					pwm_cha=pid_angle(yaw_my,yaw_set[0]+10);
//					ji_time=0;
//				}
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag==0)//�������������ҷ���������
				{
					zhuang=2;
				}
			}
			if(zhuang==2)//��ѭ��
			{
				
			if(pian_num!=-1)
				{
					pwm_cha=pid_cha(pian_num);
				}
				if(pwm_cha>2) pwm_cha=2;
				
//				pwm_cha=pid_cha(pian_num);
//				if(pian_num==-1)
//			{
//				pwm_cha=0;
//			}
//			if(pwm_cha>0) pwm_cha=-pwm_cha;//���

		    pwm_motor2=pid_left(my_z[3],speed*(1+pwm_cha/40.0));
				pwm_motor1=pid_right(my_y[3],speed*(1-pwm_cha/40.0));
				if(pian_num==-1&&time_kai==0&&abs_sin(yaw_my)>180)//�뿪����
				{
					zhuang=7;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
					sum_flag=0;
					time8_flag=1;
					
				}
			}
			if(zhuang==7)//����7
			{
				pwm_cha=pid_angle(yaw_my,-yaw_set[1]);
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag==0)//�������������ҷ���������
				{
					zhuang=3;
					ji_flag=1;
				}
			}
			if(zhuang==3)//8��б����
			{
				if(ji_time<time_dajiao)
				{
				pwm_cha=pid_angle(yaw_my,yaw_set[3]+10);
				}
				else
				{
				pwm_cha=pid_angle(yaw_my,yaw_set[1]-20);
				}
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);

				if(pian_num!=-1&&time_kai==0)//�������������ҷ���������
				{
					zhuang=4;
					huang_flag=0;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
					time8_flag=1;
					ji_flag=0;
					
					
				}
			
			}
			if(zhuang==4)//����
			{
//				if(ji_time<time_dajiao)
//				{
//					pwm_cha=pid_angle(yaw_my,yaw_set[1]-30);
//					ji_time=0;
//				}
//				else
//				{
					pwm_cha=pid_angle(yaw_my,-yaw_set[1]+10);
					ji_time=0;
//				}
				
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag==0)//�������������ҷ���������
				{
					zhuang=5;
//					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
//					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
//					time_kai=1;
				}
			
			}
			if(zhuang==5)//��ѭ��
			{
				if(pian_num!=-1)
				{
					pwm_cha=pid_cha(pian_num);
				}
				if(pwm_cha<-2) pwm_cha=-2;
//				pwm_cha=pid_cha(pian_num);
//				if(pian_num==-1)
//			{
//				pwm_cha=0;
//			}
//			if(pwm_cha<0) pwm_cha=-pwm_cha;//�ҹ�
		    pwm_motor2=pid_left(my_z[3],speed*(1+pwm_cha/40.0));
				pwm_motor1=pid_right(my_y[3],speed*(1-pwm_cha/40.0));
				if(pian_num==-1&&time_kai==0&&abs_sin(yaw_my)<30)//�뿪����
				{
					zhuang=6;
					gpio_set(GPIOA, DL_GPIO_PIN_15,0);//������
					gpio_set(GPIOB, DL_GPIO_PIN_27,1);//led��
					time_kai=1;
					time8_flag1=1;
				}
			}
			if(zhuang==6)//����6
			{
				pwm_cha=pid_angle(yaw_my,yaw_set[0]);
				pwm_motor2=pid_left(my_z[3],speed_base-pwm_cha);
				pwm_motor1=pid_right(my_y[3],speed_base+pwm_cha);
				if(time8_flag1==0)//�������������ҷ���������
				{
					if(task==3)
					{
						stop_flag=0;
					}
					if(task==4)
					{
						task4_num++;
						ji_flag=1;
					}
					if(task4_num==4)
						stop_flag=0;
					zhuang=0;
				}
			}
		
		}
		

		
		
	}
}


int time8=0;
int time8_1=0;
//int time81=0;
int time8_11=0;
void TIMG8_IRQHandler()
{
	if(DL_TimerG_getPendingInterrupt(TIMG8) == DL_TIMER_IIDX_LOAD)
	{
						// �˴���д�жϺ���
		AHRS_Geteuler();
		if(time_kai)
		{
			time8++;
			if(time8>20)
			{
				time8=0;
				time_kai=0;
				gpio_set(GPIOA, DL_GPIO_PIN_15,1);//������
				gpio_set(GPIOB, DL_GPIO_PIN_27,0);//led��
			}
		}
		if(time8_flag)
		{
			time8_1++;
			if(task==2)
			{
				if(time8_1>5)
				{
					time8_1=0;
					time8_flag=0;
				}
			}
			else
			{
				if(time8_1>4)
				{
					time8_1=0;
					time8_flag=0;
				}
			}
		}
		if(time8_flag1)
		{
			time8_11++;
			if(zhuang==6)
			{
				if(time8_11>5)
				{
					time8_11=0;
					time8_flag1=0;
					
				}
			}
			else
			{
				if(time8_11>2)
				{
					time8_11=0;
					time8_flag1=0;
					
				}
			}
		}
		if(ji_flag)
		{
			ji_time++;
		}
		if(zi_flag==1||zi_flag==2)
			zi_time++;


	}
}

void TIMG12_IRQHandler()
{
	if(DL_TimerG_getPendingInterrupt(TIMG12) == DL_TIMER_IIDX_LOAD)
	{
						// �˴���д�жϺ���

	}
}


// �����ж�
void UART0_IRQHandler(void)
{
	if(DL_UART_getPendingInterrupt(UART0) == DL_UART_IIDX_RX)
	{
						// �˴���д�жϺ���

	}
 
}


void UART1_IRQHandler(void)
{
	if(DL_UART_getPendingInterrupt(UART1) == DL_UART_IIDX_RX)
	{
						// �˴���д�жϺ���
		//imu_uart_callback();

	}
}


void UART2_IRQHandler(void)
{
	if(DL_UART_getPendingInterrupt(UART2) == DL_UART_IIDX_RX)
	{
						// �˴���д�жϺ���

	}
}


void UART3_IRQHandler(void)
{
	if(DL_UART_getPendingInterrupt(UART3) == DL_UART_IIDX_RX)
	{
						// �˴���д�жϺ���

	}
}


// �ⲿ�ж�
void GROUP1_IRQHandler(void)//Group1���жϷ�����
{
		if(GPIOA->CPU_INT.MIS & DL_GPIO_PIN_12) // PA12�ⲿ�ж�
		{
			if(gpio_get(GPIOA, DL_GPIO_PIN_13))
				Encoder_count1 --;
			else
				Encoder_count1 ++;
			
			GPIOA->CPU_INT.ICLR |= DL_GPIO_PIN_12; // ����жϱ�־λ
		}	
		else if(GPIOA->CPU_INT.MIS & DL_GPIO_PIN_15) // PA15�ⲿ�ж�
		{
			if(gpio_get(GPIOA, DL_GPIO_PIN_16))
				Encoder_count2 --;
			else
				Encoder_count2 ++;
			
			GPIOA->CPU_INT.ICLR |= DL_GPIO_PIN_15; // ����жϱ�־λ
		}
		
// �������Ҫ�����ⲿ�ж����� ���԰��������ģ���������
//		if(GPIOA->CPU_INT.MIS & DL_GPIO_PIN_0) // PA0�ⲿ�ж�
//		{
//        // �˴���д�ж�ִ�к���
		
//			GPIOA->CPU_INT.ICLR |= DL_GPIO_PIN_0; // ����жϱ�־λ
//		}

}




