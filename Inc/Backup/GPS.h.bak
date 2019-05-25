#ifndef GPS_H
#define GPS_H

#include <stdint.h>

/* Messsages to change GPS setting */
#define STOP_GLL  											"$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n"  
#define STOP_GGA												"$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n" 
#define START_GGA												"$PUBX,40,GGA,0,1,0,0,0,0*5B\r\n"
#define STOP_GLL  											"$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n" 
#define START_GLL												"$PUBX,40,GLL,0,1,0,0,0,0*5D\r\n" 
#define STOP_GSA												"$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n"
#define START_GSA												"$PUBX,40,GSA,0,1,0,0,0,0*4F\r\n"
#define STOP_GSV												"$PUBX,40,GSV,0,0,0,0,0,0*59\r\n"
#define START_GSV												"$PUBX,40,GSV,0,1,0,0,0,0*58\r\n"
#define SET_BAUDRATE_38400							"$PUBX,41,1,0007,0003,38400,0*20\r\n"

#define STOP_GLL_LENGTH									(uint8_t)29
#define STOP_GGA_LENGTH									(uint8_t)29
#define START_GGA_LENGTH								(uint8_t)29
#define STOP_GLL_LENGTH									(uint8_t)29
#define START_GLL_LENGTH								(uint8_t)29
#define STOP_GSA_LENGTH									(uint8_t)29
#define START_GSA_LENGTH								(uint8_t)29
#define STOP_GSV_LENGTH									(uint8_t)29
#define START_GSV_LENGTH								(uint8_t)29
#define SET_BAUDRATE_57600_LENGTH				(uint8_t)33
#define SET_BAUDRATE_38400_LENGTH				(uint8_t)33
#define GPS_SET_FIX_RATE_10HZ_LENGTH 		(uint8_t)14
#define GPS_MAX_LENGTH									((uint8_t)83)

/* Message type defines */
#define MESSAGE_TYPE_GGA								(uint8_t)1
#define MESSAGE_TYPE_GLL								(uint8_t)2
#define MESSAGE_TYPE_GSA 								(uint8_t)3
#define MESSAGE_TYPE_GSV 								(uint8_t)4
#define MESSAGE_TYPE_RMC 								(uint8_t)5
#define MESSAGE_TYPE_VTG								(uint8_t)6
#define MESSAGE_TYPE_UNKNOWN						(uint8_t)255


typedef struct UTC_time {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t decimal_seconds;
} UTC_time_t;

typedef struct UTC_dateb{
	uint8_t day;
	uint8_t month;
	uint8_t year;
} UTC_date_t;

typedef struct latitude {
	uint8_t degrees;
	uint8_t minutes;
	double decimal_minutes;
} latitude_t;

typedef struct longitude {
	uint16_t degrees;
	uint8_t minutes;
	double decimal_minutes;
} longitude_t;

typedef struct Course_over_ground {
	uint16_t gradi;
	uint8_t decimi;
} Course_over_ground_t;

typedef struct speed {
	uint8_t unit;
	double decimal;
} speed_t;

typedef struct NMEA_GGA_type {
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
	uint8_t geoidal_separation;
	char Units;
	uint8_t age_of_diff_corr;
	uint8_t checksum;
} NMEA_GGA_type_t;

typedef struct NMEA_GLL_type {
	latitude_t latitude;
	char N_S;
	longitude_t longitude;
	char E_W;
	UTC_time_t UTC_time;
	char status;
	char mode_indicator;
	uint8_t checksum;
} NMEA_GLL_type_t;

typedef struct NMEA_GSA_type {
	char GSA_mode;
	uint8_t fix_mode;
	uint8_t satellites_used_ch1;
	uint8_t satellites_used_ch2;
	uint8_t PDOP;
	uint8_t HDOP;
	uint8_t VDOP;
	uint8_t checksum;
} NMEA_GSA_type_t;

typedef struct NMEA_RMC_type {
	UTC_time_t UTC_time;
	char RMC_status;
	latitude_t latitude;
	char N_S;
	longitude_t longitude;
	char E_W;
	speed_t speed;
	uint8_t heading;
	UTC_date_t UTC_date;
	char RMC_mode;
	uint8_t checksum;
} NMEA_RMC_type_t;

typedef struct NMEA_VTG_type {
	Course_over_ground_t Course_over_ground;
	char reference;
	speed_t speed1;
	char speed1_unit;
	speed_t speed2;
	char speed2_unit;
	char VTG_mode;
	uint8_t checksum;
} NMEA_VTG_type_t;

typedef struct NMEA_output_types {
	NMEA_GGA_type_t NMEA_GGA_type;
	NMEA_GLL_type_t NMEA_GLL_type;
	NMEA_VTG_type_t NMEA_VTG_type;
	NMEA_RMC_type_t NMEA_RMC_type;
	NMEA_GSA_type_t NMEA_GSA_type;
} NMEA_output_types_t;


extern void GPS_Init(void);
extern inline void GPS_DataConversion(uint8_t * GPSRawBuffer);
static inline uint8_t GPS_get_messageID(uint8_t * buffer);
static inline void GPS_GGA_conversion(uint8_t * buffer);
static inline void  GPS_GLL_conversion(uint8_t * buffer);
static inline void  GPS_GSA_conversion(uint8_t * buffer); 
static inline void  GPS_RMC_conversion(uint8_t * buffer);
static inline void  GPS_VTG_conversion(uint8_t * buffer);
static inline float GPS_minuts_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi, uint8_t decimillesimi, uint8_t centimillesimi);
static inline float GPS_speed_decimal_conversion(uint8_t decimi, uint8_t  centesimi, uint8_t millesimi);
static inline uint16_t GPS_StrToInt(uint8_t centinaia, uint8_t decine, uint8_t unita);
static inline float GPS_str_to_float(uint8_t decimi, uint8_t  centesimi);

#endif
