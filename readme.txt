DC-DC降压
	1.功能：
		通过PID调节控制输出PWM将输入电压稳定输出为指定电压，
		通过矩阵按键对输出电压进行调节，步长0.1V，OLED显示
		降压效果
	2.占用资源：
		PB15
		PF0
		PF2
		PF4
		PD9
		PD14
		PD15
		PE8			矩阵按键接口
		PA8			IIC时钟线
		PC9			IIC数据线
		PA3			TIM2 CH4 输出PWM
		PA5			ADC1 CH5 采集输出电压（0—3.3V）
		DMA2 STREAM0 CH0  		传输ADC采集的数据
		DMA2 STREAM0 IT_TC      响应优先级0，抢占优先级0  处理传输的数据
	3.说明：
		1）按键第一次按下只能识别行或者列，第一次之后可准确识别按键具体行列位置
			