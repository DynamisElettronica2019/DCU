#ifndef GPS_H
#define GPS_H

#include "stdint.h"


// NMEA command defines
#define NMEA_TEST 											"$PMTK000*32\r\n"								// Testing the communication between the receiver and host.
#define NMEA_CMD_HOT_START 							"$PMTK101*32\r\n"								// Hot Restart: Use all available data in the NV Store.
#define NMEA_CMD_WARM_START 						"$PMTK102*31\r\n"								// Warm Restart: Don't use Ephemeris at re-start.
#define NMEA_CMD_COLD_START							"$PMTK103*30\r\n"								// Cold Restart: Don't use Time, Position, Almanacs and Ephemeris data at re-start.
#define NMEA_CMD_FULL_COLD_START 				"$PMTK104*37\r\n"								// Full Cold Restart: Reset the receiver to the factory status.
#define NMEA_CLEAR_EPO_FILE 						"$PMTK127,0*2A\r\n"							// Clear predicted Ephemeris file (EPO) from flash memory. Reply: "$PMTK001,127,3*34".
#define NMEA_SET_BAUDRATE_DEFAULT 			"$PMTK251,0*28\r\n" 						// Set NMEA port baudrate to the defult value (9600).
#define NMEA_SET_BAUDRATE_4800 					"$PMTK251,4800*14\r\n" 					// Set NMEA port baudrate to 4800.
#define NMEA_SET_BAUDRATE_9600 					"$PMTK251,9600*17\r\n" 					// Set NMEA port baudrate to 9600.
#define NMEA_SET_BAUDRATE_14400 				"$PMTK251,14400*29\r\n" 				// Set NMEA port baudrate to 14400.
#define NMEA_SET_BAUDRATE_19200 				"$PMTK251,19200*22\r\n" 				// Set NMEA port baudrate to 19200.
#define NMEA_SET_BAUDRATE_38400 				"$PMTK251,38400*27\r\n" 				// Set NMEA port baudrate to 38400.
#define NMEA_SET_BAUDRATE_57600 				"$PMTK251,57600*2C\r\n" 				// Set NMEA port baudrate to 57600.
#define NMEA_SET_BAUDRATE_115200 				"$PMTK251,115200*1F\r\n" 				// Set NMEA port baudrate to 115200.
#define NMEA_SET_5_HZ_NAVIGATION 				"$PMTK300,200,0,0,0,0*2F\r\n"		// Command for setting the Fix Rate to 5 Hz. Use baudrate equal to 38400. Reply: "$PMTK001,300,3*33".
#define NMEA_SET_10_HZ_NAVIGATION 			"$PMTK300,100,0,0,0,0*2C\r\n" 	// Command for setting the Fix Rate to 10 Hz. Use baudrate equal to 115200. Reply: "$PMTK001,300,3*33".
#define NMEA_API_SET_DGPS_MODE_DISABLE 	"$PMTK301,0*2C\r\n" 						// Disable DGPS correction data source mode. Reply: "$PMTK001,301,3*32".
#define NMEA_API_SET_DGPS_MODE_RTCM 		"$PMTK301,1*2D\r\n" 						// RTCM DGPS correction data source mode. Reply: "$PMTK001,301,3*32".
#define NMEA_API_SET_DGPS_MODE_WAAS 		"$PMTK301,2*2E\r\n" 						// WAAS DGPS correction data source mode. Reply: "$PMTK001,301,3*32".
#define NMEA_API_SET_SBAS_DISABLE 			"$PMTK313,0*2F\r\n" 						// Disable search of SBAS satellite. Reply: "$PMTK001,313,3*31".
#define NMEA_API_SET_SBAS_ENABLED 			"$PMTK313,1*2E\r\n" 						// Enable search of SBAS satellite. Reply: "$PMTK001,313,3*31". Can be used only with 1 Hz output rate.
#define NEMA_RESTORE_OUTPUT 						"$PMTK314,-1*04\r\n" 						// Restore the output system default setting.
#define NMEA_API_SET_DATUM_WGS1984 			"$PMTK330,0*2E\r\n" 						// Set default datum as WGS1984 (region: International). Reply: "$PMTK001,330,3*30".
#define NMEA_API_SET_DATUM_EURO50 			"$PMTK330,67*1F\r\n" 						// Set default datum as European 1950 (region: Italy, Germany, Netherlands and other). Reply: "$PMTK001,330,3*30".
#define NEMA_API_SET_DATUM_S42 					"$PMTK330,180*27\r\n"						// Set default datum as European 1950 (region: Hungary). Reply: "$PMTK001,330,3*30".
#define NMEA_API_Q_FIX_CTL 							"$PMTK400*36\r\n" 							// Returns the position fix interval.
#define API_Q_DGPS_MODE 								"$PMTK401*37\r\n" 							// Returns the DGPS mode.
#define NMEA_API_Q_SBAS_ENABLED 				"$PMTK413*34\r\n" 							// Returns the SBAS status.
#define NMEA_API_Q_NMEA_OUTPUT 					"$PMTK414*33\r\n" 							// Returns the current NMEA sentence output frequencies.
#define NMEA_API_Q_DATUM 								"$PMTK430*35\r\n" 							// Returns the default datum.
#define NMEA_API_GET_USER_OPTION 				"$PMTK490*3F\r\n" 							// Returns the current user setting from the flash memory.
#define NMEA_QUERY_FIRMWARE_INFO 				"$PMTK605*31\r\n" 							// Returns the firmware release information. Reply: "$PMTK705,AXN_1.30,9156,Fastrax IT500,*6C".
#define NMEA_QUERY_EPO_STATUS 					"$PMTK607,0*2F\r\n" 						// Returns the status of EPO file loaded into flash memory.

// Message type defines.
#define MESSAGE_TYPE_GGA								((uint8_t)1)
#define MESSAGE_TYPE_GLL								((uint8_t)2)
#define MESSAGE_TYPE_GSA 								((uint8_t)3)
#define MESSAGE_TYPE_GSV 								((uint8_t)4)
#define MESSAGE_TYPE_RMC 								((uint8_t)5)
#define MESSAGE_TYPE_VTG								((uint8_t)6)
#define MESSAGE_TYPE_UNKNOWN						((uint8_t) 255)

#define GGA_MAX_LENGTH 									((uint8_t)38) //che poi in realtà mi interesa anche la lunghezza minima
#define GLL_MAX_LENGTH									((uint8_t)24)
#define GSA_MAX_LENGTH									((uint8_t)39)
#define GSV_MAX_LENGTH  								((uint8_t)36)
#define RMC_MAX_LENGTH									
#define VTG_MAX_LENGTH

#define GPS_MIN_LENGTH									((uint8_t)13)	//il messaggio piu breve che possa ricevere è un GLL senza dati con sole ,
																											//questo numero è molto importante per la velocità del programma, maggiore questa dimensione minore il polling per cercare la fine del messaggio
#define GPS_MAX_LENGTH									((uint8_t)83) //ricontrollare questo valore

// latitude_direction values.
#define DIRECTION_NORTH 									((char)'N')
#define DIRECTION_SOUTH 									((char)'S')

// longitude_direction values.
#define DIRECTION_EAST 									((char)'E')
#define DIRECTION_WEST 									((char)'W')

#define FALSE 													((uint8_t)1)
#define TRUE														((uint8_t)0)

#define FIX_VALID												((uint8_t)0)
#define FIX_NOT_VALID										((uint8_t)1)


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
	uint16_t decimal_minutes;
}latitude_t;

typedef struct longitude
{
	uint16_t degrees;
	uint8_t minutes;
	uint16_t decimal_minutes;
}longitude_t;

typedef struct Course_over_ground
{
	uint16_t gradi;
	uint8_t decimi;
	
}Course_over_ground_t;

typedef struct speed
{
	uint8_t unit;
	uint8_t decimal;
	
}speed_t;


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
	
}NMEA_GGA_type_t;


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

typedef struct NMEA_GSV_type
{
	char message_number;
	uint8_t number_of_satellites_in_view;
	uint8_t satellite_ID;
	uint8_t satellite_elevation_degrees;
	uint16_t satellite_azimuth;
	uint8_t SNR;
	
}NMEA_GSV_type_t;

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
	NMEA_GSV_type_t NMEA_GSV_type;
	NMEA_GSA_type_t NMEA_GSA_type;
}NMEA_output_types_t;



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


#endif

