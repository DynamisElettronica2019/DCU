#ifndef __data_conversion_H
#define __data_conversion_H

#define ADC_LSB 														(3.3f/4095.0f)
#define INA_GAIN 														(uint8_t)100
#define SHUNT_RESISTOR_MAIN_CURRENT  				0.022f /* [Ohm] */
#define SHUNT_RESISTOR_DCU_CURRENT   				0.022f /* [Ohm] */
#define SHUNT_RESISTOR_XBEE_CURRENT  				0.062f /* [Ohm] */

#define TEMP_SENSE_VOLTAGE_OFFSET						100.0f /* [mV] */
#define TEMP_SENSE_TEMPERATURE_GAIN 				0.1f /* [°C/mV] */
#define TEMP_SENSE_TEMPERATURE_OFFSET				40.0f /* [°C] */
#define _12V_POST_DIODE_SENSE_HIGH_GAIN			0.213f
#define _5V_DCU_GAIN												0.595f
#define _3V3_MCU_GAIN												0.833f
#define VBAT_GAIN														4.0f
#define ANALOG_AUX_1_VOLTAGE_GAIN						1.56f	
#define ANALOG_AUX_2_VOLTAGE_GAIN						1.56f
#define ANALOG_AUX_3_VOLTAGE_GAIN						1.56f

#define LOAD_CELL_FR_OFFEST 								0.0f 	/* [N] */
#define LOAD_CELL_FL_OFFEST 								0.0f	/* [N] */
#define LOAD_CELL_RR_OFFEST 								0.0f	/* [N] */
#define LOAD_CELL_RL_OFFEST 								0.0f	/* [N] */
#define VREF_DAU_FR  												5.0f	/* [V] */
#define VREF_DAU_FL  												5.0f	/* [V] */
#define VREF_DAU_RR  												5.0f	/* [V] */
#define VREF_DAU_RL  												5.0f	/* [V] */


extern inline float EFI_TEMPERATURE_DataConversion(uint16_t input);
extern inline float T_H20_ENGINE_DataConversion(uint16_t input);
extern inline float BATTERY_VOLTAGE_DataConversion(uint16_t input);
extern inline float TPS_DataConversion(uint16_t input);
extern inline float WATER_PRESSURE_DataConversion(uint16_t input);
extern inline float EXHAUST_TEMPERATURE_DataConversion(uint16_t input);
extern inline float SLIP_DataCoversion(int16_t input);
extern inline float H20_PUMP_DUTY_CYCLE_DataConversion(uint16_t input);
extern inline float FUEL_LEVEL_DataConversion(uint16_t input);
extern inline float LOAD_CELL_FR_DataConversion(int16_t input);
extern inline float LOAD_CELL_FL_DataConversion(int16_t input);
extern inline float LOAD_CELL_RR_DataConversion(int16_t input);
extern inline float LOAD_CELL_RL_DataConversion(int16_t input);
extern inline float LINEAR_FR_DataConversion(uint16_t input);
extern inline float LINEAR_FL_DataConversion(uint16_t input);
extern inline float LINEAR_RR_DataConversion(uint16_t input);
extern inline float LINEAR_RL_DataConversion(uint16_t input);
extern inline float BPS_DataConversion(uint16_t input);
extern inline float APPS_DataConversion(uint16_t input);
extern inline float STEERING_WHEEL_ANGLE_DataConversion(int16_t input);
extern inline float IR_BRAKE_FR_DataConversion(uint16_t input);
extern inline float IR_BRAKE_FL_DataConversion(uint16_t input);
extern inline float IR_BRAKE_RR_DataConversion(uint16_t input);
extern inline float IR_BRAKE_RL_DataConversion(uint16_t input);
extern inline float IR_WHEEL_FR_DataConversion(uint16_t input);
extern inline float IR_WHEEL_FL_DataConversion(uint16_t input);
extern inline float IR_WHEEL_RR_DataConversion(uint16_t input);
extern inline float IR_WHEEL_RL_DataConversion(uint16_t input);
extern inline float DATRON_Velocity_DataConversion(int16_t input);
extern inline uint16_t DCU_MainCurrentSenseConversion(uint32_t rawData);
extern inline uint16_t DCU_CurrentSenseConversion(uint32_t rawData);
extern inline uint16_t DCU_XbeeCurrentSenseConversion(uint32_t rawData);
extern inline float DCU_3v3SenseConversion(uint32_t rawData);
extern inline float DCU_5vSenseConversion(uint32_t rawData);
extern inline float DCU_12vSenseConversion(uint32_t rawData);
extern inline float DCU_TempSenseConversion(uint32_t rawData);
extern inline float DCU_VbatConversion(uint32_t rawData);
extern inline float analogAux1Conversion(uint32_t rawData);
extern inline float analogAux2Conversion(uint32_t rawData);
extern inline float analogAux3Conversion(uint32_t rawData);
extern void writeSensorCalibrations(void);

#endif
