#ifndef GPS_H
#define GPS_H

#include "stdint.h"


// NMEA command defines

#define GPS_SET_FIX_RATE_10HZ 				"0xB5, 0x62,0x06, 0x08,0x06, 0x00,0x64, 0x00, 0x01, 0x00,0x01, 0x00,0x7A, 0x12"
#define GPS_SET_FIX_RATE_10HZ_LENGTH	(uint8_t)14

#define GPS_POLL_FIX_RATE							"0xB5,0x62,0x06,0x08,0x00,0x00,0x0E,0x30"
#define GPS_POLL_FIX_RATE_LENGTH			(uint8_t)8

#define STOP_GLL  										"$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n"  
#define STOP_GLL_LENGTH								(uint8_t) 29

/*scrivere start e stop di tutti gli altri messagi*/

#define START_GLL											"$PUBX,40,GLL,0,1,0,0,0,0*5D\r\n" 
#define START_GLL_LENGTH							(uint8_t) 29

#define SET_BAUDRATE_57600						"$PUBX,41,1,0007,0003,57600,0*2B\r\n" /*blocca il programma*/
#define SET_BAUDRATE_57600_LENGTH			(uint8_t) 33

#define SET_BAUDRATE_38400						"$PUBX,41,1,0007,0003,38400,0*20\r\n" 
#define SET_BAUDRATE_38400_LENGTH			(uint8_t) 33
												


// Message type defines.
#define MESSAGE_TYPE_GGA								((uint8_t)1)
#define MESSAGE_TYPE_GLL								((uint8_t)2)
#define MESSAGE_TYPE_GSA 								((uint8_t)3)
#define MESSAGE_TYPE_GSV 								((uint8_t)4)
#define MESSAGE_TYPE_RMC 								((uint8_t)5)
#define MESSAGE_TYPE_VTG								((uint8_t)6)
#define MESSAGE_TYPE_UNKNOWN						((uint8_t) 255)

#define GGA_MAX_LENGTH 									((uint8_t)38) //che poi in realt� mi interesa anche la lunghezza minima
#define GLL_MAX_LENGTH									((uint8_t)24)
#define GSA_MAX_LENGTH									((uint8_t)39)
#define GSV_MAX_LENGTH  								((uint8_t)36)
#define RMC_MAX_LENGTH									
#define VTG_MAX_LENGTH

#define GPS_MIN_LENGTH									((uint8_t)13)	//il messaggio piu breve che possa ricevere � un GLL senza dati con sole ,
																											//questo numero � molto importante per la velocit� del programma, maggiore questa dimensione minore il polling per cercare la fine del messaggio
#define GPS_MAX_LENGTH									((uint8_t)83) //ricontrollare questo valore

// latitude_direction values.
#define DIRECTION_NORTH 									((char)'N')
#define DIRECTION_SOUTH 									((char)'S')

// longitude_direction values.
#define DIRECTION_EAST 									((char)'E')
#define DIRECTION_WEST 									((char)'W')

#define FIX_VALID												((uint8_t)0)
#define FIX_NOT_VALID										((uint8_t)1)


typedef struct UTC_time
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t decimal_seconds;
} UTC_time_t;

typedef struct UTC_date
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
} UTC_date_t;

typedef struct latitude
{
	uint8_t degrees;
	uint8_t minutes;
	uint16_t decimal_minutes;
} latitude_t;

typedef struct longitude
{
	uint16_t degrees;
	uint8_t minutes;
	uint16_t decimal_minutes;
} longitude_t;

typedef struct Course_over_ground
{
	uint16_t gradi;
	uint8_t decimi;
	
} Course_over_ground_t;

typedef struct speed
{
	uint8_t unit;
	uint8_t decimal;
	
} speed_t;


/*$GPGGA,065500.00,2447.65027,N,12100.78318,E,2,12,0.91,69.8,M,16.3,M,,*65*/
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
	
} NMEA_GGA_type_t;


/*$GPGLL,2447.65027,N,12100.78318,E,065500.00,A,D*6E*/
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
	
} NMEA_GLL_type_t;

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
	
} NMEA_GSA_type_t;


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
} NMEA_RMC_type_t;

typedef struct NMEA_GSV_type
{
	char message_number;
	uint8_t number_of_satellites_in_view;
	uint8_t satellite_ID;
	uint8_t satellite_elevation_degrees;
	uint16_t satellite_azimuth;
	uint8_t SNR;
	
} NMEA_GSV_type_t;

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
} NMEA_VTG_type_t;


typedef struct NMEA_output_types
{
	NMEA_GGA_type_t NMEA_GGA_type;
	NMEA_GLL_type_t NMEA_GLL_type;
	NMEA_VTG_type_t NMEA_VTG_type;
	NMEA_RMC_type_t NMEA_RMC_type;
	NMEA_GSV_type_t NMEA_GSV_type;
	NMEA_GSA_type_t NMEA_GSA_type;
} NMEA_output_types_t;



//user functions

void GPS_Rx_Cplt(void);
void GPS_parse_data(uint8_t * buffer);
uint8_t GPS_get_messageID(uint8_t * buffer);
void GPS_init(void);
void GPS_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second);
void GPS_get_date(uint8_t *day, uint8_t *month, uint8_t *year);
uint8_t GPS_is_fix_valid(void);

NMEA_GGA_type_t GPS_GGA_conversion(uint8_t * buffer);
NMEA_GLL_type_t GPS_GLL_conversion(uint8_t * buffer);
NMEA_GSA_type_t GPS_GSA_conversion(uint8_t * buffer); 
NMEA_GSV_type_t GPS_GSV_conversion(uint8_t * buffer);
NMEA_RMC_type_t GPS_RMC_conversion(uint8_t * buffer);
NMEA_VTG_type_t GPS_VTG_conversion(uint8_t * buffer);

uint16_t GPS_str_to_int(uint8_t centinaia, uint8_t decine, uint8_t unita);
double    GPS_str_to_float(uint8_t decimi, uint8_t centesimi);
void GPS_clear_buffer(uint8_t *buffer, uint8_t length);
void GPS_USART2_UART_Init_38400(void);
void GPS_USART2_UART_Init_57600(void);


#endif
