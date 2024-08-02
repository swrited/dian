#include "ml_oled_font.h"
#include "math.h"
#include "ml_oled.h"
#include "ml_delay.h"
// oled����ο��Խ�Э�Ƽ� �ش�˵��

/*��������*/
#define OLED_W_SCL(x)		gpio_set(OLED_GPIO,OLED_SCL_Pin,x)
#define OLED_W_SDA(x)		gpio_set(OLED_GPIO,OLED_SDA_Pin,x)


/*���ų�ʼ��*/
void OLED_I2C_Init(void)
{
  gpio_init(OLED_GPIO,OLED_SCL_Pin,OLED_SCL,OUT);
  gpio_init(OLED_GPIO,OLED_SDA_Pin,OLED_SDA,OUT);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C��ʼ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2Cֹͣ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL(0);
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	I2C_WaitAck();
	OLED_I2C_SendByte(0x40);		//д����
	I2C_WaitAck();
	OLED_I2C_SendByte(Data);
	I2C_WaitAck();
	OLED_I2C_Stop();
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�õ�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�ø�4λ
}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED��ʾ������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  int_Length Ҫ��ʾ�����ĳ��ȣ���Χ��1~16
  * @param  float_Length Ҫ��ʾС���ĳ��ȣ���Χ��1~16
  * @retval ��
  */

void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Number, uint8_t int_Length, uint8_t float_Length)
{
	int temp1, temp2;
	temp1 = (int)Number;
	temp2 = (Number - temp1) * pow(10, float_Length);
	OLED_ShowSignedNum(Line, Column, temp1, int_Length);
  OLED_ShowString(Line, Column+int_Length+1, ".");
	OLED_ShowNum(Line, Column+int_Length+2, temp2, float_Length);
}


/**
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	delay_ms(20);
	
	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);//--turn off oled panel
	OLED_WriteCommand(0x00);//---set low column address
	OLED_WriteCommand(0x10);//---set high column address
	OLED_WriteCommand(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCommand(0x81);//--set contrast control register
	OLED_WriteCommand(0xCF); // Set SEG Output Current Brightness
	OLED_WriteCommand(0xA0);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WriteCommand(0xC0);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WriteCommand(0xA6);//--set normal display
	OLED_WriteCommand(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WriteCommand(0x3f);//--1/64 duty
	OLED_WriteCommand(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCommand(0x00);//-not offset
	OLED_WriteCommand(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WriteCommand(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCommand(0xD9);//--set pre-charge period
	OLED_WriteCommand(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteCommand(0xDA);//--set com pins hardware configuration
	OLED_WriteCommand(0x12);
	OLED_WriteCommand(0xDB);//--set vcomh
	OLED_WriteCommand(0x40);//Set VCOM Deselect Level
	OLED_WriteCommand(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCommand(0x02);//
	OLED_WriteCommand(0x8D);//--set Charge Pump enable/disable
	OLED_WriteCommand(0x14);//--set(0x10) disable
	OLED_WriteCommand(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteCommand(0xA6);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WriteCommand(0xAF); /*display ON*/ 
	OLED_Clear();
	OLED_WriteCommand(0xAF); /*display ON*/ 
}



/***********����������
��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),
x�ķ�Χ0��127��
yΪҳ�ķ�Χ0��8
*****************/
 
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetCursor(y,x0);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WriteData(BMP[j++]);	    	
	    }
	}
} 

