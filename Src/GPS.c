#include "GPS.h"
#include "usart.h"
#include "cmsis_os.h"

//user variables

NMEA_output_types_t NMEA_output;	//dove salvo i risultati della conversione
uint8_t GPSMessageID;							//globale solo per comodità di debug per adesso

uint8_t GPSFirstChar;
uint8_t GPSParsingChar = '$';
uint8_t i = 0;
uint8_t GPSRawBuffer[GPS_MAX_LENGTH];

extern osSemaphoreId GPSUnboxSemHandle;
extern osSemaphoreId GPSSetSemHandle;
BaseType_t xGPSHigherPriorityTaskWoken;


void GPS_Rx_Cplt(void){
		if (GPSFirstChar == '$'){								//se ho trovato il carattere iniziale della stringa
			
			GPSRawBuffer[i] = GPSParsingChar;			//lo salvo
			i++;
			
			HAL_UART_Receive_DMA(&huart2,&GPSParsingChar, 1);		//e comincio a leggere i caratteri successivi
	}
	 if (GPSParsingChar == '\n'){								//se sono arrivato alla fine del messaggio
			GPSRawBuffer[i] = '\n';									//salvo l'ultimo carattere
			i = 0;
			GPSParsingChar = '$';										//riinizializzo le variabili che mi servono
			xSemaphoreGiveFromISR(GPSUnboxSemHandle,&xGPSHigherPriorityTaskWoken );			//chiamo la task di spacchettamento
			portYIELD_FROM_ISR(xGPSHigherPriorityTaskWoken);
	}
	 if ((GPSFirstChar != '$') && (GPSParsingChar != '\n')){			//se non ho ancora trovato il carattere iniziale continuo a cercarlo
		 HAL_UART_Receive_DMA(&huart2,&GPSFirstChar, 1);
	 }
		
}
void GPS_parse_data(uint8_t * GPSRawBuffer){
	
	GPSMessageID = GPS_get_messageID(GPSRawBuffer);		//capisco che tipo di messaggio ho ricevuto
	
	switch (GPSMessageID){											// in base al messaggio che mi è arrivato faccio la conversione del pacchetto
		
		case MESSAGE_TYPE_GGA :
			NMEA_output.NMEA_GGA_type = GPS_GGA_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_GLL :
			//NMEA_output.NMEA_GLL_type = GPS_GLL_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_GSA :
			//NMEA_output.NMEA_GSA_type = GPS_GSA_conversion(GPSRawBuffer); 
			break;
		
		case MESSAGE_TYPE_GSV :
			//NMEA_output.NMEA_GSV_type = GPS_GSV_conversion(GPSRawBuffer); 
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
		
		default :						//non dovrei mai arrivare qui in ogni caso
			break;
	}
	HAL_UART_Receive_DMA(&huart2,&GPSFirstChar, 1);			//finita la conversionie, ricomincio a cercare il carattere iniziale
}
uint8_t GPS_get_messageID(uint8_t * buffer){										//questa funzione funziona solo se in buffer i caratteri identificatori del messaggio sono contenuti nei primo 5 elementi

	if (buffer[4] == 'G')
		return MESSAGE_TYPE_GGA;
	else if(buffer[4] == 'L')
		return MESSAGE_TYPE_GLL;
	else if (buffer[4] == 'S')
		return MESSAGE_TYPE_GSA;
	else if(buffer[5] == 'V')								
		return MESSAGE_TYPE_GSV;
	else if(buffer[3] == 'R')
		return MESSAGE_TYPE_RMC;
	else if(buffer[3] == 'V')
		return MESSAGE_TYPE_VTG;
	else
		return MESSAGE_TYPE_UNKNOWN;

}

NMEA_GGA_type_t GPS_GGA_conversion(uint8_t * buffer){
		
	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato
	
	NMEA_GGA_type_t GPSOutputGGA;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
	  else{
			switch(skipped){
				
				case 0:		//UTC TIME
					GPSOutputGGA.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));		/*riverifica che effettivamente vengano mandati cosi i valori*/
					GPSOutputGGA.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGGA.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputGGA.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); //salto il punto
					i = i+ 8;		//Non incremento di 9 perchè alla fine dell'else c'è gia un incremento
					break;
				
				case 1:		//LATITUDE
					GPSOutputGGA.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputGGA.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGGA.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+5], buffer[i+6])*60));
					i = i +9; //salto gli ultimi 3 decimali
					break;
				
				case 2:		//N_S
					GPSOutputGGA.N_S = buffer[i];
					break;
				
				case 3:		//LONGITUDE
					GPSOutputGGA.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGGA.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGGA.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+6], buffer[i+7])*60));
					i = i+10; //salto gli ulyimi 3 decimali
					break;
				
				case 4: 	//E_W
					GPSOutputGGA.E_W = buffer[i];
					break;
				
				case 5:		//POSITION FIX INDICATOR
					GPSOutputGGA.GGA_fix_valid = buffer[i] - 48;
					break;
				
				case 6:		//SATELITTES USED
					if (buffer[i+1] != ','){											//se ho piu di 9 satelliti in vista
						GPSOutputGGA.number_satellites_used = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])); 
						i++;
					}
					else
						GPSOutputGGA.number_satellites_used = buffer[i] - 48;
					break;
				
				case 7:		//HDOP
					GPSOutputGGA.HDOP = GPS_str_to_float(buffer[i+2],buffer[i+3]);		//salto lo zero e il punto
					i = i+3;
					break;
				
				case 8:		//MLS ALTITUDE
					if(buffer[i + 2] == '.'){		//se sono sotto i 100 metri di altitudine
						GPSOutputGGA.MSL_Altitude = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i = i +3; 								//salto anche il punto e la prima cifra decimale
					}
					else if(buffer[i + 3] == '.'){	//se sono tra i 100 e i 1000 metri di altitudine
						GPSOutputGGA.MSL_Altitude = ((uint8_t)GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]));
						i = i +4;									//salto anche il punto e la prima cifra decimale
					}
					break;
				
				case 9:		//UNIT
					GPSOutputGGA.Unit = buffer[i];
					break;
				
				default:  //se si vogliono leggere anche altre parti del messaggio basta continuare a implementare questo switch case. per velocizzare il tempo di spacchettamento tengo solo i dati necessari per adesso
					buffer[i] = '*';	//se entro nel default forzo la terminazione della conversione
			}
		}
		
		i++;
	}
	return GPSOutputGGA;
}
NMEA_GLL_type_t GPS_GLL_conversion(uint8_t * buffer){
	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato
	
	NMEA_GLL_type_t GPSOutputGLL;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
	  else{
			switch(skipped){
				
				case 0:		//LATITUDE
					GPSOutputGLL.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputGLL.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGLL.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+5], buffer[i+6])*60));
					i = i +9;
					break;
				
				case 1:		//N_S
					GPSOutputGLL.N_S = buffer[i];
					break;
				
				case 2:		//LONGITUDE
					GPSOutputGLL.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGLL.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGLL.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+6], buffer[i+7])*60));
					i = i+10;
					break;
				
				case 3:		//E_W
					GPSOutputGLL.E_W = buffer[i];
					break;
				
				case 4: 	//UTC TIME
					GPSOutputGLL.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));		/*riverifica che effettivamente vengano mandati cosi i valori*/
					GPSOutputGLL.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGLL.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputGLL.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); //salto il punto
					i = i+ 8;		//Non incremento di 9 perchè alla fine dell'else c'è gia un incremento
					break;
				
				case 5:		//STATUS
					GPSOutputGLL.status = buffer[i];
					break;
				
				case 6:		//MODE INDICATOR
					GPSOutputGLL.mode_indicator = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	//forzo la terminazione della conversione
			}
		}
		
		i++;
	}
	return GPSOutputGLL;
}
NMEA_GSA_type_t GPS_GSA_conversion(uint8_t * buffer){
	
	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato

	NMEA_GSA_type_t GPSOutputGSA;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum
		
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
					if (buffer[i+2] == ','){		//se ho piu di 9 satelliti sul primo canale
						GPSOutputGSA.satellites_used_ch1 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch1 = buffer[i] - 48;
					break;
				
				case 3:		//SATELLITE ON CHANNEL 2
					if (buffer[i+2] == ','){		//se ho piu di 9 satelliti sul secondo canale
						GPSOutputGSA.satellites_used_ch2 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch2 = buffer[i] - 48;
					break;
				
				default: 
					buffer[i] = '*';	//se si vogliono leggere anche altre parti del messaggio basta continuare a implementare questo switch case. per velocizzare il tempo di spacchettamento tengo solo i dati necessari per adesso
			}
		}
		
		i++;
	}
	return GPSOutputGSA;

}

NMEA_GSV_type_t GPS_GSV_conversion(uint8_t * buffer){ //in teoria potrei fornire i dati di tutti e 4 i gps, per semplicità in prima scrittura tengo solo un messaggio, che sarà l'ultimo di quelli arrivati. inoltre vado a leggere i dati solo del primo canale

	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;
	NMEA_GSV_type_t GPSOutputGSV;
	uint8_t NumberOfMessage = buffer[i];
	i++;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//MESSAGE NUMBER
					GPSOutputGSV.message_number = buffer[i] - 48;
					break;
				
				case 1:		//SATELLITES IN VIEW
					if(buffer[i+1]!= ','){					//se ho piu di 10 satelliti in vista
						GPSOutputGSV.number_of_satellites_in_view = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else 
						GPSOutputGSV.number_of_satellites_in_view = buffer[i] - 48;
					break;
				
				case 2:		//SATELLITE ID NUMBER
					GPSOutputGSV.satellite_ID = ((uint8_t)GPS_str_to_int(0,buffer[i], buffer[i+1]));
					i++;
					break;
				
				case 3:		//ELEVATION
					GPSOutputGSV.satellite_elevation_degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					i++;
					break;
				
				case 4: 	//AZIMUTH
					GPSOutputGSV.satellite_azimuth = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					i = i+2;
					break;
				
				case 5:		//SNR
					GPSOutputGSV.SNR = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					i++;
					break;
				
				default: 
					buffer[i] = '*';	//forzo la terminazione della conversione
			}
		}
		
		i++;
 }
	return GPSOutputGSV;
}


NMEA_RMC_type_t GPS_RMC_conversion(uint8_t * buffer){		
	
	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;
	NMEA_RMC_type_t GPSOutputRMC;
	
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//UTC TIME
					GPSOutputRMC.UTC_time.hours = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));		/*riverifica che effettivamente vengano mandati cosi i valori*/
					GPSOutputRMC.UTC_time.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputRMC.UTC_time.seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+4], buffer[i+5]));
					GPSOutputRMC.UTC_time.decimal_seconds = ((uint8_t)GPS_str_to_int(0, buffer[i+7], buffer[i+8])); //salto il punto
					i = i+ 8;		//Non incremento di 9 perchè alla fine dell'else c'è gia un incremento
					
					break;
				
				case 1:		//STATUS
					GPSOutputRMC.RMC_status = buffer[i];
					break;
				
				case 2:		//LATITUDE
					GPSOutputRMC.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputRMC.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputRMC.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+5], buffer[i+6])*60));
					i = i +9;
					break;
				
				case 3:		//N_S
					GPSOutputRMC.N_S = buffer[i];
					break;
				
				case 4: 	//LONGITUDE
					GPSOutputRMC.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputRMC.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputRMC.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(buffer[i+6], buffer[i+7])*60));
					i = i+10;
					break;
				
				case 5:		//E_W
					GPSOutputRMC.E_W = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND (KNOTS)
					if(buffer[i+1] != '.' ){  						//>10 nodi
						GPSOutputRMC.speed.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputRMC.speed.decimal = ((uint8_t)GPS_str_to_float(buffer[i+4], buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else{
						GPSOutputRMC.speed.unit = buffer[i] - 48;
						GPSOutputRMC.speed.decimal = (GPS_str_to_float(buffer[i+2], buffer[i+3])*100);
						i = i+4;	//salto l'ultima cifra decimale
					}
					
					break;
				
				case 7:		//COURSE OVER GROUND 
					/*campo vuoto per il momento*/
					break;
				
				case 8:		//DATE
					GPSOutputRMC.UTC_date.day = ((uint8_t)(GPS_str_to_int(0, buffer[i], buffer[i+1])));
					GPSOutputRMC.UTC_date.month = ((uint8_t)(GPS_str_to_int(0, buffer[i+2], buffer[i+3])));
					GPSOutputRMC.UTC_date.year = ((uint8_t)(GPS_str_to_int(0, buffer[i+4], buffer[i+5])));
					i = i +5;
					break;
				
				default: 
					buffer[i] = '*';	//forzo la terminazione della conversione
			}
		}
		
		i++;
 }
	return GPSOutputRMC;
 }

NMEA_VTG_type_t GPS_VTG_conversion(uint8_t * buffer){
	
	uint8_t i = 7;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato

	NMEA_VTG_type_t GPSOutputVTG;
	while((buffer[i] != '*') && (i < GPS_MAX_LENGTH)){								//fintanto che non arrivo al checksum o ho superato la lunghezza massima del buffer
		
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
					i = i + 5;
					//campo vuoto
					break;
				
				case 3:		//REFERENCE
					//campo vuoto
					break;
				
				case 4: 	//SPEED OVER GROUD(KNOTS)
					if(buffer[i+1] != '.' ){  						//>10 nodi
						GPSOutputVTG.speed1.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputVTG.speed1.decimal = ((uint8_t)GPS_str_to_float(buffer[i+4], buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else{
						GPSOutputVTG.speed1.unit = buffer[i] - 48;
						GPSOutputVTG.speed1.decimal = (GPS_str_to_float(buffer[i+2], buffer[i+3])*100);
						i = i+4;	//salto l'ultima cifra decimale
					}
					break;
				
				case 5:		//UNIT
					GPSOutputVTG.speed1_unit = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND(KM/H)
					if(buffer[i+1] != '.' && buffer[i+2] == '.'){  						//>10 km/h && <100km/h
						GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputVTG.speed2.decimal = ((uint8_t)GPS_str_to_float(buffer[i+4], buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else if(buffer[i + 2] != '.'){															// > 100 km/h
					GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]));
					GPSOutputVTG.speed2.decimal = ((uint8_t)GPS_str_to_float(buffer[i+4], buffer[i+5]));
					i = i +6; //salto l'ultima cifra decimale
					}
					else{			// < 10km/h
						GPSOutputVTG.speed2.unit = buffer[i] - 48;
						GPSOutputVTG.speed2.decimal = (GPS_str_to_float(buffer[i+2],buffer[i+3])*100);
						i = i+4;	//salto l'ultima cifra decimale
					}
					break;
				
				case 7:		//UNIT
					GPSOutputVTG.speed2_unit = buffer[i];
					break;
				
				case 8: 	//MODE INDICATOR
					GPSOutputVTG.VTG_mode = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	//forzo la teminazione della conversione
			}
		}
		
		i++;
	}
	return GPSOutputVTG;
}



uint16_t GPS_str_to_int(uint8_t centinaia, uint8_t decine, uint8_t unita){
	
	if (centinaia != 0){
		centinaia = centinaia -48;		/*converto da carattere a numero*/
		decine  = decine -48;
		unita = unita - 48;
		return centinaia*100 + decine*10 + unita;
	}
	else if (centinaia == 0){		//non si presenta mai il caso decine = 0 per come uso io questa funzione
		decine  = decine -48;
		unita = unita - 48;
		return decine*10 + unita;
	}
}

double GPS_str_to_float(uint8_t decimi, uint8_t  centesimi){
	
	return ((double)((decimi -48)*10 + (centesimi -48)))/100;
	
}


void GPS_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second){  /*da testare*/
	if (GPS_is_fix_valid() == FIX_VALID){									//se i dati che ho a disposizione non hanno problemi, riempo le variabili passate con dati coerenti
		*hour = NMEA_output.NMEA_GGA_type.UTC_time.hours;
		*minut = NMEA_output.NMEA_GGA_type.UTC_time.minutes;
		*second = NMEA_output.NMEA_GGA_type.UTC_time.seconds;
	}
	else{																		//altrimenti gli riempo con dati non coerenti
		*hour = 255;
		*minut = 255;
		*second = 255;
	}
}

uint8_t GPS_is_fix_valid(void){		//ritorna FIX_VALID  se i dati presenti nella variabile globale sono efettivamente coerenti, altrimenti ritorna FIX_NOT_VALID
	if (NMEA_output.NMEA_GGA_type.GGA_fix_valid == 1 || NMEA_output.NMEA_GGA_type.GGA_fix_valid == 2){
		if(NMEA_output.NMEA_RMC_type.RMC_status == 'A')
			return FIX_VALID;
	}
	return FIX_NOT_VALID;		/*da testare*/
}


void GPS_get_date(uint8_t *day, uint8_t *month, uint8_t *year){		//se i dati sono validi, restituisco effettivamente i dati giusti
	if (GPS_is_fix_valid() == FIX_VALID){
		*day = NMEA_output.NMEA_RMC_type.UTC_date.day;
		*month = NMEA_output.NMEA_RMC_type.UTC_date.month;
		*year = NMEA_output.NMEA_RMC_type.UTC_date.year;
	}
	else{														//altrimenti gli riempo con dati non coerenti
		*day = 255;
		*month = 255;
		*year = 255;		/*da testare*/
	}
}


void GPS_init(void){		/*questi tipi di messaggio sono sbagliati*/
	HAL_UART_Transmit(&huart2,(uint8_t *)NMEA_SET_GGA_RMC_VTG, NMEA_SET_GGA_RMC_VTG_LENGTH, 100);		//mandami solo RMC, GGA, VTG
	HAL_UART_Transmit(&huart2, (uint8_t *)NMEA_SET_BAUDRATE_57600, NMEA_SET_BAUDRATE_57600_LENGTH, 100); //setta la comunicazione del gps a 57600
	GPS_USART2_UART_Init();																																				//setta la uart2 a 57600
	HAL_UART_Transmit(&huart2, (uint8_t *)NMEA_SET_10_HZ_NAVIGATION , NMEA_SET_10_HZ_NAVIGATION_LENGTH, 100); //aumenta il fix rate a 10Hz
}

void GPS_clear_buffer(uint8_t *buffer, uint8_t length){

	uint8_t i = 0;
	while((buffer[i] != '\r') && (buffer[i] != '\n') && (i < GPS_MAX_LENGTH)){
		buffer[i] = 0;
		i++;
	}

}

void GPS_USART2_UART_Init(void){			//reinizializza la uart2 a 57600
	
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


