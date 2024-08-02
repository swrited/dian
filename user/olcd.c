#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
 
 
u8 OLED_GRAM[144][8];
 
//��ʼ�ź�
void I2C_Start(void)
{
	OLED_SDIN_Set();delay(1);
	OLED_SCLK_Set();delay(1);
	OLED_SDIN_Clr();delay(1);
	OLED_SCLK_Clr();delay(1);
}
 
//�����ź�
void I2C_Stop(void)
{
	OLED_SCLK_Set();delay(1);
	OLED_SDIN_Clr();delay(1);
	OLED_SDIN_Set();delay(1);
}
 
//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED_SCLK_Set();delay(1);
	OLED_SCLK_Clr();delay(1);
}
 
//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		delay(1);
		if(dat&0x80)//��dat��8λ�����λ����д��
		{
			OLED_SDIN_Set();delay(1);
    }
		else
		{
			OLED_SDIN_Clr();delay(1);
    }
		OLED_SCLK_Set();//��ʱ���ź�����Ϊ�ߵ�ƽ
		delay(1);
		OLED_SCLK_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		delay(1);
		dat<<=1;
  }
}
 
//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
	else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}
 
//�����Դ浽OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
 
//��������
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}
 
//���� 
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}
 
//���һ����
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}
 
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
//ȡģ��ʽ ����ʽ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
		else return;
				for(m=0;m<8;m++)           //д������
				{
					if(temp&0x80)OLED_DrawPoint(x,y);
					else OLED_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}
 
 
//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //����
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}
 
//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//ȡģ��ʽ ����ʽ
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk1[chr1][i];}//����16*16����
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//����24*24����
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//����32*32����
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//����64*64����
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
 
//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//ȡģ��ʽ ����ʽ
void OLED_ShowChinese2(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk5[chr1][i];}//����16*16����
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//����24*24����
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//����32*32����
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//����64*64����
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
 
//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//ȡģ��ʽ ����ʽ
void OLED_ShowChinese3(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk6[chr1][i];}//����16*16����
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
 
//OLED�ĳ�ʼ��
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��PORTA~E,PORTGʱ��
 
	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 
	
	delay_ms(200);
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--�ر�OLED���
	OLED_WR_Byte(0x00,OLED_CMD);//---���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);//---���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//--������ʼ�е�ַ ����ӳ�� RAM ��ʾ��ʼ�� (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--���öԱȶȿ��ƼĴ���
	OLED_WR_Byte(0xCF,OLED_CMD);// ���� SEG �����������
	OLED_WR_Byte(0xA1,OLED_CMD);//--���� SEG/��ӳ��     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//���� COM/��ɨ�跽��   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--����������ʾ
	OLED_WR_Byte(0xA8,OLED_CMD);//--���ø��ñȣ�1 �� 64��
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-������ʾƫ���� ��λӳ�� RAM ������ (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-������
	OLED_WR_Byte(0xd5,OLED_CMD);//--������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);//--���÷�Ƶ�ȣ�����ʱ��Ϊ 100 ֡/��
	OLED_WR_Byte(0xD9,OLED_CMD);//--����Ԥ�����
	OLED_WR_Byte(0xF1,OLED_CMD);//��Ԥ�������Ϊ 15 ��ʱ�ӣ����ŵ�����Ϊ 1 ��ʱ��
	OLED_WR_Byte(0xDA,OLED_CMD);//--���� com ����Ӳ������
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--���� vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//���� VCOM ȡ��ѡ�񼶱�
	OLED_WR_Byte(0x20,OLED_CMD);//-����ҳ��Ѱַģʽ��0x00/0x01/0x02��
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--���õ�ɱ�����/����
	OLED_WR_Byte(0x14,OLED_CMD);//--���ã�0x10������
	OLED_WR_Byte(0xA4,OLED_CMD);// ����������ʾ (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// ���÷�����ʾ (0xa6/a7)
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}