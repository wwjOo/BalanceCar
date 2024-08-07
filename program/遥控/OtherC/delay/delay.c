#include "delay.h"



void delay_us(u32 us)
{
	u32 temp;
	
	SysTick->LOAD=9*us;		//使用的是8分频，即时钟为9MHz。
	SysTick->VAL=0X00;			//清空计时器，值变为9*nus.
	SysTick->CTRL=0X01;			//使能计数器
			 /*控制寄存器，第0位为使能位：0，关闭；1，开启。第1位为中断使能位：
			 0，关闭；1，开启。第2位为时钟源选择位：0，HLK/8；1，HLK。第16位为
			 计数比较标志位：初值减到0时，该位变为1，其余时为0.*/
	do
	{
		temp=SysTick->CTRL;					//读取当前倒数计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));	//等待时间达到，即计时结束
	
	SysTick->CTRL=0x00;						//关闭计数器
	SysTick->VAL=0X00;						//清空计数器
}



void delay_ms(u32 ms)
{
	u32 temp;
	
	SysTick->LOAD=9000*ms;		//使用的是8分频，即时钟为9MHz。
	SysTick->VAL=0X00;			//清空计时器，值变为9000*nus.
	SysTick->CTRL=0X01;			//使能计数器
			 /*控制寄存器，第0位为使能位：0，关闭；1，开启。第1位为中断使能位：
			 0，关闭；1，开启。第2位为时钟源选择位：0，HLK/8；1，HLK。第16位为
			 计数比较标志位：初值减到0时，该位变为1，其余时为0.*/
	do
	{
		temp=SysTick->CTRL;					//读取当前倒数计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));	//等待时间达到，即计时结束
	
	SysTick->CTRL=0x00;						//关闭计数器
	SysTick->VAL=0X00;						//清空计数器
}
