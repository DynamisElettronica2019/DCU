#ifndef __data_H
#define __data_H

#include "can.h"

#define DECIMAL_SEPARATOR 														(uint8_t)'.'

#define NUMBER_OF_RECEIVED_PACKETS										(uint8_t)25
#define NUMBER_OF_ACQUIRED_CHANNELS										(uint8_t)91

#define EFI_HALL_WHEEL_ID_COUNTER_INDEX 							(uint8_t)0
#define EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX 				(uint8_t)1
#define EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX 				(uint8_t)2
#define EFI_GEAR_RPM_TPS_APPS_ID_COUNTER_INDEX 				(uint8_t)3
#define EFI_TRACTION_CONTROL_ID_COUNTER_INDEX 				(uint8_t)4
#define EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX 			(uint8_t)5
#define EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX 		(uint8_t)6
#define EFI_LOIL_EXHAUST_ID_COUNTER_INDEX 						(uint8_t)7
#define DAU_FR_ID_COUNTER_INDEX 											(uint8_t)8
#define DAU_FL_ID_COUNTER_INDEX 											(uint8_t)9
#define DAU_REAR_ID_COUNTER_INDEX 										(uint8_t)10
#define DAU_FL_IR_ID_COUNTER_INDEX 										(uint8_t)11
#define DAU_FR_IR_ID_COUNTER_INDEX 										(uint8_t)12
#define DAU_REAR_IR_RL_ID_COUNTER_INDEX 							(uint8_t)13
#define DAU_REAR_IR_RR_ID_COUNTER_INDEX 							(uint8_t)14
#define IMU1_DATA_1_ID_COUNTER_INDEX 									(uint8_t)15
#define IMU1_DATA_2_ID_COUNTER_INDEX 									(uint8_t)16
#define IMU2_DATA_1_ID_COUNTER_INDEX 									(uint8_t)17
#define IMU2_DATA_2_ID_COUNTER_INDEX 									(uint8_t)18
#define DAU_FR_DEBUG_ID_COUNTER_INDEX 								(uint8_t)19
#define DAU_FL_DEBUG_ID_COUNTER_INDEX 								(uint8_t)20
#define DAU_REAR_DEBUG_ID_COUNTER_INDEX 							(uint8_t)21
#define SW_DEBUG_ID_COUNTER_INDEX 										(uint8_t)22
#define GCU_DEBUG_1_ID_COUNTER_INDEX 									(uint8_t)23
#define GCU_DEBUG_2_ID_COUNTER_INDEX 									(uint8_t)24

#define HALL_EFFECT_FR_CSV_INDEX 											(uint16_t)0
#define HALL_EFFECT_FL_CSV_INDEX 											(uint16_t)1
#define HALL_EFFECT_RR_CSV_INDEX 											(uint16_t)2
#define HALL_EFFECT_RL_CSV_INDEX 											(uint16_t)3
#define T_H20_SX_IN_CSV_INDEX 												(uint16_t)4
#define T_H20_SX_OUT_CSV_INDEX 												(uint16_t)5
#define T_H20_DX_IN_CSV_INDEX 												(uint16_t)6
#define T_H20_DX_OUT_CSV_INDEX												(uint16_t)7
#define T_OIL_IN_CSV_INDEX 														(uint16_t)8
#define T_OIL_OUT_CSV_INDEX 													(uint16_t)9
#define T_H20_ENGINE_CSV_INDEX 												(uint16_t)10
#define BATTERY_VOLTAGE_CSV_INDEX 										(uint16_t)11
#define GEAR_CSV_INDEX 																(uint16_t)12
#define RPM_CSV_INDEX 																(uint16_t)13
#define TPS1_CSV_INDEX 																(uint16_t)14
#define PH2O_CSV_INDEX 																(uint16_t)15
#define VH_SPEED_CSV_INDEX 														(uint16_t)16
#define SLIP_TARGET_CSV_INDEX 												(uint16_t)17
#define SLIP_CSV_INDEX																(uint16_t)18
#define MANUAL_LIMITER_ACTIVE_CSV_INDEX 							(uint16_t)19
#define FAN_CSV_INDEX 																(uint16_t)20
#define H20_PUMP_DUTY_CYCLE_CSV_INDEX 								(uint16_t)21
#define PIT_LANE_ACTIVE_CSV_INDEX 										(uint16_t)22
#define FUEL_PRESSURE_CSV_INDEX 											(uint16_t)23
#define OIL_PRESSURE_CSV_INDEX 												(uint16_t)24
#define LAMBDA_CSV_INDEX 															(uint16_t)25
#define FLAG_SMOT_CSV_INDEX 													(uint16_t)26
#define L_FUEL_CSV_INDEX 															(uint16_t)27
#define T_SCARICO_1_CSV_INDEX 												(uint16_t)28
#define T_SCARICO_2_CSV_INDEX 												(uint16_t)29

#define LINEARE_FR_CSV_INDEX 													(uint16_t)30
#define LOAD_CELL_FR_CSV_INDEX 												(uint16_t)31
#define BPS_FRONT_CSV_INDEX 													(uint16_t)32
#define APPS_CSV_INDEX 																(uint16_t)33
#define LINEARE_FL_CSV_INDEX													(uint16_t)34
#define LOAD_CELL_FL_CSV_INDEX 												(uint16_t)35
#define BPS_REAR_CSV_INDEX 														(uint16_t)36
#define STEERING_WHEEL_ANGLE_CSV_INDEX 								(uint16_t)37
#define LINEARE_RL_CSV_INDEX 													(uint16_t)38
#define LOAD_CELL_RL_CSV_INDEX 												(uint16_t)39
#define LINEARE_RR_CSV_INDEX 													(uint16_t)40
#define LOAD_CELL_RR_CSV_INDEX 												(uint16_t)41
#define IR1_FL_CSV_INDEX 															(uint16_t)42
#define IR2_FL_CSV_INDEX 															(uint16_t)43
#define IR3_FL_CSV_INDEX 															(uint16_t)44
#define IR1_FR_CSV_INDEX 															(uint16_t)45
#define IR2_FR_CSV_INDEX 															(uint16_t)46
#define IR3_FR_CSV_INDEX 															(uint16_t)47
#define IR1_RL_CSV_INDEX 															(uint16_t)48
#define IR2_RL_CSV_INDEX 															(uint16_t)49
#define IR3_RL_CSV_INDEX 															(uint16_t)50
#define IR1_RR_CSV_INDEX 															(uint16_t)51
#define IR2_RR_CSV_INDEX 															(uint16_t)52
#define IR3_RR_CSV_INDEX 															(uint16_t)53

#define IMU1_ACC_X_CSV_INDEX 													(uint16_t)54
#define IMU1_ACC_Y_CSV_INDEX 													(uint16_t)55
#define IMU1_GYR_X_CSV_INDEX 													(uint16_t)56
#define IMU1_GYR_Z_CSV_INDEX 													(uint16_t)57
#define IMU1_HEADING_CSV_INDEX 												(uint16_t)58
#define IMU1_ACC_Z_CSV_INDEX 													(uint16_t)59
#define IMU1_GYR_Y_CSV_INDEX 													(uint16_t)60
#define IMU2_ACC_X_CSV_INDEX 													(uint16_t)61
#define IMU2_ACC_Y_CSV_INDEX 													(uint16_t)62
#define IMU2_GYR_X_CSV_INDEX 													(uint16_t)63
#define IMU2_GYR_Z_CSV_INDEX 													(uint16_t)64
#define IMU2_HEADING_CSV_INDEX 												(uint16_t)65
#define IMU2_ACC_Z_CSV_INDEX 													(uint16_t)66
#define IMU2_GYR_Y_CSV_INDEX 													(uint16_t)67

#define DAU_FR_TEMP_CSV_INDEX 												(uint16_t)68
#define DAU_FR_CURRENT_CSV_INDEX 											(uint16_t)69
#define DAU_FL_TEMP_CSV_INDEX 												(uint16_t)70
#define DAU_FL_CURRENT_CSV_INDEX 											(uint16_t)71
#define DAU_REAR_TEMP_CSV_INDEX 											(uint16_t)72
#define DAU_REAR_CURRENT_CSV_INDEX 										(uint16_t)73
#define SW_TEMP_CSV_INDEX 														(uint16_t)74
#define SW_CURRENT_CSV_INDEX 													(uint16_t)75
#define GCU_TEMP_CSV_INDEX 														(uint16_t)76
#define GCU_CURR_CSV_INDEX 														(uint16_t)77
#define H2O_PUMP_CURRENT_CSV_INDEX 										(uint16_t)78
#define FUEL_PUMP_CURRENT_CSV_INDEX 									(uint16_t)79
#define GEARMOTOR_CURRENT_CSV_INDEX 									(uint16_t)80
#define CLUTCH_CURRENT_CSV_INDEX 											(uint16_t)81
#define FAN_SX_CURRENT_CSV_INDEX 											(uint16_t)82
#define FAN_DX_CURRENT_CSV_INDEX 											(uint16_t)83
#define DCU_TEMP_CSV_INDEX 														(uint16_t)84
#define DCU_CURRENT_CSV_INDEX 												(uint16_t)85
#define XBEE_CURRENT_CSV_INDEX 												(uint16_t)86
#define DUC_3V3_CURRENT_CSV_INDEX 										(uint16_t)87
#define DCU_12V_VOLTAGE_CSV_INDEX 										(uint16_t)88
#define DCU_5V_VOLTAGE_CSV_INDEX 											(uint16_t)89
#define DCU_3V3_VOLTAGE_CSV_INDEX 										(uint16_t)90

extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData);
extern void packetCounterReset(void);

#endif
