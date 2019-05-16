#ifndef __data_H
#define __data_H

#include "can.h"

#define CHANNEL_SEPARATION														(uint8_t)';'
#define DECIMAL_SEPARATOR 														(uint8_t)'.'
#define END_LINE																			(uint8_t)'\n'

#define NUMBER_OF_RECEIVED_PACKETS										(uint8_t)25
#define NUMBER_OF_ACQUIRED_CHANNELS										(uint8_t)91
#define BUFFER_STATE_LEN 															(uint8_t)19
#define BUFFER_BLOCK_LEN 															(uint16_t)1024
#define BUFFER_COMMAND_LEN          									(uint8_t)4
#define BUFFER_RTC_SET_LEN          									(uint8_t)20
#define ERROR_MSG_LEN               									(uint8_t)4
#define COMMAND_ACK_MSG_LEN         									(uint8_t)4
#define ERROR_MSG_IDENTIFIER_POS    									(uint8_t)2
#define COMMAND_ACK_IDENTIFIER_POS  									(uint8_t)2
#define HALF_DATA_INDEX  															(uint16_t)256

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

#define HALL_EFFECT_FR_CSV_INDEX 											(uint16_t)0
#define HALL_EFFECT_FL_CSV_INDEX 											(uint16_t)6
#define HALL_EFFECT_RR_CSV_INDEX 											(uint16_t)12
#define HALL_EFFECT_RL_CSV_INDEX 											(uint16_t)18
#define T_H20_SX_IN_CSV_INDEX 												(uint16_t)24
#define T_H20_SX_OUT_CSV_INDEX 												(uint16_t)30
#define T_H20_DX_IN_CSV_INDEX 												(uint16_t)36
#define T_H20_DX_OUT_CSV_INDEX												(uint16_t)42
#define T_OIL_IN_CSV_INDEX 														(uint16_t)48
#define T_OIL_OUT_CSV_INDEX 													(uint16_t)54
#define T_H20_ENGINE_CSV_INDEX 												(uint16_t)60
#define BATTERY_VOLTAGE_CSV_INDEX 										(uint16_t)66
#define GEAR_CSV_INDEX 																(uint16_t)72
#define RPM_CSV_INDEX 																(uint16_t)78
#define TPS1_CSV_INDEX 																(uint16_t)84
#define PH2O_CSV_INDEX 																(uint16_t)90
#define VH_SPEED_CSV_INDEX 														(uint16_t)96
#define SLIP_TARGET_CSV_INDEX 												(uint16_t)102
#define SLIP_CSV_INDEX																(uint16_t)108
#define MANUAL_LIMITER_ACTIVE_CSV_INDEX 							(uint16_t)114
#define FAN_CSV_INDEX 																(uint16_t)120
#define H20_PUMP_DUTY_CYCLE_CSV_INDEX 								(uint16_t)126
#define PIT_LANE_ACTIVE_CSV_INDEX 										(uint16_t)132
#define FUEL_PRESSURE_CSV_INDEX 											(uint16_t)138
#define OIL_PRESSURE_CSV_INDEX 												(uint16_t)144
#define LAMBDA_CSV_INDEX 															(uint16_t)150
#define FLAG_SMOT_CSV_INDEX 													(uint16_t)156
#define L_FUEL_CSV_INDEX 															(uint16_t)162
#define T_SCARICO_1_CSV_INDEX 												(uint16_t)168
#define T_SCARICO_2_CSV_INDEX 												(uint16_t)174
#define LINEARE_FR_CSV_INDEX 													(uint16_t)180
#define LOAD_CELL_FR_CSV_INDEX 												(uint16_t)186
#define BPS_FRONT_CSV_INDEX 													(uint16_t)192
#define APPS_CSV_INDEX 																(uint16_t)198
#define LINEARE_FL_CSV_INDEX													(uint16_t)204
#define LOAD_CELL_FL_CSV_INDEX 												(uint16_t)210
#define BPS_REAR_CSV_INDEX 														(uint16_t)216
#define STEERING_WHEEL_ANGLE_CSV_INDEX 								(uint16_t)222
#define LINEARE_RL_CSV_INDEX 													(uint16_t)228
#define LOAD_CELL_RL_CSV_INDEX 												(uint16_t)234
#define LINEARE_RR_CSV_INDEX 													(uint16_t)240
#define LOAD_CELL_RR_CSV_INDEX 												(uint16_t)246
#define IR1_FL_CSV_INDEX 															(uint16_t)252
#define IR2_FL_CSV_INDEX 															(uint16_t)258
#define IR3_FL_CSV_INDEX 															(uint16_t)264
#define IR1_FR_CSV_INDEX 															(uint16_t)270
#define IR2_FR_CSV_INDEX 															(uint16_t)276
#define IR3_FR_CSV_INDEX 															(uint16_t)282
#define IR1_RL_CSV_INDEX 															(uint16_t)288
#define IR2_RL_CSV_INDEX 															(uint16_t)294
#define IR3_RL_CSV_INDEX 															(uint16_t)300
#define IR1_RR_CSV_INDEX 															(uint16_t)306
#define IR2_RR_CSV_INDEX 															(uint16_t)312
#define IR3_RR_CSV_INDEX 															(uint16_t)318
#define IMU1_ACC_X_CSV_INDEX 													(uint16_t)324
#define IMU1_ACC_Y_CSV_INDEX 													(uint16_t)330
#define IMU1_GYR_X_CSV_INDEX 													(uint16_t)336
#define IMU1_GYR_Z_CSV_INDEX 													(uint16_t)342
#define IMU1_HEADING_CSV_INDEX 												(uint16_t)348
#define IMU1_ACC_Z_CSV_INDEX 													(uint16_t)354
#define IMU1_GYR_Y_CSV_INDEX 													(uint16_t)360
#define IMU2_ACC_X_CSV_INDEX 													(uint16_t)366
#define IMU2_ACC_Y_CSV_INDEX 													(uint16_t)372
#define IMU2_GYR_X_CSV_INDEX 													(uint16_t)378
#define IMU2_GYR_Z_CSV_INDEX 													(uint16_t)384
#define IMU2_HEADING_CSV_INDEX 												(uint16_t)390
#define IMU2_ACC_Z_CSV_INDEX 													(uint16_t)396
#define IMU2_GYR_Y_CSV_INDEX 													(uint16_t)402
#define IMU_DCU_ACC_X_CSV_INDEX 											(uint16_t)408
#define IMU_DCU_ACC_Y_CSV_INDEX 											(uint16_t)414
#define IMU_DCU_GYR_X_CSV_INDEX 											(uint16_t)420
#define IMU_DCU_GYR_Z_CSV_INDEX 											(uint16_t)426
#define IMU_DCU_HEADING_CSV_INDEX 										(uint16_t)432
#define IMU_DCU_ACC_Z_CSV_INDEX 											(uint16_t)438
#define IMU_DCU_GYR_Y_CSV_INDEX 											(uint16_t)444
#define DAU_FR_TEMP_CSV_INDEX 												(uint16_t)450
#define DAU_FR_CURRENT_CSV_INDEX 											(uint16_t)456
#define DAU_FL_TEMP_CSV_INDEX 												(uint16_t)462
#define DAU_FL_CURRENT_CSV_INDEX 											(uint16_t)468
#define DAU_REAR_TEMP_CSV_INDEX 											(uint16_t)474
#define DAU_REAR_CURRENT_CSV_INDEX 										(uint16_t)480
#define SW_TEMP_CSV_INDEX 														(uint16_t)486
#define SW_CURRENT_CSV_INDEX 													(uint16_t)492
#define GCU_TEMP_CSV_INDEX 														(uint16_t)498
#define GCU_CURR_CSV_INDEX 														(uint16_t)504
#define H2O_PUMP_CURRENT_CSV_INDEX 										(uint16_t)510
#define FUEL_PUMP_CURRENT_CSV_INDEX 									(uint16_t)516
#define GEARMOTOR_CURRENT_CSV_INDEX 									(uint16_t)522
#define CLUTCH_CURRENT_CSV_INDEX 											(uint16_t)528
#define FAN_SX_CURRENT_CSV_INDEX 											(uint16_t)534
#define FAN_DX_CURRENT_CSV_INDEX 											(uint16_t)540
#define DCU_TEMP_CSV_INDEX 														(uint16_t)546
#define DCU_CURRENT_CSV_INDEX 												(uint16_t)552
#define XBEE_CURRENT_CSV_INDEX 												(uint16_t)558
#define DUC_3V3_CURRENT_CSV_INDEX 										(uint16_t)564
#define DCU_12V_VOLTAGE_CSV_INDEX 										(uint16_t)570
#define DCU_5V_VOLTAGE_CSV_INDEX 											(uint16_t)576
#define DCU_3V3_VOLTAGE_CSV_INDEX 										(uint16_t)582
#define END_DATA_CSV_INDEX 														(uint16_t)588
#define END_ROW_CSV_INDEX 														(uint16_t)1023

//#define HALL_EFFECT_FR_CSV_SEPARATOR									(HALL_EFFECT_FR_CSV_INDEX - 1)
#define HALL_EFFECT_FL_CSV_SEPARATOR									(HALL_EFFECT_FL_CSV_INDEX - 1)
#define HALL_EFFECT_RR_CSV_SEPARATOR 									(HALL_EFFECT_RR_CSV_INDEX - 1)
#define HALL_EFFECT_RL_CSV_SEPARATOR 									(HALL_EFFECT_RL_CSV_INDEX - 1)
#define T_H20_SX_IN_CSV_SEPARATOR 										(T_H20_SX_IN_CSV_INDEX - 1)
#define T_H20_SX_OUT_CSV_SEPARATOR 										(T_H20_SX_OUT_CSV_INDEX - 1)
#define T_H20_DX_IN_CSV_SEPARATOR											(T_H20_DX_IN_CSV_INDEX - 1)
#define T_H20_DX_OUT_CSV_SEPARATOR										(T_H20_DX_OUT_CSV_INDEX - 1)
#define T_OIL_IN_CSV_SEPARATOR 												(T_OIL_IN_CSV_INDEX - 1)
#define T_OIL_OUT_CSV_SEPARATOR												(T_OIL_OUT_CSV_INDEX - 1)
#define T_H20_ENGINE_CSV_SEPARATOR										(T_H20_ENGINE_CSV_INDEX - 1)
#define BATTERY_VOLTAGE_CSV_SEPARATOR								  (BATTERY_VOLTAGE_CSV_INDEX - 1)
#define GEAR_CSV_SEPARATOR														(GEAR_CSV_INDEX - 1)
#define RPM_CSV_SEPARATOR 														(RPM_CSV_INDEX - 1)
#define TPS1_CSV_SEPARATOR 														(TPS1_CSV_INDEX - 1)
#define PH2O_CSV_SEPARATOR 														(PH2O_CSV_INDEX - 1)
#define VH_SPEED_CSV_SEPARATOR 												(VH_SPEED_CSV_INDEX - 1)
#define SLIP_TARGET_CSV_SEPARATOR 										(SLIP_TARGET_CSV_INDEX - 1)
#define SLIP_CSV_SEPARATOR														(SLIP_CSV_INDEX - 1)
#define MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR 					(MANUAL_LIMITER_ACTIVE_CSV_INDEX - 1)
#define FAN_CSV_SEPARATOR															(FAN_CSV_INDEX - 1)
#define H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR 						(H20_PUMP_DUTY_CYCLE_CSV_INDEX - 1)
#define PIT_LANE_ACTIVE_CSV_SEPARATOR 								(PIT_LANE_ACTIVE_CSV_INDEX - 1)
#define FUEL_PRESSURE_CSV_SEPARATOR 									(FUEL_PRESSURE_CSV_INDEX - 1)
#define OIL_PRESSURE_CSV_SEPARATOR 										(OIL_PRESSURE_CSV_INDEX - 1)
#define LAMBDA_CSV_SEPARATOR 													(LAMBDA_CSV_INDEX - 1)
#define FLAG_SMOT_CSV_SEPARATOR 											(FLAG_SMOT_CSV_INDEX - 1)
#define L_FUEL_CSV_SEPARATOR 													(L_FUEL_CSV_INDEX - 1)
#define T_SCARICO_1_CSV_SEPARATOR 										(T_SCARICO_1_CSV_INDEX - 1)
#define T_SCARICO_2_CSV_SEPARATOR 										(T_SCARICO_2_CSV_INDEX - 1)
#define LINEARE_FR_CSV_SEPARATOR 											(LINEARE_FR_CSV_INDEX - 1)
#define LOAD_CELL_FR_CSV_SEPARATOR										(LOAD_CELL_FR_CSV_INDEX - 1)
#define BPS_FRONT_CSV_SEPARATOR 											(BPS_FRONT_CSV_INDEX - 1)
#define APPS_CSV_SEPARATOR 														(APPS_CSV_INDEX - 1)
#define LINEARE_FL_CSV_SEPARATOR											(LINEARE_FL_CSV_INDEX - 1)
#define LOAD_CELL_FL_CSV_SEPARATOR 										(LOAD_CELL_FL_CSV_INDEX - 1)
#define BPS_REAR_CSV_SEPARATOR 												(BPS_REAR_CSV_INDEX - 1)
#define STEERING_WHEEL_ANGLE_CSV_SEPARATOR 						(STEERING_WHEEL_ANGLE_CSV_INDEX - 1)
#define LINEARE_RL_CSV_SEPARATOR 											(LINEARE_RL_CSV_INDEX - 1)
#define LOAD_CELL_RL_CSV_SEPARATOR 										(LOAD_CELL_RL_CSV_INDEX - 1)
#define LINEARE_RR_CSV_SEPARATOR 											(LINEARE_RR_CSV_INDEX - 1)
#define LOAD_CELL_RR_CSV_SEPARATOR 										(LOAD_CELL_RR_CSV_INDEX - 1)
#define IR1_FL_CSV_SEPARATOR 													(IR1_FL_CSV_INDEX - 1)
#define IR2_FL_CSV_SEPARATOR 													(IR2_FL_CSV_INDEX - 1)
#define IR3_FL_CSV_SEPARATOR 													(IR3_FL_CSV_INDEX - 1)
#define IR1_FR_CSV_SEPARATOR 													(IR1_FR_CSV_INDEX - 1)
#define IR2_FR_CSV_SEPARATOR 													(IR2_FR_CSV_INDEX - 1)
#define IR3_FR_CSV_SEPARATOR 													(IR3_FR_CSV_INDEX - 1)
#define IR1_RL_CSV_SEPARATOR 													(IR1_RL_CSV_INDEX - 1)
#define IR2_RL_CSV_SEPARATOR 													(IR2_RL_CSV_INDEX - 1)
#define IR3_RL_CSV_SEPARATOR 													(IR3_RL_CSV_INDEX - 1)
#define IR1_RR_CSV_SEPARATOR 													(IR1_RR_CSV_INDEX - 1)
#define IR2_RR_CSV_SEPARATOR 													(IR2_RR_CSV_INDEX - 1)
#define IR3_RR_CSV_SEPARATOR 													(IR3_RR_CSV_INDEX - 1)
#define IMU1_ACC_X_CSV_SEPARATOR 											(IMU1_ACC_X_CSV_INDEX - 1)
#define IMU1_ACC_Y_CSV_SEPARATOR 											(IMU1_ACC_Y_CSV_INDEX - 1)
#define IMU1_GYR_X_CSV_SEPARATOR 											(IMU1_GYR_X_CSV_INDEX - 1)
#define IMU1_GYR_Z_CSV_SEPARATOR 											(IMU1_GYR_Z_CSV_INDEX - 1)
#define IMU1_HEADING_CSV_SEPARATOR 										(IMU1_HEADING_CSV_INDEX - 1)
#define IMU1_ACC_Z_CSV_SEPARATOR 											(IMU1_ACC_Z_CSV_INDEX - 1)
#define IMU1_GYR_Y_CSV_SEPARATOR 											(IMU1_GYR_Y_CSV_INDEX - 1)
#define IMU2_ACC_X_CSV_SEPARATOR 											(IMU2_ACC_X_CSV_INDEX - 1)
#define IMU2_ACC_Y_CSV_SEPARATOR 											(IMU2_ACC_Y_CSV_INDEX - 1)
#define IMU2_GYR_X_CSV_SEPARATOR 											(IMU2_GYR_X_CSV_INDEX - 1)
#define IMU2_GYR_Z_CSV_SEPARATOR 											(IMU2_GYR_Z_CSV_INDEX - 1)
#define IMU2_HEADING_CSV_SEPARATOR 										(IMU2_HEADING_CSV_INDEX - 1)
#define IMU2_ACC_Z_CSV_SEPARATOR 											(IMU2_ACC_Z_CSV_INDEX - 1)
#define IMU2_GYR_Y_CSV_SEPARATOR 											(IMU2_GYR_Y_CSV_INDEX - 1)
#define IMU_DCU_ACC_X_CSV_SEPARATOR 									(IMU_DCU_ACC_X_CSV_INDEX - 1)
#define IMU_DCU_ACC_Y_CSV_SEPARATOR 									(IMU_DCU_ACC_Y_CSV_INDEX - 1)
#define IMU_DCU_GYR_X_CSV_SEPARATOR 									(IMU_DCU_GYR_X_CSV_INDEX - 1)
#define IMU_DCU_GYR_Z_CSV_SEPARATOR 									(IMU_DCU_GYR_Z_CSV_INDEX - 1)
#define IMU_DCU_HEADING_CSV_SEPARATOR 								(IMU_DCU_HEADING_CSV_INDEX - 1)
#define IMU_DCU_ACC_Z_CSV_SEPARATOR 									(IMU_DCU_ACC_Z_CSV_INDEX - 1)
#define IMU_DCU_GYR_Y_CSV_SEPARATOR 									(IMU_DCU_GYR_Y_CSV_INDEX - 1)
#define DAU_FR_TEMP_CSV_SEPARATOR 										(DAU_FR_TEMP_CSV_INDEX - 1)
#define DAU_FR_CURRENT_CSV_SEPARATOR 									(DAU_FR_CURRENT_CSV_INDEX - 1)
#define DAU_FL_TEMP_CSV_SEPARATOR 										(DAU_FL_TEMP_CSV_INDEX - 1)
#define DAU_FL_CURRENT_CSV_SEPARATOR 									(DAU_FL_CURRENT_CSV_INDEX - 1)
#define DAU_REAR_TEMP_CSV_SEPARATOR 									(DAU_REAR_TEMP_CSV_INDEX - 1)
#define DAU_REAR_CURRENT_CSV_SEPARATOR 								(DAU_REAR_CURRENT_CSV_INDEX - 1)
#define SW_TEMP_CSV_SEPARATOR 												(SW_TEMP_CSV_INDEX - 1)
#define SW_CURRENT_CSV_SEPARATOR 											(SW_CURRENT_CSV_INDEX - 1)
#define GCU_TEMP_CSV_SEPARATOR 												(GCU_TEMP_CSV_INDEX - 1)
#define GCU_CURR_CSV_SEPARATOR 												(GCU_CURR_CSV_INDEX - 1)
#define H2O_PUMP_CURRENT_CSV_SEPARATOR 								(H2O_PUMP_CURRENT_CSV_INDEX - 1)
#define FUEL_PUMP_CURRENT_CSV_SEPARATOR 							(FUEL_PUMP_CURRENT_CSV_INDEX - 1)
#define GEARMOTOR_CURRENT_CSV_SEPARATOR 							(GEARMOTOR_CURRENT_CSV_INDEX - 1)
#define CLUTCH_CURRENT_CSV_SEPARATOR 									(CLUTCH_CURRENT_CSV_INDEX - 1)
#define FAN_SX_CURRENT_CSV_SEPARATOR 									(FAN_SX_CURRENT_CSV_INDEX - 1)
#define FAN_DX_CURRENT_CSV_SEPARATOR 									(FAN_DX_CURRENT_CSV_INDEX - 1)
#define DCU_TEMP_CSV_SEPARATOR 												(DCU_TEMP_CSV_INDEX - 1)
#define DCU_CURRENT_CSV_SEPARATOR 										(DCU_CURRENT_CSV_INDEX - 1)
#define XBEE_CURRENT_CSV_SEPARATOR 										(XBEE_CURRENT_CSV_INDEX - 1)
#define DUC_3V3_CURRENT_CSV_SEPARATOR 								(DUC_3V3_CURRENT_CSV_INDEX - 1)
#define DCU_12V_VOLTAGE_CSV_SEPARATOR 								(DCU_12V_VOLTAGE_CSV_INDEX - 1)
#define DCU_5V_VOLTAGE_CSV_SEPARATOR 									(DCU_5V_VOLTAGE_CSV_INDEX - 1)
#define DCU_3V3_VOLTAGE_CSV_SEPARATOR 								(DCU_3V3_VOLTAGE_CSV_INDEX - 1)
#define END_CSV_SEPARATOR															(END_CSV_INDEX - 1)

#define ACQUISITION_OFF_STATE 												(uint8_t)'1'
#define ACQUISITION_ON_FROM_SW_STATE									(uint8_t)'2'
#define ACQUISITION_ON_FROM_TELEMETRY_STATE 					(uint8_t)'3'
#define ACQUISITION_ON_FROM_AUTO_STATE 								(uint8_t)'4'
#define ACQUISITION_ON_FROM_DEBUG_STATE 							(uint8_t)'5'
#define ACQUISITION_ON_SW_REQUEST 										(uint8_t)'6'
#define ACQUISITION_ON_TELEMETRY_REQUEST 							(uint8_t)'7'
#define ACQUISITION_ON_AUTO_REQUEST 									(uint8_t)'8'
#define ACQUISITION_ON_DEBUG_REQUEST 									(uint8_t)'9'
#define ACQUISITION_OFF_SW_REQUEST 										(uint8_t)'A'
#define ACQUISITION_OFF_TELEMETRY_REQUEST 						(uint8_t)'B'
#define ACQUISITION_OFF_AUTO_REQUEST 									(uint8_t)'C'
#define ACQUISITION_OFF_DEBUG_REQUEST 								(uint8_t)'D'
#define ACQUISITION_IDLE_REQUEST											(uint8_t)'E'

#define IDLE_COMMAND																	(uint8_t)'I'
#define ACQUISITION_ON_SW_COMMAND											(uint8_t)'S'
#define ACQUISITION_OFF_SW_COMMAND 										(uint8_t)'W'
#define ACQUISITION_ON_TELEMETRY_COMMAND 							(uint8_t)'T'
#define ACQUISITION_OFF_TELEMETRY_COMMAND 						(uint8_t)'M'
#define ACQUISITION_ON_ETH_COMMAND 										(uint8_t)'E'
#define ACQUISITION_OFF_ETH_COMMAND 									(uint8_t)'H'
#define ACQUISITION_ON_DEBUG_COMMAND 									(uint8_t)'D'
#define ACQUISITION_OFF_DEBUG_COMMAND 								(uint8_t)'B'
#define STATE_ON 																			(uint8_t)'1'
#define STATE_OFF																			(uint8_t)'0'

#define STATE_USB_PRESENT_INDEX 											(uint8_t)3
#define STATE_USB_READY_INDEX 												(uint8_t)5
#define STATE_uSD_PRESENT_INDEX 											(uint8_t)7
#define STATE_uSD_READY_INDEX 												(uint8_t)9
#define STATE_ACQUISITION_ON_INDEX 										(uint8_t)11
#define STATE_TELEMETRY_ON_INDEX  										(uint8_t)13

#define ACK_MSG                     									"[A-]"
#define ERROR_MSG                   									"[E-]"
#define SECOND_HALF_TX_FLAG         									(uint8_t)'S'
#define NORMAL_MODE_TX_FLAG         									(uint8_t)'N'
#define MESSAGE_INIT_ID             									(uint8_t)'['
#define MESSAGE_END_ID              									(uint8_t)']'
#define RESET_TELEM_ID              									(uint8_t)'M'
#define START_ACQ_ID                									(uint8_t)'B'
#define STOP_ACQ_ID                 									(uint8_t)'V'
#define GIVE_RTC_TIME_ID            									(uint8_t)'J'
#define GIVE_RTC_DATE_ID            									(uint8_t)'H'
#define SET_RTC_ID                  									(uint8_t)'R'
#define CMD_READ_ERR_ID          											(uint8_t)'B'
#define UART1_CLEAR_FLAG															(uint8_t)'C'

extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData);
extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent);
static inline void USB_OpenFile(void);
static inline void USB_CloseFile(void);
extern inline uint8_t getUsbReadyState(void);
extern inline uint8_t getAcquisitionState(void);
extern inline void setUsbPresentState(void);
extern inline void setUsbReadyState(void);
extern inline void setAcquisitionState(void);
extern inline void resetUsbPresentState(void);
extern inline void resetUsbReadyState(void);
extern inline void resetAcquisitionState(void);
extern void dataPacketReset(void);
extern void packetCounterReset(void);
static inline void USB_WriteLen(uint8_t *buffer);

#endif
