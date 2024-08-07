#include "main.h"



#define TARGET_ANGLE		0			/* 目标角度 */
#define SPEED_MAX			60.0f
#define TSPEED_MAX			5000.0f


_Car Car;
_PID balance;
_PID speed;
_PID turn;


void balance_PID(void)
{
	balance.err = imu_data.pit - TARGET_ANGLE;
	
	balance.out = balance.err * balance.kp + icm.Gyr_filter[Y] * balance.kd; /* Gyr (-200~200) */
}


void speed_PID(void)
{
	static float err_lpf;
	static float err_last;
	
	err_lpf = Motor.encode - Car.Speed_Set;
	
	speed.err += 0.7 * (err_lpf - speed.err);
	
	speed.integral += speed.err;
	
	speed.dout = (speed.err - err_last) * speed.kd;
	
	err_last = speed.err;
	
	speed.out = speed.err * speed.kp + speed.integral * speed.ki + speed.dout;
	
	/* 清除积分 */
	if(ABS(imu_data.pit) > STOP_ANGLE || Motor.Switch == 0)
		speed.integral = 0;
}

void turn_PID(void)
{
	static float turn_err_err,turn_err_last;
	
	turn.err = Car.Tspeed_Set - icm.Gyr_filter[Z];
	
	turn_err_err = turn.err - turn_err_last;
	
	turn.out = turn.err * turn.kp + turn_err_err * turn.kd;
	
	turn_err_last = turn.err;
}




/* 电机输出(0~8400) */
void PID_Init(void)
{
	balance.kp = 220.0f;
	balance.ki = 0.0f;
	balance.kd = 1.4f;
	
	speed.kp = 90.0f;
	speed.ki = 0.1f;
	speed.kd = 0.0f;

	turn.kp = 0.7f; 
	turn.ki = 0.0f;
	turn.kd = 0.3f;
	
}




void Car_Ctrl(void)
{
	balance_PID();
	speed_PID();
	turn_PID();
	
	Car.Speed_Set  = SPEED_MAX  * (RC.x/50.0f);
	Car.Tspeed_Set = -TSPEED_MAX * (RC.y/50.0f);
	
	if(Car.Speed_Set != 0)
	{
		speed.integral = 0;
	}
}





 

