#include "headfile.h"


#define GYRO_GATHER   	700 //ԭ����100
#define RtA 			57.324841f				
#define AtR    			0.0174533f				
#define Acc_G 			0.0011963f				
#define Gyro_G 			0.03051756f				
#define Gyro_Gr			0.0005426f

#define Offset_Times 	200.0		//�ϵ�У׼����
#define Sampling_Time	0.05		//������ȡʱ��10ms

#define IIR_ORDER     4      //ʹ��IIR�˲����Ľ���
double b_IIR[IIR_ORDER+1] ={0.0008f, 0.0032f, 0.0048f, 0.0032f, 0.0008f};  //ϵ��b
double a_IIR[IIR_ORDER+1] ={1.0000f, -3.0176f, 3.5072f, -1.8476f, 0.3708f};//ϵ��a
double InPut_IIR[3][IIR_ORDER+1]  = {0};
double OutPut_IIR[3][IIR_ORDER+1] = {0};
MPU6050_DEF mpu6050;
//int time=0;
void MPU6050_ReadDatas_init(void){
	
	for(int i=0;i<Offset_Times;i++)
//	if(time<Offset_Times)//�����ʼУ׼ֵ
	{
		MPU6050_GetData();
		if(gz<20&&gz>-20)
		{
				mpu6050.Gyro_Offset[0] +=(float)gx/Offset_Times;
				mpu6050.Gyro_Offset[1] +=(float)gy/Offset_Times;//��ȡ���ݼ���ƫ��
				mpu6050.Gyro_Offset[2] +=(float)gz/Offset_Times;//��ȡ���ݼ���ƫ��
				//delay_ms(1);
//				printf("%d\n\r",gz);
		}

	}
//	printf("%f\n\r",mpu6050.Gyro_Offset[2]);
	
	
}
float my_data[8]={0,0,0,0,0,0,0,0};
int cnt=0;
float sum_my=0;
void MPU6050_ReadDatas_Proc(void){
//	delay_ms(100);
//	static uint16_t time=0;//��ʼ��У׼����
	MPU6050_GetData();
//	else
		// ���ٶ�ֵ��ֵ����ȥ��Ư��
		mpu6050.Accel_Calulate[0] = ax;// - mpu6050.Accel_Offset[0];//�Ǽ��ٶȲ���
		mpu6050.Accel_Calulate[1] = ay;// - mpu6050.Accel_Offset[1];
		mpu6050.Accel_Calulate[2] = az;// - mpu6050.Accel_Offset[2];
		// ������ֵ��ֵ����ȥ��Ư��
		mpu6050.Gyro_Calulate[0] = gx - mpu6050.Gyro_Offset[0];
		mpu6050.Gyro_Calulate[1] = gy - mpu6050.Gyro_Offset[1];
		mpu6050.Gyro_Calulate[2] = gz - mpu6050.Gyro_Offset[2];
//			OLED_ShowSignedNum(2,1,mpu6050.Gyro_Calulate[0],4);
//			OLED_ShowSignedNum(3,1,mpu6050.Gyro_Calulate[1],4);
//			OLED_ShowSignedNum(4,1,mpu6050.Gyro_Calulate[2],4);
		/***********�Ǽ��ٶ��˲���������ѡһ��***********/
	
		//һ���Ǽ��ٶ�IIR�˲�
//		mpu6050.Accel_Average[0] = IIR_I_Filter(mpu6050.Accel_Calulate[0], InPut_IIR[0], OutPut_IIR[0], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
//		mpu6050.Accel_Average[1] = IIR_I_Filter(mpu6050.Accel_Calulate[1], InPut_IIR[1], OutPut_IIR[1], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
//		mpu6050.Accel_Average[2] = IIR_I_Filter(mpu6050.Accel_Calulate[2], InPut_IIR[2], OutPut_IIR[2], b_IIR, IIR_ORDER+1, a_IIR, IIR_ORDER+1);
		//�����Ǽ��ٶȿ������˲�������
//		static struct KalmanFilter EKF[3]={{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543}};
//		kalmanfiter(&EKF[0],(float)mpu6050.Accel_Calulate[0]);  
//		mpu6050.Accel_Average[0] =  (int16_t)EKF[0].Out;
//		kalmanfiter(&EKF[1],(float)mpu6050.Accel_Calulate[1]);  
//		mpu6050.Accel_Average[1] =  (int16_t)EKF[1].Out;
//		kalmanfiter(&EKF[2],(float)mpu6050.Accel_Calulate[2]);  
//		mpu6050.Accel_Average[2] =  (int16_t)EKF[2].Out;
		
		/*******************���ٶ��˲�********************/
		static float x,y,z;
//		//������ֵһ�׵�ͨ�˲����ϸ����ݣ����ڵ����ݣ������˲���ϵ����Ҳ�ƻ����˲���
//		mpu6050.Gyro_Average[0] = LPF_1st(x,mpu6050.Gyro_Calulate[0],0.386f);	x = mpu6050.Gyro_Average[0];
//		mpu6050.Gyro_Average[1]=  LPF_1st(y,mpu6050.Gyro_Calulate[1],0.386f);	y = mpu6050.Gyro_Average[1];
		if(slect==1)
		{
			mpu6050.Gyro_Average[2] = LPF_1st(z,mpu6050.Gyro_Calulate[2],0.386f);   z = mpu6050.Gyro_Average[2];
		}
		else if(slect==2)
		{
			
			mpu6050.Gyro_Average[1] = LPF_1st(z,mpu6050.Gyro_Calulate[2],0.386f);   z = mpu6050.Gyro_Average[1];
			//�����˲�
			sum_my=0;
			my_data[cnt]=mpu6050.Gyro_Average[1];
			cnt++;
			if(cnt>8) cnt=0;
			for(int i=0;i<8;i++)
			{
				sum_my+=my_data[i];
			}
			mpu6050.Gyro_Average[2]=sum_my/8;
	}
}
#define MPU_Aceel_Gyro_Kp	0.95
float pitch2,roll2,Yaw;
float Gyro_Z_Measeure = 0;
void AHRS_Geteuler(void){
	MPU6050_ReadDatas_Proc();//��ȡ�˲�����
	
//	float ax,ay,az;
//	ax=mpu6050.Accel_Average[0];
//	ay=mpu6050.Accel_Average[1];
//	az=mpu6050.Accel_Average[2];
	
	//һ���Ǽ��ٶȺͽ��ٶȽ���� ������ �� ����� ���н��
//////	float pitch1	= 	RtA*atan(ay/sqrtf(ax*ax+az*az));  // ������
//////	float roll1		=	-RtA*atan(ax/sqrtf(ay*ay+az*az)); // �����
//////	pitch2  += (mpu6050.Gyro_Average[0])*2000/32768*Sampling_Time;//������
//////	roll2	+= (mpu6050.Gyro_Average[1])*2000/32768*Sampling_Time;//�����
//////	mpu6050.Pitch =	 pitch1*MPU_Aceel_Gyro_Kp+pitch2*(1-MPU_Aceel_Gyro_Kp);		// ������
//////	mpu6050.Roll  =  roll1*MPU_Aceel_Gyro_Kp+roll2*(1-MPU_Aceel_Gyro_Kp);	 		// �����
	//�����Ǽ��ٶȽ���� ������ �� �����
//	mpu6050.Pitch =	RtA*atan(ay/sqrtf(ax*ax+az*az)); // ������
//	mpu6050.Roll = -RtA*atan(ax/sqrtf(ay*ay+az*az)); // �����
	//�������ٶȽ���� ������ �� �����
//	mpu6050.Pitch 	+= (mpu6050.Gyro_Average[0])*2000/32768*Sampling_Time; // ������
//	mpu6050.Roll 	+= (mpu6050.Gyro_Average[1])*2000/32768*Sampling_Time; // �����

	//z�᲻��Ҫ���ģ��㹻�ȶ���
	Gyro_Z_Measeure = (mpu6050.Gyro_Average[2])*2000/32768.0;
	Yaw += Gyro_Z_Measeure*Sampling_Time;
	mpu6050.Yaw  = (	Yaw + Yaw*0.16667);//�������0.16667��Ϊ�˲����Ƕ�
	if(mpu6050.Yaw>420)
		Yaw=0;
	if(mpu6050.Yaw<-420)
		Yaw=0;
	if(mpu6050.Yaw>210)
	{
		yaw_my=mpu6050.Yaw-420;
	}
	else if(mpu6050.Yaw<-210)
	{
		yaw_my=mpu6050.Yaw+420;
	}
	else
		yaw_my=mpu6050.Yaw;
//		yaw_my=210/2-mpu6050.Yaw;
//	else if(mpu6050.Yaw>210)
//		yaw_my=0;
//	else
//		yaw_my=mpu6050.Yaw;
//			OLED_ShowFloat(2,1,mpu6050.Pitch,3,2);
//			OLED_ShowFloat(3,1,mpu6050.Roll,3,2);
			
	
}
//-------------------------------------------MyFilter-------------------------------------------------//
/*====================================================================================================*/
/*====================================================================================================*
** ��������: IIR_I_Filter
** ��������: IIRֱ��I���˲���
** ��    ��: InData Ϊ��ǰ����
**           *x     ����δ�˲�������
**           *y     �����˲��������
**           *b     ����ϵ��b
**           *a     ����ϵ��a
**           nb     ����*b�ĳ���
**           na     ����*a�ĳ���
**           LpfFactor
** ��    ��: OutData         
** ˵    ��: ��
** ����ԭ��: y(n) = b0*x(n) + b1*x(n-1) + b2*x(n-2) -
                    a1*y(n-1) - a2*y(n-2)
**====================================================================================================*/
/*====================================================================================================*/
double IIR_I_Filter(double InData, double *x, double *y, double *b, short nb, double *a, short na){
  double z1,z2;
  short i;
  double OutData;
  
  for(i=nb-1; i>0; i--)
  {
    x[i]=x[i-1];
  }
  
  x[0] = InData;
  
  for(z1=0,i=0; i<nb; i++)
  {
    z1 += x[i]*b[i];
  }
  
  for(i=na-1; i>0; i--)
  {
    y[i]=y[i-1];
  }
  
  for(z2=0,i=1; i<na; i++)
  {
    z2 += y[i]*a[i];
  }
  
  y[0] = z1 - z2; 
  OutData = y[0];
    
  return OutData;
}
///*====================================================================================================*/
///*====================================================================================================*
//**���� : LPF_1st
//**���� : һ�׵�ͨ�˲�
//**���� :  
//**ݔ�� : None
//**��ע : None
//**====================================================================================================*/
/*====================================================================================================*/
float LPF_1st(float oldData, float newData, float lpf_factor){
//	if(oldData-newData>250||oldData-newData<-250)
//	{
//		return oldData;
//	}
	return oldData * (1 - lpf_factor) + newData * lpf_factor;    //�ϸ�����*һ���ı���+���ڵ�����*һ���ı���
}
//һά�������˲�
void kalmanfiter(struct KalmanFilter *EKF,float input){
	EKF->NewP = EKF->LastP + EKF->Q;
	EKF->Kg = EKF->NewP / (EKF->NewP + EKF->R);
	EKF->Out = EKF->Out + EKF->Kg * (input - EKF->Out);
	EKF->LastP = (1 - EKF->Kg) * EKF->NewP;
}