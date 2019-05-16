#include "data.h"
#include "id_can.h"
#include "string_utility.h"

uint8_t blockBuffer [BUFFER_BLOCK_LEN];
//uint8_t dcuStateBuffer [BUFFER_STATE_LEN] = "[S;0;0;0;0;0;0;0;0]";
uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_RECEIVED_PACKETS];
uint32_t CAN_ReceivedChannelsCounter [NUMBER_OF_ACQUIRED_CHANNELS];


extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
	uint16_t data1 = (unpackedData->CAN_RxPacket_Data[0] << 8) | unpackedData->CAN_RxPacket_Data[1]; 			/* First data in the packet */
	uint16_t data2 = (unpackedData->CAN_RxPacket_Data[2] << 8) | unpackedData->CAN_RxPacket_Data[3]; 			/* Second data in the packet */
	uint16_t data3 = (unpackedData->CAN_RxPacket_Data[4] << 8) | unpackedData->CAN_RxPacket_Data[5]; 			/* Third data in the packet */
	uint16_t data4 = (unpackedData->CAN_RxPacket_Data[6] << 8) | unpackedData->CAN_RxPacket_Data[7]; 			/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->CAN_RxPacket_Header.StdId) {
		
		/* EFI ID range */
		
		case EFI_HALL_WHEEL_ID:
			CAN_ReceivedPacketsCounter[EFI_HALL_WHEEL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[HALL_EFFECT_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[HALL_EFFECT_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[HALL_EFFECT_RR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[HALL_EFFECT_RL_CSV_INDEX], 5);
			break;
		
		case EFI_WATER_TEMPERATURE_ID:
			CAN_ReceivedPacketsCounter[EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[T_H20_SX_IN_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[T_H20_SX_OUT_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[T_H20_DX_IN_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[T_H20_DX_OUT_CSV_INDEX], 5);
			break;
		
		case EFI_OIL_T_ENGINE_BAT_ID:
			CAN_ReceivedPacketsCounter[EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[T_OIL_IN_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[T_OIL_OUT_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[T_H20_ENGINE_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[BATTERY_VOLTAGE_CSV_INDEX], 5);
			break;
		
		case EFI_GEAR_RPM_TPS_APPS_ID:
			CAN_ReceivedPacketsCounter[EFI_GEAR_RPM_TPS_APPS_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[GEAR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[RPM_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[TPS1_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[PH2O_CSV_INDEX], 5);
			break;
		
		case EFI_TRACTION_CONTROL_ID:
			CAN_ReceivedPacketsCounter[EFI_TRACTION_CONTROL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[VH_SPEED_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[SLIP_TARGET_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[SLIP_CSV_INDEX], 5);
			break;
		
		case EFI_FUEL_FAN_H2O_LAUNCH_ID:
			CAN_ReceivedPacketsCounter[EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[MANUAL_LIMITER_ACTIVE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[FAN_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[H20_PUMP_DUTY_CYCLE_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[PIT_LANE_ACTIVE_CSV_INDEX], 5);
			break;
		
		case EFI_PRESSURES_LAMBDA_SMOT_ID:
			CAN_ReceivedPacketsCounter[EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[FUEL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[OIL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[LAMBDA_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[FLAG_SMOT_CSV_INDEX], 5);
			break;
		
		case EFI_LOIL_EXHAUST_ID:
			CAN_ReceivedPacketsCounter[EFI_LOIL_EXHAUST_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[L_FUEL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[T_SCARICO_1_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[T_SCARICO_2_CSV_INDEX], 5);
			break;
		
		/* DAU ID range */
		
		case DAU_FR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[LINEARE_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[LOAD_CELL_FR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[BPS_FRONT_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[APPS_CSV_INDEX], 5);
			break;
		
		case DAU_FL_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[LINEARE_FL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[LOAD_CELL_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[BPS_REAR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[STEERING_WHEEL_ANGLE_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[LINEARE_RL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[LOAD_CELL_RL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[LINEARE_RR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[LOAD_CELL_RR_CSV_INDEX], 5);
			break;
		
		case DAU_FL_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_IR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IR1_FL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IR2_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IR3_FL_CSV_INDEX], 5);
			break;
		
		case DAU_FR_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_IR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IR1_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IR2_FR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IR3_FR_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_IR_RL_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IR1_RL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IR2_RL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IR3_RL_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_IR_RR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IR1_RR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IR2_RR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IR3_RR_CSV_INDEX], 5);
			break;
		
		/* IMU ID range */
		
		case IMU1_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IMU1_ACC_X_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IMU1_ACC_Y_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IMU1_GYR_X_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[IMU1_GYR_Z_CSV_INDEX], 5);
			break;
		
		case IMU1_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_2_ID_COUNTER_INDEX]++;
		intToStringUnsigned(data1, &blockBuffer[IMU1_HEADING_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IMU1_ACC_Z_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IMU1_GYR_Y_CSV_INDEX], 5);
			break;
		
		case IMU2_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IMU2_ACC_X_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IMU2_ACC_Y_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IMU2_GYR_X_CSV_INDEX], 5);
			intToStringUnsigned(data4, &blockBuffer[IMU2_GYR_Z_CSV_INDEX], 5);
			break;
		
		case IMU2_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_2_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[IMU2_HEADING_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[IMU2_ACC_Z_CSV_INDEX], 5);
			intToStringUnsigned(data3, &blockBuffer[IMU2_GYR_Y_CSV_INDEX], 5);
			break;
		
		/* Debug ID range */
		
		case DAU_FR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[DAU_FR_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[DAU_FR_CURRENT_CSV_INDEX], 5);
			break;
		
		case DAU_FL_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[DAU_FL_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[DAU_FL_CURRENT_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_DEBUG_ID_COUNTER_INDEX]++;	
			intToStringUnsigned(data1, &blockBuffer[DAU_REAR_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[DAU_REAR_CURRENT_CSV_INDEX], 5);
			break;
		
		case SW_DEBUG_ID:
			CAN_ReceivedPacketsCounter[SW_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[SW_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[SW_CURRENT_CSV_INDEX], 5);
			break;
		
		case GCU_DEBUG_1_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[GCU_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[GCU_CURR_CSV_INDEX], 5);
			intToStringUnsigned(data1, &blockBuffer[H2O_PUMP_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[FUEL_PUMP_CURRENT_CSV_INDEX], 5);
			break;
		
		case GCU_DEBUG_2_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_2_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &blockBuffer[GEARMOTOR_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[CLUTCH_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data1, &blockBuffer[FAN_SX_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &blockBuffer[FAN_DX_CURRENT_CSV_INDEX], 5);
			break;
	}
	
	return;
}

extern void dataPacketReset(void)
{
	for(uint16_t i = 0; i < BUFFER_BLOCK_LEN; i++) {
		blockBuffer[i] = 0;
	}
	
	//blockBuffer[HALL_EFFECT_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[HALL_EFFECT_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[HALL_EFFECT_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;	
	blockBuffer[HALL_EFFECT_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_H20_SX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_H20_SX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_H20_DX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_H20_DX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_OIL_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_OIL_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_H20_ENGINE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[BATTERY_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[GEAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[RPM_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[TPS1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[PH2O_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[VH_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[SLIP_TARGET_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[SLIP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FAN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[PIT_LANE_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FUEL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[OIL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LAMBDA_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FLAG_SMOT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[L_FUEL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_SCARICO_1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[T_SCARICO_2_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LINEARE_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LOAD_CELL_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[BPS_FRONT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[APPS_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LINEARE_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LOAD_CELL_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[BPS_REAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[STEERING_WHEEL_ANGLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LINEARE_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LOAD_CELL_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LINEARE_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[LOAD_CELL_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR1_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR2_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR3_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR1_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR2_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR3_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR1_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR2_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR3_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR1_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR2_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IR3_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU1_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU2_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[IMU_DCU_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_FR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_FR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_FL_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_FL_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_REAR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DAU_REAR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[SW_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[SW_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[GCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[GCU_CURR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[H2O_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FUEL_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[GEARMOTOR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[CLUTCH_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FAN_SX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[FAN_DX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DCU_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[XBEE_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DUC_3V3_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DCU_12V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DCU_5V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[DCU_3V3_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	blockBuffer[END_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	return;
}

extern void packetCounterReset(void)
{
	for(uint16_t i = 0; i < NUMBER_OF_ACQUIRED_CHANNELS; i++) {
		CAN_ReceivedChannelsCounter[i] = 0;
	}
	
	return;
}
