#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
 
 
u8 OLED_GRAM[144][8];
 
//起始信号
void I2C_Start(void)
{
	OLED_SDIN_Set();delay(1);
	OLED_SCLK_Set();delay(1);
	OLED_SDIN_Clr();delay(1);
	OLED_SCLK_Clr();delay(1);
}
 
//结束信号
void I2C_Stop(void)
{
	OLED_SCLK_Set();delay(1);
	OLED_SDIN_Clr();delay(1);
	OLED_SDIN_Set();delay(1);
}
 
//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SCLK_Set();delay(1);
	OLED_SCLK_Clr();delay(1);
}
 
//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		delay(1);
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			OLED_SDIN_Set();delay(1);
    }
		else
		{
			OLED_SDIN_Clr();delay(1);
    }
		OLED_SCLK_Set();//将时钟信号设置为高电平
		delay(1);
		OLED_SCLK_Clr();//将时钟信号设置为低电平
		delay(1);
		dat<<=1;
  }
}
 
//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
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
 
//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
 
//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}
 
//画点 
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
 
//清除一个点
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
 
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
				for(m=0;m<8;m++)           //写入数据
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
 
 
//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //换行
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}
 
//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
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
						{temp=Hzk1[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//调用64*64字体
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
 
//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
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
						{temp=Hzk5[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=Hzk4[chr1][i];}//调用64*64字体
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
 
//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
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
						{temp=Hzk6[chr1][i];}//调用16*16字体
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
 
//OLED的初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能PORTA~E,PORTG时钟
 
	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
 
	
	delay_ms(200);
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--关闭OLED面板
	OLED_WR_Byte(0x00,OLED_CMD);//---设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);//---设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//--设置起始行地址 设置映射 RAM 显示起始行 (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--设置对比度控制寄存器
	OLED_WR_Byte(0xCF,OLED_CMD);// 设置 SEG 输出电流亮度
	OLED_WR_Byte(0xA1,OLED_CMD);//--设置 SEG/列映射     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//设置 COM/行扫描方向   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--设置正常显示
	OLED_WR_Byte(0xA8,OLED_CMD);//--设置复用比（1 到 64）
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-设置显示偏移量 移位映射 RAM 计数器 (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-不抵消
	OLED_WR_Byte(0xd5,OLED_CMD);//--设置显示时钟分频比/振荡器频率
	OLED_WR_Byte(0x80,OLED_CMD);//--设置分频比，设置时钟为 100 帧/秒
	OLED_WR_Byte(0xD9,OLED_CMD);//--设置预充电期
	OLED_WR_Byte(0xF1,OLED_CMD);//将预充电设置为 15 个时钟，将放电设置为 1 个时钟
	OLED_WR_Byte(0xDA,OLED_CMD);//--设置 com 引脚硬件配置
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--设置 vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//设置 VCOM 取消选择级别
	OLED_WR_Byte(0x20,OLED_CMD);//-设置页面寻址模式（0x00/0x01/0x02）
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--设置电荷泵启用/禁用
	OLED_WR_Byte(0x14,OLED_CMD);//--设置（0x10）禁用
	OLED_WR_Byte(0xA4,OLED_CMD);// 禁用整个显示 (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// 禁用反向显示 (0xa6/a7)
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}