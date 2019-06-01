#ifndef __data_H
#define __data_H

#include "can.h"

#define CHANNEL_SEPARATION																		(uint8_t)';'
#define DECIMAL_SEPARATOR 																		(uint8_t)'.'
#define END_LINE																							(uint8_t)'\n'
#define NUMBER_OF_RECEIVED_PACKETS														(uint8_t)25
#define NUMBER_OF_ACQUIRED_CHANNELS														(uint8_t)91
#define BUFFER_STATE_LEN 																			(uint8_t)9
#define BUFFER_BLOCK_LEN 																			(uint16_t)577
#define BUFFER_POINTERS_NUMBER 																(uint8_t)2
#define CLOSE_FILE_INTERVAL																		(uint32_t)60000

/* Packet position in CAN data packet counter vector */
#define EFI_HALL_WHEEL_ID_COUNTER_INDEX 											(uint8_t)0
#define EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX 								(uint8_t)1
#define EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX 								(uint8_t)2
#define EFI_MANUAL_LIMITER_FAN_H2O_PIT_LANE_COUNTER_INDEX			(uint8_t)3
#define EFI_TRACTION_CONTROL_ID_COUNTER_INDEX 								(uint8_t)4
#define EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX 							(uint8_t)5
#define EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX 						(uint8_t)6
#define EFI_LOIL_EXHAUST_ID_COUNTER_INDEX 										(uint8_t)7
#define DAU_FR_ID_COUNTER_INDEX 															(uint8_t)8
#define DAU_FL_ID_COUNTER_INDEX 															(uint8_t)9
#define DAU_REAR_ID_COUNTER_INDEX 														(uint8_t)10
#define DAU_FL_IR_ID_COUNTER_INDEX 														(uint8_t)11
#define DAU_FR_IR_ID_COUNTER_INDEX 														(uint8_t)12
#define DAU_REAR_IR_RL_ID_COUNTER_INDEX 											(uint8_t)13
#define DAU_REAR_IR_RR_ID_COUNTER_INDEX 											(uint8_t)14
#define IMU1_DATA_1_ID_COUNTER_INDEX 													(uint8_t)15
#define IMU1_DATA_2_ID_COUNTER_INDEX 													(uint8_t)16
#define IMU2_DATA_1_ID_COUNTER_INDEX 													(uint8_t)17
#define IMU2_DATA_2_ID_COUNTER_INDEX 													(uint8_t)18
#define DAU_FR_DEBUG_ID_COUNTER_INDEX 												(uint8_t)19
#define DAU_FL_DEBUG_ID_COUNTER_INDEX 												(uint8_t)20
#define DAU_REAR_DEBUG_ID_COUNTER_INDEX 											(uint8_t)21
#define SW_DEBUG_ID_COUNTER_INDEX 														(uint8_t)22
#define GCU_DEBUG_1_ID_COUNTER_INDEX 													(uint8_t)23
#define GCU_DEBUG_2_ID_COUNTER_INDEX 													(uint8_t)24

/* Data index in CSV file */
#define TIMESTAMP_CSV_INDEX 																	(uint16_t)0
#define HALL_EFFECT_FR_CSV_INDEX 															(uint16_t)8
#define HALL_EFFECT_FL_CSV_INDEX 															(uint16_t)14
#define HALL_EFFECT_RR_CSV_INDEX 															(uint16_t)20
#define HALL_EFFECT_RL_CSV_INDEX 															(uint16_t)26
#define T_H20_SX_IN_CSV_INDEX 																(uint16_t)32
#define T_H20_SX_OUT_CSV_INDEX 																(uint16_t)36
#define T_H20_DX_IN_CSV_INDEX 																(uint16_t)40
#define T_H20_DX_OUT_CSV_INDEX																(uint16_t)44
#define T_OIL_IN_CSV_INDEX 																		(uint16_t)48
#define T_OIL_OUT_CSV_INDEX 																	(uint16_t)52
#define T_H20_ENGINE_CSV_INDEX 																(uint16_t)56
#define BATTERY_VOLTAGE_CSV_INDEX 														(uint16_t)60
#define GEAR_CSV_INDEX 																				(uint16_t)66
#define RPM_CSV_INDEX 																				(uint16_t)68
#define TPS1_CSV_INDEX 																				(uint16_t)74
#define PH2O_CSV_INDEX 																				(uint16_t)78
#define VH_SPEED_CSV_INDEX 																		(uint16_t)84
#define SLIP_TARGET_CSV_INDEX 																(uint16_t)90
#define SLIP_CSV_INDEX																				(uint16_t)96
#define MANUAL_LIMITER_ACTIVE_CSV_INDEX 											(uint16_t)104
#define FAN_CSV_INDEX 																				(uint16_t)106
#define H20_PUMP_DUTY_CYCLE_CSV_INDEX 												(uint16_t)108
#define PIT_LANE_ACTIVE_CSV_INDEX 														(uint16_t)112
#define FUEL_PRESSURE_CSV_INDEX 															(uint16_t)114
#define OIL_PRESSURE_CSV_INDEX 																(uint16_t)120
#define LAMBDA_CSV_INDEX 																			(uint16_t)126
#define FLAG_SMOT_CSV_INDEX 																	(uint16_t)132
#define L_FUEL_CSV_INDEX 																			(uint16_t)138
#define T_SCARICO_1_CSV_INDEX 																(uint16_t)144
#define T_SCARICO_2_CSV_INDEX 																(uint16_t)148
#define LINEARE_FR_CSV_INDEX 																	(uint16_t)152
#define LOAD_CELL_FR_CSV_INDEX 																(uint16_t)158
#define BPS_FRONT_CSV_INDEX 																	(uint16_t)164
#define APPS_CSV_INDEX 																				(uint16_t)170
#define LINEARE_FL_CSV_INDEX																	(uint16_t)174
#define LOAD_CELL_FL_CSV_INDEX 																(uint16_t)180
#define BPS_REAR_CSV_INDEX 																		(uint16_t)186
#define STEERING_WHEEL_ANGLE_CSV_INDEX 												(uint16_t)192
#define LINEARE_RL_CSV_INDEX 																	(uint16_t)199
#define LOAD_CELL_RL_CSV_INDEX 																(uint16_t)205
#define LINEARE_RR_CSV_INDEX 																	(uint16_t)211
#define LOAD_CELL_RR_CSV_INDEX 																(uint16_t)217
#define IR1_FL_CSV_INDEX 																			(uint16_t)223
#define IR2_FL_CSV_INDEX 																			(uint16_t)227
#define IR3_FL_CSV_INDEX 																			(uint16_t)231
#define BRAKE_IR_FL_CSV_INDEX 																(uint16_t)235
#define IR1_FR_CSV_INDEX 																			(uint16_t)239
#define IR2_FR_CSV_INDEX 																			(uint16_t)243
#define IR3_FR_CSV_INDEX 																			(uint16_t)247
#define BRAKE_IR_FR_CSV_INDEX																	(uint16_t)251
#define IR1_RL_CSV_INDEX 																			(uint16_t)255
#define IR2_RL_CSV_INDEX 																			(uint16_t)259
#define IR3_RL_CSV_INDEX 																			(uint16_t)263
#define BRAKE_IR_RL_CSV_INDEX 																(uint16_t)267
#define IR1_RR_CSV_INDEX 																			(uint16_t)271
#define IR2_RR_CSV_INDEX 																			(uint16_t)275
#define IR3_RR_CSV_INDEX 																			(uint16_t)279
#define BRAKE_IR_RR_CSV_INDEX 																(uint16_t)283
#define IMU1_ACC_X_CSV_INDEX 																	(uint16_t)287
#define IMU1_ACC_Y_CSV_INDEX 																	(uint16_t)295
#define IMU1_GYR_X_CSV_INDEX 																	(uint16_t)303
#define IMU1_GYR_Z_CSV_INDEX 																	(uint16_t)310
#define IMU1_HEADING_CSV_INDEX 																(uint16_t)317
#define IMU1_ACC_Z_CSV_INDEX 																	(uint16_t)325
#define IMU1_GYR_Y_CSV_INDEX 																	(uint16_t)333
#define IMU1_SENSORS_CALIBRATION_CSV_INDEX										(uint16_t)340
#define IMU2_ACC_X_CSV_INDEX 																	(uint16_t)344
#define IMU2_ACC_Y_CSV_INDEX 																	(uint16_t)352
#define IMU2_GYR_X_CSV_INDEX 																	(uint16_t)360
#define IMU2_GYR_Z_CSV_INDEX 																	(uint16_t)367
#define IMU2_HEADING_CSV_INDEX 																(uint16_t)374
#define IMU2_ACC_Z_CSV_INDEX 																	(uint16_t)382
#define IMU2_GYR_Y_CSV_INDEX 																	(uint16_t)390
#define IMU2_SENSORS_CALIBRATION_CSV_INDEX 										(uint16_t)397
#define IMU_DCU_HEADING_CSV_INDEX 														(uint16_t)401
#define IMU_DCU_ACC_Z_CSV_INDEX 															(uint16_t)409
#define IMU_DCU_GYR_Y_CSV_INDEX 															(uint16_t)417
#define IMU_DCU_ACC_X_CSV_INDEX 															(uint16_t)424
#define IMU_DCU_ACC_Y_CSV_INDEX 															(uint16_t)432
#define IMU_DCU_GYR_X_CSV_INDEX 															(uint16_t)440
#define IMU_DCU_GYR_Z_CSV_INDEX 															(uint16_t)447
#define IMU_DCU_SENSORS_CALIBRATION_CSV_INDEX									(uint16_t)454
#define GPS_LATITUDE_MINUTES_CSV_INDEX												(uint16_t)458	
#define GPS_LONGITUDE_MINUTES_CSV_INDEX 											(uint16_t)464
#define GPS_SPEED_CSV_INDEX	 																	(uint16_t)470
#define DAU_FR_TEMP_CSV_INDEX 																(uint16_t)478
#define DAU_FR_CURRENT_CSV_INDEX 															(uint16_t)481
#define DAU_FL_TEMP_CSV_INDEX 																(uint16_t)485
#define DAU_FL_CURRENT_CSV_INDEX 															(uint16_t)488
#define DAU_REAR_TEMP_CSV_INDEX 															(uint16_t)492
#define DAU_REAR_CURRENT_CSV_INDEX 														(uint16_t)495
#define SW_TEMP_CSV_INDEX 																		(uint16_t)499
#define SW_CURRENT_CSV_INDEX 																	(uint16_t)502
#define GCU_TEMP_CSV_INDEX 																		(uint16_t)507
#define GCU_CURR_CSV_INDEX 																		(uint16_t)510
#define H2O_PUMP_CURRENT_CSV_INDEX 														(uint16_t)514
#define FUEL_PUMP_CURRENT_CSV_INDEX 													(uint16_t)519
#define GEARMOTOR_CURRENT_CSV_INDEX 													(uint16_t)524
#define CLUTCH_CURRENT_CSV_INDEX 															(uint16_t)529
#define FAN_SX_CURRENT_CSV_INDEX 															(uint16_t)534
#define FAN_DX_CURRENT_CSV_INDEX 															(uint16_t)539
#define DCU_TEMP_CSV_INDEX 																		(uint16_t)544
#define DCU_CURRENT_CSV_INDEX 																(uint16_t)547
#define XBEE_CURRENT_CSV_INDEX 																(uint16_t)552
#define DUC_3V3_CURRENT_CSV_INDEX 														(uint16_t)556
#define DCU_12V_VOLTAGE_CSV_INDEX 														(uint16_t)560
#define DCU_5V_VOLTAGE_CSV_INDEX 															(uint16_t)566
#define DCU_3V3_VOLTAGE_CSV_INDEX 														(uint16_t)571
#define END_CSV_INDEX 																				(uint16_t)576

/* Data separator position in CSV file */
#define HALL_EFFECT_FR_CSV_SEPARATOR													(HALL_EFFECT_FR_CSV_INDEX - 1)
#define HALL_EFFECT_FL_CSV_SEPARATOR													(HALL_EFFECT_FL_CSV_INDEX - 1)
#define HALL_EFFECT_RR_CSV_SEPARATOR 													(HALL_EFFECT_RR_CSV_INDEX - 1)
#define HALL_EFFECT_RL_CSV_SEPARATOR 													(HALL_EFFECT_RL_CSV_INDEX - 1)
#define T_H20_SX_IN_CSV_SEPARATOR 														(T_H20_SX_IN_CSV_INDEX - 1)
#define T_H20_SX_OUT_CSV_SEPARATOR 														(T_H20_SX_OUT_CSV_INDEX - 1)
#define T_H20_DX_IN_CSV_SEPARATOR															(T_H20_DX_IN_CSV_INDEX - 1)
#define T_H20_DX_OUT_CSV_SEPARATOR														(T_H20_DX_OUT_CSV_INDEX - 1)
#define T_OIL_IN_CSV_SEPARATOR 																(T_OIL_IN_CSV_INDEX - 1)
#define T_OIL_OUT_CSV_SEPARATOR																(T_OIL_OUT_CSV_INDEX - 1)
#define T_H20_ENGINE_CSV_SEPARATOR														(T_H20_ENGINE_CSV_INDEX - 1)
#define BATTERY_VOLTAGE_CSV_SEPARATOR								  				(BATTERY_VOLTAGE_CSV_INDEX - 1)
#define GEAR_CSV_SEPARATOR																		(GEAR_CSV_INDEX - 1)
#define RPM_CSV_SEPARATOR 																		(RPM_CSV_INDEX - 1)
#define TPS1_CSV_SEPARATOR 																		(TPS1_CSV_INDEX - 1)
#define PH2O_CSV_SEPARATOR 																		(PH2O_CSV_INDEX - 1)
#define VH_SPEED_CSV_SEPARATOR 																(VH_SPEED_CSV_INDEX - 1)
#define SLIP_TARGET_CSV_SEPARATOR 														(SLIP_TARGET_CSV_INDEX - 1)
#define SLIP_CSV_SEPARATOR																		(SLIP_CSV_INDEX - 1)
#define MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR 									(MANUAL_LIMITER_ACTIVE_CSV_INDEX - 1)
#define FAN_CSV_SEPARATOR																			(FAN_CSV_INDEX - 1)
#define H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR 										(H20_PUMP_DUTY_CYCLE_CSV_INDEX - 1)
#define PIT_LANE_ACTIVE_CSV_SEPARATOR 												(PIT_LANE_ACTIVE_CSV_INDEX - 1)
#define FUEL_PRESSURE_CSV_SEPARATOR 													(FUEL_PRESSURE_CSV_INDEX - 1)
#define OIL_PRESSURE_CSV_SEPARATOR 														(OIL_PRESSURE_CSV_INDEX - 1)
#define LAMBDA_CSV_SEPARATOR 																	(LAMBDA_CSV_INDEX - 1)
#define FLAG_SMOT_CSV_SEPARATOR 															(FLAG_SMOT_CSV_INDEX - 1)
#define L_FUEL_CSV_SEPARATOR 																	(L_FUEL_CSV_INDEX - 1)
#define T_SCARICO_1_CSV_SEPARATOR 														(T_SCARICO_1_CSV_INDEX - 1)
#define T_SCARICO_2_CSV_SEPARATOR 														(T_SCARICO_2_CSV_INDEX - 1)
#define LINEARE_FR_CSV_SEPARATOR 															(LINEARE_FR_CSV_INDEX - 1)
#define LOAD_CELL_FR_CSV_SEPARATOR														(LOAD_CELL_FR_CSV_INDEX - 1)
#define BPS_FRONT_CSV_SEPARATOR 															(BPS_FRONT_CSV_INDEX - 1)
#define APPS_CSV_SEPARATOR 																		(APPS_CSV_INDEX - 1)
#define LINEARE_FL_CSV_SEPARATOR															(LINEARE_FL_CSV_INDEX - 1)
#define LOAD_CELL_FL_CSV_SEPARATOR 														(LOAD_CELL_FL_CSV_INDEX - 1)
#define BPS_REAR_CSV_SEPARATOR 																(BPS_REAR_CSV_INDEX - 1)
#define STEERING_WHEEL_ANGLE_CSV_SEPARATOR 										(STEERING_WHEEL_ANGLE_CSV_INDEX - 1)
#define LINEARE_RL_CSV_SEPARATOR 															(LINEARE_RL_CSV_INDEX - 1)
#define LOAD_CELL_RL_CSV_SEPARATOR 														(LOAD_CELL_RL_CSV_INDEX - 1)
#define LINEARE_RR_CSV_SEPARATOR 															(LINEARE_RR_CSV_INDEX - 1)
#define LOAD_CELL_RR_CSV_SEPARATOR 														(LOAD_CELL_RR_CSV_INDEX - 1)
#define IR1_FL_CSV_SEPARATOR 																	(IR1_FL_CSV_INDEX - 1)
#define IR2_FL_CSV_SEPARATOR 																	(IR2_FL_CSV_INDEX - 1)
#define IR3_FL_CSV_SEPARATOR 																	(IR3_FL_CSV_INDEX - 1)
#define BRAKE_IR_FL_CSV_SEPARATOR															(BRAKE_IR_FL_CSV_INDEX - 1)
#define IR1_FR_CSV_SEPARATOR 																	(IR1_FR_CSV_INDEX - 1)
#define IR2_FR_CSV_SEPARATOR 																	(IR2_FR_CSV_INDEX - 1)
#define IR3_FR_CSV_SEPARATOR 																	(IR3_FR_CSV_INDEX - 1)
#define BRAKE_IR_FR_CSV_SEPARATOR 														(BRAKE_IR_FR_CSV_INDEX - 1)
#define IR1_RL_CSV_SEPARATOR 																	(IR1_RL_CSV_INDEX - 1)
#define IR2_RL_CSV_SEPARATOR 																	(IR2_RL_CSV_INDEX - 1)
#define IR3_RL_CSV_SEPARATOR 																	(IR3_RL_CSV_INDEX - 1)
#define BRAKE_IR_RL_CSV_SEPARATOR 														(BRAKE_IR_RL_CSV_INDEX - 1)
#define IR1_RR_CSV_SEPARATOR 																	(IR1_RR_CSV_INDEX - 1)
#define IR2_RR_CSV_SEPARATOR 																	(IR2_RR_CSV_INDEX - 1)
#define IR3_RR_CSV_SEPARATOR 																	(IR3_RR_CSV_INDEX - 1)
#define BRAKE_IR_RR_CSV_SEPARATOR 														(BRAKE_IR_RR_CSV_INDEX - 1)
#define IMU1_ACC_X_CSV_SEPARATOR 															(IMU1_ACC_X_CSV_INDEX - 1)
#define IMU1_ACC_Y_CSV_SEPARATOR 															(IMU1_ACC_Y_CSV_INDEX - 1)
#define IMU1_GYR_X_CSV_SEPARATOR 															(IMU1_GYR_X_CSV_INDEX - 1)
#define IMU1_GYR_Z_CSV_SEPARATOR 															(IMU1_GYR_Z_CSV_INDEX - 1)
#define IMU1_HEADING_CSV_SEPARATOR 														(IMU1_HEADING_CSV_INDEX - 1)
#define IMU1_ACC_Z_CSV_SEPARATOR 															(IMU1_ACC_Z_CSV_INDEX - 1)
#define IMU1_GYR_Y_CSV_SEPARATOR 															(IMU1_GYR_Y_CSV_INDEX - 1)
#define IMU1_SENSORS_CALIBRATION_CSV_SEPARATOR								(IMU1_SENSORS_CALIBRATION_CSV_INDEX - 1)
#define IMU2_ACC_X_CSV_SEPARATOR 															(IMU2_ACC_X_CSV_INDEX - 1)
#define IMU2_ACC_Y_CSV_SEPARATOR 															(IMU2_ACC_Y_CSV_INDEX - 1)
#define IMU2_GYR_X_CSV_SEPARATOR 															(IMU2_GYR_X_CSV_INDEX - 1)
#define IMU2_GYR_Z_CSV_SEPARATOR 															(IMU2_GYR_Z_CSV_INDEX - 1)
#define IMU2_HEADING_CSV_SEPARATOR 														(IMU2_HEADING_CSV_INDEX - 1)
#define IMU2_ACC_Z_CSV_SEPARATOR 															(IMU2_ACC_Z_CSV_INDEX - 1)
#define IMU2_GYR_Y_CSV_SEPARATOR 															(IMU2_GYR_Y_CSV_INDEX - 1)
#define IMU2_SENSORS_CALIBRATION_CSV_SEPARATOR 								(IMU2_SENSORS_CALIBRATION_CSV_INDEX - 1)
#define IMU_DCU_ACC_X_CSV_SEPARATOR 													(IMU_DCU_ACC_X_CSV_INDEX - 1)
#define IMU_DCU_ACC_Y_CSV_SEPARATOR 													(IMU_DCU_ACC_Y_CSV_INDEX - 1)
#define IMU_DCU_GYR_X_CSV_SEPARATOR 													(IMU_DCU_GYR_X_CSV_INDEX - 1)
#define IMU_DCU_GYR_Z_CSV_SEPARATOR 													(IMU_DCU_GYR_Z_CSV_INDEX - 1)
#define IMU_DCU_HEADING_CSV_SEPARATOR 												(IMU_DCU_HEADING_CSV_INDEX - 1)
#define IMU_DCU_ACC_Z_CSV_SEPARATOR 													(IMU_DCU_ACC_Z_CSV_INDEX - 1)
#define IMU_DCU_GYR_Y_CSV_SEPARATOR 													(IMU_DCU_GYR_Y_CSV_INDEX - 1)
#define IMU_DCU_SENSORS_CALIBRATION_CSV_SEPARATOR 						(IMU_DCU_SENSORS_CALIBRATION_CSV_INDEX - 1)
#define GPS_LATITUDE_MINUTES_CSV_SEPARATOR 										(GPS_LATITUDE_MINUTES_CSV_INDEX - 1)
#define GPS_LONGITUDE_MINUTES_CSV_SEPARATOR  									(GPS_LONGITUDE_MINUTES_CSV_INDEX - 1)
#define GPS_SPEED_CSV_SEPARATOR  															(GPS_SPEED_CSV_INDEX - 1)
#define DAU_FR_TEMP_CSV_SEPARATOR 														(DAU_FR_TEMP_CSV_INDEX - 1)
#define DAU_FR_CURRENT_CSV_SEPARATOR 													(DAU_FR_CURRENT_CSV_INDEX - 1)
#define DAU_FL_TEMP_CSV_SEPARATOR 														(DAU_FL_TEMP_CSV_INDEX - 1)
#define DAU_FL_CURRENT_CSV_SEPARATOR 													(DAU_FL_CURRENT_CSV_INDEX - 1)
#define DAU_REAR_TEMP_CSV_SEPARATOR 													(DAU_REAR_TEMP_CSV_INDEX - 1)
#define DAU_REAR_CURRENT_CSV_SEPARATOR 												(DAU_REAR_CURRENT_CSV_INDEX - 1)
#define SW_TEMP_CSV_SEPARATOR 																(SW_TEMP_CSV_INDEX - 1)
#define SW_CURRENT_CSV_SEPARATOR 															(SW_CURRENT_CSV_INDEX - 1)
#define GCU_TEMP_CSV_SEPARATOR 																(GCU_TEMP_CSV_INDEX - 1)
#define GCU_CURR_CSV_SEPARATOR 																(GCU_CURR_CSV_INDEX - 1)
#define H2O_PUMP_CURRENT_CSV_SEPARATOR 												(H2O_PUMP_CURRENT_CSV_INDEX - 1)
#define FUEL_PUMP_CURRENT_CSV_SEPARATOR 											(FUEL_PUMP_CURRENT_CSV_INDEX - 1)
#define GEARMOTOR_CURRENT_CSV_SEPARATOR 											(GEARMOTOR_CURRENT_CSV_INDEX - 1)
#define CLUTCH_CURRENT_CSV_SEPARATOR 													(CLUTCH_CURRENT_CSV_INDEX - 1)
#define FAN_SX_CURRENT_CSV_SEPARATOR 													(FAN_SX_CURRENT_CSV_INDEX - 1)
#define FAN_DX_CURRENT_CSV_SEPARATOR 													(FAN_DX_CURRENT_CSV_INDEX - 1)
#define DCU_TEMP_CSV_SEPARATOR 																(DCU_TEMP_CSV_INDEX - 1)
#define DCU_CURRENT_CSV_SEPARATOR 														(DCU_CURRENT_CSV_INDEX - 1)
#define XBEE_CURRENT_CSV_SEPARATOR 														(XBEE_CURRENT_CSV_INDEX - 1)
#define DUC_3V3_CURRENT_CSV_SEPARATOR 												(DUC_3V3_CURRENT_CSV_INDEX - 1)
#define DCU_12V_VOLTAGE_CSV_SEPARATOR 												(DCU_12V_VOLTAGE_CSV_INDEX - 1)
#define DCU_5V_VOLTAGE_CSV_SEPARATOR 													(DCU_5V_VOLTAGE_CSV_INDEX - 1)
#define DCU_3V3_VOLTAGE_CSV_SEPARATOR 												(DCU_3V3_VOLTAGE_CSV_INDEX - 1)
#define END_CSV_SEPARATOR																			(END_CSV_INDEX - 1)

/* Calibration buffer index */
#define CALIBRATION_BUFFER_DATA_NUMBER												(uint8_t)10
#define APPS_CALIBRATION_INDEX 																(uint8_t)0
#define STEER_ANGLE_CALIBRATION_INDEX 												(uint8_t)1
#define LINEAR_FL_CALIBRATION_INDEX														(uint8_t)2
#define LINEAR_FR_CALIBRATION_INDEX 													(uint8_t)3
#define LINEAR_RR_CALIBRATION_INDEX 													(uint8_t)4
#define LINEAR_RL_CALIBRATION_INDEX 													(uint8_t)5
#define LOAD_CELL_FL_CALIBRATION_INDEX 												(uint8_t)6
#define LOAD_CELL_FR_CALIBRATION_INDEX 												(uint8_t)7
#define LOAD_CELL_RR_CALIBRATION_INDEX 												(uint8_t)8
#define LOAD_CELL_RL_CALIBRATION_INDEX 												(uint8_t)9

/* Calibration SW ack */
#define APPS_ZERO_CALIBRATION_DONE 														(uint8_t)1
#define APPS_FULL_CALIBRATION_DONE 														(uint8_t)2
#define STEER_ANGLE_CALIBRATION_DONE 													(uint8_t)3
#define LINEAR_CALIBRATION_DONE 															(uint8_t)4
#define LOAD_CELL_CALIBRATION_DONE 														(uint8_t)5

#define SW_ACQUISITION_CAN_REQUEST 														(uint16_t)1
#define SW_START_ACQUISITION_CAN_REQUEST 											(uint16_t)1
#define TO_SW_ACQUISITION_IS_OFF															(uint16_t)2
#define SW_STOP_ACQUISITION_CAN_REQUEST 											(uint8_t)2
#define TO_SW_ACQUISITION_IS_ON																(uint8_t)1
#define SW_CALIBRATIONS_CAN_REQUEST 													(uint8_t)2
#define SW_APPS_ZERO_CALIBRATION_REQUEST 											(uint8_t)1
#define SW_APPS_FULL_CALIBRATION_REQUEST 											(uint8_t)2
#define SW_STEER_ANGLE_CALIBRATION_REQUEST 										(uint8_t)3
#define SW_LINEAR_CALIBRATION_REQUEST 												(uint8_t)4
#define SW_LOAD_CELL_CALIBRATION_REQUEST 											(uint8_t)5
#define EFI_IS_ALIVE_RESET 																		(uint8_t)0
#define EFI_IS_ALIVE_SET 																			(uint8_t)1

#define ACQUISITION_OFF_STATE 																(uint8_t)'1'
#define ACQUISITION_ON_FROM_SW_STATE													(uint8_t)'2'
#define ACQUISITION_ON_FROM_TELEMETRY_STATE 									(uint8_t)'3'
#define ACQUISITION_ON_FROM_AUTO_STATE 												(uint8_t)'4'
#define ACQUISITION_ON_FROM_DEBUG_STATE 											(uint8_t)'5'
#define ACQUISITION_ON_SW_REQUEST 														(uint8_t)'6'
#define ACQUISITION_ON_TELEMETRY_REQUEST 											(uint8_t)'7'
#define ACQUISITION_ON_AUTO_REQUEST 													(uint8_t)'8'
#define ACQUISITION_ON_DEBUG_REQUEST 													(uint8_t)'9'
#define ACQUISITION_OFF_SW_REQUEST 														(uint8_t)'A'
#define ACQUISITION_OFF_TELEMETRY_REQUEST 										(uint8_t)'B'
#define ACQUISITION_OFF_AUTO_REQUEST 													(uint8_t)'C'
#define ACQUISITION_OFF_DEBUG_REQUEST 												(uint8_t)'D'
#define ACQUISITION_IDLE_REQUEST															(uint8_t)'E'

#define STATE_ON 																							(uint8_t)'1'
#define STATE_OFF																							(uint8_t)'0'
#define STATE_USB_READY_INDEX 															  (uint8_t)0
#define STATE_uSD_PRESENT_INDEX 															(uint8_t)2
#define STATE_uSD_READY_INDEX 																(uint8_t)4
#define STATE_ACQUISITION_ON_INDEX 														(uint8_t)6
#define STATE_TELEMETRY_ON_INDEX  														(uint8_t)8

extern inline void DATA_CanParser(CAN_RxPacket_t *unpackedData);
extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent);
extern inline void DATA_ResetAcquisitionStateMachine(void);
extern inline void DATA_CheckEfiIsAlive(void);
extern inline void DATA_AutomaticStartAcquisitionManager(void);
extern inline void DATA_SwapDataPackePointers(void);
extern inline void DATA_SetEfiIsAlive(void);
extern inline void DATA_ResetEfiIsAlive(void);
extern inline uint8_t DATA_GetUsbReadyState(void);
extern inline uint8_t DATA_GetAcquisitionState(void);
extern inline uint8_t DATA_GetTelemetryState(void);
extern inline void DATA_SetUsbReadyState(void);
extern inline void DATA_SetAcquisitionState(void);
extern inline void DATA_SetTelemetryState(void);
extern inline void DATA_ResetUsbPresentState(void);
extern inline void DATA_ResetUsbReadyState(void);
extern inline void DATA_ResetAcquisitionState(void);
extern inline void DATA_ResetTelemetryState(void);
extern void DATA_PacketReset(void);
extern void DATA_ResetStateBuffer(void);
static inline void DATA_SW_CAN_Management(uint8_t byte1, uint8_t byte2);

#endif
