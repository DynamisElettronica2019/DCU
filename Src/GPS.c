#include "GPS.h"
#include "usart.h"
#include <stdlib.h>

//user variables

uint8_t GPSFirstChar = 0;
uint8_t GPSMessaggeBegin = 0;
uint8_t GPSRawBuffer[GPS_MAX_LENGTH];
uint8_t GPSComplitingPacket = 0;
uint8_t GPSTerminationChar = 0;
uint8_t GPSSingleCharCplt = 0;
uint8_t GPSMessageID;
uint8_t GPSMessaggeLength = 0;		//è il numero di tutti i caratteri del messagio, escluso $ e inclusi gli identificatori del messaggio
uint8_t GPSIndex = 0;

NMEA_output_types_t NMEA_output;	//dove salvo i risultati della conversione

/*variabili che mi servono per controllare se avviene qualche errore nella conversione*/
uint8_t GGAConversionError = FALSE;
uint8_t GLLConversionError = FALSE;
uint8_t GSAConversionError = FALSE;
uint8_t GSVConversionError = FALSE;
uint8_t VTGConversionError = FALSE;
uint8_t RMCConversionError = FALSE;



void GPS_parse_data(uint8_t * buffer){
	
	GPSMessaggeLength = GPS_MIN_LENGTH;					//se ho chiamato la funzione è perchè ho letto GPS_MIN_LENGTH caratteri, escluso l'iniziale $
	
	if((buffer[GPS_MIN_LENGTH - 1] != '\r') || (buffer[GPS_MIN_LENGTH - 1] != '\n'))	//se non ho ancora letto tutto il messagio
		GPS_complete_packet(buffer);							//questa funzione si occupa di completare il buffer fino al carattere terminatore
	
	GPSMessageID = GPS_get_messageID(buffer);		//capisco che tipo di messaggio ho ricevuto
	
	switch (GPSMessageID){											// in base al messaggio che mi è arrivato faccio la conversione del pacchetto
		
		case MESSAGE_TYPE_GGA :
			NMEA_output.NMEA_GGA_type = GPS_GGA_conversion(buffer); 
			break;
		
		case MESSAGE_TYPE_GLL :
			NMEA_output.NMEA_GLL_type = GPS_GLL_conversion(buffer); 
			break;
		
		case MESSAGE_TYPE_GSA :
			NMEA_output.NMEA_GSA_type = GPS_GSA_conversion(buffer); 
			break;
		
		case MESSAGE_TYPE_GSV :
			NMEA_output.NMEA_GSV_type = GPS_GSV_conversion(buffer); 
			break;
		
		case MESSAGE_TYPE_RMC :
			NMEA_output.NMEA_RMC_type = GPS_RMC_conversion(buffer); 
			break;
		
		case MESSAGE_TYPE_VTG :
			NMEA_output.NMEA_VTG_type = GPS_VTG_conversion(buffer); 
			break; 
		
		case MESSAGE_TYPE_UNKNOWN :
			/**/
			break;
		
		default :						//non dovrei mai arrivare qui in ogni caso
			break;
	}
}


void GPS_complete_packet(uint8_t * buffer){
	
	GPSComplitingPacket = 1;								//avviso che adesso sto finendo di completare il pacchetto 
	GPSIndex = GPS_MIN_LENGTH ;					//Serve per indicizzare il buffer che adesso vado a riempire. ovviamente i primi GPS_MIN_LENGTH - 1 posti sono gia riempiti, quindi parto da GPS_MIN_LENGTH
	HAL_UART_Receive_DMA(&huart2, &GPSTerminationChar, 1);	//comincio a leggere carattere per carattere
	
	while(GPSTerminationChar != '\r'){			//finche non è arrivato l'ultimo carattere del messaggio
	
		if(GPSSingleCharCplt == 1){						// se ha finito la funzione di callback del singolo carattere
			
			GPSSingleCharCplt = 0;
			GPSMessaggeLength ++;
			buffer[GPSIndex] = GPSTerminationChar;										//concateno l'ultimo carattere letto al buffer di partenza
			GPSIndex++;																								//incremento l'indice
			HAL_UART_Receive_DMA(&huart2, &GPSTerminationChar, 1 );		//e continuo a leggere carattere per carattere
		}
	}
	buffer[GPSIndex] = '\r';																			//completo il buffer con l'ultimo elemento
	GPSComplitingPacket = 0;																			//finito di completare il pacchetto, segnalo che ho finito e ritorno
	return;
	
}

uint8_t GPS_get_messageID(uint8_t * buffer){										//questa funzione funziona solo se in buffer i caratteri identificatori del messaggio sono contenuti nei primo 5 elementi

	if (buffer[3] == 'G')
		return MESSAGE_TYPE_GGA;
	else if(buffer[3] == 'L')
		return MESSAGE_TYPE_GLL;
	else if (buffer[3] == 'S')
		return MESSAGE_TYPE_GSA;
	else if(buffer[3]== 'S' && buffer[4] == 'V')									//se non ci fosse da considerare il GSV si potrebbe fare uno switch piu performante
		return MESSAGE_TYPE_GSV;
	else if(buffer[2] == 'R')
		return MESSAGE_TYPE_RMC;
	else if(buffer[2] == 'V')
		return MESSAGE_TYPE_VTG;
	else
		return MESSAGE_TYPE_UNKNOWN;

}

NMEA_GGA_type_t GPS_GGA_conversion(uint8_t * buffer){
		
	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato
	
	NMEA_GGA_type_t GPSOutputGGA;
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
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
					GPSOutputGGA.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+5], &buffer[i+6])*60));
					i = i +6;
					break;
				
				case 2:		//N_S
					GPSOutputGGA.N_S = buffer[i];
					break;
				
				case 3:		//LONGITUDE
					GPSOutputGGA.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGGA.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGGA.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+6], &buffer[i+7])*60));
					i = i+7;
					break;
				
				case 4: 	//E_W
					GPSOutputGGA.E_W = buffer[i];
					break;
				
				case 5:		//POSITION FIX INDICATOR
					GPSOutputGGA.GGA_fix_valid = buffer[i];
					break;
				
				case 6:		//SATELITTES USED
					if (buffer[i+1] != ','){											//se ho piu di 9 satelliti in vista
						GPSOutputGGA.number_satellites_used = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1])); 
						i++;
					}
					else
						GPSOutputGGA.number_satellites_used = buffer[i];
					break;
				
				case 7:		//HDOP
					GPSOutputGGA.HDOP = GPS_str_to_float(&buffer[i+2],&buffer[i+3]);		//salto lo zero e il punto
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
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
												Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
												cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			GGAConversionError = TRUE;		//segnalo che è avvenuto un errore di qualche tipo
		}
	}
	return GPSOutputGGA;
}
NMEA_GLL_type_t GPS_GLL_conversion(uint8_t * buffer){
	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato
	
	NMEA_GLL_type_t GPSOutputGLL;
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
	  else{
			switch(skipped){
				
				case 0:		//LATITUDE
					GPSOutputGLL.latitude.degrees = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
					GPSOutputGLL.latitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+2], buffer[i+3]));
					GPSOutputGLL.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+5], &buffer[i+6])*60));
					i = i +6;
					break;
				
				case 1:		//N_S
					GPSOutputGLL.N_S = buffer[i];
					break;
				
				case 2:		//LONGITUDE
					GPSOutputGLL.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputGLL.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputGLL.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+6], &buffer[i+7])*60));
					i = i+7;
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
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
														Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
														cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			GLLConversionError = TRUE;		//segnalo che è avvenuto un errore
		}
	}
	return GPSOutputGLL;
}
NMEA_GSA_type_t GPS_GSA_conversion(uint8_t * buffer){
	
	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato

	NMEA_GSA_type_t GPSOutputGSA;
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//MODE 1
					GPSOutputGSA.GSA_mode = buffer[i];
					break;
				
				case 1:		//MODE 2
					GPSOutputGSA.fix_mode = buffer[i];
					break;
				
				case 2:		//SATELLITE ON CHANNEL 1
					if (buffer[i+2] == ','){		//se ho piu di 9 satelliti sul primo canale
						GPSOutputGSA.satellites_used_ch1 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch1 = buffer[i];
					break;
				
				case 3:		//SATELLITE ON CHANNEL 2
					if (buffer[i+2] == ','){		//se ho piu di 9 satelliti sul secondo canale
						GPSOutputGSA.satellites_used_ch2 = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else
						GPSOutputGSA.satellites_used_ch2 = buffer[i];
					break;
				
				default: 
					buffer[i] = '*';	//se si vogliono leggere anche altre parti del messaggio basta continuare a implementare questo switch case. per velocizzare il tempo di spacchettamento tengo solo i dati necessari per adesso
			}
		}
		
		i++;
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
														Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
														cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			GSAConversionError = TRUE;			//segnalo che è avcenuto un errore
		}
	}
	return GPSOutputGSA;

}

NMEA_GSV_type_t GPS_GSV_conversion(uint8_t * buffer){ //in teoria potrei fornire i dati di tutti e 4 i gps, per semplicità in prima scrittura tengo solo un messaggio, che sarà l'ultimo di quelli arrivati. inoltre vado a leggere i dati solo del primo canale

	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;
	NMEA_GSV_type_t GPSOutputGSV;
	uint8_t NumberOfMessage = buffer[i];
	i++;
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//MESSAGE NUMBER
					GPSOutputGSV.message_number = buffer[i];
					break;
				
				case 1:		//SATELLITES IN VIEW
					if(buffer[i+1]!= ','){					//se ho piu di 10 satelliti in vista
						GPSOutputGSV.number_of_satellites_in_view = ((uint8_t)GPS_str_to_int(0, buffer[i], buffer[i+1]));
						i++;
					}
					else 
						GPSOutputGSV.number_of_satellites_in_view = buffer[i];
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
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
														Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
														cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			GSVConversionError = TRUE;
		}
 }
	return GPSOutputGSV;
}
	


NMEA_RMC_type_t GPS_RMC_conversion(uint8_t * buffer){		
	
	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;
	NMEA_RMC_type_t GPSOutputRMC;
	
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
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
					GPSOutputRMC.latitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+5], &buffer[i+6])*60));
					i = i +6;
					break;
				
				case 3:		//N_S
					GPSOutputRMC.N_S = buffer[i];
					break;
				
				case 4: 	//LONGITUDE
					GPSOutputRMC.longitude.degrees = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputRMC.longitude.minutes = ((uint8_t)GPS_str_to_int(0, buffer[i+3], buffer[i+4]));
					GPSOutputRMC.longitude.decimal_minutes = ((uint16_t)(GPS_str_to_float(&buffer[i+6], &buffer[i+7])*60));
					i = i+7;
					break;
				
				case 5:		//E_W
					GPSOutputRMC.E_W = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND (KNOTS)
					if(buffer[i+1] != '.' ){  						//>10 nodi
						GPSOutputRMC.speed.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputRMC.speed.decimal = ((uint8_t)GPS_str_to_float(&buffer[i+4], &buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else{
						GPSOutputRMC.speed.unit = buffer[i];
						GPSOutputRMC.speed.decimal = (GPS_str_to_float(&buffer[i+2], &buffer[i+3])*100);
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
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
														Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
														cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			RMCConversionError = TRUE;
		}
 }
	return GPSOutputRMC;
 }
	

NMEA_VTG_type_t GPS_VTG_conversion(uint8_t * buffer){
	
	uint8_t i = 6;													//il primo carattere "utile" dopo l'identificatore di messaggio e la ,
	uint8_t skipped = 0;										//serve per tenere conto di quante volte ho saltato

	NMEA_VTG_type_t GPSOutputVTG;
	while(buffer[i] != '*'){								//fintanto che non arrivo al checksum
		
		if (buffer[i] == ','){
			skipped++;
		}
		else{
			switch(skipped){
				
				case 0:		//COURSE OVER GROUND
					GPSOutputVTG.Course_over_ground.gradi = GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]);
					GPSOutputVTG.Course_over_ground.decimi = ((uint8_t)(GPS_str_to_float(&buffer[i+4],&buffer[i+5]))*60);
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
						GPSOutputVTG.speed1.decimal = ((uint8_t)GPS_str_to_float(&buffer[i+4], &buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else{
						GPSOutputVTG.speed1.unit = buffer[i];
						GPSOutputVTG.speed1.decimal = (GPS_str_to_float(&buffer[i+2], &buffer[i+3])*100);
						i = i+4;	//salto l'ultima cifra decimale
					}
					break;
				
				case 5:		//UNIT
					GPSOutputVTG.speed1_unit = buffer[i];
					break;
				
				case 6:		//SPEED OVER GROUND(KM/H)
					if(buffer[i+1] != '.' && buffer[i+2] == '.'){  						//>10 km/h && <100km/h
						GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(0, buffer[i+ 1], buffer[i+2]));
						GPSOutputVTG.speed2.decimal = ((uint8_t)GPS_str_to_float(&buffer[i+4], &buffer[i+5])*100);
						i = i+ 6; //salto l'ultima cifra decimale
					}
					else if(buffer[i + 2] != '.'){															// > 100 km/h
					GPSOutputVTG.speed2.unit = ((uint8_t)GPS_str_to_int(buffer[i], buffer[i+1], buffer[i+2]));
					GPSOutputVTG.speed2.decimal = ((uint8_t)GPS_str_to_float(&buffer[i+4], &buffer[i+5]));
					i = i +6; //salto l'ultima cifra decimale
					}
					else{			// < 10km/h
						GPSOutputVTG.speed2.unit = buffer[i];
						GPSOutputVTG.speed2.decimal = (GPS_str_to_float(&buffer[i+2],&buffer[i+3])*100);
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
		if (buffer[i] != ','){	/*in teoria qui il buffer dovrebbe trovarsi nella posizione di una virgola, se ho fatto bene i conti in tutte le altre parti del programma. 
														Dato che questa cosa è cruciale, poichè altrimenti rientrerei nello switch e rischierei di rovinare i dati salvati precedentemente, mi riporto a pari nella posizione della virgola
														cosi facendo perdo i dati del campo in cui ho sbagliato a fare i conti, ma almeno non corrompo alcun dato. segnalo questa cosa con un flag di errore*/
			while(buffer[i] != ','){
				i++;
			}
			VTGConversionError = TRUE;
		}
		
	}
	return GPSOutputVTG;
}



uint16_t GPS_str_to_int(uint8_t centinaia, uint8_t decine, uint8_t unita){
	
	return centinaia*100 + decine*10 + unita;
}


double GPS_str_to_float(uint8_t *decimi, uint8_t * centesimi){
	
	return atof((const char *)decimi)/10 + atof((const char *)centesimi)/100;
	
}
