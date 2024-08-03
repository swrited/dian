//这是电脑端

//"这是pid"
#include "headfile.h"

extern int state[7];

float abs_my(float x,float y)
{
	if((x-y)<0)
		return y-x;
	else
		return x-y;
}
float dis_my(float x,float y,float z)
{
	float dis1=0;
	float dis2=0;
	if(x*y>0)
	{
		return abs_my(x,y);
	}
	else
	{
		if(x>0)
			dis1=z-x;
		else
			dis1=x+z;
		
		if(y>0)
			dis2=z-y;
		else
			dis2=y+z;
		
		return dis1+dis2;
	}
}
struct pid
{
    float kp;
    float ki;
    float kd;
    float last;
    float sum;
		float out;
};
struct pid motor_z;
int pid_left(int encoder,int set)
{
	int delat=set-encoder;
	motor_z.out+=motor_z.kp*delat+motor_z.kd*(delat-motor_z.last);
	motor_z.last=delat;
	if(motor_z.out>100)
		motor_z.out=100;
	else if(motor_z.out<0)
		motor_z.out=0;
	return motor_z.out;
}
struct pid motor_y;
int pid_right(int encoder,int set)
{
	int delat=set-encoder;
	motor_y.out+=motor_y.kp*delat+motor_y.kd*(delat-motor_y.last);
	motor_y.last=delat;
	if(motor_y.out>100)
		motor_y.out=100;
	else if(motor_y.out<0)
		motor_y.out=0;
	return motor_y.out;
}
struct pid motor_c;
int pid_cha(int num)
{

	int delat=0;
	if(pian_num_last!=pian_num)
	{
		
//		if(task==3||task==4)
//		{
//			if(zhuang==2)
//			{
//				motor_c.kp=16;
//				motor_c.kd=10;
//				delat=1.2*(state[3]-state[1])+1.5*(state[4]-state[0]);
//			}
//			else
//			{
//				motor_c.kp=8;
//				motor_c.kd=1;
//				delat=1.2*(state[3]-state[1])+1.5*(state[4]-state[0])+1.5*(state[6]-state[5]);
//			}
//		}
//		else
//		{
			delat=1.0*(state[3]-state[1])+1.5*(state[4]-state[0])+1.8*(state[6]-state[5]);
		
//		}
		
		if(pian_num==2)
			delat=0;
		motor_c.out=motor_c.kp*delat+motor_c.kd*(delat-motor_c.last);
		motor_c.last=delat;
		return motor_c.out;
		pian_num_last=pian_num;
	}
	return motor_c.out;
	
}
struct pid motor_a;
float pid_angle(float now,float set)
{
	float delat;
	int dir=0;
	if(task==2)
	{
		
		if(zhuang==0)
		{
			delat=set-now;
			if(delat>0)
				dir=1;
			else
			{
				delat=-delat;
				dir=-1;
			}
			
		}
		if(zhuang==2)
		{
			delat=dis_my(set,now,210);
			if(set>0)
			{
				if(now<set&&now>0)
					dir=1;
				else
					dir=-1;
			}
			else if(set<0)
			{
				if(now>set&&now<0)
					dir=-1;
				else
					dir=1;
			}
		}
		
		}
	if(task==3||task==4)
	{
		
		if(zhuang==0||zhuang==1)
		{
			delat=set-now;
			if(delat>0)
				dir=1;
			else
			{
				delat=-delat;
				dir=-1;
			}
			
		}
		else if(zhuang==3||zhuang==4)
		{
			delat=dis_my(set,now,210);
			if(set>0)
			{
				if(now<set&&now>0)
					dir=1;
				else
					dir=-1;
			}
			else if(set<0)
			{
				if(now>set&&now<0)
					dir=-1;
				else
					dir=1;
			}
		}
	}
	
	
	//OLED_ShowSignedNum(2,0,dir,3);
	motor_a.out=motor_a.kp*delat+motor_a.kd*(delat-motor_a.last);
	motor_a.last=delat;
	return dir*motor_a.out;

}
void init_pid_my()
{
	motor_z.kp=0.1;
	motor_z.kd=0.08;
	motor_z.last=0;
	motor_z.out=0;
	
	motor_y.kp=0.1;
	motor_y.kd=0.08;
	motor_y.last=0;
	motor_y.out=0;
	
	motor_c.kp=10;
	motor_c.kd=0;
	motor_c.last=0;
	motor_c.out=0;
	
	motor_a.kp=0.18;
	motor_a.kd=0.1;
	motor_a.last=0;
	motor_a.out=0;
	
	
}
