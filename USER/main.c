#include "stm32f4xx.h"
#include "adc.h"
#include "pwm.h"
#include "oled.h"
#include "keyboard.h"

float     G_VOL  = 8,				    //目标电压
		  Offset = 0,					//目标偏置（手动改变）
		  RT_G_VAL, 					//实时目标ADC值
		  RT_G_VOL;						//实时目标电压值

#define 	P  		0.1				//比例环节
#define 	I  		0.05			//积分环节
#define 	D		0.05			//微分环节
float     	K   =   4095 / 11 / 3.3;	//电压转换系数（减少运算量）

float VAL[20];							//ADC采集值
volatile u16 AVE = 0;					//ADC采集的平均值
			
void PID_Adjust()			//PID调节
{
	static int error[3] = {0, 0, 0}, PWM = 500;
	int error_sum;
	float error_speed;
	error[2] = error[1];
	error[1] = error[0];
	error[0] = RT_G_VAL - AVE;
	error_sum = error[0] + error[1] + error[2];
	error_speed = (error[0] - error[2]) / 2;
	PWM = PWM + P * error[0] + I * error_sum + D * error_speed;
	if(PWM > 1000 | PWM < 0)	//限幅
	{
		PWM = 500;
		error_sum = 0;
	}
	TIM_SetCompare4(TIM2, PWM);
}

int main()
{
	u8 key;
	float VOL;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Key_Init();
	OLED_Init();
	OLED_Clear();
	Pwm_Init(1 -1, 1000 - 1);
	Adc_Init();
	Dma_Init(20, DMA_Channel_0, (u32)VAL, 0x40012000 + 0x4C);
	ADC_SoftwareStartConv(ADC1);	//初始化
	
	OLED_ShowString(0, 0, "G_VAL:", 16);
	OLED_ShowString(0, 2, "VAL  :", 16);	
	OLED_ShowString(0, 4, "G_VAL:  .   V", 16);
	OLED_ShowString(0, 6, "VOL  :  .   V", 16);		//固定字符在OLED
	
	while(1)
	{
		key = Key_Scan();
		Key_Pro(key);
		RT_G_VOL = G_VOL + Offset;									  
		RT_G_VAL = RT_G_VOL * K;			        				   
		VOL = AVE / K;							   					   //实时电压
		
		OLED_ShowNum(48, 0, (int)RT_G_VAL, 4, 16);					   //显示ADC目标值(整数部分)
		OLED_ShowNum(48, 4, (int)RT_G_VOL, 2, 16);
		OLED_ShowNum(72, 4, (u16)(RT_G_VOL * 1000) % 1000, 3, 16);     //显示目标电压
		OLED_ShowNum(48, 2, AVE, 4, 16);							   //显示实时ADC值
		OLED_ShowNum(48, 6, (int)VOL, 2, 16);
		OLED_ShowNum(72, 6, (u16)(VOL * 1000) % 1000, 3, 16);		   //显示实时电压
		OLED_ShowNum(100, 0, key, 2, 16);							   //显示键值（调试使用）
		PID_Adjust();												   
	}
}

/*******显示效果******/
/*G_VAL:2978	 0  */
/*VAL  :2778	 	*/	
/*G_VOL: 8.000V		*/	
/*VOL  : 8.459V		*/	
/********************/

