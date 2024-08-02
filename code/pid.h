#ifndef __PID_H_
#define __PID_H_

int pid_left(int encoder,int set);
int pid_right(int encoder,int set);
int pid_cha(int num);
float pid_angle(float now,float set);
void init_pid_my();
#endif