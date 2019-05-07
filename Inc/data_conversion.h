#ifndef __data_conversion_H
#define __data_conversion_H

#define ADC_NUMBER_OF_BITS									(uint8_t)12
#define ADC_FSR 														3.3f /* [V] */
#define ADC_LSB 														(ADC_FSR/((float)(2^ADC_NUMBER_OF_BITS)))
#define INA_GAIN 														(uint8_t)100
#define SHUNT_RESISTOR_MAIN_CURRENT  				0.022f /* [Ohm] */
#define SHUNT_RESISTOR_DCU_CURRENT   				0.022f /* [Ohm] */
#define SHUNT_RESISTOR_XBEE_CURRENT  				0.062f /* [Ohm] */
#define TEMP_SENSE_VOLTAGE_OFFSET						0.1f /* [mV] */
#define TEMP_SENSE_TEMPERATURE_GAIN 				0.1f /* [°C/mV] */
#define TEMP_SENSE_TEMPERATURE_OFFSET				40.0f /* [°C] */
#define _12V_POST_DIODE_SENSE_HIGH_GAIN			0.2126f
#define _5V_DCU_GAIN												0.5952f
#define _3V3_MCU_GAIN												0.8333f
#define MAIN_CURRENT_GAIN										(1 / (SHUNT_RESISTOR_MAIN_CURRENT * INA_GAIN)) /* [Ohm] */
#define DCU_CURRENT_GAIN										(1 / (SHUNT_RESISTOR_DCU_CURRENT * INA_GAIN)) /* [Ohm] */
#define XBEE_CURRENT_GAIN										(1 / (SHUNT_RESISTOR_XBEE_CURRENT * INA_GAIN)) /* [Ohm] */
#define ANALOG_AUX_1_GAIN										0.6410f	
#define ANALOG_AUX_2_GAIN										0.6410f
#define ANALOG_AUX_3_GAIN										0.6410f

extern inline uint16_t mainCurrentSenseConversion(uint32_t rawData);
extern inline uint16_t dcuCurrentSenseConversion(uint32_t rawData);
extern inline uint16_t xbeeCurrentSenseConversion(uint32_t rawData);
extern inline float _3v3SenseConversion(uint32_t rawData);
extern inline float _5vSenseConversion(uint32_t rawData);
extern inline float _12vSenseConversion(uint32_t rawData);
extern inline float dcuTempSenseConversion(uint32_t rawData);
extern inline float vbatConversion(uint32_t rawData);
extern inline float analogAux1Conversion(uint32_t rawData);
extern inline float analogAux2Conversion(uint32_t rawData);
extern inline float analogAux3Conversion(uint32_t rawData);

#endif
