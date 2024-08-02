#ifndef __SYSTEM_H_
#define __SYSTEM_H_

extern int pwm_motor1;
extern int pwm_motor2;
extern int my_z[4];
extern int my_y[4];
extern int pian_num;
extern int stop_flag;
extern int key_my;
extern int key_my2;
extern int key_my3;
extern int pwm_cha;

extern float yaw_my;
extern float yaw_set[];
extern float yaw_set_my[];
extern int yaw_num;
extern int task;
extern int time_kai;
extern int zhuang;
extern int sum;
extern int sum_flag;

extern int task4_num;

extern int pian_num_last;

extern int key_state[];
extern int temp;
extern float temperature;
extern int slect;
extern int ji_flag;
void key_action();
#endif