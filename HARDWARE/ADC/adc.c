#include "adc.h"

void Adc_Init()
{

	GPIO_InitTypeDef GpioIniture;
	ADC_CommonInitTypeDef AdcComIniture;
	ADC_InitTypeDef AdcIniture;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//ADC≈‰÷√∏¥Œª
	
	GpioIniture.GPIO_Mode  = GPIO_Mode_AN; 
	GpioIniture.GPIO_Pin   = GPIO_Pin_5;
	GpioIniture.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GpioIniture);

	AdcComIniture.ADC_DMAAccessMode	   = ADC_DMAAccessMode_1;	
	AdcComIniture.ADC_Mode			   = ADC_Mode_Independent;
	AdcComIniture.ADC_Prescaler		   = ADC_Prescaler_Div4;
	AdcComIniture.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&AdcComIniture);
	
	AdcIniture.ADC_ContinuousConvMode   = ENABLE;
	AdcIniture.ADC_DataAlign			= ADC_DataAlign_Right;
	AdcIniture.ADC_ExternalTrigConv	    = ADC_ExternalTrigConv_T1_CC1;
	AdcIniture.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	AdcIniture.ADC_NbrOfConversion	    = 1;
	AdcIniture.ADC_Resolution  		    = ADC_Resolution_12b;
	AdcIniture.ADC_ScanConvMode 	    = DISABLE;
	ADC_Init(ADC1, &AdcIniture);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_28Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
}

void Dma_Init(u32 BufferSize, u32 Channel, u32 memory, u32 peripheral)
{
	
	DMA_InitTypeDef DmaIniture;
	NVIC_InitTypeDef NvicIniture;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream0);
	
	DmaIniture.DMA_Channel			  = Channel;
	DmaIniture.DMA_BufferSize		  = BufferSize;
	DmaIniture.DMA_FIFOMode			  = DMA_FIFOMode_Disable;
	DmaIniture.DMA_FIFOThreshold	  = DMA_FIFOThreshold_HalfFull;
	DmaIniture.DMA_DIR				  = DMA_DIR_PeripheralToMemory;
	DmaIniture.DMA_Mode				  = DMA_Mode_Circular;
	DmaIniture.DMA_Priority			  = DMA_Priority_VeryHigh;
	DmaIniture.DMA_Memory0BaseAddr	  = memory;
	DmaIniture.DMA_PeripheralBaseAddr = peripheral;
	DmaIniture.DMA_MemoryDataSize	  = DMA_MemoryDataSize_HalfWord;
	DmaIniture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DmaIniture.DMA_MemoryBurst		  = DMA_MemoryBurst_Single;
	DmaIniture.DMA_PeripheralBurst	  = DMA_PeripheralBurst_Single;
	DmaIniture.DMA_MemoryInc		  = DMA_MemoryInc_Enable;
	DmaIniture.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;	
	DMA_Init(DMA2_Stream0, &DmaIniture);
	while(DMA_GetCmdStatus(DMA2_Stream0) == ENABLE);

	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	
	NvicIniture.NVIC_IRQChannel					  = DMA2_Stream0_IRQn;
	NvicIniture.NVIC_IRQChannelCmd				  = ENABLE;
	NvicIniture.NVIC_IRQChannelPreemptionPriority = 0;
	NvicIniture.NVIC_IRQChannelSubPriority		  = 0;
	NVIC_Init(&NvicIniture);
	
	DMA_Cmd(DMA2_Stream0, ENABLE);

}

extern u16 VAL[20];
extern volatile u16 AVE;
void DMA2_Stream0_IRQHandler()
{
	u8 i;
	u32 ave = 0;
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		for(i = 0; i < 20; i ++)
		{
			ave = ave + VAL[i];
		}
		AVE = ave / 20;
	}
}
