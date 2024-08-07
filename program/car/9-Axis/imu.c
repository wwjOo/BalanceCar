/**
  *****************************************************************************
  * @file				imu.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#include "imu.h"



/*
俯视
	 前
     +x
     |
 +y--|--
     |
	 后	 
*/



void w2h_2d_trans(float w[XYZ],float ref_ax[XYZ],float h[XYZ])
{
	h[X] =  w[X] *  ref_ax[X]  + w[Y] *ref_ax[Y];
	h[Y] =  w[X] *(-ref_ax[Y]) + w[Y] *ref_ax[X];
}

void h2w_2d_trans(float h[XYZ],float ref_ax[XYZ],float w[XYZ])
{
	w[X] = h[X] *ref_ax[X] + h[Y] *(-ref_ax[Y]);
	w[Y] = h[X] *ref_ax[Y] + h[Y] *  ref_ax[X];
}


/* 把载体的罗盘数据转到水平面 */
void simple_3d_trans(float ref[XYZ], float in[XYZ], float out[XYZ]) /* 该函数只有在水平面附近一个有限的范围内正确 */
{
	static char pn;
	static float h_tmp_x,h_tmp_y;
	
	h_tmp_x = Sqrt(POW(ref[Z]) + POW(ref[Y]));
	h_tmp_y = Sqrt(POW(ref[Z]) + POW(ref[X]));
	
	pn = ref[Z] < 0 ? -1 : 1;
	
	out[X] = (h_tmp_x *in[X] - pn *ref[X] *in[Z]);
	out[Y] = (pn *h_tmp_y *in[Y] - ref[Y] *in[Z]);
	
	out[Z] = ref[X] *in[X] + ref[Y] *in[Y] + ref[Z] *in[Z];
}

void vec_3dh_transition(float ref[XYZ], float in[XYZ], float out[XYZ])
{
	simple_3d_trans(ref,in,out);
}

float mag_yaw_calculate(float dT,float mag_val[XYZ],float g_z_vec[XYZ],float h_mag_val[XYZ])//
{
	vec_3dh_transition(g_z_vec, mag_val, h_mag_val);
	return (fast_arctan(h_mag_val[Y], h_mag_val[X]) *57.3f);
}



_IMU imu_data = {1,0,0,0,
				   0,0,0,
				  {0,0,0},
				  {0,0,0},
			  	  {0,0,0},
				  {0,0,0}};

_IMU_STATE imu_state = {1,1,1,1,1,1};

float t[3][3];			


#define USE_MAG
void IMU_update(float T_ms,_IMU_STATE *state,float gyr[XYZ], int acc[XYZ],short mag[XYZ],_IMU *imu)
{
	u8 i;
	static u16 cnt;
	static float imu_reset_val;
	static float mag_yaw_err,mag_val_f[XYZ];
	static float kp_use=0,ki_use=0,mkp_use=0;
	static float vector_err[XYZ],vector_err_i[XYZ];	
	
	float norm_acc[XYZ];
	float acc_recipNorm,q_recipNorm;

	float	q0q1 = imu->w * imu->x,
			q0q2 = imu->w * imu->y,
			q1q1 = imu->x * imu->x,
			q1q3 = imu->x * imu->z,
			q2q2 = imu->y * imu->y,
			q2q3 = imu->y * imu->z,
			q3q3 = imu->z * imu->z,
			q1q2 = imu->x * imu->y,
			q0q3 = imu->w * imu->z;
	
	/* 四元数归一化(单位化) */
	q_recipNorm = InvSqrt(imu->w*imu->w + imu->x*imu->x + imu->y*imu->y + imu->z*imu->z);
    imu->w *= q_recipNorm;
    imu->x *= q_recipNorm;
    imu->y *= q_recipNorm;
    imu->z *= q_recipNorm;
		
	/* 三维Acc向量归一化(单位化) */
	acc_recipNorm = InvSqrt(POW(acc[X]) + POW(acc[Y]) + POW(acc[Z])); 
	norm_acc[X] = acc[X] * acc_recipNorm;				/* 机体坐标系下ACC在X轴的向量(已单位化) */
	norm_acc[Y] = acc[Y] * acc_recipNorm;				/* 机体坐标系下ACC在X轴的向量(已单位化) */
	norm_acc[Z] = acc[Z] * acc_recipNorm;				/* 机体坐标系下ACC在X轴的向量(已单位化) */
	
	/* 地理坐标系的Z轴轴重力向量转到机体坐标系 */
	/* 相当于理论的ACC在机体坐标系中的三维坐标 */
    t[2][0] = imu->z_vector[X] = 2*q1q3 - 2*q0q2;		/* 地理坐标系Z轴与机体系X轴之间的方向余弦向量 */
    t[2][1] = imu->z_vector[Y] = 2*q0q1 + 2*q2q3;		/* 地理坐标系Z轴与机体系Y轴之间的方向余弦向量 */
    t[2][2] = imu->z_vector[Z] = 1 - (2*q1q1 + 2*q2q2);	/* 地理坐标系Z轴与机体系Z轴之间的方向余弦向量 */
	
    /* 实际与理论向量的叉积(叉积 ≈ 小角度误差) */ 
	/* 叉积U×V = (Uy*Vz - Uz*Vy , Ux*Vz - Uz*Vx , Ux*Vy - Uy*Vx) */
    vector_err[X] = norm_acc[Y] * imu->z_vector[Z] - norm_acc[Z] * imu->z_vector[Y];
    vector_err[Y] = norm_acc[Z] * imu->z_vector[X] - norm_acc[X] * imu->z_vector[Z];
    vector_err[Z] = norm_acc[X] * imu->z_vector[Y] - norm_acc[Y] * imu->z_vector[X];

	#ifdef USE_MAG
	
		t[0][0] = imu->x_vector[X] = 1 - (2*q2q2 + 2*q3q3);
		t[0][1] = imu->x_vector[Y] = 2*q1q2 - 2*q0q3;
		t[0][2] = imu->x_vector[Z] = 2*q1q3 + 2*q0q2;
		                    
		t[1][0] = imu->y_vector[X] = 2*q1q2 + 2*q0q3;
		t[1][1] = imu->y_vector[Y] = 1 - (2*q1q1 + 2*q3q3);
		t[1][2] = imu->y_vector[Z] = 2*q2q3 - 2*q0q1;
	
		/* 计算航向yaw误差 */
		for(i=0; i<3; i++)
		{
			/* 准换为浮点型 - 值传递 */
			mag_val_f[i] = (float)mag[i]; 
		}

		if(!(mag[X] ==0 && mag[Y] == 0 && mag[Z] == 0))
		{
			mag_yaw_err = mag_yaw_calculate(T_ms, mag_val_f, imu->z_vector, imu->h_mag) - imu->yaw;
			mag_yaw_err = RANGE_TO_180DEG(mag_yaw_err);
		}
	#endif
	
	/* 误差补偿 */
	for(i=0; i<3; i++)
	{
		/* 误差积分(积分限值) */
 		vector_err_i[i] += LIMIT(vector_err[i],-0.01f,0.01f) * ki_use * T_ms;

		/* 补偿陀螺仪角速度 */
		#ifdef USE_MAG
			gyr[i] = gyr[i] + (vector_err[i] * kp_use + vector_err_i[i]) - mag_yaw_err *imu->z_vector[i] *mkp_use *RAD_PER_DEG;
		#else
			gyr[i] = gyr[i] + (vector_err[i] * kp_use + vector_err_i[i]);
		#endif
	}
	
    /* 更新四元数 */
    imu->w = imu->w + (-imu->x*gyr[X] - imu->y*gyr[Y] - imu->z*gyr[Z]) * (T_ms / 2);
    imu->x = imu->x + ( imu->w*gyr[X] - imu->z*gyr[Y] + imu->y*gyr[Z]) * (T_ms / 2);
    imu->y = imu->y + ( imu->z*gyr[X] + imu->w*gyr[Y] - imu->x*gyr[Z]) * (T_ms / 2);
    imu->z = imu->z + (-imu->y*gyr[X] + imu->x*gyr[Y] + imu->w*gyr[Z]) * (T_ms / 2);
		
	#ifdef USE_MAG
	
		if(state->Mag_fix_enable==0) /* 传感器异常 */
		{
			mkp_use = 0; /* 不修正 */
			state->Mag_reset = 0; /* 罗盘修正不复位,清除复位标记 */
		}
		else
		{
			if(state->Mag_reset)
			{
				/* 通过增量进行对准 */	
				mkp_use = 5.0f;
				if(mag_yaw_err != 0 && ABS(mag_yaw_err)<2)
				{
					state->Mag_reset = 0; /* 误差小于2的时候,清除复位标记 */
				}
			}
			else
			{
				mkp_use = state->mkp; /* 正常修正 */
			}
		}
	#endif
		
	/* 正常修正 */
	if(state->Gyr_reset == 0) 
	{
		kp_use = state->gkp;
		ki_use = state->gki;
	}
	/* 快速修正,通过增量进行对准 */
	else
	{
		kp_use = 10.0f;
		ki_use = 0.0f;
		
		imu_reset_val = (ABS(vector_err[X]) + ABS(vector_err[Y]));
		imu_reset_val = LIMIT(imu_reset_val,0,1.0f); /* 限值 */
		
		/* 误差若被控制 且 .... */
		if((imu_reset_val < 0.05f) && (state->Mag_reset == 0))
		{
			/* 计时 */
			cnt += 2;
			
			if(cnt > 500)
			{
				cnt = 0;
				state->Gyr_reset = 0;/* 快速修正完成,进入正常修正 */
			}
		}
		else
		{
			cnt = 0;
		}
	}
}


/* 获取姿态角 */
void Angle_calculate(void)
{	
	float temp;
	
	temp = LIMIT(1-POW(t[2][0]),0,1);
	
	/* 避免奇点的运算 */
	if(ABS(imu_data.z_vector[Z]) > 0.05f)
	{
		imu_data.pit = -fast_arctan(t[2][0],Sqrt(temp))*57.30f;
		imu_data.rol =  fast_arctan(t[2][1], t[2][2])  *57.30f; 
		imu_data.yaw = -fast_arctan(t[1][0], t[0][0])  *57.30f; 
	}
}



/* end of imu.c */



