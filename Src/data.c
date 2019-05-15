#include "data.h"

uint32_t packetsReceivedCounter1;
uint32_t packetsReceivedCounter2;


extern void packetCounterReset(void)
{
	/*for(uint16_t i = 0; i < 256; i++) {
		packetsReceivedCounter[i] = 0;
	}*/
	
	packetsReceivedCounter1 = 0;
	packetsReceivedCounter2 = 0;
	
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
	switch(unpackedData->CAN_RxPacket_Header.StdId) {
		case 0x1F4:
			packetsReceivedCounter1++;
			break;
		
		case 0x1A4:
			packetsReceivedCounter2++;
			break;
	}
	
	return;
}
