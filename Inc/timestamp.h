#ifndef __timestamp_H
#define __timestamp_H

#include <stdint.h>

#define GPS_TIMESTAMP_READY (uint8_t)1

typedef struct {
	uint8_t hoursFromRtc;
	uint8_t minutesFromRtc;
	uint8_t secondsFromRtc;
	uint8_t subsecondsFromRtc;
	uint8_t weekdayFromRtc;
	uint8_t monthFromRtc;
	uint8_t dateNumberFromRtc;
	uint8_t yearFromRtc;
	uint8_t hoursFromGps;
	uint8_t minutesFromGps;
	uint8_t secondsFromGps;
	uint8_t monthFromGps;
	uint8_t dateNumberFromGps;
	uint8_t yearFromGps;
	uint8_t GPS_TimestampReady;
} actualTimestamp_t;

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} time_t;

typedef struct {
	uint8_t month;
	uint8_t date;
	uint8_t year;
} date_t;

extern inline void timestampUpdate(void);
extern void resetActualTimestamp(void);
extern inline void setTimestampTimeFormRtc(void);
extern inline void setTimestampDateFormRtc(void);
extern inline void setTimestampTimeFormGps(void);
extern inline void setTimestampDateFormGps(void);
extern inline void getTimestampTimeFormRtc(time_t *time);
extern inline void getTimestampDateFormRtc(date_t *date);
extern inline void getTimestampTimeFormGps(time_t *time);
extern inline void getTimestampDateFormGps(date_t *date);
extern inline uint8_t getTimestampRtcHours(void);
extern inline uint8_t getTimestampRtcMinutes(void);
extern inline uint8_t getTimestampRtcSeconds(void);
extern inline uint32_t getTimestampRtcSubseconds(void);
extern inline uint8_t getTimestampRtcWeekday(void);
extern inline uint8_t getTimestampRtcMonth(void);
extern inline uint8_t getTimestampRtcDateNumber(void);
extern inline uint8_t getTimestampRtcYear(void);
extern inline uint8_t getTimestampGpsHours(void);
extern inline uint8_t getTimestampGpsMinutes(void);
extern inline uint8_t getTimestampGpsSeconds(void);
extern inline uint8_t getTimestampGpsMonth(void);
extern inline uint8_t getTimestampGpsDateNumber(void);
extern inline uint8_t getTimestampGpsYear(void);
extern inline uint32_t getDataTimestamp(void);
extern inline void incrementDataTimestamp(void);
extern inline void resetDataTimestamp(void);

#endif
