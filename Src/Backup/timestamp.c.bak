#include "timestamp.h"

static actualTimestamp_t actualTimestamp;


extern void resetActualTimestamp(void)
{
	actualTimestamp.hoursFromRtc = 0;
	actualTimestamp.minutesFromRtc = 0;
	actualTimestamp.secondsFromRtc = 0;
	actualTimestamp.subsecondsFromRtc = 0;
	actualTimestamp.weekdayFromRtc = 0;
	actualTimestamp.monthFromRtc = 0;
	actualTimestamp.dateNumberFromRtc = 0;
	actualTimestamp.yearFromRtc = 0;
	actualTimestamp.hoursFromGps = 0;
	actualTimestamp.minutesFromGps = 0;
	actualTimestamp.secondsFromGps = 0;
	actualTimestamp.monthFromGps = 0;
	actualTimestamp.dateNumberFromGps = 0;
	actualTimestamp.yearFromGps = 0;
	return;
}

extern inline void setTimestampTimeFormRtc(void)
{
	RTC_TimeTypeDef time;
	uint32_t subseconds;
	
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN); 																									/* Read time data from RTC */
	subseconds = ((time.SecondFraction - time.SubSeconds) * 1000) / (time.SecondFraction + 1); 			/* From STM example codes */
	actualTimestamp.hoursFromRtc = time.Hours; 																											/* Update the hours of the actual timestamp struct */
	actualTimestamp.minutesFromRtc = time.Minutes; 																									/* Update the minutes of the actual timestamp struct */
	actualTimestamp.secondsFromRtc = time.Seconds; 																									/* Update the seconds of the actual timestamp struct */
	actualTimestamp.subsecondsFromRtc = subseconds; 																								/* Update the ms value of the actual timestamp struct */
	return;
}

extern inline void setTimestampDateFormRtc(void)
{
	RTC_DateTypeDef date;
	
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN); 																									/* Read date data from RTC */
	actualTimestamp.weekdayFromRtc = date.WeekDay;																									/* Update the weekday of the actual timestamp struct */
	actualTimestamp.monthFromRtc = date.Month;																											/* Update the month of the actual timestamp struct */
	actualTimestamp.dateNumberFromRtc = date.Date;																									/* Update the date number of the actual timestamp struct */
	actualTimestamp.yearFromRtc = date.Year;																												/* Update the year of the actual timestamp struct */
	return;
}

extern inline void setTimestampTimeFormGps(void) 
{
	/* Must be implemented using the GPS library */
	return;
}

extern inline void setTimestampDateFormGps(void)
{
	/* Must be implemented using the GPS library */
	return;
}

extern inline void getTimestampTimeFormRtc(time_t *time)
{
	time->hours = actualTimestamp.hoursFromRtc;
	time->minutes = actualTimestamp.minutesFromRtc;
	time->seconds = actualTimestamp.secondsFromRtc;
	return;
}

extern inline void getTimestampDateFormRtc(date_t *date)
{
	date->date = actualTimestamp.dateNumberFromRtc;
	date->month = actualTimestamp.monthFromRtc;
	date->year = actualTimestamp.yearFromRtc;
	return;
}

extern inline void getTimestampTimeFormGps(time_t *time)
{
	time->hours = actualTimestamp.hoursFromGps;
	time->minutes = actualTimestamp.minutesFromGps;
	time->seconds = actualTimestamp.secondsFromGps;
	return;
}

extern inline void getTimestampDateFormGps(date_t *date)
{
	date->date = actualTimestamp.dateNumberFromGps;
	date->month = actualTimestamp.monthFromGps;
	date->year = actualTimestamp.yearFromGps;
	return;
}

extern inline uint8_t getTimestampRtcHours(void)
{
	return actualTimestamp.hoursFromRtc;
}

extern inline uint8_t getTimestampRtcMinutes(void)
{
	return actualTimestamp.minutesFromRtc;
}

extern inline uint8_t getTimestampRtcSeconds(void)
{
	return actualTimestamp.secondsFromRtc;
}

extern inline uint32_t getTimestampRtcSubseconds(void)
{
	return actualTimestamp.subsecondsFromRtc;
}

extern inline uint8_t getTimestampRtcWeekday(void)
{
	return actualTimestamp.weekdayFromRtc;
}

extern inline uint8_t getTimestampRtcMonth(void)
{
	return actualTimestamp.monthFromRtc;
}

extern inline uint8_t getTimestampRtcDateNumber(void)
{
	return actualTimestamp.dateNumberFromRtc;
}

extern inline uint8_t getTimestampRtcYear(void)
{
	return actualTimestamp.yearFromRtc;
}

extern inline uint8_t getTimestampGpsHours(void)
{
	return actualTimestamp.hoursFromGps;
}

extern inline uint8_t getTimestampGpsMinutes(void)
{
	return actualTimestamp.minutesFromGps;
}

extern inline uint8_t getTimestampGpsSeconds(void)
{
	return actualTimestamp.secondsFromGps;
}

extern inline uint8_t getTimestampGpsMonth(void)
{
	return actualTimestamp.monthFromGps;
}

extern inline uint8_t getTimestampGpsDateNumber(void)
{
	return actualTimestamp.dateNumberFromGps;
}

extern inline uint8_t getTimestampGpsYear(void)
{
	return actualTimestamp.yearFromGps;
}
