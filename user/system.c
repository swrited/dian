#include "headfile.h"
int pwm_motor1=0;//右
int pwm_motor2=0;//左
int my_z[]={0,0,0,0};
int my_y[]={0,0,0,0};
int pian_num=0;
int stop_flag=0;
int key_my=1;
int key_my2=0;
int key_my3=1;
int pwm_cha=0;
float yaw_my=0;
float yaw_set[]={0,209,-45,-160};
//float yaw_set_my[]={0,209,-45,-170};
int yaw_num=0;
int task=1;
int time_kai=0;
int zhuang=0;
int sum=0;
int sum_flag=0;
int task4_num=0;
int pian_num_last=2;
int key_state[]={0,0,0,0};
int temp=0;
float temperature=0;
int slect=1;
int ji_flag=0;
static int ye=0;
static int hang=0;
void oled_show()
{
		OLED_ShowString(hang+1,15,"O");
		if(ye==0)//页面0用来确定任务目标和选择陀螺仪滤波方式
		{
			
			OLED_ShowString(1,1,"angle");
			OLED_ShowFloat(1,7,yaw_my,3,2);
			OLED_ShowString(2,1,"task");
			OLED_ShowNum(2,10,task,2); 
			OLED_ShowString(3,1,"lvbo");
			OLED_ShowNum(3,10,slect,2); 
			
			
//			OLED_ShowSignedNum(1,10,yaw_set[0],3);
//			OLED_ShowSignedNum(2,10,yaw_set[1],3);
//			OLED_ShowSignedNum(3,10,yaw_set[2],3);
//			OLED_ShowSignedNum(4,10,yaw_set[3],3);
		
		}
		if(ye==1)
		{
			OLED_ShowString(1,1,"jiao1");
			OLED_ShowString(2,1,"jiao2");
			OLED_ShowString(3,1,"jiao3");
			OLED_ShowString(4,1,"jiao4");
			OLED_ShowSignedNum(1,10,yaw_set[0],3);
			OLED_ShowSignedNum(2,10,yaw_set[1],3);
			OLED_ShowSignedNum(3,10,yaw_set[2],3);
			OLED_ShowSignedNum(4,10,yaw_set[3],3);
		
		}
			
//			OLED_ShowSignedNum(3,0,pwm_cha,3);
//			OLED_ShowSignedNum(3,0,yaw_set[3],3);
}
void key_action()
{

			key_state[0]=gpio_get(GPIOB, DL_GPIO_PIN_10);
			key_state[1]=gpio_get(GPIOB, DL_GPIO_PIN_11);
			key_state[3]=gpio_get(GPIOB, DL_GPIO_PIN_8);
			key_state[2]=gpio_get(GPIOB, DL_GPIO_PIN_12);
			key_my=gpio_get(GPIOB, DL_GPIO_PIN_21);

			if(key_my==0)//停开车
			{
				while(key_my==0)
				{
					key_my=gpio_get(GPIOB, DL_GPIO_PIN_21);
				}
				if(stop_flag==1)
				{
					stop_flag=0;
//					if(task==2)
						zhuang=0;
					
					
				}
				else
				{
					stop_flag=1;
					OLED_Clear();
					if(task==3||task==4)
					{
						ji_flag=1;
					}
				}
			}
			
			key_my2=gpio_get(GPIOA, DL_GPIO_PIN_18);
			if(key_my2==1)//记录四个角度点,记录四个任务
			{
				while(key_my2==1)
				{
					key_my2=gpio_get(GPIOA, DL_GPIO_PIN_18);
				}
				yaw_num++;
				yaw_num=yaw_num%4;
				task=yaw_num+1;
//				if(yaw_num>4)
//					yaw_num=0;
////				yaw_set[yaw_num]=yaw_my;
//				yaw_num++;
//				task=yaw_num;
			}
			if(key_state[0]==0)//按键1   换页键
			{
				while(key_state[0]==0)
				{
					key_state[0]=gpio_get(GPIOB, DL_GPIO_PIN_10);
				}
				ye++;
				ye=ye%3;
				OLED_Clear();
			}
			if(key_state[1]==0)//按键2   选择
			{
				while(key_state[1]==0)
				{
					key_state[1]=gpio_get(GPIOB, DL_GPIO_PIN_11);
				}
				hang++;
				hang=hang%4;
				OLED_Clear();
			}
			if(key_state[2]==0)//按键3   增减
			{
				while(key_state[2]==0)
				{
					key_state[2]=gpio_get(GPIOB, DL_GPIO_PIN_12);
				}
				if(ye==0)
				{
						if(slect==1)
							slect=2;
						else
							slect=1;
				
				}
				else if(ye==1)
				{
//					if(hang==1)
//					{
						yaw_set[hang]++;
//					}
				
				}
			}
			if(key_state[3]==0)//按键3   增减
			{
				while(key_state[3]==0)
				{
					key_state[3]=gpio_get(GPIOB, DL_GPIO_PIN_8);
				}
				if(ye==0)
				{
						if(slect==1)
							slect=2;
						else
							slect=1;
				
				}
				else if(ye==1)
				{
					yaw_set[hang]--;
				}
			}
			
			
			
			
			
			
			//显示
			if(stop_flag==0)
			{
				oled_show();
			}

}