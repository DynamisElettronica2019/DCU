#ifndef GPS_H
#define GPS_H

#include "stdint.h"

/*messsage used to change gps setting*/

#define STOP_GLL  							"$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n"  
#define STOP_GLL_LENGTH					(uint8_t) 29


#define STOP_GGA								"$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n" 
#define STOP_GGA_LENGTH					(uint8_t) 29

#define START_GGA								"$PUBX,40,GGA,0,1,0,0,0,0*5B\r\n"
#define START_GGA_LENGTH				(uint8_t) 29

#define STOP_GLL  							"$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n" 
#define STOP_GLL_LENGTH					(uint8_t) 29

#define START_GLL								"$PUBX,40,GLL,0,1,0,0,0,0*5D\r\n" 
#define START_GLL_LENGTH					(uint8_t) 29

#define STOP_GSA								"$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n"
#define STOP_GSA_LENGTH					(uint8_t)29

#define START_GSA								"$PUBX,40,GSA,0,1,0,0,0,0*4F\r\n"
#define START_GSA_LENGTH				(uint8_t)29

#define STOP_GSV								"$PUBX,40,GSV,0,0,0,0,0,0*59\r\n"
#define STOP_GSV_LENGTH					(uint8_t)29

#define START_GSV								"$PUBX,40,GSV,0,1,0,0,0,0*58\r\n"
#define START_GSV_LENGTH				(uint8_t)29

#define SET_BAUDRATE_57600							"$PUBX,41,1,0007,0003,57600,0*2B\r\n" /*blocca il programma*/
#define SET_BAUDRATE_57600_LENGTH				(uint8_t) 33

#define SET_BAUDRATE_38400							"$PUBX,41,1,0007,0003,38400,0*20\r\n" 
#define SET_BAUDRATE_38400_LENGTH				(uint8_t) 33

#define GPS_SET_FIX_RATE_10HZ_LENGTH 		(uint8_t)14


/*message type defines*/
#define MESSAGE_TYPE_GGA								((uint8_t)1)
#define MESSAGE_TYPE_GLL								((uint8_t)2)
#define MESSAGE_TYPE_GSA 								((uint8_t)3)
#define MESSAGE_TYPE_GSV 								((uint8_t)4)
#define MESSAGE_TYPE_RMC 								((uint8_t)5)
#define MESSAGE_TYPE_VTG								((uint8_t)6)
#define MESSAGE_TYPE_UNKNOWN						((uint8_t) 255)

/*maximum length of nmea message being outputted*/
#define GPS_MAX_LENGTH									((uint8_t)83)

#define FIX_VALID												((uint8_t)0)
#define FIX_NOT_VALID										((uint8_t)1)



/*user typedef*/
typedef struct UTC_time
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t decimal_seconds;
}UTC_time_t;

typedef struct UTC_date
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
}UTC_date_t;

typedef struct latitude
{
	uint8_t degrees;
	uint8_t minutes;
	double decimal_minutes;
}latitude_t;

typedef struct longitude
{
	uint16_t degrees;
	uint8_t minutes;
	double decimal_minutes;
}longitude_t;

typedef struct Course_over_ground
{
	uint16_t gradi;
	uint8_t decimi;
	
}Course_over_ground_t;

typedef struct speed
{
	uint8_t unit;
	double decimal;
	
}speed_t;



typedef struct NMEA_GGA_type
{
	UTC_time_t UTC_time;
	latitude_t latitude;
	char N_S;
	longitude_t longitude;
	char E_W;
	char GGA_fix_valid;
	uint8_t number_satellites_used;
	double HDOP; 									
	uint8_t MSL_Altitude;						
	char Unit;
	uint8_t geoidal_separation; 		//ricontrollare il tipo
	char Units;
	uint8_t age_of_diff_corr;				//NULL if DGPS  is not used
	uint8_t checksum;
	
}NMEA_GGA_type_t;



typedef struct NMEA_GLL_type
{
	latitude_t latitude;
	char N_S;
	longitude_t longitude;
	char E_W;
	UTC_time_t UTC_time;
	char status;
	char mode_indicator;
	uint8_t checksum;
	
}NMEA_GLL_type_t;

typedef struct NMEA_GSA_type
{
	char GSA_mode;
	uint8_t fix_mode;
	uint8_t satellites_used_ch1;
	uint8_t satellites_used_ch2;
	/*qui ci possono essere un numero di messaggi variabile*/
	uint8_t PDOP;			//controllare il tipo
	uint8_t HDOP;			//controllare il tipo
	uint8_t VDOP;			//controllare il tipo
	uint8_t checksum;
	
}NMEA_GSA_type_t;


typedef struct NMEA_RMC_type
{
	UTC_time_t UTC_time;
	char RMC_status;
	latitude_t latitude;
	char N_S;
	longitude_t longitude;
	char E_W;
	speed_t speed;                          //km/h
	uint8_t heading;
	UTC_date_t UTC_date;
	char RMC_mode;
	uint8_t checksum;
}NMEA_RMC_type_t;

typedef struct NMEA_VTG_type
{
	Course_over_ground_t Course_over_ground;
	char reference;
	speed_t speed1;
	char speed1_unit;
	speed_t speed2;
	char speed2_unit;
	char VTG_mode;
	uint8_t checksum;
}NMEA_VTG_type_t;


typedef struct NMEA_output_types
{
	NMEA_GGA_type_t NMEA_GGA_type;
	NMEA_GLL_type_t NMEA_GLL_type;
	NMEA_VTG_type_t NMEA_VTG_type;
	NMEA_RMC_type_t NMEA_RMC_type;
	NMEA_GSA_type_t NMEA_GSA_type;
}NMEA_output_types_t;




/*user functions*/

void GPS_Rx_Cplt(void);
void GPS_data_conversion(uint8_t * buffer);
uint8_t GPS_get_messageID(uint8_t * buffer);
void GPS_init(void);
void GPS_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second);
void GPS_get_date(uint8_t *day, uint8_t *month, uint8_t *year);
uint8_t GPS_is_fix_valid(void);

NMEA_GGA_type_t GPS_GGA_conversion(uint8_t * buffer);
NMEA_GLL_type_t GPS_GLL_conversion(uint8_t * buffer);
NMEA_GSA_type_t GPS_GSA_conversion(uint8_t * buffer); 
NMEA_RMC_type_t GPS_RMC_conversion(uint8_t * buffer);
NMEA_VTG_type_t GPS_VTG_conversion(uint8_t * buffer);


void GPS_USART2_UART_Init_38400(void);
void GPS_USART2_UART_Init_57600(void);

double GPS_minuts_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi, uint8_t decimillesimi, uint8_t centimillesimi);
double GPS_speed_decimal_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi);
uint16_t GPS_str_to_int(uint8_t centinaia, uint8_t decine, uint8_t unita);
double   GPS_str_to_float(uint8_t decimi, uint8_t centesimi);

#endif

