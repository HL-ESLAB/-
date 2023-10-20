#ifndef __MY_PACKET_H
#define __MY_PACKET_H

#include "sys.h"

//typedef struct
//{
//	u8 head;
//	u8 tail;
//	u8 direction;
//	u8 data[4];
//}send_pack;

//extern send_pack tx_pack;
void usart_senddata(USART_TypeDef * USARTx,
	unsigned short ShengLi_ADC,unsigned short NiuLi_ADC,unsigned short Speed);

#endif

