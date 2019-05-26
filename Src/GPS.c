#include "GPS.h"
#include "cmsis_os.h"
#include "usart.h"
#include "data.h"
#include "telemetry.h"
#include "string_utility.h"

uint8_t GPS_MessageID;
uint8_t GPS_ParsingChar = '$';
uint8_t GPS_FirstChar;
uint8_t i = 0;
uint8_t GPS_RawBuffer [GPS_MAX_LENGTH];		/* Buffer of raw NMEA messages */
NMEA_output_types_t NMEA_output;					/* Struct where conversion results are stored */
NMEA_GGA_type_t GPS_OutputGGA;
NMEA_GLL_type_t GPS_OutputGLL;
NMEA_GSA_type_t GPS_OutputGSA;
NMEA_RMC_type_t GPS_OutputRMC;
NMEA_VTG_type_t GPS_OutputVTG;
extern uint8_t DATA_BlockBuffer [BUFFER_BLOCK_LEN];
extern osMessageQId ErrorQueueHandle;

uint8_t GPS_FixRate10Hz[] = {0xB5, 0x62, 	/*header*/
														 0x06, 0x08, 	/*ID*/
														 0x06, 0x00, 	/*length*/
														 0x64, 0x00, 	/*measRate*/
														 0x01, 0x00, 	/*navRate*/
														 0x01, 0x00, 	/*timeRef*/
														 0x7A, 0x12};	/*CK_A, CK_B*/


extern void GPS_Init(void)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GLL, STOP_GLL_LENGTH, 100);											 	/* Not send GLL messages */
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GSV, STOP_GSV_LENGTH, 100);												/* Not send GSV messages */
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GGA, STOP_GGA_LENGTH, 100);												/* Not send GGA messages */
	HAL_UART_Transmit(&huart2, (uint8_t *)SET_BAUDRATE_38400, SET_BAUDRATE_38400_LENGTH, 100);		/* Baudrate at 38400 */
	USART2_Init_38400();																																					/* UART init at 38400 baud */
	HAL_UART_Transmit(&huart2, GPS_FixRate10Hz, GPS_SET_FIX_RATE_10HZ_LENGTH, 100); 							/* Set fix rate at 10 Hz */
}

extern inline void GPS_DataConversion(uint8_t * GPSRawBuffer)
{
	uint8_t errorLetter = GPS_NMEA_UNKNOW_ERROR;
	GPS_MessageID = GPS_get_messageID(GPS_RawBuffer);		/* Get message type, needed for conversion */
	
	/* Based on message type recived, converts the raw NMEA buffer */
	switch(GPS_MessageID) {
		case MESSAGE_TYPE_GGA:
			GPS_GGA_conversion(GPS_RawBuffer);
			NMEA_output.NMEA_GGA_type = GPS_OutputGGA;
			break;
		
		case MESSAGE_TYPE_GLL:
			GPS_GLL_conversion(GPS_RawBuffer);
			NMEA_output.NMEA_GLL_type = GPS_OutputGLL;
			break;
		
		case MESSAGE_TYPE_GSA:
			GPS_GSA_conversion(GPS_RawBuffer); 
			NMEA_output.NMEA_GSA_type = GPS_OutputGSA;
			break;
		
		case MESSAGE_TYPE_RMC:
			GPS_RMC_conversion(GPS_RawBuffer);
			NMEA_output.NMEA_RMC_type = GPS_OutputRMC;
			break;
		
		case MESSAGE_TYPE_VTG:
			GPS_VTG_conversion(GPS_RawBuffer);
			NMEA_output.NMEA_VTG_type = GPS_OutputVTG;
			break; 
		
		case MESSAGE_TYPE_UNKNOWN:
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
			break;
		
		default:
			break;
	}
	
	HAL_UART_Receive_DMA(&huart2, &GPS_FirstChar, 1);			/* At the end of the conversion, restar searching for first char $ */
}

static inline uint8_t GPS_get_messageID(uint8_t * buffer)
{		
	switch(buffer[4]) {
		case 'G': return MESSAGE_TYPE_GGA;
		case 'L': return MESSAGE_TYPE_GLL;
		case 'S': return MESSAGE_TYPE_GSA;
		case 'M': return MESSAGE_TYPE_RMC;
		case 'T': return MESSAGE_TYPE_VTG;		
		default: return MESSAGE_TYPE_UNKNOWN;
	}
}

static inline void GPS_GGA_conversion(uint8_t * buffer)
{		
	uint8_t i = 7;					/* Jump to first interesting char, after message type and ',' */
	uint8_t skipped = 0;		/* Keeps track of how many filed it has skipped over */
	
	/* Until it reached checksum char or it has gone over buffer length */
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){	
		if(buffer[i] == ',') {
			skipped++;
		}
	  else {
			
			/* Based on the field it is evaluating, perform a defined type of conversion */
			switch(skipped) {
				
				case 0:		/* UTC TIME */
					GPS_OutputGGA.UTC_time.hours = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]) + 2;		
					GPS_OutputGGA.UTC_time.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]);
					GPS_OutputGGA.UTC_time.seconds = (uint8_t)GPS_StrToInt(0, buffer[i+4], buffer[i+5]);
					GPS_OutputGGA.UTC_time.decimal_seconds = (uint8_t)GPS_StrToInt(0, buffer[i+7], buffer[i+8]); 	/* Skip the point */
					i = i + 8;		
					break;
				
				case 1:		/* LATITUDE */
					GPS_OutputGGA.latitude.degrees = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]);
					GPS_OutputGGA.latitude.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]);
					GPS_OutputGGA.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9]) * 60;
					i = i + 9; 
					break;
				
				case 2:		/* N_S */
					GPS_OutputGGA.N_S = buffer[i];
					break;
				
				case 3:		/* LONGITUDE */
					GPS_OutputGGA.longitude.degrees = GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]);
					GPS_OutputGGA.longitude.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+3], buffer[i+4]);
					GPS_OutputGGA.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10]) * 60;
					i = i + 10; 
					break;
				
				case 4: 	/* E_W */
					GPS_OutputGGA.E_W = buffer[i];
					break;
				
				case 5:		/* POSITION FIX INDICATOR */
					GPS_OutputGGA.GGA_fix_valid = buffer[i] - 48;
					break;
				
				case 6:		/* SATELITTES USED */
					/* If more than 9 satellities are in view */	
					if (buffer[i+1] != ',') {
						GPS_OutputGGA.number_satellites_used = ((uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1])); 
						i++;
					}
					else {
						GPS_OutputGGA.number_satellites_used = buffer[i] - 48;
					}
					break;
				
				case 7:		/* HDOP */
					GPS_OutputGGA.HDOP = (buffer[i]-48) + GPS_str_to_float(buffer[i+2],buffer[i+3]);		
					i = i + 3;
					break;
				
				case 8:		/* MLS ALTITUDE */
					/* Under 100 MLS */
					if(buffer[i + 2] == '.'){
						GPS_OutputGGA.MSL_Altitude = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]);
						i = i +3; 	/* Skip point and last decimal */
					}
					
					/* Beteween 100 and 1000 MLS */
					else if(buffer[i + 3] == '.') {
						GPS_OutputGGA.MSL_Altitude = (uint8_t)GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]);
						i = i +4;		/* Skip point and last decimal */
					}
					break;
				
				case 9:		/* UNIT */
					GPS_OutputGGA.Unit = buffer[i];
					break;
				
				default:  
					buffer[i] = '*';	 /* If it gest here, force the end of conversion */
					break;
			}
		}
		
		i++;
	}
}

static inline void GPS_GLL_conversion(uint8_t * buffer)
{
	uint8_t i = 7;													
	uint8_t skipped = 0;										
	
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)) {
		if (buffer[i] == ',') {
			skipped++;
		}
	  else {
			switch(skipped) {
				case 0:		/* LATITUDE */
					GPS_OutputGLL.latitude.degrees = ((uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]));
					GPS_OutputGLL.latitude.minutes = ((uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]));
					GPS_OutputGLL.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9]) * 60;
					i = i + 9;
					break;
				
				case 1:		/* N_S */
 					GPS_OutputGLL.N_S = buffer[i];
					break;
				
				case 2:		/* LONGITUDE */
					GPS_OutputGLL.longitude.degrees = GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]);
					GPS_OutputGLL.longitude.minutes = ((uint8_t)GPS_StrToInt(0, buffer[i+3], buffer[i+4]));
					GPS_OutputGLL.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10]) * 60;
					i = i + 10;
					break;
				
				case 3:		/* E_W */
					GPS_OutputGLL.E_W = buffer[i];
					break;
				
				case 4: 	/* UTC TIME */
					GPS_OutputGLL.UTC_time.hours = ((uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1])) + 2;
					GPS_OutputGLL.UTC_time.minutes = ((uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]));
					GPS_OutputGLL.UTC_time.seconds = ((uint8_t)GPS_StrToInt(0, buffer[i+4], buffer[i+5]));
					GPS_OutputGLL.UTC_time.decimal_seconds = ((uint8_t)GPS_StrToInt(0, buffer[i+7], buffer[i+8]));
					i = i + 8;		
					break;
				
				case 5:		/* STATUS */
					GPS_OutputGLL.status = buffer[i];
					break;
				
				case 6:		/* MODE INDICATOR */
					GPS_OutputGLL.mode_indicator = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	
			}
		}
		
		i++;
	}
}

void static inline GPS_GSA_conversion(uint8_t * buffer)
{
	uint8_t i = 7;													
	uint8_t skipped = 0;										

	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)) {
		if (buffer[i] == ',') {
			skipped++;
		}
		else {
			switch(skipped) {
				case 0:		/* MODE 1 */
					GPS_OutputGSA.GSA_mode = buffer[i];
					break;
				
				case 1:		/* MODE 2 */
					GPS_OutputGSA.fix_mode = buffer[i] - 48;
					break;
				
				case 2:		/* SATELLITE ON CHANNEL 1 */
					/* If more than 9 satellites are on view on channel 1 */
					if (buffer[i+2] == ',') {
						GPS_OutputGSA.satellites_used_ch1 = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]);
						i++;
					}
					else {
						GPS_OutputGSA.satellites_used_ch1 = buffer[i] - 48;
					}
					break;
				
				case 3:		/* SATELLITE ON CHANNEL 2 */
					/* If more than 9 satellites are on view on channel 2*/
					if (buffer[i+2] == ',') {
						GPS_OutputGSA.satellites_used_ch2 = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]);
						i++;
					}
					else {
						GPS_OutputGSA.satellites_used_ch2 = buffer[i] - 48;
					}
					break;
				
				default: 
					buffer[i] = '*';	
					break;
			}
		}
		
		i++;
	}
}

static inline void GPS_RMC_conversion(uint8_t * buffer)
{	
	uint8_t i = 7;											
	uint8_t skipped = 0;
	float GPS_LatitudeTemp;
	float GPS_LongitudeTemp;
	
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)) {
		if (buffer[i] == ',') {
			skipped++;
		}
		else {
			switch(skipped) {
				case 0:		/* UTC TIME */
					GPS_OutputRMC.UTC_time.hours = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]) + 2;		
					GPS_OutputRMC.UTC_time.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]);
					GPS_OutputRMC.UTC_time.seconds = (uint8_t)GPS_StrToInt(0, buffer[i+4], buffer[i+5]);
					GPS_OutputRMC.UTC_time.decimal_seconds = (uint8_t)GPS_StrToInt(0, buffer[i+7], buffer[i+8]); 
					i = i + 8;
					break;
				
				case 1:		/* STATUS */
					GPS_OutputRMC.RMC_status = buffer[i];
					break;
				
				case 2:		/* LATITUDE */
					GPS_OutputRMC.latitude.degrees = (uint8_t)GPS_StrToInt(0, buffer[i], buffer[i+1]);
					GPS_OutputRMC.latitude.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+2], buffer[i+3]);
					GPS_OutputRMC.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9]) * 60;
					GPS_LatitudeTemp = (GPS_OutputRMC.latitude.decimal_minutes / 60.0f) * 100000.0f;
					intToStringUnsigned((uint16_t)GPS_LatitudeTemp, &DATA_BlockBuffer[GPS_LATITUDE_MINUTES_CSV_INDEX], 5);
					i = i + 9;
					break;
				
				case 3:		/* N_S */
					GPS_OutputRMC.N_S = buffer[i];
					break;
				
				case 4: 	/* LONGITUDE */
					GPS_OutputRMC.longitude.degrees = GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]);
					GPS_OutputRMC.longitude.minutes = (uint8_t)GPS_StrToInt(0, buffer[i+3], buffer[i+4]);
					GPS_OutputRMC.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10]) * 60;
					GPS_LongitudeTemp = (GPS_OutputRMC.longitude.decimal_minutes / 60.0f) * 100000.0f;
					intToStringUnsigned((uint16_t)GPS_LongitudeTemp, &DATA_BlockBuffer[GPS_LONGITUDE_MINUTES_CSV_INDEX], 5);
					i = i + 10;
					break;
				
				case 5:		/* E_W */
					GPS_OutputRMC.E_W = buffer[i];
					break;
				
				case 6:		/* SPEED OVER GROUND (KNOTS) */
				/* Speed greater than 9 knots */	
				if(buffer[i+1] != '.' ) {
						GPS_OutputRMC.speed.unit = (uint8_t)GPS_StrToInt(0, buffer[i+ 1], buffer[i+2]);
						GPS_OutputRMC.speed.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6]) * 1000;
						i = i + 6; 
					}
					else {
						GPS_OutputRMC.speed.unit = buffer[i] - 48;
						GPS_OutputRMC.speed.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4]) * 1000;
						i = i + 4;	
					}
					break;
				
				case 8:		/* DATE */
					GPS_OutputRMC.UTC_date.day = (uint8_t)(GPS_StrToInt(0, buffer[i], buffer[i+1]));
					GPS_OutputRMC.UTC_date.month = (uint8_t)(GPS_StrToInt(0, buffer[i+2], buffer[i+3]));
					GPS_OutputRMC.UTC_date.year = (uint8_t)(GPS_StrToInt(0, buffer[i+4], buffer[i+5]));
					i = i + 5;
					break;
				
				default:
					buffer[i] = '*';
					break;
			}
		}
		
		i++;
 }
}

static inline void GPS_VTG_conversion(uint8_t * buffer)
{
	uint8_t i = 7;										
	uint8_t skipped = 0;										

	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)) {
		if (buffer[i] == ',') {
			skipped++;
		}
		else {
			switch(skipped) {
				case 0:		/* COURSE OVER GROUND */
					GPS_OutputVTG.Course_over_ground.gradi = GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]);
					GPS_OutputVTG.Course_over_ground.decimi = (uint8_t)(GPS_str_to_float(buffer[i+4],buffer[i+5])) * 60;
					i = i + 5;
					break;
				
				case 1:		/* REFERENCE */
					GPS_OutputVTG.reference = buffer[i];
					break;
				
				case 4: 	/* SPEED OVER GROUD(KNOTS) */
					/* Speed greater than 9 knot */
					if(buffer[i+1] != '.' ) {
						GPS_OutputVTG.speed1.unit = (uint8_t)GPS_StrToInt(0, buffer[i+ 1], buffer[i+2]);
						GPS_OutputVTG.speed1.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6]) * 1000;
						i = i+ 6; 
					}
					else {
						GPS_OutputVTG.speed1.unit = buffer[i] - 48;
						GPS_OutputVTG.speed1.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4]) * 1000;
						i = i + 4;	
					}
					break;
				
				case 5:		/* UNIT */
					GPS_OutputVTG.speed1_unit = buffer[i];
					break;
				
				case 6:		/* SPEED OVER GROUND (KM/H) */
					/* Speed between 10 and 100 km/h */
					if(buffer[i+1] != '.' && buffer[i+2] == '.'){
						GPS_OutputVTG.speed2.unit = ((uint8_t)GPS_StrToInt(0, buffer[i+ 1], buffer[i+2]));
						GPS_OutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6])*1000;
						i = i+ 6; 
					}
					
					/* Speed greater than 100 km/h */
					else if(buffer[i + 2] != '.' && buffer[i+1] != '.') {
						GPS_OutputVTG.speed2.unit = ((uint8_t)GPS_StrToInt(buffer[i], buffer[i+1], buffer[i+2]));
						GPS_OutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6]) * 1000;
						i = i +6; 
					}
					
					/* Speed under 10 km/h*/
					else {
						GPS_OutputVTG.speed2.unit = buffer[i] - 48;
						GPS_OutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4]) * 1000;
						i = i+4;	
					}
					
					/* Saving into CSV data buffer */
					intToStringUnsigned((uint16_t)GPS_OutputVTG.speed2.unit, &DATA_BlockBuffer[GPS_SPEED_CSV_INDEX], 3);
					DATA_BlockBuffer[GPS_SPEED_CSV_INDEX + 3] = '.';
					intToStringUnsigned((uint16_t)GPS_OutputVTG.speed2.decimal, &DATA_BlockBuffer[GPS_SPEED_CSV_INDEX + 4], 3);
					break;
				
				case 7:		/* UNIT */
					GPS_OutputVTG.speed2_unit = buffer[i];
					break;
				
				case 8: 	/* MODE INDICATOR */
					GPS_OutputVTG.VTG_mode = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';
					break;
			}
		}
		
		i++;
	}
}

static inline float GPS_minuts_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi, uint8_t decimillesimi, uint8_t centimillesimi)
{
	float result;
	
	decimi = decimi - 48;										/* Conversion from char to uint */
	centesimi = centesimi - 48;							/* Conversion from char to uint */
	millesimi = millesimi - 48;							/* Conversion from char to uint */
	decimillesimi = decimillesimi - 48;			/* Conversion from char to uint */
	centimillesimi = centimillesimi - 48;		/* Conversion from char to uint */
	result = (float)decimi/10.0f + (float)centesimi/100.0f + (float)millesimi/1000.0f;
	result = result + (float)decimillesimi/10000.0f + (float)centimillesimi/100000.0f;
	return	result;
}

static inline float GPS_speed_decimal_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi)
{
	float result;
	
	decimi = decimi - 48;					/* Conversion from char to uint */
	centesimi = centesimi - 48;		/* Conversion from char to uint */
	millesimi = millesimi - 48;		/* Conversion from char to uint */
	result = (float)decimi/10.0f + (float)centesimi/100.0f + (float)millesimi/1000.0f;
	return  result;
}

static inline uint16_t GPS_StrToInt(uint8_t centinaia, uint8_t decine, uint8_t unita)
{
	uint16_t result;
	
	if(centinaia != 0) {
		centinaia = centinaia - 48; 	/* Con1version from char to uint */
		decine  = decine - 48; 				/* Conversion from char to uint */
		unita = unita - 48; 					/* Conversion from char to uint */
		result = centinaia*100 + decine*10 + unita;
	}
	
	/* It will never happen decine = 0 */
	else if (centinaia == 0) {
		decine  = decine - 48;				/* Conversion from char to uint */
		unita = unita - 48; 					/* Conversion from char to uint */
		result = decine*10 + unita;
	}
	
	return result;
}

static inline float GPS_str_to_float(uint8_t decimi, uint8_t  centesimi)
{
	float result;
	
	decimi = decimi - 48;					/* Conversion from char to uint */	
	centesimi = centesimi - 48;		/* Conversion from char to uint */
	result = ((float)decimi*10.0f + (float)centesimi) / 100.0f;
	return result;
}
