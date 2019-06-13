#include <stdint.h>
#include <stdio.h>
#include "data_conversion.h"
#include "usb_host.h"

uint16_t DATA_LOAD_CELL_FR_CalibrationOffset = 0;		/* [LSB] */
uint16_t DATA_LOAD_CELL_FL_CalibrationOffset = 0;		/* [LSB] */
uint16_t DATA_LOAD_CELL_RR_CalibrationOffset = 0;		/* [LSB] */
uint16_t DATA_LOAD_CELL_RL_CalibrationOffset = 0;		/* [LSB] */
uint16_t DATA_LINEAR_FR_CalibrationOffset = 0;			/* [LSB] */
uint16_t DATA_LINEAR_FL_CalibrationOffset = 0;			/* [LSB] */
uint16_t DATA_LINEAR_RR_CalibrationOffset = 0;			/* [LSB] */
uint16_t DATA_LINEAR_RL_CalibrationOffset = 0;			/* [LSB] */
uint16_t DATA_APPS_ZeroCalibrationOffset = 0;				/* [LSB] */
uint16_t DATA_APPS_FullCalibrationOffset = 1;				/* [LSB] */
uint16_t DATA_STEER_ANGLE_CalibrationOffset = 0;		/* [LSB] */
uint8_t sersorCalibrationsbuffer [50];


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

extern inline float SLIP_DataCoversion(int16_t input)
{
	if(input <= (-9999.0f)) {
		return (-9999.0f);
	}
	else {
		return (float)input;
	}
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

extern inline float LOAD_CELL_FR_DataConversion(int16_t input)
{ 
	float temp;
	
	temp = (float)(input - DATA_LOAD_CELL_FR_CalibrationOffset);
	temp = temp * (VREF_DAU_FR/4095);
	temp = temp * 4448.0f;
	temp = temp - ((VREF_DAU_FR/2) * 4448.0f);
	temp = temp - LOAD_CELL_FR_OFFEST;
	return temp;
}

extern inline float LOAD_CELL_FL_DataConversion(int16_t input)
{ 
	float temp;
	
	temp = (float)(input - DATA_LOAD_CELL_FL_CalibrationOffset);
	temp = temp * (VREF_DAU_FL/4095);
	temp = temp * 4448.0f;
	temp = temp - ((VREF_DAU_FL/2) * 4448.0f);
	temp = temp - LOAD_CELL_FL_OFFEST;
	return temp;
}

extern inline float LOAD_CELL_RR_DataConversion(int16_t input)
{ 
	float temp;
	
	temp = (float)(input - DATA_LOAD_CELL_RR_CalibrationOffset);
	temp = temp * (VREF_DAU_RR/4095);
	temp = temp * 4448.0f;
	temp = temp - ((VREF_DAU_RR/2) * 4448.0f);
	temp = temp - LOAD_CELL_RR_OFFEST;
	return temp;
}

extern inline float LOAD_CELL_RL_DataConversion(int16_t input)
{ 
	float temp;
	
	temp = (float)(input - DATA_LOAD_CELL_RL_CalibrationOffset);
	temp = temp * (VREF_DAU_RL/4095);
	temp = temp * 4448.0f;
	temp = temp - ((VREF_DAU_RL/2) * 4448.0f);
	temp = temp - LOAD_CELL_RL_OFFEST;
	return temp;
}

extern inline float LINEAR_FR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_LINEAR_FR_CalibrationOffset);
	temp = temp * (50.0f / 4095.0f);
  return temp;
}

extern inline float LINEAR_FL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_LINEAR_FL_CalibrationOffset);
	temp = temp * (50.0f / 4095.0f);
  return temp;
}

extern inline float LINEAR_RR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_LINEAR_RR_CalibrationOffset);
	temp = temp * (50.0f / 4095.0f);
  return temp;
}

extern inline float LINEAR_RL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_LINEAR_RL_CalibrationOffset);
	temp = temp * (50.0f / 4095.0f);
  return temp;
}

extern inline float BPS_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * 0.045787546f;
	temp = temp - 18.75f;
	return temp;
}

extern inline float APPS_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_APPS_ZeroCalibrationOffset);
	temp = temp / ((float)(DATA_APPS_FullCalibrationOffset - DATA_APPS_ZeroCalibrationOffset));
	temp = (float)input;
	temp = temp * 0.0012210012210012f;   								/* LSB to voltage */
	temp = ((temp - 0.490842490f) / (0.184371185f));		/* (val-val_min) / (val_max-val_min) */
	temp = temp * 100.0f;
  return temp;
}

extern inline float STEERING_WHEEL_ANGLE_DataConversion(int16_t input)
{
	float temp;
	
	temp = (float)(input - DATA_STEER_ANGLE_CalibrationOffset);
	temp = temp * 0.10989011f;
	temp = temp - 225.0f;
	return temp;
}

extern inline float IR_BRAKE_FL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_FL/4095);
	temp = temp / 0.005f;
	temp = temp - 100.0f;
	return temp;
}

extern inline float IR_BRAKE_FR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_FR/4095);
	temp = temp / 0.005f;
	temp = temp - 100.0f;
	return temp;
}

extern inline float IR_BRAKE_RR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_RR/4095);
	temp = temp / 0.005f;
	temp = temp - 100.0f;
	return temp;
}

extern inline float IR_BRAKE_RL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_RL/4095);
	temp = temp / 0.005f;
	temp = temp - 100.0f;
	return temp;
}

extern inline float IR_WHEEL_FR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_FR/4095);
	temp = temp / 0.03f;
	temp = temp - 13.33333333f;
	return temp;
}

extern inline float IR_WHEEL_FL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_FL/4095);
	temp = temp / 0.03f;
	temp = temp - 13.33333333f;
	return temp;
}

extern inline float IR_WHEEL_RR_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_RR/4095);
	temp = temp / 0.03f;
	temp = temp - 13.33333333f;
	return temp;
}

extern inline float IR_WHEEL_RL_DataConversion(uint16_t input)
{
	float temp;
	
	temp = (float)input * (VREF_DAU_RL/4095);
	temp = temp / 0.03f;
	temp = temp - 13.33333333f;
	return temp;
}

extern inline float DATRON_Velocity_DataConversion(int16_t input)
{
	float temp;
	
	temp = (float)input / 100.0f;
	temp = temp * 3.6f;
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

extern void writeSensorCalibrations(void)
{
	sprintf((char *)sersorCalibrationsbuffer, "Load cell FR offset [LSB signed] = %d;\n", DATA_LOAD_CELL_FR_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Load cell FL offset [LSB signed] = %d;\n", DATA_LOAD_CELL_FL_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Load cell RR offset [LSB signed] = %d;\n", DATA_LOAD_CELL_RR_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Load cell RL offset [LSB signed] = %d;\n", DATA_LOAD_CELL_RL_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Liner FR offset [LSB] = %d;\n", DATA_LINEAR_FR_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Liner FL offset [LSB] = %d;\n", DATA_LINEAR_FL_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Liner RR offset [LSB] = %d;\n", DATA_LINEAR_RR_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Liner RL offset [LSB] = %d;\n", DATA_LINEAR_RL_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "APPS zero [LSB] = %d;\n", DATA_APPS_ZeroCalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "APPS full [LSB] = %d;\n", DATA_APPS_FullCalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
	sprintf((char *)sersorCalibrationsbuffer, "Steer angle offset [LSB signed] = %d;\n", DATA_STEER_ANGLE_CalibrationOffset);
	USB_WriteLen(sersorCalibrationsbuffer);
}
