/**
  *****************************************************************************
  * @file				Scheduler.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#include "main.h"



static void Loop_1000Hz(void) /* 1ms执行 */
{
	/* 传感器数据读取 */
	Sensor_Get();
	
	/* 惯性传感器数据准备 */
	Icm_Data_Prepare(1);
	
	/* 姿态解算更新 */
	IMU_Update_Task(1);
}


static void Loop_500Hz(void)	/* 2ms */
{	 
	
}

static void Loop_200Hz(void)	/* 5ms */
{
	
	/* 小车控制 */
	Car_Ctrl();
	
	/* 电机控制 */
	Motor_Ctrl();
}

static void Loop_100Hz(void)	/* 10ms */
{	
	/* 获取姿态角 */
	Angle_calculate();

}

static void Loop_50Hz(void)	/* 20ms执行 */
{	
	/* 罗盘数据处理任务 */
	Mag_Update_Task(20);
	
	Power_UpdateTask(20);
	/* 编码器读取 */
	Encode_Task(20);
}

char test[50];
u8 len;
static void Loop_20Hz(void)	/* 50ms执行 */
{	
	Oled_Refresh();
	
	sprintf(test,"*G%0.2f,%0.2f* ",imu_data.pit,Motor.encode);
	len = strlen(test);
	usart1_send((u8 *)test,len);
	
}

static void Loop_2Hz(void)	/* 500ms执行 */
{
}


/* 任务调度器 */
_SCHEDULER_TASK scheduler_tasks[] = 
{
	{Loop_1000Hz,1000,  0, 0},
	{Loop_500Hz , 500,  0, 0},
	{Loop_200Hz , 200,  0, 0},
	{Loop_100Hz , 100,  0, 0},
	{Loop_50Hz  ,  50,  0, 0},
	{Loop_20Hz  ,  20,  0, 0},
	{Loop_2Hz   ,   2,  0, 0},
};

/* 计算任务(线程)数量 */
#define TASK_NUM (sizeof(scheduler_tasks)/sizeof(_SCHEDULER_TASK))

/* 初始化任务表 */
void Scheduler_Setup(void)
{
	u8 index = 0; /* 标志 */
	
	for(index=0; index<TASK_NUM; index++)
	{
		/* 计算每个任务的延时周期数 */
		scheduler_tasks[index].interval_ticks = 1000/scheduler_tasks[index].rate_hz; /* 频率倒数 */
		
		/* 最短周期为1，也就是1ms */
		if(scheduler_tasks[index].interval_ticks < 1)
		{
			scheduler_tasks[index].interval_ticks = 1;
		}
	}
}

//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Scheduler_Run(void)
{
	u8 index = 0;
	
	/* 循环判断所有线程,是否应该执行 */
	for(index=0; index<TASK_NUM; index++)
	{
		/* 获取系统当前时间,单位MS */
		u32 T_now = getsystick_ms();
		
		/* 如果当前时间减去上一次执行的时间,大于等于该线程的执行周期,则执行线程 */
		if(T_now - scheduler_tasks[index].last_run >= scheduler_tasks[index].interval_ticks)
		{
			/* 更新线程的执行时间 */
			scheduler_tasks[index].last_run = T_now;
			
			/* 执行线程函数 */
			scheduler_tasks[index].task_func();
		}	 
	}
}



/* end of Scheduler.c */


