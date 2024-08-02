#ifndef _IMU_H_
#define _IMU_H_
#include "headfile.h"

typedef struct 
{	//ŷ����
	float Pitch;
	float Roll;
	float Yaw;
	//�¶�
	float temperature;
	//ԭʼֵ
	int16_t  Accel_Original[3];
	int16_t Gyro_Original[3];
	//У׼ֵ
	float Accel_Offset[3];
	float Gyro_Offset[3];
	//У׼���ֵ
	float Accel_Calulate[3];
	float Gyro_Calulate[3]; 
	//�˲����ֵ
	float Accel_Average[3];
	float Gyro_Average[3];
	
}MPU6050_DEF;

extern MPU6050_DEF mpu6050;
extern float Gyro_Z_Measeure;
void MPU6050_ReadDatas_init(void);
void MPU6050_ReadDatas_Proc(void);

void AHRS_Geteuler(void);

//�������˲������ṹ��
struct KalmanFilter{
	float LastP;			//��һ��Э����
	float NewP;				//���µ�Э����
	float Out;				//���������
	float Kg;				//����������
	float Q;				//����������Э����
	float R;				//�۲�������Э����
};

//MyMath
#define M_PI  				(float)3.1415926535f
#define squa( Sq )       	(((float)Sq)*((float)Sq))
#define toRad( Math_D )	  	((float)(Math_D)*0.0174532925f)
#define toDeg( Math_R )	  	((float)(Math_R)*57.2957795f)
#define absu16( Math_X )  	(Math_X<0? -Math_X:Math_X)
#define absFloat( Math_X )	(Math_X<0? -Math_X:Math_X)
double IIR_I_Filter(double InData, double *x, double *y, double *b, short nb, double *a, short na);
float LPF_1st(float oldData, float newData, float lpf_factor);
void kalmanfiter(struct KalmanFilter *EKF,float input);

#endif
