#include "data.h"
#include "usb_host.h"
#include "id_can.h"
#include "string_utility.h"


uint8_t DATA_BlockBuffer [BUFFER_BLOCK_LEN];
uint8_t DATA_StateBuffer [BUFFER_STATE_LEN] = "[S;0;0;0;0;0;0;0;0]";
uint8_t acquisitionState = ACQUISITION_OFF_STATE;
extern uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_ACQUIRED_CHANNELS];


extern inline void DATA_CanParser(CAN_RxPacket_t *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
	uint16_t data1 = (unpackedData->packetData[0] << 8) | unpackedData->packetData[1]; 			/* First data in the packet */
	uint16_t data2 = (unpackedData->packetData[2] << 8) | unpackedData->packetData[3]; 			/* Second data in the packet */
	uint16_t data3 = (unpackedData->packetData[4] << 8) | unpackedData->packetData[5]; 			/* Third data in the packet */
	uint16_t data4 = (unpackedData->packetData[6] << 8) | unpackedData->packetData[7]; 			/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->packetHeader.StdId) {
		
		/* EFI ID range */
		
		case EFI_HALL_WHEEL_ID:
			CAN_ReceivedPacketsCounter[EFI_HALL_WHEEL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[HALL_EFFECT_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[HALL_EFFECT_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[HALL_EFFECT_RR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[HALL_EFFECT_RL_CSV_INDEX], 5);
			break;
		
		case EFI_WATER_TEMPERATURE_ID:
			CAN_ReceivedPacketsCounter[EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[T_H20_SX_IN_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[T_H20_SX_OUT_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[T_H20_DX_IN_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[T_H20_DX_OUT_CSV_INDEX], 5);
			break;
		
		case EFI_OIL_T_ENGINE_BAT_ID:
			CAN_ReceivedPacketsCounter[EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[T_OIL_IN_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[T_OIL_OUT_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[T_H20_ENGINE_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[BATTERY_VOLTAGE_CSV_INDEX], 5);
			break;
		
		case EFI_GEAR_RPM_TPS_PH2O_ID:
			CAN_ReceivedPacketsCounter[EFI_MANUAL_LIMITER_FAN_H2O_PIT_LANE_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[GEAR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[RPM_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[TPS1_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[PH2O_CSV_INDEX], 5);
			break;
		
		case EFI_TRACTION_CONTROL_ID:
			CAN_ReceivedPacketsCounter[EFI_TRACTION_CONTROL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[VH_SPEED_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[SLIP_TARGET_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[SLIP_CSV_INDEX], 5);
			break;
		
		case EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX:
			CAN_ReceivedPacketsCounter[EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[MANUAL_LIMITER_ACTIVE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[FAN_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[H20_PUMP_DUTY_CYCLE_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[PIT_LANE_ACTIVE_CSV_INDEX], 5);
			break;
		
		case EFI_PRESSURES_LAMBDA_SMOT_ID:
			CAN_ReceivedPacketsCounter[EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[FUEL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[OIL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[LAMBDA_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[FLAG_SMOT_CSV_INDEX], 5);
			break;
		
		case EFI_LOIL_EXHAUST_ID:
			CAN_ReceivedPacketsCounter[EFI_LOIL_EXHAUST_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[L_FUEL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[T_SCARICO_1_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[T_SCARICO_2_CSV_INDEX], 5);
			break;
		
		/* DAU ID range */
		
		case DAU_FR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[LINEARE_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[LOAD_CELL_FR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[BPS_FRONT_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[APPS_CSV_INDEX], 5);
			break;
		
		case DAU_FL_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[LINEARE_FL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[LOAD_CELL_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[BPS_REAR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[STEERING_WHEEL_ANGLE_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[LINEARE_RL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[LOAD_CELL_RL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[LINEARE_RR_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[LOAD_CELL_RR_CSV_INDEX], 5);
			break;
		
		case DAU_FL_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_IR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IR1_FL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IR2_FL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IR3_FL_CSV_INDEX], 5);
			break;
		
		case DAU_FR_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_IR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IR1_FR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IR2_FR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IR3_FR_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_IR_RL_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RL_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IR1_RL_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IR2_RL_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IR3_RL_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_IR_RR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RR_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IR1_RR_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IR2_RR_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IR3_RR_CSV_INDEX], 5);
			break;
		
		/* IMU ID range */
		
		case IMU1_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IMU1_ACC_X_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IMU1_ACC_Y_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IMU1_GYR_X_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[IMU1_GYR_Z_CSV_INDEX], 5);
			break;
		
		case IMU1_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_2_ID_COUNTER_INDEX]++;
		intToStringUnsigned(data1, &DATA_BlockBuffer[IMU1_HEADING_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IMU1_ACC_Z_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IMU1_GYR_Y_CSV_INDEX], 5);
			break;
		
		case IMU2_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IMU2_ACC_X_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IMU2_ACC_Y_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IMU2_GYR_X_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[IMU2_GYR_Z_CSV_INDEX], 5);
			break;
		
		case IMU2_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_2_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[IMU2_HEADING_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[IMU2_ACC_Z_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[IMU2_GYR_Y_CSV_INDEX], 5);
			break;
		
		/* Debug ID range */
		
		case DAU_FR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DAU_FR_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DAU_FR_CURRENT_CSV_INDEX], 5);
			break;
		
		case DAU_FL_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DAU_FL_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DAU_FL_CURRENT_CSV_INDEX], 5);
			break;
		
		case DAU_REAR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_DEBUG_ID_COUNTER_INDEX]++;	
			intToStringUnsigned(data1, &DATA_BlockBuffer[DAU_REAR_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DAU_REAR_CURRENT_CSV_INDEX], 5);
			break;
		
		case SW_DEBUG_ID:
			CAN_ReceivedPacketsCounter[SW_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[SW_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[SW_CURRENT_CSV_INDEX], 5);
			break;
		
		case GCU_DEBUG_1_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[GCU_TEMP_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[GCU_CURR_CSV_INDEX], 5);
			intToStringUnsigned(data1, &DATA_BlockBuffer[H2O_PUMP_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[FUEL_PUMP_CURRENT_CSV_INDEX], 5);
			break;
		
		case GCU_DEBUG_2_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_2_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[GEARMOTOR_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[CLUTCH_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data1, &DATA_BlockBuffer[FAN_SX_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[FAN_DX_CURRENT_CSV_INDEX], 5);
			break;
	}
	
	return;
}

extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent)
{
	/* NOTE: Events are coded using chars NOT numbers! */
	switch(acquisitionState) {
		case ACQUISITION_OFF_STATE:
			USB_CloseFile(); 			/* Check if acquisition is on, close USB file and set the state variable */
		
			/* Get next state, based on the request received */
			switch(startAcquisitionEvent) {
				case ACQUISITION_ON_SW_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_SW_STATE;
					break;
						
				case ACQUISITION_ON_TELEMETRY_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_TELEMETRY_STATE;
					break;
				
				case ACQUISITION_ON_AUTO_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_AUTO_STATE;
					break;
				
				case ACQUISITION_ON_DEBUG_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_DEBUG_STATE;
					break;
				
				default:
					acquisitionState = ACQUISITION_OFF_STATE;
					break;
			}
					
			break;
		
		case ACQUISITION_ON_FROM_SW_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
			
			/* Get next state, based on the request received */
			if((startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) || 
				 (startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_SW_STATE;
			}
		
			break;
		
		case ACQUISITION_ON_FROM_TELEMETRY_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
			
			/* Get next state, based on the request received */
					if((startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) || 
						 (startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) ||
				     (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_TELEMETRY_STATE;
			}
			
			break;
		
		case ACQUISITION_ON_FROM_AUTO_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
			
			/* Get next state, based on the request received */
			if((startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) || 
			   (startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_AUTO_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_AUTO_STATE;
			}
			
			break;
		
		case ACQUISITION_ON_FROM_DEBUG_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
			
			/* Get next state, based on the request received */
			if(startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_DEBUG_STATE;
			}
			
			break;
		
		default:
			/* Should be never go there, but anyway reset the state machine */
			/* Put here error managing code */
			acquisitionState = ACQUISITION_OFF_STATE;
			break;
	}

	return;
}

extern inline uint8_t DATA_GetUsbReadyState(void)
{
	return DATA_StateBuffer[STATE_USB_READY_INDEX];
}

extern inline uint8_t DATA_GetAcquisitionState(void)
{
	return DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX];
}

extern inline void DATA_SetUsbPresentState(void)
{
	DATA_StateBuffer[STATE_USB_PRESENT_INDEX] = STATE_ON;
	return;
}

extern inline void DATA_SetUsbReadyState(void)
{
	DATA_StateBuffer[STATE_USB_READY_INDEX] = STATE_ON;
	return;
}

extern inline void DATA_SetAcquisitionState(void)
{
	DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_ON;
	return;
}

extern inline void DATA_ResetUsbPresentState(void)
{
	DATA_StateBuffer[STATE_USB_PRESENT_INDEX] = STATE_OFF;
	return;
}

extern inline void DATA_ResetUsbReadyState(void)
{
	DATA_StateBuffer[STATE_USB_READY_INDEX] = STATE_OFF;
	return;
}

extern inline void DATA_ResetAcquisitionState(void)
{
	DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_OFF;
	return;
}

extern void DATA_PacketReset(void)
{
	for(uint16_t i = 0; i < BUFFER_BLOCK_LEN; i++) {
		DATA_BlockBuffer[i] = '0';
	}
	
	DATA_BlockBuffer[HALL_EFFECT_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[HALL_EFFECT_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[HALL_EFFECT_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;	
	DATA_BlockBuffer[HALL_EFFECT_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_H20_SX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_H20_SX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_H20_DX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_H20_DX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_OIL_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_OIL_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_H20_ENGINE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[BATTERY_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[GEAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[RPM_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[TPS1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[PH2O_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[VH_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[SLIP_TARGET_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[SLIP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FAN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[PIT_LANE_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FUEL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[OIL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LAMBDA_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FLAG_SMOT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[L_FUEL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_SCARICO_1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[T_SCARICO_2_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LINEARE_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LOAD_CELL_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[BPS_FRONT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[APPS_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LINEARE_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LOAD_CELL_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[BPS_REAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[STEERING_WHEEL_ANGLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LINEARE_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LOAD_CELL_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LINEARE_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[LOAD_CELL_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR1_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR2_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR3_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR1_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR2_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR3_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR1_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR2_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR3_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR1_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR2_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IR3_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU1_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU2_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[IMU_DCU_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_FR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_FR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_FL_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_FL_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_REAR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DAU_REAR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[SW_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[SW_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[GCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[GCU_CURR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[H2O_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FUEL_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[GEARMOTOR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[CLUTCH_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FAN_SX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[FAN_DX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DCU_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[XBEE_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DUC_3V3_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DCU_12V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DCU_5V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[DCU_3V3_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[END_DATA_CSV_INDEX] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[END_ROW_CSV_INDEX - 1] = CHANNEL_SEPARATION;
	DATA_BlockBuffer[END_ROW_CSV_INDEX] = END_LINE;
	return;
}
