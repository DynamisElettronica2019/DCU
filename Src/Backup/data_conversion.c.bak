#include <stdint.h>
#include "data_conversion.h"


extern inline uint16_t mainCurrentSenseConversion(uint32_t rawData) {
	float data = (float)rawData * ADC_LSB * MAIN_CURRENT_GAIN * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t dcuCurrentSenseConversion(uint32_t rawData) {
	float data = (float)rawData * ADC_LSB * DCU_CURRENT_GAIN * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t xbeeCurrentSenseConversion(uint32_t rawData) {
	float data = (float)rawData * ADC_LSB * XBEE_CURRENT_GAIN * 1000.0f;
	return (uint16_t)data;
}

extern inline float _3v3SenseConversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * _3V3_MCU_GAIN);
}

extern inline float _5vSenseConversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * _5V_DCU_GAIN);
}

extern inline float _12vSenseConversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * _12V_POST_DIODE_SENSE_HIGH_GAIN);
}

extern inline float dcuTempSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB * 1000.0f;
	data -= TEMP_SENSE_VOLTAGE_OFFSET;
	data *= TEMP_SENSE_TEMPERATURE_GAIN;
	data -= TEMP_SENSE_TEMPERATURE_OFFSET;
	return data;
}

extern inline float vbatConversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB);
}

extern inline float analogAux1Conversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * ANALOG_AUX_1_GAIN);
}

extern inline float analogAux2Conversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * ANALOG_AUX_2_GAIN);
}

extern inline float analogAux3Conversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB * ANALOG_AUX_3_GAIN);
}
