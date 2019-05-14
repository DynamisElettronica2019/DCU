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
#define _12V_POST_DIODE_SENSE_HIGH_GAIN			0.1293f
#define _5V_DCU_GAIN												0.3237f
#define _3V3_MCU_GAIN												0.5313f
#define VBAT_GAIN														4.0f
#define ANALOG_AUX_1_GAIN										0.641f	
#define ANALOG_AUX_2_GAIN										0.641f
#define ANALOG_AUX_3_GAIN										0.641f

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
