#include <stdint.h>
#include "data_conversion.h"


extern inline uint16_t mainCurrentSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_MAIN_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t dcuCurrentSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_DCU_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t xbeeCurrentSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_XBEE_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline float _3v3SenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _3V3_MCU_GAIN;
	return data;
}

extern inline float _5vSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _5V_DCU_GAIN;
	return data;
}

extern inline float _12vSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _12V_POST_DIODE_SENSE_HIGH_GAIN;
	return data;
}

extern inline float dcuTempSenseConversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB * 1000.0f;
	data = data - TEMP_SENSE_VOLTAGE_OFFSET;
	data = data * TEMP_SENSE_TEMPERATURE_GAIN;
	data = data - TEMP_SENSE_TEMPERATURE_OFFSET;
	return data;
}

extern inline float vbatConversion(uint32_t rawData) {
	return ((float)rawData * ADC_LSB);
}

extern inline float analogAux1Conversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / ANALOG_AUX_1_GAIN;
	return data;
}

extern inline float analogAux2Conversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / ANALOG_AUX_2_GAIN;
	return data;
}

extern inline float analogAux3Conversion(uint32_t rawData) {
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / ANALOG_AUX_2_GAIN;
	return data;
}
