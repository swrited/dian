#include "headfile.h"

/*
*
* ���������ݰ�Ȩ���� Bվ�û����ҵĴ���û���� ���У�δ��������������ҵ��;
* 

* ���Ŷ������£�

* ����
* UART0 -- tx: PA10 rx: PA11    ʹ��printf����
* UART1 -- tx: PA8  rx: PA9
* UART2 -- tx: PA21 rx: PA22
* UART3 -- tx: PB2  rx: PB3

* PWM
* TIMG0_CH0  PA12
* TIMG0_CH1  PA13
* TIMG6_CH0  PB6
* TIMG6_CH1  PB7
* TIMG7_CH0  PA17
* TIMG7_CH1  PA18
* TIMG8_CH0  PA26
* TIMG8_CH1  PA27 
* TIMG12_CH0  PA14
* TIMG12_CH1  PB24

* �������
* PWMA   PA17
* AIN1   PB24
* AIN2   PB20
* E1A    PA12
* E1B    PA13
* PWMB   PA18
* BIN1   PB19
* BIN2   PB18
* E2A	   PA15
* E2B    PA16
*
* OLED ����
* SCL  PA0
* SDA  PA1
*
* MPU6050 ����
* SCL  PB6
* SDA  PB7
*/
int num=0;
int state[]={0,0,0,0,0,0,0};



int main(void)
{
		// ϵͳ��ʼ�� ϵͳƵ��80MHZ �����
		system_init();
		delay_ms(500); // ��ʱ0.5s �ȴ���Դ�ȶ�
	
		// ���ڳ�ʼ��
		uart_init(UART0,115200,1); 
		// OLED ʹ�����I2CЭ�� ���ſ���ml_oled.h�������޸�
			OLED_Init();
			OLED_Init();
		
//	
//		// MPU6050 ʹ�����I2CЭ�� ���ſ���ml_i2c.h�������޸�
//	  // MPU6050 ��ʼ��֮ǰҪ�ǵ��Ƚ���I2C��ʼ��
		I2C_Init();
		MPU6050_Init();
//	MPU6050_ReadDatas_Proc();
	MPU6050_ReadDatas_init();
	//���⴫����
//	gpio_init(GPIOB, DL_GPIO_PIN_12, PB12, IN_UP);//7
	gpio_init(GPIOA, DL_GPIO_PIN_28, PA28, IN_UP);//4
	gpio_init(GPIOB, DL_GPIO_PIN_17, PB17, IN_UP);//3
	gpio_init(GPIOB, DL_GPIO_PIN_15, PB15, IN_UP);//2
	gpio_init(GPIOA, DL_GPIO_PIN_31, PA31, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_13, PB13, IN_UP);//0
//	gpio_init(GPIOB, DL_GPIO_PIN_20, PB20, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_1, PB1, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_16, PB16, IN_UP);//1
	//������
	gpio_init(GPIOA, DL_GPIO_PIN_15, PA15, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_15,1
	
	
	);
		//OLED_Init();
	//���pwm
	gpio_init(GPIOA, DL_GPIO_PIN_27, PA27, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_27,0);
	gpio_init(GPIOA, DL_GPIO_PIN_24, PA24, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_24,0);
	//����ܿ���
	gpio_init(GPIOA, DL_GPIO_PIN_22, PA22, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_22,1);
	
	//���õ������
		gpio_init(GPIOB,DL_GPIO_PIN_24,PB24,OUT);
		gpio_init(GPIOB,DL_GPIO_PIN_18,PB18,OUT);
		gpio_init(GPIOB,DL_GPIO_PIN_19,PB19,OUT);
		gpio_init(GPIOA,DL_GPIO_PIN_8,PA8,OUT);
		
		gpio_set(GPIOB,DL_GPIO_PIN_24,1);
		gpio_set(GPIOB,DL_GPIO_PIN_18,0);
		gpio_set(GPIOB,DL_GPIO_PIN_19,1);
		gpio_set(GPIOA,DL_GPIO_PIN_8,0);
		//��������ʼ��
		gpio_init(GPIOB,DL_GPIO_PIN_2,PB2,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_7,PB7,IN_UP);
		//����
		gpio_init(GPIOB,DL_GPIO_PIN_21,PB21,IN_UP);
		gpio_init(GPIOA,DL_GPIO_PIN_18,PA18,IN_UP);
		
		//�µİ���
		gpio_init(GPIOB,DL_GPIO_PIN_10,PB10,IN_UP);//����  B10 B11  B8 B12
		gpio_init(GPIOB,DL_GPIO_PIN_11,PB11,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_8,PB8,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_12,PB12,IN_UP);
//	led��
		gpio_init(GPIOB, DL_GPIO_PIN_27, PB27, OUT);//1
		gpio_set(GPIOB, DL_GPIO_PIN_27,0);
//		// ����ͱ�������ʼ��
	//	motor_init();
//		encoder_init();
//	
//		// PID��ʼ��
			init_pid_my();
//		pid_init(&motorA, DELTA_PID, 0, 5, 0);
//		pid_init(&motorB, DELTA_PID, 0, 5, 0);
//		
//	  // ��ʱ���жϳ�ʼ�� �ж��ڷŵ��PID���Ƴ���
		tim_interrupt_ms_init(TIMG0, 1, 1);//���������
		tim_interrupt_ms_init(TIMG6, 1, 1);//���������
		tim_interrupt_ms_init(TIMG7, 10, 2);//����д���ư�
		tim_interrupt_ms_init(TIMG8, 50, 1);//����������
		
    while (1) 
		{

//			printf("%f  %d\n\r",temperature,gz);
//			zhuang=2;
			if(stop_flag==0)
			{
				pwm_motor1=0;
				pwm_motor2=0;
			}
			key_action();
			
			
//			printf("%d   ",my_z[2]);
			
//			printf("%d   %d",my_z[3],my_y[3]);
//			printf("\n\r");
//			printf("%d   ",pian_num);
			//motorA_duty(2000);

	//������ԭ����

			state[4] = gpio_get(GPIOA, DL_GPIO_PIN_28);
			state[3] = gpio_get(GPIOB, DL_GPIO_PIN_17);
			state[2] = gpio_get(GPIOB, DL_GPIO_PIN_15);
			state[1] = gpio_get(GPIOA, DL_GPIO_PIN_31);
			state[0] = gpio_get(GPIOB, DL_GPIO_PIN_13);
			
			state[5] = gpio_get(GPIOB, DL_GPIO_PIN_1);
			state[6] = gpio_get(GPIOB, DL_GPIO_PIN_16);
		
			for(int i=0;i<7;i++)
			printf("%d",state[i]);
			printf("%d",pian_num);
			printf("\n\r");
			

			if(state[5]==1) pian_num=5;
			else if(state[6]==1) pian_num=6;
			else if(state[4]==1) pian_num=4;
			else if(state[0]==1) pian_num=0;
			else if(state[3]==1) pian_num=3;
			else if(state[1]==1) pian_num=1;
			else if(state[2]==1) pian_num=2;
			else pian_num=-1;//˵���ѹ�����·
			
			
			
			

			
    }
}








