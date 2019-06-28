#include "data.h"
#include "usb_host.h"
#include "telemetry.h"
#include "id_can.h"
#include "data_conversion.h"
#include "string_utility.h"
#include "rtc.h"
#include "fatfs.h"
#include "flash_utility.h"


uint8_t DATA_BlockWriteIndex = 0;
uint8_t DATA_BlockReadIndex = 0;
uint8_t EFI_OffCounter = 0;
uint8_t EFI_IsAlive = EFI_IS_ALIVE_RESET;
uint8_t acquisitionState = ACQUISITION_OFF_STATE;
uint8_t DATA_BlockBuffer [BUFFER_POINTERS_NUMBER][BUFFER_BLOCK_LEN];
uint8_t DATA_StateBuffer [BUFFER_STATE_LEN];
uint16_t DATA_LapFlag = 0;
uint16_t data1 = 0;
uint16_t data2 = 0;
uint16_t data3 = 0;
uint16_t data4 = 0;
uint16_t DATA_RawCalibrationData [CALIBRATION_BUFFER_DATA_NUMBER];
uint32_t DATRON_Distance = 0;
float fData1 = 0.0f;
float fData2 = 0.0f;
float fData3 = 0.0f;
float fData4 = 0.0f;
float BPS_Front = 0.0f;
float BPS_Rear = 0.0f;
OffsetHandler_t OffsetHandler;
BaseType_t EFI_IsAlive_xHigherPriorityTaskWoken = pdFALSE;
extern uint8_t DATA_BlockWriteIndex;
extern uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_RECEIVED_PACKETS];
extern osSemaphoreId automaticStartAcquisitionSemaphoreHandle;
extern osMessageQId startAcquisitionEventHandle;
extern osMessageQId ErrorQueueHandle;


extern inline void DATA_CanParser_FIFO0(CAN_RxPacket_t *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
	data1 = (unpackedData->packetData[0] << 8) | unpackedData->packetData[1]; 		/* First data in the packet */
	data2 = (unpackedData->packetData[2] << 8) | unpackedData->packetData[3]; 		/* Second data in the packet */
	data3 = (unpackedData->packetData[4] << 8) | unpackedData->packetData[5]; 		/* Third data in the packet */
	data4 = (unpackedData->packetData[6] << 8) | unpackedData->packetData[7]; 		/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->packetHeader.StdId) {
		
		/* EFI ID range */
		
		case EFI_HALL_WHEEL_ID:
			CAN_ReceivedPacketsCounter[EFI_HALL_WHEEL_ID_COUNTER_INDEX]++;
			decimalToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_FR_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_FL_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_RR_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][HALL_EFFECT_RL_CSV_INDEX], 3, 1); 		/* Taking into account the division by 10 */
			break;
		
		case EFI_WATER_TEMPERATURE_ID:
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
			CAN_ReceivedPacketsCounter[EFI_GEAR_RPM_TPS_PH2O_COUNTER_INDEX]++;
			fData3 = TPS_DataConversion(data3);
			fData4 = WATER_PRESSURE_DataConversion(data4);
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][GEAR_CSV_INDEX], 1);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][RPM_CSV_INDEX], 5);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][TPS1_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][PH2O_CSV_INDEX], 5);
			break;
		
		case EFI_TRACTION_CONTROL_ID:
			fData3 = SLIP_DataCoversion((int16_t)data3);
			CAN_ReceivedPacketsCounter[EFI_TRACTION_CONTROL_ID_COUNTER_INDEX]++;
			decimalToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][VH_SPEED_CSV_INDEX], 3, 1); 			/* Taking into account the division by 10 */
			decimalToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][SLIP_TARGET_CSV_INDEX], 3, 1); 	/* Taking into account the division by 10 */
			decimalToString((int16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][SLIP_CSV_INDEX], 4, 1); 				/* Taking into account the division by 10 */
			intToStringUnsigned((uint16_t)data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][GEAR_AD_BITS_CSV_INDEX], 5);
			break;
		
		case EFI_MANUAL_LIMITER_FAN_H2O_PIT_LANE_ID:
			CAN_ReceivedPacketsCounter[EFI_MANUAL_LIMITER_FAN_H2O_PIT_LANE_COUNTER_INDEX]++;
			fData3 = H20_PUMP_DUTY_CYCLE_DataConversion(data3);
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][MANUAL_LIMITER_ACTIVE_CSV_INDEX], 1);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_CSV_INDEX], 1);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][H20_PUMP_DUTY_CYCLE_CSV_INDEX], 3);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][PIT_LANE_ACTIVE_CSV_INDEX], 1);
			break;
		
		case EFI_PRESSURES_LAMBDA_SMOT_ID:
			CAN_ReceivedPacketsCounter[EFI_PRESSURES_LAMBDA_SMOT_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][FUEL_PRESSURE_CSV_INDEX], 5);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][OIL_PRESSURE_CSV_INDEX], 5);
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][LAMBDA_CSV_INDEX], 1, 3);		/* Taking into account the division by 1000 */
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FLAG_SMOT_CSV_INDEX], 5);
			break;
		
		case EFI_LOIL_EXHAUST_ID:
			CAN_ReceivedPacketsCounter[EFI_LOIL_EXHAUST_ID_COUNTER_INDEX]++;
			fData1 = FUEL_LEVEL_DataConversion(data1) * 100.0f;						/* Taking into account the division by 100 */
			fData2 = EXHAUST_TEMPERATURE_DataConversion(data2);
			fData3 = EXHAUST_TEMPERATURE_DataConversion(data3);
			decimalToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][L_FUEL_CSV_INDEX], 2, 2);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_SCARICO_1_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][T_SCARICO_2_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][TC_ACTIVE_CSV_INDEX], 1);
			break;
		
		/* SW ID range */
		
		case SW_ACQUISITION_DCU_ID:
			CAN_ReceivedPacketsCounter[SW_ACQUISITION_DCU_ID_COUNTER_INDEX]++;
			DATA_SW_CAN_Management(data1, data2);
			break;
		
		/* GCU ID range */
		
		case GCU_CLUTCH_MODE_MAP_SW_ID:
			CAN_ReceivedPacketsCounter[GCU_CLUTCH_MODE_MAP_SW_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][CLUTCH_FEEDBACK_CSV_INDEX], 3);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][MODE_FEEDBACK_CSV_INDEX], 1);
			intToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][MAP_FEEDBACK_CSV_INDEX], 1);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][ANTISTALL_FEEDBACK_CSV_INDEX], 1);
			break;
	
		case GCU_TRACTION_LIMITER_AUTOG_ACC_SW_ID:
			CAN_ReceivedPacketsCounter[GCU_TRACTION_LIMITER_AUTOG_ACC_SW_ID_COUNTER_INDEX]++;
			intToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][TRACTION_FEEDBACK_CSV_INDEX], 1);
			intToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][RPM_LIMITER_FEEDBACK_CSV_INDEX], 3);
			intToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][AUTOGEARSHIFT_FEEDBACK_CSV_INDEX], 1);
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][ACCELERATION_FEEDBACK_CSV_INDEX], 1);
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
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][H2O_PUMP_CURRENT_CSV_INDEX], 2, 1);		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FUEL_PUMP_CURRENT_CSV_INDEX], 2, 1);		/* Taking into account the division by 10 */
			break;
		
		case GCU_DEBUG_2_ID:
			CAN_ReceivedPacketsCounter[GCU_DEBUG_2_ID_COUNTER_INDEX]++;
			decimalToStringUnsigned(data1, &DATA_BlockBuffer[DATA_BlockWriteIndex][GEARMOTOR_CURRENT_CSV_INDEX], 2, 1);		/* Taking into account the division by 10 */
			decimalToStringUnsigned(data2, &DATA_BlockBuffer[DATA_BlockWriteIndex][CLUTCH_CURRENT_CSV_INDEX], 2, 1);			/* Taking into account the division by 10 */
			decimalToStringUnsigned(data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_SX_CURRENT_CSV_INDEX], 2, 1);			/* Taking into account the division by 10 */
			decimalToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][FAN_DX_CURRENT_CSV_INDEX], 2, 1);			/* Taking into account the division by 10 */
			break;
		
		default:
			break;
	}
}

extern inline void DATA_CanParser_FIFO1(CAN_RxPacket_t *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
	data1 = (unpackedData->packetData[0] << 8) | unpackedData->packetData[1]; 		/* First data in the packet */
	data2 = (unpackedData->packetData[2] << 8) | unpackedData->packetData[3]; 		/* Second data in the packet */
	data3 = (unpackedData->packetData[4] << 8) | unpackedData->packetData[5]; 		/* Third data in the packet */
	data4 = (unpackedData->packetData[6] << 8) | unpackedData->packetData[7]; 		/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	switch(unpackedData->packetHeader.StdId) {

		/* DAU ID range */
		
		case DAU_FR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_ID_COUNTER_INDEX]++;
			DATA_RawCalibrationData[LINEAR_FR_CALIBRATION_INDEX] = data1;
			DATA_RawCalibrationData[LOAD_CELL_FR_CALIBRATION_INDEX] = data2;
			DATA_RawCalibrationData[APPS_CALIBRATION_INDEX] = data4;
			fData1 = LINEAR_FR_DataConversion(data1) * 100.0f;		/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_FR_DataConversion((int16_t)data2);
			fData3 = BPS_DataConversion(data3);
			BPS_Front = fData3;						/* Data for brake partition */
			fData3 = fData3 * 100.0f;			/* Taking into account the division by 100 */	
			fData4 = APPS_DataConversion(data4);
			decimalToString((int16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_FR_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_FR_CSV_INDEX], 4);
			decimalToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][BPS_FRONT_CSV_INDEX], 2, 2);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][APPS_CSV_INDEX], 3);
			break;
		
		case DAU_FL_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_ID_COUNTER_INDEX]++;
			DATA_RawCalibrationData[LINEAR_FL_CALIBRATION_INDEX] = data1;
			DATA_RawCalibrationData[LOAD_CELL_FL_CALIBRATION_INDEX] = data2;
			DATA_RawCalibrationData[STEER_ANGLE_CALIBRATION_INDEX] = data4;
			fData1 = LINEAR_FL_DataConversion(data1) * 100.0f;							/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_FL_DataConversion((int16_t)data2);
			fData3 = BPS_DataConversion(data3);
			BPS_Rear = fData3;						/* Data for brake partition */
			fData3 = fData3 * 100.0f;			/* Taking into account the division by 100 */
			fData4 = STEERING_WHEEL_ANGLE_DataConversion((int16_t)data4) * 10.0f ; 		/* Taking into account the division by 10 */
			decimalToString((int16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_FL_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_FL_CSV_INDEX], 4);
			decimalToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][BPS_REAR_CSV_INDEX], 2, 2);
			decimalToString((int16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][STEERING_WHEEL_ANGLE_CSV_INDEX], 3, 1);
			break;
		
		case DAU_REAR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_ID_COUNTER_INDEX]++;
			DATA_RawCalibrationData[LINEAR_RL_CALIBRATION_INDEX] = data1;
			DATA_RawCalibrationData[LOAD_CELL_RL_CALIBRATION_INDEX] = data2;
			DATA_RawCalibrationData[LINEAR_RR_CALIBRATION_INDEX] = data3;
			DATA_RawCalibrationData[LOAD_CELL_RR_CALIBRATION_INDEX] = data4;
			fData1 = LINEAR_RL_DataConversion(data1) * 100.0f;			/* Taking into account the division by 100 */
			fData2 = LOAD_CELL_RL_DataConversion((int16_t)data2);
			fData3 = LINEAR_RR_DataConversion(data3) * 100.0f;			/* Taking into account the division by 100 */
			fData4 = LOAD_CELL_RR_DataConversion((int16_t)data4);
			decimalToString((int16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_RL_CSV_INDEX], 2, 2);
			intToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_RL_CSV_INDEX], 4);
			decimalToString((int16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][LINEARE_RR_CSV_INDEX], 2, 2);
			intToString((int16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][LOAD_CELL_RR_CSV_INDEX], 4);
			break;
		
		case DAU_FL_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FL_IR_ID_COUNTER_INDEX]++;
			fData1 = IR_WHEEL_FL_DataConversion(data1);
			fData2 = IR_WHEEL_FL_DataConversion(data2);
			fData3 = IR_WHEEL_FL_DataConversion(data3);
			fData4 = IR_BRAKE_FL_DataConversion(data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_FL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_FL_CSV_INDEX], 3);
			break;
		
		case DAU_FR_IR_ID:
			CAN_ReceivedPacketsCounter[DAU_FR_IR_ID_COUNTER_INDEX]++;
			fData1 = IR_WHEEL_FR_DataConversion(data1);
			fData2 = IR_WHEEL_FR_DataConversion(data2);
			fData3 = IR_WHEEL_FR_DataConversion(data3);
			fData4 = IR_BRAKE_FR_DataConversion(data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_FR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_FR_CSV_INDEX], 3);
			break;
		
		case DAU_REAR_IR_RL_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RL_ID_COUNTER_INDEX]++;
			fData1 = IR_WHEEL_RL_DataConversion(data1);
			fData2 = IR_WHEEL_RL_DataConversion(data2);
			fData3 = IR_WHEEL_RL_DataConversion(data3);
			fData4 = IR_BRAKE_RL_DataConversion(data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_RL_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_RL_CSV_INDEX], 3);
			break;
		
		case DAU_REAR_IR_RR_ID:
			CAN_ReceivedPacketsCounter[DAU_REAR_IR_RR_ID_COUNTER_INDEX]++;
			fData1 = IR_WHEEL_RR_DataConversion(data1);
			fData2 = IR_WHEEL_RR_DataConversion(data2);
			fData3 = IR_WHEEL_RR_DataConversion(data3);
			fData4 = IR_BRAKE_RR_DataConversion(data4);
			intToStringUnsigned((uint16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR1_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR2_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData3, &DATA_BlockBuffer[DATA_BlockWriteIndex][IR3_RR_CSV_INDEX], 3);
			intToStringUnsigned((uint16_t)fData4, &DATA_BlockBuffer[DATA_BlockWriteIndex][BRAKE_IR_RR_CSV_INDEX], 3);
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
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU1_SENSORS_CALIBRATION_CSV_INDEX], 3);
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
			intToStringUnsigned(data4, &DATA_BlockBuffer[DATA_BlockWriteIndex][IMU2_SENSORS_CALIBRATION_CSV_INDEX], 3);
			break;
		
		/* DATRON ID range */
		
		case DATRON_1_ID:
			CAN_ReceivedPacketsCounter[DATRON_1_ID_COUNTER_INDEX]++;
			DATRON_Distance = ((data3 << 16) & 0xFFFF0000) | data4;
			int32ToString((int32_t)DATRON_Distance, &DATA_BlockBuffer[DATA_BlockWriteIndex][DATRON_DISTANCE_CSV_INDEX], 10);
			break;
		
		case DATRON_2_ID:
			CAN_ReceivedPacketsCounter[DATRON_2_ID_COUNTER_INDEX]++;
			fData1 = DATRON_Velocity_DataConversion((int16_t)data1) * 100.0f; 			/* Taking into account the division by 100 */
			fData2 = DATRON_Velocity_DataConversion((int16_t)data2) * 100.0f; 			/* Taking into account the division by 100 */
			decimalToString((int16_t)fData1, &DATA_BlockBuffer[DATA_BlockWriteIndex][DATRON_X_CSV_INDEX], 3, 2);
			decimalToString((int16_t)fData2, &DATA_BlockBuffer[DATA_BlockWriteIndex][DATRON_Y_CSV_INDEX], 3, 2);
			decimalToString((int16_t)data3, &DATA_BlockBuffer[DATA_BlockWriteIndex][DATRON_ANGLE_CSV_INDEX], 3, 2);   	/* Taking into account the division by 100 */
			break;
		
		default:
			break;
	}
}

extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent)
{
	uint8_t errorLetter = STATE_MACHINE_ERROR;
	
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
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
			acquisitionState = ACQUISITION_OFF_STATE;
			break;
	}
}

extern inline void DATA_ResetAcquisitionStateMachine(void)
{
	acquisitionState = ACQUISITION_OFF_STATE;
}

extern inline void DATA_CheckEfiIsAlive(void)
{
	xSemaphoreGiveFromISR(automaticStartAcquisitionSemaphoreHandle, &EFI_IsAlive_xHigherPriorityTaskWoken); 		/* Give semaphore to task when DMA is clear */
	portYIELD_FROM_ISR(EFI_IsAlive_xHigherPriorityTaskWoken); 																									/* Do context-switch if needed */
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

extern inline uint16_t DATA_GetLapFlag(void)
{
	return DATA_LapFlag;
}

extern inline void DATA_SetLapFlag(void)
{
	DATA_LapFlag = 1;
}

extern inline void DATA_ResetLapFlag(void)
{
	DATA_LapFlag = 0;
}

extern void DATA_PacketReset(void)
{
	DATA_BlockWriteIndex = 0;
	DATA_BlockReadIndex = 0;
	DATA_LapFlag = 0;
	
	for(uint8_t j = 0; j < BUFFER_POINTERS_NUMBER; j++) {
		for(uint16_t i = 0; i < BUFFER_BLOCK_LEN; i++) {
			DATA_BlockBuffer[j][i] = '0';
		}
		
		DATA_BlockBuffer[j][HALL_EFFECT_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][HALL_EFFECT_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][HALL_EFFECT_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;	
		DATA_BlockBuffer[j][HALL_EFFECT_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_H20_SX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_H20_SX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_H20_DX_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_H20_DX_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_OIL_IN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_OIL_OUT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_H20_ENGINE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BATTERY_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GEAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][RPM_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][TPS1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][PH2O_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][VH_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][SLIP_TARGET_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][SLIP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GEAR_AD_BITS_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][MANUAL_LIMITER_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FAN_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][H20_PUMP_DUTY_CYCLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][PIT_LANE_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FUEL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][OIL_PRESSURE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LAMBDA_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FLAG_SMOT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][L_FUEL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_SCARICO_1_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][T_SCARICO_2_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][TC_ACTIVE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LINEARE_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LOAD_CELL_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BPS_FRONT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][APPS_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LINEARE_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LOAD_CELL_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BPS_REAR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][STEERING_WHEEL_ANGLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LINEARE_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LOAD_CELL_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LINEARE_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LOAD_CELL_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR1_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR2_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR3_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BRAKE_IR_FL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR1_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR2_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR3_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BRAKE_IR_FR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR1_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR2_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR3_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BRAKE_IR_RL_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR1_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR2_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IR3_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][BRAKE_IR_RR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU1_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU2_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_HEADING_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_ACC_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_GYR_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_ACC_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_ACC_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_GYR_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_GYR_Z_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][IMU_DCU_SENSORS_CALIBRATION_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GPS_LATITUDE_MINUTES_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GPS_LONGITUDE_MINUTES_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GPS_SPEED_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][CLUTCH_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][MODE_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][MAP_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][ANTISTALL_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][TRACTION_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][RPM_LIMITER_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][AUTOGEARSHIFT_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][ACCELERATION_FEEDBACK_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][LAP_FLAG_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_FR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_FR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_FL_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_FL_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_REAR_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DAU_REAR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][SW_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][SW_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GCU_CURR_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][H2O_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FUEL_PUMP_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][GEARMOTOR_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][CLUTCH_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FAN_SX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][FAN_DX_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DCU_TEMP_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DCU_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][XBEE_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DUC_3V3_CURRENT_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DCU_12V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DCU_5V_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DCU_3V3_VOLTAGE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][END_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DATRON_DISTANCE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DATRON_X_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DATRON_Y_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][DATRON_ANGLE_CSV_SEPARATOR] = CHANNEL_SEPARATION;
		DATA_BlockBuffer[j][END_CSV_INDEX] = END_LINE;
	}
}

extern void DATA_ResetStateBuffer(void)
{
	DATA_StateBuffer[0] = '0';
	DATA_StateBuffer[1] = ';';
	DATA_StateBuffer[2] = '0';
	DATA_StateBuffer[3] = ';';
	DATA_StateBuffer[4] = '0';
	DATA_StateBuffer[5] = ';';
	DATA_StateBuffer[6] = '0';
	DATA_StateBuffer[7] = ';';
	DATA_StateBuffer[8] = '0';
}

static inline void DATA_SW_CAN_Management(uint8_t data1, uint8_t data2)
{
	uint8_t startAquisitionEvent = ACQUISITION_IDLE_REQUEST;
	BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
		
	switch(data1) {
		case SW_ACQUISITION_CAN_REQUEST:
			switch(data2) {
				case SW_START_ACQUISITION_CAN_REQUEST:
					startAquisitionEvent = ACQUISITION_ON_SW_REQUEST;		/* Start acquisition */
					xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);	
					break;
				
				case SW_STOP_ACQUISITION_CAN_REQUEST:
					startAquisitionEvent = ACQUISITION_OFF_SW_REQUEST;	/* Stop acquisition */
					xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
					break;
				
				default:
					break;
			}
			break;
		
		case SW_CALIBRATIONS_CAN_REQUEST:
			switch(data2) {
				case SW_APPS_ZERO_CALIBRATION_REQUEST:
					OffsetHandler.DATA_APPS_ZeroCalibrationOffset = DATA_RawCalibrationData[APPS_CALIBRATION_INDEX];
					Flash_Save_Calibration(&OffsetHandler);
					CAN_SW_CalibrationSendAck(APPS_ZERO_CALIBRATION_DONE);
					break;
				
				case SW_APPS_FULL_CALIBRATION_REQUEST:
					OffsetHandler.DATA_APPS_FullCalibrationOffset = DATA_RawCalibrationData[APPS_CALIBRATION_INDEX];
					Flash_Save_Calibration(&OffsetHandler);
					CAN_SW_CalibrationSendAck(APPS_FULL_CALIBRATION_DONE);
					break;
				
				case SW_LINEAR_CALIBRATION_REQUEST:
					OffsetHandler.DATA_LINEAR_FR_CalibrationOffset = DATA_RawCalibrationData[LINEAR_FR_CALIBRATION_INDEX];
					OffsetHandler.DATA_LINEAR_FL_CalibrationOffset = DATA_RawCalibrationData[LINEAR_FL_CALIBRATION_INDEX];
					OffsetHandler.DATA_LINEAR_RR_CalibrationOffset = DATA_RawCalibrationData[LINEAR_RR_CALIBRATION_INDEX];
					OffsetHandler.DATA_LINEAR_RL_CalibrationOffset = DATA_RawCalibrationData[LINEAR_RL_CALIBRATION_INDEX];
					Flash_Save_Calibration(&OffsetHandler);
					CAN_SW_CalibrationSendAck(LINEAR_CALIBRATION_DONE);
					break;
				
				case SW_LOAD_CELL_CALIBRATION_REQUEST:
					OffsetHandler.DATA_LOAD_CELL_FR_CalibrationOffset = DATA_RawCalibrationData[LOAD_CELL_FR_CALIBRATION_INDEX];				
					OffsetHandler.DATA_LOAD_CELL_FL_CalibrationOffset = DATA_RawCalibrationData[LOAD_CELL_FL_CALIBRATION_INDEX];				
					OffsetHandler.DATA_LOAD_CELL_RR_CalibrationOffset = DATA_RawCalibrationData[LOAD_CELL_RR_CALIBRATION_INDEX];				
					OffsetHandler.DATA_LOAD_CELL_RL_CalibrationOffset = DATA_RawCalibrationData[LOAD_CELL_RL_CALIBRATION_INDEX];
					Flash_Save_Calibration(&OffsetHandler);
					CAN_SW_CalibrationSendAck(LOAD_CELL_CALIBRATION_DONE);
					break;
				
				default:
					break;
			}
			break;
			
		default:
			break;
	}
}
