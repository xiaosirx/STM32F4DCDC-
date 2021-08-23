#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stm32f4xx.h"
#include "mydelay.h"

/***************************************************/
/*         IO操作（参考了正点原子的sys.h代码）	   */
/***************************************************/

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO操作函数
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入 

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入 

/***************************************************/



void Key_Init(void);
u8 Key_Scan(void);
void Key_Pro(u8 key);

#endif
