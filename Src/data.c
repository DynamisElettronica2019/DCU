#include "data.h"
#include "id_can.h"

uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_RECEIVED_PACKETS];
uint32_t CAN_ReceivedChannelsCounter [NUMBER_OF_ACQUIRED_CHANNELS];

extern void packetCounterReset(void)
{
	for(uint16_t i = 0; i < NUMBER_OF_ACQUIRED_CHANNELS; i++) {
		CAN_ReceivedChannelsCounter[i] = 0;
	}
	
	return;
}

extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
//	uint16_t data1 = (unpackedData->CAN_RxPacket_Data[0] << 8) | unpackedData->CAN_RxPacket_Data[1]; 			/* First data in the packet */
//	uint16_t data2 = (unpackedData->CAN_RxPacket_Data[2] << 8) | unpackedData->CAN_RxPacket_Data[3]; 			/* Second data in the packet */
//	uint16_t data3 = (unpackedData->CAN_RxPacket_Data[4] << 8) | unpackedData->CAN_RxPacket_Data[5]; 			/* Third data in the packet */
//	uint16_t data4 = (unpackedData->CAN_RxPacket_Data[6] << 8) | unpackedData->CAN_RxPacket_Data[7]; 			/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->CAN_RxPacket_Header.StdId) {
		
		/* EFI ID range */
		
		case EFI_HALL_WHEEL_ID:
			CAN_ReceivedPacketsCounter[EFI_HALL_WHEEL_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[HALL_EFFECT_FR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[HALL_EFFECT_FL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[HALL_EFFECT_RR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[HALL_EFFECT_RL_CSV_INDEX]++;
			break;
		
		case EFI_WATER_TEMPERATURE_ID:
			CAN_ReceivedPacketsCounter[EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[T_H20_SX_IN_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_H20_SX_OUT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_H20_DX_IN_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_H20_DX_OUT_CSV_INDEX]++;
			break;
		
		case EFI_OIL_T_ENGINE_BAT_ID:
			CAN_ReceivedPacketsCounter[EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[T_OIL_IN_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_OIL_OUT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_H20_ENGINE_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[BATTERY_VOLTAGE_CSV_INDEX]++;
			break;
		
		case EFI_GEAR_RPM_TPS_APPS_ID:
			CAN_ReceivedPacketsCounter[EFI_GEAR_RPM_TPS_APPS_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[GEAR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[RPM_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[TPS1_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[PH2O_CSV_INDEX]++;
			break;
		
		case EFI_TRACTION_CONTROL_ID:
			CAN_ReceivedPacketsCounter[EFI_TRACTION_CONTROL_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[VH_SPEED_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[SLIP_TARGET_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[SLIP_CSV_INDEX]++;
			break;
		
		case EFI_FUEL_FAN_H2O_LAUNCH_ID:
			CAN_ReceivedPacketsCounter[EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[MANUAL_LIMITER_ACTIVE_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[FAN_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[H20_PUMP_DUTY_CYCLE_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[PIT_LANE_ACTIVE_CSV_INDEX]++;
			break;
		
		case EFI_PRESSURES_LAMBDA_SMOT_ID:
			CAN_ReceivedPacketsCounter[EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[FUEL_PRESSURE_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[OIL_PRESSURE_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LAMBDA_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[FLAG_SMOT_CSV_INDEX]++;
			break;
		
		case EFI_LOIL_EXHAUST_ID:
			CAN_ReceivedPacketsCounter[EFI_LOIL_EXHAUST_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[L_FUEL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_SCARICO_1_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[T_SCARICO_2_CSV_INDEX]++;
			break;
		
		/* DAU ID range */
		
		case DAU_FR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[LINEARE_FR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LOAD_CELL_FR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[BPS_FRONT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[APPS_CSV_INDEX]++;
			break;
		
		case DAU_FL_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[LINEARE_FL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LOAD_CELL_FL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[BPS_REAR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[STEERING_WHEEL_ANGLE_CSV_INDEX]++;
			break;
		
		case DAU_REAR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[LINEARE_RL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LOAD_CELL_RL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LINEARE_RR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[LOAD_CELL_RR_CSV_INDEX]++;
			break;
		
		case DAU_FL_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_IR_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IR1_FL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR2_FL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR3_FL_CSV_INDEX]++;
			break;
		
		case DAU_FR_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_IR_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IR1_FR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR2_FR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR3_FR_CSV_INDEX]++;
			break;
		
		case DAU_REAR_IR_RL_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RL_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IR1_RL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR2_RL_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR3_RL_CSV_INDEX]++;
			break;
		
		case DAU_REAR_IR_RR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RR_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IR1_RR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR2_RR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IR3_RR_CSV_INDEX]++;
			break;
		
		/* IMU ID range */
		
		case IMU1_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_1_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_ACC_X_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_ACC_Y_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_GYR_X_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_GYR_Z_CSV_INDEX]++;
			break;
		
		case IMU1_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_2_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_HEADING_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_ACC_Z_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU1_GYR_Y_CSV_INDEX]++;
			break;
		
		case IMU2_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_1_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_ACC_X_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_ACC_Y_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_GYR_X_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_GYR_Z_CSV_INDEX]++;
			break;
		
		case IMU2_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_2_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_HEADING_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_ACC_Z_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[IMU2_GYR_Y_CSV_INDEX]++;
			break;
		
		/* Debug ID range */
		
		case DAU_FR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_DEBUG_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_FR_TEMP_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_FR_CURRENT_CSV_INDEX]++;
			break;
		
		case DAU_FL_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_DEBUG_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_FL_TEMP_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_FL_CURRENT_CSV_INDEX]++;
			break;
		
		case DAU_REAR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_DEBUG_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_REAR_TEMP_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[DAU_REAR_CURRENT_CSV_INDEX]++;
			break;
		
		case SW_DEBUG_ID:
			CAN_ReceivedPacketsCounter[SW_DEBUG_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[SW_TEMP_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[SW_CURRENT_CSV_INDEX]++;
			break;
		
		case GCU_DEBUG_1_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_1_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[GCU_TEMP_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[GCU_CURR_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[H2O_PUMP_CURRENT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[FUEL_PUMP_CURRENT_CSV_INDEX]++;
			break;
		
		case GCU_DEBUG_2_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_2_ID_COUNTER_INDEX]++;
			CAN_ReceivedChannelsCounter[GEARMOTOR_CURRENT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[CLUTCH_CURRENT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[FAN_SX_CURRENT_CSV_INDEX]++;
			CAN_ReceivedChannelsCounter[FAN_DX_CURRENT_CSV_INDEX]++;
			break;
	}
	
	return;
}
