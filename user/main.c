#include "headfile.h"

/*
*
* 本工程内容版权均属 B站用户：我的代码没问题 所有，未经允许不得用于商业用途
* 

* 引脚定义如下：

* 串口
* UART0 -- tx: PA10 rx: PA11    使用printf函数
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

* 电机接线
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
* OLED 接线
* SCL  PA0
* SDA  PA1
*
* MPU6050 接线
* SCL  PB6
* SDA  PB7
*/
int num=0;
int state[]={0,0,0,0,0,0,0};



int main(void)
{
		// 系统初始化 系统频率80MHZ 必须加
		system_init();
		delay_ms(500); // 延时0.5s 等待电源稳定
	
		// 串口初始化
		uart_init(UART0,115200,1); 
		// OLED 使用软件I2C协议 引脚可在ml_oled.h里自由修改
			OLED_Init();
			OLED_Init();
		
//	
//		// MPU6050 使用软件I2C协议 引脚可在ml_i2c.h里自由修改
//	  // MPU6050 初始化之前要记得先进行I2C初始化
		I2C_Init();
		MPU6050_Init();
//	MPU6050_ReadDatas_Proc();
	MPU6050_ReadDatas_init();
	//红外传感器
//	gpio_init(GPIOB, DL_GPIO_PIN_12, PB12, IN_UP);//7
	gpio_init(GPIOA, DL_GPIO_PIN_28, PA28, IN_UP);//4
	gpio_init(GPIOB, DL_GPIO_PIN_17, PB17, IN_UP);//3
	gpio_init(GPIOB, DL_GPIO_PIN_15, PB15, IN_UP);//2
	gpio_init(GPIOA, DL_GPIO_PIN_31, PA31, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_13, PB13, IN_UP);//0
//	gpio_init(GPIOB, DL_GPIO_PIN_20, PB20, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_1, PB1, IN_UP);//1
	gpio_init(GPIOB, DL_GPIO_PIN_16, PB16, IN_UP);//1
	//蜂鸣器
	gpio_init(GPIOA, DL_GPIO_PIN_15, PA15, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_15,1
	
	
	);
		//OLED_Init();
	//电机pwm
	gpio_init(GPIOA, DL_GPIO_PIN_27, PA27, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_27,0);
	gpio_init(GPIOA, DL_GPIO_PIN_24, PA24, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_24,0);
	//电机总控制
	gpio_init(GPIOA, DL_GPIO_PIN_22, PA22, OUT);//1
	gpio_set(GPIOA, DL_GPIO_PIN_22,1);
	
	//设置电机方向
		gpio_init(GPIOB,DL_GPIO_PIN_24,PB24,OUT);
		gpio_init(GPIOB,DL_GPIO_PIN_18,PB18,OUT);
		gpio_init(GPIOB,DL_GPIO_PIN_19,PB19,OUT);
		gpio_init(GPIOA,DL_GPIO_PIN_8,PA8,OUT);
		
		gpio_set(GPIOB,DL_GPIO_PIN_24,1);
		gpio_set(GPIOB,DL_GPIO_PIN_18,0);
		gpio_set(GPIOB,DL_GPIO_PIN_19,1);
		gpio_set(GPIOA,DL_GPIO_PIN_8,0);
		//编码器初始化
		gpio_init(GPIOB,DL_GPIO_PIN_2,PB2,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_7,PB7,IN_UP);
		//按键
		gpio_init(GPIOB,DL_GPIO_PIN_21,PB21,IN_UP);
		gpio_init(GPIOA,DL_GPIO_PIN_18,PA18,IN_UP);
		
		//新的按键
		gpio_init(GPIOB,DL_GPIO_PIN_10,PB10,IN_UP);//可用  B10 B11  B8 B12
		gpio_init(GPIOB,DL_GPIO_PIN_11,PB11,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_8,PB8,IN_UP);
		gpio_init(GPIOB,DL_GPIO_PIN_12,PB12,IN_UP);
//	led灯
		gpio_init(GPIOB, DL_GPIO_PIN_27, PB27, OUT);//1
		gpio_set(GPIOB, DL_GPIO_PIN_27,0);
//		// 电机和编码器初始化
	//	motor_init();
//		encoder_init();
//	
//		// PID初始化
			init_pid_my();
//		pid_init(&motorA, DELTA_PID, 0, 5, 0);
//		pid_init(&motorB, DELTA_PID, 0, 5, 0);
//		
//	  // 定时器中断初始化 中断内放电机PID控制程序
		tim_interrupt_ms_init(TIMG0, 1, 1);//电机在这里
		tim_interrupt_ms_init(TIMG6, 1, 1);//这里编码器
		tim_interrupt_ms_init(TIMG7, 10, 2);//这里写控制吧
		tim_interrupt_ms_init(TIMG8, 50, 1);//这里陀螺仪
		
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

	//陀螺仪原数据

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
			else pian_num=-1;//说明脱轨进入断路
			
			
			
			

			
    }
}








