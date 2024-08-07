/**
  *****************************************************************************
  * @file				icm20602.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#ifndef __ICM20602_H
#define __ICM20602_H
#include "sys.h"
#include "structural.h"


#define MPU_WHOAMI_20602				0x12

#define MPUREG_XG_OFFS_TC               0x00
#define MPUREG_YG_OFFS_TC               0x01
#define MPUREG_ZG_OFFS_TC               0x02
#define MPUREG_X_FINE_GAIN              0x03
#define MPUREG_Y_FINE_GAIN              0x04
#define MPUREG_Z_FINE_GAIN              0x05
#define MPUREG_XA_OFFS_H                0x06
#define MPUREG_XA_OFFS_L                0x07
#define MPUREG_YA_OFFS_H                0x08
#define MPUREG_YA_OFFS_L                0x09
#define MPUREG_ZA_OFFS_H                0x0A
#define MPUREG_ZA_OFFS_L                0x0B
#define MPUREG_PRODUCT_ID               0x0C
#define MPUREG_XG_OFFS_USRH             0x13
#define MPUREG_XG_OFFS_USRL             0x14
#define MPUREG_YG_OFFS_USRH             0x15
#define MPUREG_YG_OFFS_USRL             0x16
#define MPUREG_ZG_OFFS_USRH             0x17
#define MPUREG_ZG_OFFS_USRL             0x18
#define MPUREG_SMPLRT_DIV               0x19
#define MPUREG_SMPLRT_1000HZ            0x00
#define MPUREG_SMPLRT_500HZ             0x01
#define MPUREG_SMPLRT_250HZ             0x03
#define MPUREG_SMPLRT_200HZ             0x04
#define MPUREG_SMPLRT_100HZ             0x09
#define MPUREG_SMPLRT_50HZ              0x13
#define MPUREG_CONFIG                   0x1A
#define MPUREG_CONFIG_EXT_SYNC_SHIFT    3
#define MPUREG_CONFIG_EXT_SYNC_GX       0x02
#define MPUREG_CONFIG_EXT_SYNC_GY       0x03
#define MPUREG_CONFIG_EXT_SYNC_GZ       0x04
#define MPUREG_CONFIG_EXT_SYNC_AX       0x05
#define MPUREG_CONFIG_EXT_SYNC_AY       0x06
#define MPUREG_CONFIG_EXT_SYNC_AZ       0x07
#define MPUREG_CONFIG_FIFO_MODE_STOP    0x40
#define MPUREG_GYRO_CONFIG              0x1B

#define BITS_GYRO_FS_250DPS             0x00
#define BITS_GYRO_FS_500DPS             0x01
#define BITS_GYRO_FS_1000DPS            0x02
#define BITS_GYRO_FS_2000DPS            0x03
#define BITS_GYRO_FS_MASK               0x18
#define BITS_GYRO_ZGYRO_SELFTEST        0x20
#define BITS_GYRO_YGYRO_SELFTEST        0x40
#define BITS_GYRO_XGYRO_SELFTEST        0x80
#define MPUREG_ACCEL_CONFIG             0x1C
#define BITS_ACCEL_FS_2 				0x00
#define BITS_ACCEL_FS_4 				0x01
#define BITS_ACCEL_FS_8 				0x02
#define BITS_ACCEL_FS_16				0x03
#define MPUREG_MOT_THR    				0x1F 
#define MPUREG_MOT_DUR    				0x20
#define MPUREG_ZRMOT_THR  				0x21
#define MPUREG_ZRMOT_DUR  				0x22
#define MPUREG_FIFO_EN    				0x23
#define BIT_TEMP_FIFO_EN				0x80
#define BIT_XG_FIFO_EN					0x40
#define BIT_YG_FIFO_EN					0x20
#define BIT_ZG_FIFO_EN					0x10
#define BIT_ACCEL_FIFO_EN				0x08
#define BIT_SLV2_FIFO_EN				0x04
#define BIT_SLV1_FIFO_EN                0x02
#define BIT_SLV0_FIFI_EN0               0x01
#define MPUREG_I2C_MST_CTRL             0x24
#define BIT_I2C_MST_P_NSR               0x10
#define BIT_I2C_MST_CLK_400KHZ          0x0D
#define MPUREG_I2C_SLV0_ADDR 			0x25
#define MPUREG_I2C_SLV1_ADDR 			0x28
#define MPUREG_I2C_SLV2_ADDR 			0x2B
#define MPUREG_I2C_SLV3_ADDR 			0x2E
#define MPUREG_INT_PIN_CFG   			0x37
#define BIT_BYPASS_EN           		0x02
#define BIT_INT_RD_CLEAR        		0x10
#define BIT_LATCH_INT_EN        		0x20
#define MPUREG_I2C_SLV4_CTRL            0x34
#define MPUREG_INT_ENABLE               0x38

#define BIT_RAW_RDY_EN                  0x01
#define BIT_DMP_INT_EN                  0x02 
#define BIT_UNKNOWN_INT_EN              0x04
#define BIT_I2C_MST_INT_EN              0x08
#define BIT_FIFO_OFLOW_EN               0x10
#define BIT_ZMOT_EN                     0x20
#define BIT_MOT_EN                      0x40
#define BIT_FF_EN                       0x80
#define MPUREG_INT_STATUS               0x3A

#define BIT_RAW_RDY_INT                 0x01
#define BIT_DMP_INT                     0x02
#define BIT_UNKNOWN_INT                 0x04
#define BIT_I2C_MST_INT                 0x08
#define BIT_FIFO_OFLOW_INT              0x10
#define BIT_ZMOT_INT                    0x20
#define BIT_MOT_INT                     0x40
#define BIT_FF_INT                      0x80
#define MPUREG_ACCEL_XOUT_H             0x3B
#define MPUREG_ACCEL_XOUT_L             0x3C
#define MPUREG_ACCEL_YOUT_H             0x3D
#define MPUREG_ACCEL_YOUT_L             0x3E
#define MPUREG_ACCEL_ZOUT_H             0x3F
#define MPUREG_ACCEL_ZOUT_L             0x40
#define MPUREG_TEMP_OUT_H               0x41
#define MPUREG_TEMP_OUT_L               0x42
#define MPUREG_GYRO_XOUT_H              0x43
#define MPUREG_GYRO_XOUT_L              0x44
#define MPUREG_GYRO_YOUT_H              0x45
#define MPUREG_GYRO_YOUT_L              0x46
#define MPUREG_GYRO_ZOUT_H              0x47
#define MPUREG_GYRO_ZOUT_L              0x48
#define MPUREG_EXT_SENS_DATA_00         0x49
#define MPUREG_I2C_SLV0_DO              0x63
#define MPUREG_I2C_MST_DELAY_CTRL       0x67
#define BIT_I2C_SLV0_DLY_EN				0x01
#define BIT_I2C_SLV1_DLY_EN				0x02
#define BIT_I2C_SLV2_DLY_EN				0x04
#define BIT_I2C_SLV3_DLY_EN				0x08
#define MPUREG_USER_CTRL                0x6A

#define BIT_USER_CTRL_SIG_COND_RESET    0x01
#define BIT_USER_CTRL_I2C_MST_RESET     0x02
#define BIT_USER_CTRL_FIFO_RESET        0x04
#define BIT_USER_CTRL_DMP_RESET         0x08
#define BIT_USER_CTRL_I2C_IF_DIS        0x10
#define BIT_USER_CTRL_I2C_MST_EN        0x20
#define BIT_USER_CTRL_FIFO_EN           0x40
#define BIT_USER_CTRL_DMP_EN            0x80
#define MPUREG_PWR_MGMT_1				0x6B
#define BIT_PWR_MGMT_1_CLK_INTERNAL     0x00
#define BIT_PWR_MGMT_1_CLK_XGYRO        0x01
#define BIT_PWR_MGMT_1_CLK_YGYRO        0x02
#define BIT_PWR_MGMT_1_CLK_ZGYRO        0x03
#define BIT_PWR_MGMT_1_CLK_EXT32KHZ     0x04
#define BIT_PWR_MGMT_1_CLK_EXT19MHZ     0x05
#define BIT_PWR_MGMT_1_CLK_STOP         0x07
#define BIT_PWR_MGMT_1_TEMP_DIS         0x08
#define BIT_PWR_MGMT_1_CYCLE            0x20
#define BIT_PWR_MGMT_1_SLEEP            0x40
#define BIT_PWR_MGMT_1_DEVICE_RESET     0x80
#define MPUREG_PWR_MGMT_2				0x6C
#define MPUREG_BANK_SEL					0x6D
#define MPUREG_MEM_START_ADDR			0x6E
#define MPUREG_MEM_R_W					0x6F
#define MPUREG_DMP_CFG_1      			0x70
#define MPUREG_DMP_CFG_2      			0x71
#define MPUREG_FIFO_COUNTH    			0x72
#define MPUREG_FIFO_COUNTL    			0x73
#define MPUREG_FIFO_R_W       			0x74
#define MPUREG_WHOAMI         			0x75

#define ICM_PWR1_DEVICE_RESET_BIT  		7
#define ICM_PWR1_SLEEP_BIT         		6
#define ICM_PWR1_CYCLE_BIT         		5
#define ICM_PWR1_TEMP_DIS_BIT      		3
#define ICM_PWR1_CLKSEL_BIT        		2
#define ICM_PWR1_CLKSEL_LENGTH     		3
		
#define ICM_GCONFIG_FS_SEL_BIT     		4
#define ICM_GCONFIG_FS_SEL_LENGTH  		2

#define ICM_ACONFIG_XA_ST_BIT           7
#define ICM_ACONFIG_YA_ST_BIT           6
#define ICM_ACONFIG_ZA_ST_BIT           5
#define ICM_ACONFIG_AFS_SEL_BIT         4
#define ICM_ACONFIG_AFS_SEL_LENGTH      2
#define ICM_ACONFIG_ACCEL_HPF_BIT       2
#define ICM_ACONFIG_ACCEL_HPF_LENGTH    3

#define ICM_CFG_DLPF_CFG_BIT    		2
#define ICM_CFG_DLPF_CFG_LENGTH 		3

#define ICM_DLPF_BW_256         		0x00
#define ICM_DLPF_BW_188         		0x01
#define ICM_DLPF_BW_98          		0x02
#define ICM_DLPF_BW_42          		0x03
#define ICM_DLPF_BW_20          		0x04
#define ICM_DLPF_BW_10          		0x05
#define ICM_DLPF_BW_5           		0x06

#define ICM_INTCFG_INT_LEVEL_BIT        7
#define ICM_INTCFG_INT_OPEN_BIT         6
#define ICM_INTCFG_LATCH_INT_EN_BIT     5
#define ICM_INTCFG_INT_RD_CLEAR_BIT     4
#define ICM_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define ICM_INTCFG_FSYNC_INT_EN_BIT     2
#define ICM_INTCFG_I2C_BYPASS_EN_BIT    1
#define ICM_INTCFG_CLKOUT_EN_BIT        0

#define ICM_INTMODE_ACTIVEHIGH  		0x00
#define ICM_INTMODE_ACTIVELOW   		0x01
		
#define ICM_INTDRV_PUSHPULL     		0x00
#define ICM_INTDRV_OPENDRAIN    		0x01
		
#define ICM_INTLATCH_50USPULSE  		0x00
#define ICM_INTLATCH_WAITCLEAR  		0x01
		
#define ICM_INTCLEAR_STATUSREAD 		0x00
#define ICM_INTCLEAR_ANYREAD    		0x01

#define ICM_INTERRUPT_FF_BIT            7
#define ICM_INTERRUPT_MOT_BIT           6
#define ICM_INTERRUPT_ZMOT_BIT          5
#define ICM_INTERRUPT_FIFO_OFLOW_BIT    4
#define ICM_INTERRUPT_I2C_MST_INT_BIT   3
#define ICM_INTERRUPT_PLL_RDY_INT_BIT   2
#define ICM_INTERRUPT_DMP_INT_BIT       1
#define ICM_INTERRUPT_DATA_RDY_BIT      0


#define MPU_RA_XG_OFFS_TC       		0x00
#define MPU_RA_YG_OFFS_TC       		0x01
#define MPU_RA_ZG_OFFS_TC       		0x02
#define MPU_RA_X_FINE_GAIN      		0x03
#define MPU_RA_Y_FINE_GAIN      		0x04
#define MPU_RA_Z_FINE_GAIN      		0x05
#define MPU_RA_XA_OFFS_H        		0x06
#define MPU_RA_XA_OFFS_L_TC     		0x07
#define MPU_RA_YA_OFFS_H        		0x08
#define MPU_RA_YA_OFFS_L_TC     		0x09
#define MPU_RA_ZA_OFFS_H        		0x0A
#define MPU_RA_ZA_OFFS_L_TC     		0x0B
#define MPU_RA_PRODUCT_ID       		0x0C
#define MPU_RA_XG_OFFS_USRH     		0x13
#define MPU_RA_XG_OFFS_USRL     		0x14
#define MPU_RA_YG_OFFS_USRH     		0x15
#define MPU_RA_YG_OFFS_USRL     		0x16
#define MPU_RA_ZG_OFFS_USRH     		0x17
#define MPU_RA_ZG_OFFS_USRL     		0x18
#define MPU_RA_SMPLRT_DIV       		0x19
#define MPU_RA_CONFIG           		0x1A
#define MPU_RA_GYRO_CONFIG      		0x1B
#define MPU_RA_ACCEL_CONFIG     		0x1C
#define MPU_RA_FF_THR           		0x1D
#define MPU_RA_FF_DUR           		0x1E
#define MPU_RA_MOT_THR          		0x1F
#define MPU_RA_MOT_DUR          		0x20
#define MPU_RA_ZRMOT_THR        		0x21
#define MPU_RA_ZRMOT_DUR        		0x22
#define MPU_RA_FIFO_EN          		0x23
#define MPU_RA_I2C_MST_CTRL     		0x24
#define MPU_RA_I2C_SLV0_ADDR    		0x25
#define MPU_RA_I2C_SLV0_REG     		0x26
#define MPU_RA_I2C_SLV0_CTRL    		0x27
#define MPU_RA_I2C_SLV1_ADDR    		0x28
#define MPU_RA_I2C_SLV1_REG     		0x29
#define MPU_RA_I2C_SLV1_CTRL    		0x2A
#define MPU_RA_I2C_SLV2_ADDR    		0x2B
#define MPU_RA_I2C_SLV2_REG     		0x2C
#define MPU_RA_I2C_SLV2_CTRL    		0x2D
#define MPU_RA_I2C_SLV3_ADDR    		0x2E
#define MPU_RA_I2C_SLV3_REG     		0x2F
#define MPU_RA_I2C_SLV3_CTRL    		0x30
#define MPU_RA_I2C_SLV4_ADDR    		0x31
#define MPU_RA_I2C_SLV4_REG     		0x32
#define MPU_RA_I2C_SLV4_DO      		0x33
#define MPU_RA_I2C_SLV4_CTRL    		0x34
#define MPU_RA_I2C_SLV4_DI      		0x35
#define MPU_RA_I2C_MST_STATUS   		0x36
#define MPU_RA_INT_PIN_CFG      		0x37
#define MPU_RA_INT_ENABLE       		0x38
#define MPU_RA_DMP_INT_STATUS   		0x39
#define MPU_RA_INT_STATUS       		0x3A
#define MPU_RA_ACCEL_XOUT_H     		0x3B
#define MPU_RA_ACCEL_XOUT_L     		0x3C
#define MPU_RA_ACCEL_YOUT_H     		0x3D
#define MPU_RA_ACCEL_YOUT_L     		0x3E
#define MPU_RA_ACCEL_ZOUT_H     		0x3F
#define MPU_RA_ACCEL_ZOUT_L     		0x40
#define MPU_RA_TEMP_OUT_H       		0x41
#define MPU_RA_TEMP_OUT_L       		0x42
#define MPU_RA_GYRO_XOUT_H      		0x43
#define MPU_RA_GYRO_XOUT_L      		0x44
#define MPU_RA_GYRO_YOUT_H      		0x45
#define MPU_RA_GYRO_YOUT_L      		0x46
#define MPU_RA_GYRO_ZOUT_H      		0x47
#define MPU_RA_GYRO_ZOUT_L      		0x48
#define MPU_RA_EXT_SENS_DATA_00 		0x49
#define MPU_RA_MOT_DETECT_STATUS    	0x61
#define MPU_RA_I2C_SLV0_DO      		0x63
#define MPU_RA_I2C_SLV1_DO      		0x64
#define MPU_RA_I2C_SLV2_DO      		0x65
#define MPU_RA_I2C_SLV3_DO      		0x66
#define MPU_RA_I2C_MST_DELAY_CTRL   	0x67
#define MPU_RA_SIGNAL_PATH_RESET    	0x68
#define MPU_RA_MOT_DETECT_CTRL      	0x69
#define MPU_RA_USER_CTRL        		0x6A
#define MPU_RA_PWR_MGMT_1       		0x6B
#define MPU_RA_PWR_MGMT_2       		0x6C
#define MPU_RA_BANK_SEL         		0x6D
#define MPU_RA_MEM_START_ADDR   		0x6E
#define MPU_RA_MEM_R_W          		0x6F
#define MPU_RA_DMP_CFG_1        		0x70
#define MPU_RA_DMP_CFG_2        		0x71
#define MPU_RA_FIFO_COUNTH      		0x72
#define MPU_RA_FIFO_COUNTL      		0x73
#define MPU_RA_FIFO_R_W         		0x74
#define MPU_RA_WHO_AM_I         		0x75

#define ICM20602_LPF_250HZ       		0
#define ICM20602_LPF_176HZ       		1
#define ICM20602_LPF_92HZ        		2
#define ICM20602_LPF_41HZ        		3
#define ICM20602_LPF_20HZ        		4
#define ICM20602_LPF_10HZ        		5
#define ICM20602_LPF_5HZ         		6
#define ICM20602_LPF_3281HZ      		7

#define PN2000_TO_DEG_P_S	0.061037f	/* GYR-ADC���ݱ仯1 <==> ���ٶȱ仯0.061037f deg/s */
#define PN2000_TO_RAD_P_S	0.001065f	/* GYR-ADC���ݱ仯1 <==> ���ٶȱ仯0.001065f rad/s */
#define PN8G_TO_CM_P_SS		0.2393f		/* ACC-ADC����ת��1 <==> ���ٶȱ仯0.2393f cm/s2*/



typedef struct
{
	float distance[XYZ]; 	/* ��λ:cm */
	float gyr_rad_v[XYZ]; 	/* GYR���ٶ� */
	float gyr_rad_v_old[XYZ];
	float gyr_rad_a[XYZ];	/* GYR�Ǽ��ٶ� */
	float centri_a[XYZ];	/* ���ļ��ٶ� */
	
}_CENTER_POINT_OFFSET;



typedef struct 
{
	/* У׼��־λ */
	u8 Acc_CALIB;
	u8 Gyr_CALIB;
	u8 Acc_Z_CALIB;
	
	/* ԭʼ���ݴ洢 */
	short Acc_Original[XYZ];
	short Gyr_Original[XYZ];
	
	float Acc_filter[XYZ];
	float Gyr_filter[XYZ];
	
	float Gyr_deg[XYZ];
	float Gyr_rad[XYZ];
	int Acc_cmss[XYZ];
	
	short Tempreature_Original;
	float Tempreature;
	
}_ICM;



void Icm20602_CSPin_Init(void);
u8 Icm20602_Reg_Init(void);
void Icm20602_Read(void);
void Icm_Data_Prepare(u8 T_ms);
void Center_offset_Set(void);



extern _ICM icm;



#endif




/* end of icm20602.h */



