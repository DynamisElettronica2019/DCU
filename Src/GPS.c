#include "GPS.h"
#include "usart.h"
#include "cmsis_os.h"

/*global variables*/

NMEA_output_types_t NMEA_output;	/*global variable where conversion result are stored*/


uint8_t GPSFirstChar;							
uint8_t GPSParsingChar = '$';
uint8_t i = 0;
uint8_t GPSRawBuffer [GPS_MAX_LENGTH];	/*global buffer where raw nmea message are stored*/

extern osSemaphoreId GPSUnboxSemHandle;
BaseType_t xGPSHigherPriorityTaskWoken;


void GPS_Rx_Cplt(void){											/*uart interrupt callback function*/
		if (GPSFirstChar == '$'){								/*if initial string charater has been found*/
			
			GPSRawBuffer[i] = GPSParsingChar;			/*the char is saved as first element */
			i++;
			
			HAL_UART_Receive_DMA(&huart2,&GPSParsingChar, 1);		/*then it starts reciving the successive chars*/
	}
	 if (GPSParsingChar == '\n'){								/*if last char has been saved*/
			GPSRawBuffer[i] = '\n';									/*it stores the last char*/
			i = 0;
			GPSParsingChar = '$';										/*reinitialised variables needed*/
			xSemaphoreGiveFromISR(GPSUnboxSemHandle,&xGPSHigherPriorityTaskWoken );			/*calls the unboxing task*/
			portYIELD_FROM_ISR(xGPSHigherPriorityTaskWoken);
	}
	 if ((GPSFirstChar != '$') && (GPSParsingChar != '\n')){			/*if last char has not arrived yet, keep searching for it*/
		 HAL_UART_Receive_DMA(&huart2,&GPSFirstChar, 1);
	 }
		
}
void GPS_data_conversion(uint8_t * GPSRawBuffer){
	
	uint8_t GPSMessageID = GPS_get_messageID(GPSRawBuffer);		/*gest message type, needed for conversion*/
	
	switch (GPSMessageID){											/*based on message type recived, converts the raw nmea buffer*/
		
		case MESSAGE_TYPE_GGA :
			NMEA_output.NMEA_GGA_type = GPS_GGA_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_GLL :
			NMEA_output.NMEA_GLL_type = GPS_GLL_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_GSA :
			NMEA_output.NMEA_GSA_type = GPS_GSA_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_RMC :
			NMEA_output.NMEA_RMC_type = GPS_RMC_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_VTG :
			NMEA_output.NMEA_VTG_type = GPS_VTG_conversion(GPSRawBuffer); 
			break; 
		
		case MESSAGE_TYPE_UNKNOWN :
			/**/
			break;
		
		default :						/*should never arrive here, otherswise an error has occured*/
			break;
	}
	HAL_UART_Receive_DMA(&huart2,&GPSFirstChar, 1);			/*at the end of the conversion, restar searching for first char $*/
}
uint8_t GPS_get_messageID(uint8_t * buffer){										

	if (buffer[4] == 'G')
		return MESSAGE_TYPE_GGA;
	else if(buffer[4] == 'L')
		return MESSAGE_TYPE_GLL;
	else if (buffer[4] == 'S')
		return MESSAGE_TYPE_GSA;
	else if(buffer[3] == 'R')
		return MESSAGE_TYPE_RMC;
	else if(buffer[3] == 'V')
		return MESSAGE_TYPE_VTG;
	else
		return MESSAGE_TYPE_UNKNOWN;

}

NMEA_GGA_type_t GPS_GGA_conversion(uint8_t * buffer){
		
	uint8_t i = 7;													/*jump to first interesting char, after message type and ','*/
	uint8_t skipped = 0;										/*this variables keeps track of how many filed it has skipped over*/
	
	NMEA_GGA_type_t GPSOutputGGA;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								/*until it reached checksum char or it has gone over buffer length*/
		
		if (buffer[i] == ','){
			skipped++;
		}
	  else{
			switch(skipped){			/*based on the filed it is evaluating, act a defined type of conversion*/
				
				case 0:		//UTC TIME
					GPSOutputGGA.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])) + 2;		
					GPSOutputGGA.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGGA.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputGGA.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); /*skip the point*/
					i = i+ 8;		
					break;
				
				case 1:		//LATITUDE
					GPSOutputGGA.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputGGA.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGGA.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9])*60;
					i = i +9; 
					break;
				
				case 2:		//N_S
					GPSOutputGGA.N_S = buffer[i];
					break;
				
				case 3:		//LONGITUDE
					GPSOutputGGA.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGGA.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGGA.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10])*60;
					i = i+10; 
					break;
				
				case 4: 	//E_W
					GPSOutputGGA.E_W = buffer[i];
					break;
				
				case 5:		//POSITION FIX INDICATOR
					GPSOutputGGA.GGA_fix_valid = buffer[i] - 48;
					break;
				
				case 6:		//SATELITTES USED
					if (buffer[i+1] != ','){											/*if more than 9 satellities are in view*/
						GPSOutputGGA.number_satellites_used = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])); 
						i++;
					}
					else
						GPSOutputGGA.number_satellites_used = buffer[i] - 48;
					break;
				
				case 7:		//HDOP
					GPSOutputGGA.HDOP = (buffer[i]-48) + GPS_str_to_float(buffer[i+2],buffer[i+3]);		
					i = i+3;
					break;
				
				case 8:		//MLS ALTITUDE
					if(buffer[i + 2] == '.'){		/*under 100 MLS*/
						GPSOutputGGA.MSL_Altitude = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i = i +3; 								/*skip point and last decimal*/
					}
					else if(buffer[i + 3] == '.'){	/*beteween 100 and 1000 MLS*/
						GPSOutputGGA.MSL_Altitude = ((uint8_t)GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]));
						i = i +4;									/*skip point and last decimal*/
					}
					break;
				
				case 9:		//UNIT
					GPSOutputGGA.Unit = buffer[i];
					break;
				
				default:  
					buffer[i] = '*';	/*if it gest here, force the end of conversion*/
			}
		}
		
		i++;
	}
	return GPSOutputGGA;
}
NMEA_GLL_type_t GPS_GLL_conversion(uint8_t * buffer){
	uint8_t i = 7;													
	uint8_t skipped = 0;										
	
	NMEA_GLL_type_t GPSOutputGLL;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								
		
		if (buffer[i] == ','){
			skipped++;
		}
	  else{
			switch(skipped){
				
				case 0:		//LATITUDE
					GPSOutputGLL.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputGLL.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGLL.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9])*60;
					i = i +9;
					break;
				
				case 1:		//N_S
					GPSOutputGLL.N_S = buffer[i];
					break;
				
				case 2:		//LONGITUDE
					GPSOutputGLL.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGLL.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGLL.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10])*60;
					i = i+10;
					break;
				
				case 3:		//E_W
					GPSOutputGLL.E_W = buffer[i];
					break;
				
				case 4: 	//UTC TIME
					GPSOutputGLL.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])) + 2;		/*riverifica che effettivamente vengano mandati cosi i valori*/
					GPSOutputGLL.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGLL.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputGLL.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); //salto il punto
					i = i+ 8;		
					break;
				
				case 5:		//STATUS
					GPSOutputGLL.status = buffer[i];
					break;
				
				case 6:		//MODE INDICATOR
					GPSOutputGLL.mode_indicator = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	
			}
		}
		i++;
	}
	return GPSOutputGLL;
}
NMEA_GSA_type_t GPS_GSA_conversion(uint8_t * buffer){
	
	uint8_t i = 7;													
	uint8_t skipped = 0;										

	NMEA_GSA_type_t GPSOutputGSA;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){						
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//MODE 1
					GPSOutputGSA.GSA_mode = buffer[i];
					break;
				
				case 1:		//MODE 2
					GPSOutputGSA.fix_mode = buffer[i] - 48;
					break;
				
				case 2:		//SATELLITE ON CHANNEL 1
					if (buffer[i+2] == ','){		/*if more than 9 satellites are on view on channel 1*/
						GPSOutputGSA.satellites_used_ch1 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch1 = buffer[i] - 48;
					break;
				
				case 3:		//SATELLITE ON CHANNEL 2
					if (buffer[i+2] == ','){		/*if more than 9 satellites are on view on channel 2*/
						GPSOutputGSA.satellites_used_ch2 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch2 = buffer[i] - 48;
					break;
				
				default: 
					buffer[i] = '*';	
			}
		}
		
		i++;
	}
	return GPSOutputGSA;

}
NMEA_RMC_type_t GPS_RMC_conversion(uint8_t * buffer){		
	
	uint8_t i = 7;													
	uint8_t skipped = 0;
	NMEA_RMC_type_t GPSOutputRMC;
	
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//UTC TIME
					GPSOutputRMC.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])) + 2;		
					GPSOutputRMC.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputRMC.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputRMC.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); 
					i = i+ 8;		
					
					break;
				
				case 1:		//STATUS
					GPSOutputRMC.RMC_status = buffer[i];
					break;
				
				case 2:		//LATITUDE
					GPSOutputRMC.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputRMC.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputRMC.latitude.decimal_minutes = GPS_minuts_conversion(buffer[i+5], buffer[i+6], buffer[i+7], buffer[i+8], buffer[i+9])*60;
					i = i +9;
					break;
				
				case 3:		//N_S
					GPSOutputRMC.N_S = buffer[i];
					break;
				
				case 4: 	//LONGITUDE
					GPSOutputRMC.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputRMC.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputRMC.longitude.decimal_minutes = GPS_minuts_conversion(buffer[i+6],buffer[i+7],buffer[i+8],buffer[i+9],buffer[i+10])*60;
					i = i+10;
					break;
				
				case 5:		//E_W
					GPSOutputRMC.E_W = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND (KNOTS)
					if(buffer[i+1] != '.' ){  						/*speed greater than 9 knots*/
						GPSOutputRMC.speed.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputRMC.speed.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6])*1000;
						i = i+ 6; 
					}
					else{
						GPSOutputRMC.speed.unit = buffer[i] - 48;
						GPSOutputRMC.speed.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4])*1000;
						i = i+4;	
					}
					
					break;
				
				case 7:		//COURSE OVER GROUND 
					/*empty filed*/
					break;
				
				case 8:		//DATE
					GPSOutputRMC.UTC_date.day = ((uint8_t)(GPS_str_to_int(0, buffer[i], buffer[i+1])));
					GPSOutputRMC.UTC_date.month = ((uint8_t)(GPS_str_to_int(0, buffer[i+2], buffer[i+3])));
					GPSOutputRMC.UTC_date.year = ((uint8_t)(GPS_str_to_int(0, buffer[i+4], buffer[i+5])));
					i = i +5;
					break;
				
				default: 
					buffer[i] = '*';	
			}
		}
		
		i++;
 }
	return GPSOutputRMC;
 }

NMEA_VTG_type_t GPS_VTG_conversion(uint8_t * buffer){
	
	uint8_t i = 7;													
	uint8_t skipped = 0;										

	NMEA_VTG_type_t GPSOutputVTG;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//COURSE OVER GROUND
					GPSOutputVTG.Course_over_ground.gradi = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputVTG.Course_over_ground.decimi = ((uint8_t)(GPS_str_to_float(buffer[i+4],buffer[i+5]))*60);
					i = i +5;
					break;
				
				case 1:		//REFERENCE
					GPSOutputVTG.reference = buffer[i];
					break;
				
				case 2:		//COURSE OVER GROUND MAGNETIC
					/*empty filed*/
					break;
				
				case 3:		//REFERENCE
					/*empty filed*/
					break;
				
				case 4: 	//SPEED OVER GROUD(KNOTS)
					if(buffer[i+1] != '.' ){  						/*speed greater than 9 knot*/
						GPSOutputVTG.speed1.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputVTG.speed1.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6])*1000;
						i = i+ 6; 
					}
					else{
						GPSOutputVTG.speed1.unit = buffer[i] - 48;
						GPSOutputVTG.speed1.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4])*1000;
						i = i+4;	
					}
					break;
				
				case 5:		//UNIT
					GPSOutputVTG.speed1_unit = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND(KM/H)
					if(buffer[i+1] != '.' && buffer[i+2] == '.'){  						/*speed between 10 and 100 km/h*/
						GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6])*1000;
						i = i+ 6; 
					}
					else if(buffer[i + 2] != '.' && buffer[i+1] != '.'){															/*speed greater than 100 km/h*/
					GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]));
					GPSOutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 4],buffer[i+ 5],buffer[i+ 6])*1000;
					i = i +6; 
					}
					else{			/*speed under 10 km/h*/
						GPSOutputVTG.speed2.unit = buffer[i] - 48;
						GPSOutputVTG.speed2.decimal = GPS_speed_decimal_conversion(buffer[i+ 2],buffer[i+ 3],buffer[i+ 4])*1000;
						i = i+4;	
					}
					break;
				
				case 7:		//UNIT
					GPSOutputVTG.speed2_unit = buffer[i];
					break;
				
				case 8: 	//MODE INDICATOR
					GPSOutputVTG.VTG_mode = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	
			}
		}
		
		i++;
	}
	return GPSOutputVTG;
}



uint16_t GPS_str_to_int(uint8_t centinaia, uint8_t decine, uint8_t unita){
	
	if (centinaia != 0){
		centinaia = centinaia -48;		/*conversion from char to uint*/
		decine  = decine -48;
		unita = unita - 48;
		return centinaia*100 + decine*10 + unita;
	}
	else if (centinaia == 0){		/*as I use this function, it will never happen decine = 0*/
		decine  = decine -48;
		unita = unita - 48;
		return decine*10 + unita;
	}
}

double GPS_str_to_float(uint8_t decimi, uint8_t  centesimi){
	
	decimi = decimi -48;			/*conversion from char to uint*/
	centesimi = centesimi -48;
	
	return ((double)((decimi)*10 + (centesimi)))/100;
	
}

double GPS_minuts_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi, uint8_t decimillesimi, uint8_t centimillesimi){
	decimi = decimi -48;							/*cenversion from char to uint*/
	centesimi = centesimi -48;
	millesimi = millesimi -48;
	decimillesimi = decimillesimi -48;
	centimillesimi = centimillesimi -48;
	
	return	((double)decimi)/10+((double)centesimi)/100+((double)millesimi)/1000+((double)decimillesimi)/10000 + ((double)centimillesimi)/100000;
	
}

double GPS_speed_decimal_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi){
	
	decimi = decimi -48;				/*conversion from char to uint*/
	centesimi = centesimi -48;
	millesimi = millesimi -48;
	
	return ((double)decimi)/10 + ((double)centesimi)/100 + ((double)millesimi)/1000 ;
}


void GPS_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second){  
	if (GPS_is_fix_valid() == FIX_VALID){									/*if data converted are valid, stores them in variables given*/
		*hour = NMEA_output.NMEA_RMC_type.UTC_time.hours;
		*minut = NMEA_output.NMEA_RMC_type.UTC_time.minutes;
		*second = NMEA_output.NMEA_RMC_type.UTC_time.seconds;
	}
	else{																									/*otherwise it stores uncoherent data*/
		*hour = 255;
		*minut = 255;
		*second = 255;
	}
}

uint8_t GPS_is_fix_valid(void){		
	if (NMEA_output.NMEA_RMC_type.RMC_status == 'A'){  /*returns FIX_VALID if stored data are valid*/
			return FIX_VALID;
	}
	return FIX_NOT_VALID;		/*da testare*/
}


void GPS_get_date(uint8_t *day, uint8_t *month, uint8_t *year){		
	if (GPS_is_fix_valid() == FIX_VALID){												/*if stored data are coherent, stores them in given variables*/
		*day = NMEA_output.NMEA_RMC_type.UTC_date.day;
		*month = NMEA_output.NMEA_RMC_type.UTC_date.month;
		*year = NMEA_output.NMEA_RMC_type.UTC_date.year;
	}
	else{																												/*otherwise it stores uncoherent data*/
		*day = 255;
		*month = 255;
		*year = 255;		
	}
}


void GPS_init(void){
	uint8_t GPSFixRate10Hz[] = {0xB5, 0x62, /*header*/
															0x06, 0x08, /*ID*/
															0x06, 0x00, /*length*/
															0x64, 0x00, /*measRate*/
															0x01, 0x00, /*navRate*/
															0x01, 0x00, /*timeRef*/
															0x7A, 0x12};/*CK_A, CK_B*/
	
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GLL, STOP_GLL_LENGTH, 100);													/*blocca i messaggi GLL*/
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GSV, STOP_GSV_LENGTH, 100);													/*blocca i messaggi GSV*/
	HAL_UART_Transmit(&huart2, (uint8_t *)STOP_GGA, STOP_GGA_LENGTH, 100);													/*blocca i messaggi GGA*/
	HAL_UART_Transmit(&huart2, (uint8_t *)SET_BAUDRATE_38400, SET_BAUDRATE_38400_LENGTH, 100);			/*setto il baudrate del gps a 38400*/
	GPS_USART2_UART_Init_38400();																																		/*reinizializzo la uart a 38400*/
	HAL_Delay(50);																																									/*necessario?*/
	HAL_UART_Transmit(&huart2,GPSFixRate10Hz, GPS_SET_FIX_RATE_10HZ_LENGTH, 100); 									/*SETTO IL FIX RATE A 10HZ*/
}
void GPS_USART2_UART_Init_38400(void){			/*initialize uart at 38400 baudrate*/
	
	huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart2.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
}
void GPS_USART2_UART_Init_57600(void){			/*initialize uart at 57600 baudrate*/
	
	huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart2.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
}


