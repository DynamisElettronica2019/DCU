#include "data.h"
#include "usb_host.h"
#include "id_can.h"
#include "data_conversion.h"
#include "string_utility.h"

uint16_t data1 = 0;
uint16_t data2 = 0;
uint16_t data3 = 0;
uint16_t data4 = 0;
float fData1 = 0.0f;
float fData2 = 0.0f;
float fData3 = 0.0f;
float fData4 = 0.0f;
uint8_t DATA_BlockWriteIndex = 0;
uint8_t DATA_BlockReadIndex = 1;
uint8_t EFI_OffCounter = 0;
uint8_t EFI_IsAlive = EFI_IS_ALIVE_RESET;
uint8_t acquisitionState = ACQUISITION_OFF_STATE;
uint8_t DATA_BlockBuffer [BUFFER_BLOCKS_NUMBER][BUFFER_BLOCK_LEN];
uint8_t DATA_StateBuffer [BUFFER_STATE_LEN] = "0;0;0;0;0";
BaseType_t EFI_IsAlive_xHigherPriorityTaskWoken = pdFALSE;
extern uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_ACQUIRED_CHANNELS];
extern osSemaphoreId automaticStartAcquisitionSemaphoreHandle;
extern osMessageQId startAcquisitionEventHandle;


extern inline void DATA_CanParser(CAN_RxPacket_t *unpackedData)
{	
	uint8_t startAquisitionEvent = ACQUISITION_IDLE_REQUEST;
	BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
	
	/* Get the four 16 bit data from the 8 bit CAN messages */
	data1 = (unpackedData->packetData[0] << 8) | unpackedData->packetData[1]; 		/* First data in the packet */
	data2 = (unpackedData->packetData[2] << 8) | unpackedData->packetData[3]; 		/* Second data in the packet */
	data3 = (unpackedData->packetData[4] << 8) | unpackedData->packetData[5]; 		/* Third data in the packet */
	data4 = (unpackedData->packetData[6] << 8) | unpackedData->packetData[7]; 		/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->packetHeader.StdId) {
		
		/* EFI ID range */
		
		case EFI_HALL_WHEEL_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_HALL_WHEEL_ID_COUNTER_INDEX]++;
			decimalToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_FR_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_FL_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_RR_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_RL_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			break;
		
		case EFI_WATER_TEMPERATURE_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_WATER_TEMPERATURE_ID_COUNTER_INDEX]++;
			fData1 = EFI_TEMPERATURE_DataConversion(data1);
			fData2 = EFI_TEMPERATURE_DataConversion(data2);
			fData3 = EFI_TEMPERATURE_DataConversion(data3);
			fData4 = EFI_TEMPERATURE_DataConversion(data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_H20_SX_IN_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_H20_SX_OUT_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_H20_DX_IN_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_H20_DX_OUT_CSV_INDEX], 3);
			break;
		
		case EFI_OIL_T_ENGINE_BAT_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_OIL_T_ENGINE_BAT_ID_COUNTER_INDEX]++;
			fData1 = EFI_TEMPERATURE_DataConversion(data1);
			fData2 = EFI_TEMPERATURE_DataConversion(data2);
			fData3 = T_H20_ENGINE_DataConversion(data3);
			fData4 = BATTERY_VOLTAGE_DataConversion(data4) * 100.0f; 		/* Taking into account the division by 100 */
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_OIL_IN_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_OIL_OUT_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_H20_ENGINE_CSV_INDEX], 3);
			decimalToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BATTERY_VOLTAGE_CSV_INDEX], 2, 2);
			break;
		
		case EFI_GEAR_RPM_TPS_PH2O_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_MANUAL_LIMITER_FAN_H2O_PIT_LANE_COUNTER_INDEX]++;
			fData3 = TPS_DataConversion(data3);
			fData4 = WATER_PRESSURE_DataConversion(data4);
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][GEAR_CSV_INDEX], 1);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][RPM_CSV_INDEX], 5);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][TPS1_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][PH2O_CSV_INDEX], 5);
			break;
		
		case EFI_TRACTION_CONTROL_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_TRACTION_CONTROL_ID_COUNTER_INDEX]++;
			decimalToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][VH_SPEED_CSV_INDEX], 3, 1); 				/* Taking into account the division by 10 */
			decimalToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][SLIP_TARGET_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][SLIP_CSV_INDEX], 4, 1); 					/* Taking into account the division by 10 */
			break;
		
		case EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_FUEL_FAN_H2O_LAUNCH_ID_COUNTER_INDEX]++;
			fData3 = H20_PUMP_DUTY_CYCLE_DataConversion(data3);
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][MANUAL_LIMITER_ACTIVE_CSV_INDEX], 1);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_CSV_INDEX], 1);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][H20_PUMP_DUTY_CYCLE_CSV_INDEX], 3);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][PIT_LANE_ACTIVE_CSV_INDEX], 1);
			break;
		
		case EFI_PRESSURES_LAMBDA_SMOT_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][FUEL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][OIL_PRESSURE_CSV_INDEX], 5);
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][LAMBDA_CSV_INDEX], 1, 3);		/* Taking into account the division by 1000 */
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FLAG_SMOT_CSV_INDEX], 5);
			break;
		
		case EFI_LOIL_EXHAUST_ID:
			DATA_SetEfiIsAlive();
			CAN_ReceivedPacketsCounter[EFI_LOIL_EXHAUST_ID_COUNTER_INDEX]++;
			fData1 = FUEL_LEVEL_DataConversion(data1) * 100.0f;						/* Taking into account the division by 100 */
			fData2 = EXHAUST_TEMPERATURE_DataConversion(data2);
			fData3 = EXHAUST_TEMPERATURE_DataConversion(data3);
			decimalToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][L_FUEL_CSV_INDEX], 2, 2);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_SCARICO_1_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_SCARICO_2_CSV_INDEX], 3);
			break;
		
		/* SW ID range */
		case SW_ACQUISITION_DCU_ID:
			if(data1 == SW_ACQUISITION_CAN_REQUEST) {
				if(data2 == SW_START_ACQUISITION_CAN_REQUEST) {
					startAquisitionEvent = ACQUISITION_ON_TELEMETRY_REQUEST;		/* Start acquisition */
					xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
				}
				else if(data2 == SW_STOP_ACQUISITION_CAN_REQUEST) {
					startAquisitionEvent = ACQUISITION_OFF_TELEMETRY_REQUEST;		/* Stop acquisition */
					xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
				}
			}
			break;
		
		/* DAU ID range */
		
		case DAU_FR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_ID_COUNTER_INDEX]++;
			fData1 = LINEAR_DataConversion((uint16_t)data1) * 100.0f;			/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_DataConversion((int16_t)data2);
			fData3 = BPS_DataConversion((uint16_t)data3) * 100.0f;				/* Taking into account the division by 100 */
			fData4 = APPS_DataConversion((uint16_t)data4);
			decimalToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_FR_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_FR_CSV_INDEX], 4);
			decimalToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][BPS_FRONT_CSV_INDEX], 2, 2);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][APPS_CSV_INDEX], 3);
			break;
		
		case DAU_FL_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_ID_COUNTER_INDEX]++;
			fData1 = LINEAR_DataConversion((uint16_t)data1) * 100.0f;									/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_DataConversion((int16_t)data2);
			fData3 = BPS_DataConversion((uint16_t)data3) * 100.0f;										/* Taking into account the division by 100 */
			fData4 = STEERING_WHEEL_ANGLE_DataConversion((int16_t)data4) * 10.0f; 		/* Taking into account the division by 10 */
			decimalToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_FL_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_FL_CSV_INDEX], 4);
			decimalToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][BPS_REAR_CSV_INDEX], 2, 2);
			decimalToString((int16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][STEERING_WHEEL_ANGLE_CSV_INDEX], 3, 1);
			break;
		
		case DAU_REAR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_ID_COUNTER_INDEX]++;
			fData1 = LINEAR_DataConversion((uint16_t)data1) * 100.0f;			/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_DataConversion((int16_t)data2);
			fData3 = LINEAR_DataConversion((uint16_t)data3) * 100.0f;			/* Taking into account the division by 100 */
			fData4 = LOAD_CELL_DataConversion((int16_t)data4);
			decimalToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_RL_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_RL_CSV_INDEX], 4);
			decimalToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_RR_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_RR_CSV_INDEX], 4);
			break;
		
		case DAU_FL_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_IR_ID_COUNTER_INDEX]++;
			fData1 = IR_DataConversion((uint16_t)data1);
			fData2 = IR_DataConversion((uint16_t)data2);
			fData3 = IR_DataConversion((uint16_t)data3);
			fData4 = IR_DataConversion((uint16_t)data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_FL_CSV_SEPARATOR], 3);
			break;
		
		case DAU_FR_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_IR_ID_COUNTER_INDEX]++;
			fData1 = IR_DataConversion((uint16_t)data1);
			fData2 = IR_DataConversion((uint16_t)data2);
			fData3 = IR_DataConversion((uint16_t)data3);
			fData4 = IR_DataConversion((uint16_t)data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_FR_CSV_SEPARATOR], 3);
			break;
		
		case DAU_REAR_IR_RL_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RL_ID_COUNTER_INDEX]++;
			fData1 = IR_DataConversion((uint16_t)data1);
			fData2 = IR_DataConversion((uint16_t)data2);
			fData3 = IR_DataConversion((uint16_t)data3);
			fData4 = IR_DataConversion((uint16_t)data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_RL_CSV_SEPARATOR], 3);
			break;
		
		case DAU_REAR_IR_RR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RR_ID_COUNTER_INDEX]++;
			fData1 = IR_DataConversion((uint16_t)data1);
			fData2 = IR_DataConversion((uint16_t)data2);
			fData3 = IR_DataConversion((uint16_t)data3);
			fData4 = IR_DataConversion((uint16_t)data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_RR_CSV_SEPARATOR], 3);
			break;
		
		/* IMU ID range */
		
		case IMU1_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_1_ID_COUNTER_INDEX]++;
			decimalToString((int16_t)data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_ACC_X_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_ACC_Y_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_GYR_X_CSV_INDEX], 3, 1);				/* Taking into account the division by 10 */
			decimalToString((int16_t)data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_GYR_Z_CSV_INDEX], 3, 1);				/* Taking into account the division by 10 */
			break;
		
		case IMU1_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU1_DATA_2_ID_COUNTER_INDEX]++;
			decimalToString((int16_t)data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_HEADING_CSV_INDEX], 3, 2);			/* Taking into account the division by 100 */
			decimalToString((int16_t)data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_ACC_Z_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_GYR_Y_CSV_INDEX], 3, 1);				/* Taking into account the division by 10 */
			break;
		
		case IMU2_DATA_1_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_1_ID_COUNTER_INDEX]++;
			decimalToString((int16_t)data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_ACC_X_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_ACC_Y_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_GYR_X_CSV_INDEX], 3, 1);				/* Taking into account the division by 10 */
			decimalToString((int16_t)data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_GYR_Z_CSV_INDEX], 3, 1);				/* Taking into account the division by 10 */
			break;
		
		case IMU2_DATA_2_ID:
			CAN_ReceivedPacketsCounter[IMU2_DATA_2_ID_COUNTER_INDEX]++;
			decimalToString((int16_t)data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_HEADING_CSV_INDEX], 3, 2);			/* Taking into account the division by 100 */
			decimalToString((int16_t)data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_ACC_Z_CSV_INDEX], 3, 2);				/* Taking into account the division by 100 */
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_GYR_Y_CSV_INDEX], 3,1);				/* Taking into account the division by 10 */
			break;
		
		/* Debug ID range */
		
		case DAU_FR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_FR_TEMP_CSV_INDEX], 2);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_FR_CURRENT_CSV_INDEX], 3);
			break;
		
		case DAU_FL_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_FL_TEMP_CSV_INDEX], 2);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_FL_CURRENT_CSV_INDEX], 3);
			break;
		
		case DAU_REAR_DEBUG_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_DEBUG_ID_COUNTER_INDEX]++;	
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_REAR_TEMP_CSV_INDEX], 2);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][DAU_REAR_CURRENT_CSV_INDEX], 3);
			break;
		
		case SW_DEBUG_ID:
			CAN_ReceivedPacketsCounter[SW_DEBUG_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][SW_TEMP_CSV_INDEX], 2);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][SW_CURRENT_CSV_INDEX], 4);
			break;
		
		case GCU_DEBUG_1_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_1_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][GCU_TEMP_CSV_INDEX], 2);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][GCU_CURR_CSV_INDEX], 3);
			intToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][H2O_PUMP_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FUEL_PUMP_CURRENT_CSV_INDEX], 5);
			break;
		
		case GCU_DEBUG_2_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_2_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][GEARMOTOR_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][CLUTCH_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_SX_CURRENT_CSV_INDEX], 5);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_DX_CURRENT_CSV_INDEX], 5);
			break;
	}
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
					if(DATA_GetUsbReadyState() == STATE_ON) {
						acquisitionState = ACQUISITION_ON_FROM_SW_STATE;
					}
					break;
						
				case ACQUISITION_ON_TELEMETRY_REQUEST:
					if(DATA_GetUsbReadyState() == STATE_ON) {
						acquisitionState = ACQUISITION_ON_FROM_TELEMETRY_STATE;
					}
					break;
				
				case ACQUISITION_ON_AUTO_REQUEST:
					if(DATA_GetUsbReadyState() == STATE_ON) {
						acquisitionState = ACQUISITION_ON_FROM_AUTO_STATE;
					}
					break;
				
				case ACQUISITION_ON_DEBUG_REQUEST:
					if(DATA_GetUsbReadyState() == STATE_ON) {
						acquisitionState = ACQUISITION_ON_FROM_DEBUG_STATE;
					}
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
}

extern inline void DATA_CheckEfiIsAlive(void)
{
	if(automaticStartAcquisitionSemaphoreHandle != NULL) { 																												/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(automaticStartAcquisitionSemaphoreHandle, &EFI_IsAlive_xHigherPriorityTaskWoken); 		/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(EFI_IsAlive_xHigherPriorityTaskWoken); 																									/* Do context-switch if needed */
	}
}

extern inline void DATA_AutomaticStartAcquisitionManager(void)
{
	uint8_t startAquisitionEvent = ACQUISITION_IDLE_REQUEST;
	BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
	
	/* Check if there are the automatic acquisition start conditions */
	if((DATA_GetAcquisitionState() == STATE_OFF) && (EFI_IsAlive == EFI_IS_ALIVE_SET)) {
		
		/* Use the write pointer because of acquisition off as default, so the pointers are not swaped */
		if((DATA_BlockBuffer[DATA_BlockWriteIndex][RPM_CSV_INDEX + 2] != '0') || (DATA_BlockBuffer[DATA_BlockWriteIndex][RPM_CSV_INDEX + 3] != '0')) {
			EFI_OffCounter = 0;
			startAquisitionEvent = ACQUISITION_ON_AUTO_REQUEST;
			xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
		}
	}
	
	/* Check if there are the automatic acquisition stop conditions */
	else if(DATA_GetAcquisitionState() == STATE_ON) {
		
		/* EFI_IsAlive is reset every second and set evert EFI packet received */
		if(EFI_IsAlive == EFI_IS_ALIVE_RESET) {
			EFI_OffCounter++;
			
			/* If EFI alive flag is off for more than 30 seconds: stop automatic acquisition */
			if((EFI_OffCounter >= 30)) {
				startAquisitionEvent = ACQUISITION_OFF_AUTO_REQUEST;
				xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
			}
		}
		else if(EFI_IsAlive == EFI_IS_ALIVE_SET) {
			EFI_OffCounter = 0;
		}
	}
}

extern inline void DATA_SwapDataPackePointers(void)
{
	uint8_t temp;
	
	for(uint16_t i = 0; i < BUFFER_BLOCK_LEN; i++)
	{
		DATA_BlockBuffer[DATA_BlockReadIndex][i] = DATA_BlockBuffer[DATA_BlockWriteIndex][i];
	}
	
	temp = DATA_BlockWriteIndex;
	DATA_BlockWriteIndex = DATA_BlockReadIndex;
	DATA_BlockReadIndex = temp;
}

extern inline void DATA_SetEfiIsAlive(void)
{
	EFI_IsAlive = EFI_IS_ALIVE_SET;
}

extern inline void DATA_ResetEfiIsAlive(void)
{
	EFI_IsAlive = EFI_IS_ALIVE_RESET;
}

extern inline uint8_t DATA_GetUsbReadyState(void)
{
	return DATA_StateBuffer[STATE_USB_READY_INDEX];
}

extern inline uint8_t DATA_GetAcquisitionState(void)
{
	return DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX];
}

extern inline uint8_t DATA_GetTelemetryState(void)
{
	return DATA_StateBuffer[STATE_TELEMETRY_ON_INDEX];
}

extern inline void DATA_SetUsbReadyState(void)
{
	DATA_StateBuffer[STATE_USB_READY_INDEX] = STATE_ON;
}

extern inline void DATA_SetAcquisitionState(void)
{
	DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_ON;
}

extern inline void DATA_SetTelemetryState(void)
{
	DATA_StateBuffer[STATE_TELEMETRY_ON_INDEX] = STATE_ON;
}

extern inline void DATA_ResetUsbReadyState(void)
{
	DATA_StateBuffer[STATE_USB_READY_INDEX] = STATE_OFF;
}

extern inline void DATA_ResetAcquisitionState(void)
{
	DATA_StateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_OFF;
}

extern inline void DATA_ResetTelemetryState(void)
{
	DATA_StateBuffer[STATE_TELEMETRY_ON_INDEX] = STATE_OFF;
}

extern void DATA_PacketReset(void)
{
	DATA_BlockWriteIndex = 0;
	DATA_BlockReadIndex = 1;
	
	for(uint8_t i = 0; i < 2; i++) {
		for(uint16_t j = 0; j < BUFFER_BLOCK_LEN; j++) {
			DATA_BlockBuffer[DATA_BlockWriteIndex][j] = '0';
		}
		
		DATA_BlockBuffer[i][HALL_EFFECT_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][HALL_EFFECT_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][HALL_EFFECT_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;	
		DATA_BlockBuffer[i][HALL_EFFECT_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_H20_SX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_H20_SX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_H20_DX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_H20_DX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_OIL_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_OIL_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_H20_ENGINE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BATTERY_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GEAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][RPM_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][TPS1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][PH2O_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][VH_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][SLIP_TARGET_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][SLIP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FAN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][PIT_LANE_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FUEL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][OIL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LAMBDA_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FLAG_SMOT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][L_FUEL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_SCARICO_1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][T_SCARICO_2_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LINEARE_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LOAD_CELL_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BPS_FRONT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][APPS_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LINEARE_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LOAD_CELL_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BPS_REAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][STEERING_WHEEL_ANGLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LINEARE_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LOAD_CELL_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LINEARE_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][LOAD_CELL_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR1_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR2_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR3_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BRAKE_IR_FL_CSV_INDEX] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR1_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR2_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR3_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BRAKE_IR_FR_CSV_INDEX] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR1_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR2_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR3_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BRAKE_IR_RL_CSV_INDEX] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR1_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR2_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IR3_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][BRAKE_IR_RR_CSV_INDEX] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU1_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU2_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][IMU_DCU_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GPS_LATITUDE_MINUTES_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GPS_LONGITUDE_MINUTES_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GPS_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_FR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_FR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_FL_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_FL_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_REAR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DAU_REAR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][SW_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][SW_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GCU_CURR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][H2O_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FUEL_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][GEARMOTOR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][CLUTCH_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FAN_SX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][FAN_DX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DCU_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][XBEE_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DUC_3V3_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DCU_12V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DCU_5V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][DCU_3V3_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][END_ROW_CSV_INDEX - 1] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[i][END_ROW_CSV_INDEX] = END_LINE;
	}
}
