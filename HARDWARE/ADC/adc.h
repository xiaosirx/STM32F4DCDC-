#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx.h"
#include "oled.h"

void Adc_Init(void);
void Dma_Init(u32 BufferSize, u32 Channel, u32 memory, u32 peripheral);

#endif
