#include "data.h"

static uint32_t packetsReceivedCounter [256];


extern void packetCounterReset(void)
{
	for(uint16_t i = 0; i < 256; i++) {
		packetsReceivedCounter[i] = 0;
	}
	
	return;
}

extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData)
{	
	/* Get the four 16 bit data from the 8 bit CAN messages */
//	uint16_t data1 = (unpackedData->CAN_RxPacket_Data[0] << 8) | unpackedData->CAN_RxPacket_Data[1]; 			/* First data in the packet */
//	uint16_t data2 = (unpackedData->CAN_RxPacket_Data[2] << 8) | unpackedData->CAN_RxPacket_Data[3]; 			/* Second data in the packet */
//	uint16_t data3 = (unpackedData->CAN_RxPacket_Data[4] << 8) | unpackedData->CAN_RxPacket_Data[5]; 			/* Third data in the packet */
//	uint16_t data4 = (unpackedData->CAN_RxPacket_Data[6] << 8) | unpackedData->CAN_RxPacket_Data[7]; 			/* Fourth data in the packet */
	
	/* Manage the conversions of all the data types */
	/*switch(unpackedData->CAN_RxPacket_Header.StdId) {
		case :
			packetsReceivedCounter[0]++;
			break;
	}*/
	
	return;
}
