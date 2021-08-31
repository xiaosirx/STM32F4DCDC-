#include "keyboard.h"

GPIO_InitTypeDef GpioIniture;

void Key_Init()
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | 
						   RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);
	
	GpioIniture.GPIO_Mode  = GPIO_Mode_OUT;
	GpioIniture.GPIO_Pin   = GPIO_Pin_15;
	GpioIniture.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GpioIniture.GPIO_OType = GPIO_OType_PP;
	GpioIniture.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GpioIniture);									//PB15
	
	GpioIniture.GPIO_Pin   = GPIO_Pin_15 | GPIO_Pin_9 | GPIO_Pin_14;//PD4, PD9, PD15
	GPIO_Init(GPIOD, &GpioIniture);
	
	GpioIniture.GPIO_Pin   = GPIO_Pin_8;							//PE8
	GPIO_Init(GPIOE, &GpioIniture);
	
	GpioIniture.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;  //PF0, PF2, PF4
	GPIO_Init(GPIOF, &GpioIniture);
	
}

u8 Key_Scan()
{
	u8 Key = 0, temp = 0;
	
	GpioIniture.GPIO_Mode  = GPIO_Mode_OUT;
	GpioIniture.GPIO_Speed = GPIO_Speed_100MHz;
	GpioIniture.GPIO_Pin   = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GpioIniture);	
	GpioIniture.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOF, &GpioIniture);	
	GpioIniture.GPIO_Mode  = GPIO_Mode_IN;
	GpioIniture.GPIO_Pin   = GPIO_Pin_8;
	GPIO_Init(GPIOE, &GpioIniture);	
	GpioIniture.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GpioIniture);		//改变IO方向
	
	PBout(15) = 1;
	PFout(0)  = 1;
	PFout(2)  = 1;
	PFout(4)  = 1;
	
	temp = PDin(9)  * 1 + PDin(14) * 2 
		 + PDin(15) * 4 + PEin(8)  * 8;//用temp的低四位记录按键按下
	if(temp != 0x00)
	{
		delay_ms(5);
		temp = PDin(9)  * 1 + PDin(14) * 2 
			 + PDin(15) * 4 + PEin(8)  * 8;
		if(temp != 0x00)
		{
			switch(temp)
			{
				case 0x01 : Key = 1; break;
				case 0x02 : Key = 2; break;
				case 0x04 : Key = 3; break;
				case 0x08 : Key = 4; break;
			}
			
			GpioIniture.GPIO_Mode  = GPIO_Mode_IN;
			GpioIniture.GPIO_Pin   = GPIO_Pin_15;
			GPIO_Init(GPIOB, &GpioIniture);	
			GpioIniture.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
			GPIO_Init(GPIOF, &GpioIniture);	
			GpioIniture.GPIO_Mode  = GPIO_Mode_OUT;
			GpioIniture.GPIO_Pin   = GPIO_Pin_8;
			GpioIniture.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOE, &GpioIniture);	
			GpioIniture.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_Init(GPIOD, &GpioIniture);		//改变IO方向
			
			PDout(9)  = 1;
			PDout(14) = 1;
			PDout(15) = 1;
			PEout(8)  = 1;
			delay_ms(5);		//等待反应
			
			temp = PBin(15) * 1 + PFin(0) * 2
				 + PFin(2)  * 4 + PFin(4) * 8;
			switch(temp)
			{
				case 0x01 : Key += 0;  break;
				case 0x02 : Key += 4;  break;
				case 0x04 : Key += 8;  break;
				case 0x08 : Key += 12; break;
			}
			while(PBin(15) | PFin(0) | PFin(2) | PFin(4));	//不支持连续按
			return Key;
		}
	}
	return 0;
}

u16 multi10(u8 num, u8 pow)	//num乘10的pow次方
{
	while(pow)
	{
		num *= 10;
		pow --;
	}
	return num;
}

float devide10(u8 num, u8 pow)	//num除10的pow次方
{
	float re = num;
	while(pow)
	{
		re = re / 10;
		pow --;
	}
	return re;
}



/***************全局变量****************/
extern float G_VOL;					   
u8 STA = 0, POW = 0, DOT = 0, DAT[9];  

/**********************************/
/*			 按键功能		      */
/**********************************/
void key_3()			//小数点
{
	DOT = POW;
}
	
void key_4()			//END
{
	if(STA == 1)
	{
		u8 i;
		float value = 0;
		if(DOT == 0)//没有输入小数点
		{
			for(i = 0; i < POW; i ++)
				value = value + multi10(DAT[i], POW - i - 1);
		}
		else//输入了小数点
		{
			for(i = 0; i < DOT; i ++)
				value = value + multi10(DAT[i], DOT - i - 1);
			for(i = DOT; i < POW; i ++)
				value = value + devide10(DAT[i], i - DOT + 1); 
		}														//得到输入值
		G_VOL = value;
		STA = 0;
		POW = 0;
		DOT = 0;
	}
}

void key_num(u8 num)    //输入数字
{
	if(STA == 1)
	{
		DAT[POW] = num;
		POW ++;
	}
}

extern float Offset;
void key_8()		   //START
{
	STA = 1;
	Offset = 0;
}

void key_12()		  //修改偏置，步长0.1
{
	if(STA == 0)	  //只有不处于输入数据模式时，才允许修改偏置
		Offset -= 0.1;	
}

void key_16()		
{
	if(STA == 0)
		Offset += 0.1;	
}

/**********************************/
/*			 按键处理		      */
/**********************************/
void Key_Pro(u8 key)
{
	switch(key)
	{
		case 2 : key_num(0); break;
		case 3 : key_3();	 break;
		case 4 : key_4();	 break;
		case 5 : key_num(1); break;
		case 6 : key_num(2); break;
		case 7 : key_num(3); break;
		case 8 : key_8();	 break;
		case 9 : key_num(4); break;
		case 10: key_num(5); break;
		case 11: key_num(6); break;
		case 12: key_12()  ; break;
		case 13: key_num(7); break;
		case 14: key_num(8); break;
		case 15: key_num(9); break;
		case 16: key_16();   break;
	}
}
