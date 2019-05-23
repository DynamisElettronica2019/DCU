#include <stdint.h>
#include "data_conversion.h"


extern inline float EFI_TEMPERATURE_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (-0.35572f);
	temp = temp + 190.95f;
  return temp;
}

extern inline float T_H20_ENGINE_DataConversion(uint16_t input)
{	
	float temp;
	
	temp = (float)input * 0.625f;
	temp = temp -10.0f;
  return temp;
}

extern inline float BATTERY_VOLTAGE_DataConversion(uint16_t input)
{
  return (0.01758f * (float)input);
}

extern inline float TPS_DataConversion(uint16_t input)
{
  return (0.39216f * (float)input);
}

extern inline float WATER_PRESSURE_DataConversion(uint16_t input)
{	
	float temp;
	
	temp = (float)input * 0.009768f;
	temp = temp - 0.996336f;
  return temp;
}

extern inline float EXHAUST_TEMPERATURE_DataConversion(uint16_t input)
{
  return (1.2219f * (float)input);
}

extern inline float H20_PUMP_DUTY_CYCLE_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input / 255.0f;
	temp = temp * 100.0f;
	return temp;
}

extern inline float FUEL_LEVEL_DataConversion(uint16_t input)
{
	float temp1;
	float temp2;

	temp1 = 0.111721f * (float)input;
	temp1 = temp1 - 18.7223f;
	temp2 = 0.007224f * (float)input;
	temp2 = temp2 + 12.29331f;
	temp1 = temp1 / temp2;
	return temp1;
}

extern inline float LOAD_CELL_DataConversion(int16_t input)
{ 
	float temp;
	
	temp = (float)input / 4095.0f;
	temp = temp - 0.5f;
	temp = temp * (4448.0f / (0.002f * 50.4f));
	return temp;
}

extern inline float LINEAR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (50.0f / 4095.0f);
	temp = temp * 100.0f;
  return temp;
}

extern inline float BPS_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (5.0f / 4095.0f);
	temp = temp - 0.5f;
	temp = temp * 37.5f * 10.0f;
	return temp;
}

extern inline float APPS_DataConversion(uint16_t input)
{
	float temp;
	
	temp = 1425.0f - (float)input;
	temp = temp / 4.36f;
  return temp;
}

extern inline float STEERING_WHEEL_ANGLE_DataConversion(int16_t input)
{
	float temp;
	
	temp = (float)input * (5.0f / 4095.0f);
	temp = temp * 90.0f;
	temp = temp - 225.0f;
	return temp;
}

extern inline float IR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (5.0f / 4095.0f);
	temp = temp - 0.4f;
	temp = temp / 0.03f;
	return temp;
}

extern inline uint16_t DCU_MainCurrentSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_MAIN_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t DCU_CurrentSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_DCU_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline uint16_t DCU_XbeeCurrentSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / (SHUNT_RESISTOR_XBEE_CURRENT * INA_GAIN);
	data = data * 1000.0f;
	return (uint16_t)data;
}

extern inline float DCU_3v3SenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _3V3_MCU_GAIN;
	return data;
}

extern inline float DCU_5vSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _5V_DCU_GAIN;
	return data;
}

extern inline float DCU_12vSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data / _12V_POST_DIODE_SENSE_HIGH_GAIN;
	return data;
}

extern inline float DCU_TempSenseConversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB * 1000.0f;
	data = data - TEMP_SENSE_VOLTAGE_OFFSET;
	data = data * TEMP_SENSE_TEMPERATURE_GAIN;
	data = data - TEMP_SENSE_TEMPERATURE_OFFSET;
	return data;
}

extern inline float DCU_VbatConversion(uint32_t rawData)
{
	return ((float)rawData * ADC_LSB * 4.0f);
}

extern inline float analogAux1Conversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data * ANALOG_AUX_1_VOLTAGE_GAIN;
	return data;
}

extern inline float analogAux2Conversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data * ANALOG_AUX_2_VOLTAGE_GAIN;
	return data;
}

extern inline float analogAux3Conversion(uint32_t rawData)
{
	float data;
	
	data = (float)rawData * ADC_LSB;
	data = data * ANALOG_AUX_3_VOLTAGE_GAIN;
	return data;
}
