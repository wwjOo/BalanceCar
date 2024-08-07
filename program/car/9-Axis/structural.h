/**
  *****************************************************************************
  * @file				structural.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#ifndef __STRUCTURAL_H
#define __STRUCTURAL_H
#include "sys.h"


#define TAN_MAP_SIZE    256
#define TAN_MAP_RES     0.003921569f     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293f


#define ABS(X) ((X > 0) ? X : -X)
#define POW(X) (X * X)

#define RANGE_TO_180DEG(X) ((X > 180) ? (X - 360) : ((X < -180) ? (X + 360) : X))

#define LIMIT(X,min,max) ((X <= min) ? min : ((X > max) ? max : X))

#define MAX(X,Y) ((X >= Y) ? X : Y)
#define MIN(X,Y) ((X <  Y) ? X : Y)




enum
{
	X=0,
	Y,
	Z,
	XYZ,
};




enum
{
	A_X=0,
	A_Y,
	A_Z,
	G_X,
	G_Y,
	G_Z,
};




typedef struct
{
	float acc_offset[XYZ];
	float gyr_offset[XYZ];
	float mag_offset[XYZ];

} _CALIB;




typedef struct
{
	u8 gyr_ok;
	u8 acc_ok;
	u8 mag_ok;
	
	u8 motionless;
	
	u8 power_warn;
	
}_FLAG;




float InvSqrt(float x);
float Sqrt(float x);
float fast_arctan(float y, float x);

extern _CALIB calib;
extern _FLAG flag;



#endif




/* end of structural.h */

