/*
	Copyright 2015 Matt Williams
	KWP2000 over VW TP2.0 CANBUS
	Needs a little work, maybe make more modular and split TP2.0 code from KWP2000 code, make TP packet encoding easier.
	Also needs better error handling.
*/

#include "MK20D7.h"
#include "can.h"

// CANbus rx/tx frames
CAN_Frame_t _RxMsg, _TxMsg;

// IDs used by VW TP2.0
uint16_t _txID, _rxID;

// sequence tracking and temp buffer for data packets
uint8_t _seq, _temp[128];

// Opens a VW TP2.0 channel and start a KWP2000 diagnostics session
// Returns:
//		 0 = Positive response
//		-1 = CAN frame read timeout
//		-2 = Negative response
//		-3 = Never got an acknowledgement
int CAN_VWKWP_Init(void)
{
	// Reset packet sequence
	_seq=0;

	// Open a channel
	// Example: 0x200 7 0x01 0xC0 0x00 0x10 0x00 0x03 0x01
	_TxMsg.MessageID=0x200; // VW TP2.0, inital ID when opening channel
	_TxMsg.FrameType=CAN_DATA_FRAME;
	_TxMsg.Length=7;
	_TxMsg.Data[0]=0x01;	// TP2.0 module ID (ECU)
	_TxMsg.Data[1]=0xC0;	// Operation (setup request)
	_TxMsg.Data[2]=0x00;	// RX ID
	_TxMsg.Data[3]=0x10; // RX prefix + valid nibble (is set invalid, module tells us what to use)
	_TxMsg.Data[4]=0x00; // TX ID
	_TxMsg.Data[5]=0x03; // TX prefix + valid nibble (we tell module what to use, transmit at CAN ID 0x300)
	_TxMsg.Data[6]=0x01; // Application type

	CAN_SendFrame(_TxMsg);

	if(!CAN_ReadFrame(&_RxMsg))
		return -1;

	// VW TP2.0, inital module TX ID is 0x200 + Module ID
	// Byte0, TP2.0 module ID (module response is always 0x00?)
	// Byte1, 0xD0 positive, 0xD6-0xD8 negative response
	// Byte2, RX ID of incoming messages
	// Byte3, RX ID prefix + valid nibble
	// Byte4, TX ID of outgoing messages
	// Byte5, TX ID prefix + valid nibble
	// Byte6, Application type
	// Example: 0x201 7 0x00 0xD0 0x00 0x03 0x40 0x07 0x01
	if(_RxMsg.MessageID==0x201&&_RxMsg.Data[1]==0xD0)
	{
		// Positive response, set rx/tx message IDs
		_txID=_RxMsg.Data[4]+(_RxMsg.Data[5]<<8);
		_rxID=_RxMsg.Data[2]+(_RxMsg.Data[3]<<8);
	}
	else
		return -2;

	// Set up timing parameters
	// Example: 0x740 6 0xA0 0x0F 0x8A 0xFF 0x32 0xFF
	// +-------------+-------------------------------------+
	// | Bit 7 Bit 6 | Bit 5 Bit 4 Bit 3 Bit 2 Bit 1 Bit 0 |
	// | Time Base   | Time								   |
	// +-------------+-------------------------------------+
	// Time Base: 00 = 0.1ms
	//			  01 = 1ms
	// 			  10 = 10ms
	//			  11 = 100ms
	// Time:	  0..63
	_TxMsg.MessageID=_txID; // VW TP2.0, ID from opening channel
	_TxMsg.FrameType=CAN_DATA_FRAME;
	_TxMsg.Length=6;
	_TxMsg.Data[0]=0xA0;	// Operation (parameter request)
	_TxMsg.Data[1]=0x0F;	// Block size, number of packets to send before expecting ACK
	_TxMsg.Data[2]=0x8A;	// T1 (tx message timeout) 0-6300ms, set to 100ms
	_TxMsg.Data[3]=0xFF;	// T2 (always 0xFF)
	_TxMsg.Data[4]=0x32;	// T3 (minimum time for tx packets) 0-6300ms, set to 5ms
	_TxMsg.Data[5]=0xFF;	// T4 (always 0xFF)

	CAN_SendFrame(_TxMsg);

	if(!CAN_ReadFrame(&_RxMsg))
		return -1;

	// Response is the module's timing parameters, same format
	// Example: 0x300 6 0xA1 0x0F 0x8A 0xFF 0x4A 0xFF
	// 0x8A = 100ms timeout
	// 0x4A = 10ms time between packets
	if(_RxMsg.MessageID==_rxID&&_RxMsg.Data[0]!=0xA1)
		return -2;

	// VWTP2.0 KWP2k startDiagnosticSession
	// Example: 0x740 5 0x10 0x00 0x02 0x10 0x89
	_TxMsg.MessageID=_txID;			// VW TP2.0, ID from opening channel
	_TxMsg.FrameType=CAN_DATA_FRAME;
	_TxMsg.Length=5;
	_TxMsg.Data[0]=0x10+(_seq++);	// VW TP 2.0 Operation (waiting for ACK, last packet)
	_TxMsg.Data[1]=0x00;			// KWP2000 Session ID?
	_TxMsg.Data[2]=0x02;			// KWP2000 2 bytes
	_TxMsg.Data[3]=0x10;			// KWP2000 startDiagnosticSession
	_TxMsg.Data[4]=0x89;			// KWP2000 Parameter 0x89 (manufacturer specific)

	CAN_SendFrame(_TxMsg);

	if(!CAN_ReadFrame(&_RxMsg))
		return -1;

	// Example: 300 1 0xB1
	// Module ACK
	// TO-DO: Sequence should be our last packet sequence + 1, track this?
	if(_RxMsg.MessageID==_rxID&&(_RxMsg.Data[0]&0xF0)==0xB0)
	{
		if(!CAN_ReadFrame(&_RxMsg))
			return -1;

		// Example: 0x300 5 0x10 0x00 0x02 0x50 0x89
		// KWP2000 startDiagnosticSession positive response
		if(_RxMsg.MessageID==_rxID&&_RxMsg.Data[0]==0x10&&_RxMsg.Data[3]==0x50&&_RxMsg.Data[4]==0x89)
		{
			// Send ACK
			_TxMsg.MessageID=_txID; // VW TP2.0 outgoing message
			_TxMsg.Length=1;
			_TxMsg.Data[0]=0xB0+(_RxMsg.Data[0]&0x0F)+1; // ACK

			CAN_SendFrame(_TxMsg);

			return 0;
		}
		else
			return -2;
	}

	return -3;
}

// Translate two byte value using correct formula
float _DecodeValue(uint8_t f, uint8_t a, uint8_t b)
{
	switch(f)
	{
		case 0x01:		//RPM
			return (float)(a*b)*0.2f;

		case 0x05:		//°C
			return (float)(a*(b-100))*0.1f;

		case 0x60:		//mbar
		case 0x7E:		//grams
			return (float)(a*b)*0.1f;

		default:		//RAW
			return (float)a*256.0f+b;
	}
}

// Polls KWP2000 readDataByLocalIdentifier data block
// Parameters:
//		LocalID =  Local Identifier parameter (RLOCID)
//		Index = Index into response data block, minus data size, response byte, and RLOCID.
// Return values:
//		 0 = Valid results
// 		-1 = CAN frame reading timeout or invalid Value pointer
//		-2 = Unexpected data
//		-3 = Negative response
//		-4 = No acknowledgement
int CAN_VWKWP_GetValue(uint8_t LocalID, uint8_t Index, float *Value)
{
	uint8_t i=0;

	if(Value==0)
		return -1;

	// Example: 0x740 5 0x10 0x00 0x02 0x21 0x72
	_TxMsg.MessageID=_txID;				// VW TP2.0 outgoing message
	_TxMsg.FrameType=CAN_DATA_FRAME;
	_TxMsg.Length=5;
	_TxMsg.Data[0]=0x10+(_seq++);	// VW TP2.0 Operation (last packet, waiting for ack)
	_TxMsg.Data[1]=0x00;			// KWP2000 Session ID?
	_TxMsg.Data[2]=0x02;			// KWP2000 2 bytes
	_TxMsg.Data[3]=0x21;			// KWP2000 readDataByLocalIdentifier
	_TxMsg.Data[4]=LocalID;			// KWP2000 Local identifier parameter

	if(_seq>0x0F) _seq=0x00;

	CAN_SendFrame(_TxMsg);

	if(!CAN_ReadFrame(&_RxMsg))
		return -1;

	// Example: 300 1 0xB1
	// Module ACK
	// TO-DO: Sequence should be our last packet sequence + 1, do we need to track this?
	if(_RxMsg.MessageID==_rxID&&(_RxMsg.Data[0]&0xF0)==0xB0)
	{
		if(!CAN_ReadFrame(&_RxMsg))
			return -1;

		// Example: 0x300 8 0x20 0x00 0x1A 0x61 0x72 0x08 0x0A 0xFF
		//			0x300 8 0x21 0x08 0x0A 0xFF 0x71 0x0A 0x80 0x71
		//			0x300 8 0x22 0x0A 0x80 0x68 0x0F 0x8F 0x7E 0x03
		//			0x300 8 0x13 0x39 0x7E 0x03 0x39 0x25 0x00 0x00
		// Data frame, not waiting for ACK, more packets to follow
		// Byte0		VW TP2.0 Operation + Number in sequence
		// Byte1		KWP2000 Session ID?
		// Byte2		KWP2000 Total data size ((x+2)/7 = number of packets)
		// Byte3		KWP2000 Response type (positive/negative)
		// Byte4		KWP2000 Local ID parameter
		// Byte5-Byte7	KWP2000 Data
		// TO-DO: This probably should track packet sequence.
		if(_RxMsg.MessageID==_rxID&&(_RxMsg.Data[0]&0xF0)==0x20&&_RxMsg.Data[3]==0x61&&_RxMsg.Data[4]==_TxMsg.Data[4])
		{
			_temp[i++]=_RxMsg.Data[3];
			_temp[i++]=_RxMsg.Data[4];
			_temp[i++]=_RxMsg.Data[5];
			_temp[i++]=_RxMsg.Data[6];
			_temp[i++]=_RxMsg.Data[7];

			while(1)
			{
				if(!CAN_ReadFrame(&_RxMsg))
					return -1;

				// Byte0		VW TP2.0 Operation + Number in sequence
				// Byte1-Byte7	KWP2000 Data

				// Data frame, not waiting for ACK, more packets to follow
				if(_RxMsg.MessageID==_rxID&&(_RxMsg.Data[0]&0xF0)==0x20)
				{
					_temp[i++]=_RxMsg.Data[1];
					_temp[i++]=_RxMsg.Data[2];
					_temp[i++]=_RxMsg.Data[3];
					_temp[i++]=_RxMsg.Data[4];
					_temp[i++]=_RxMsg.Data[5];
					_temp[i++]=_RxMsg.Data[6];
					_temp[i++]=_RxMsg.Data[7];
				}
				// Data frame, last packet, waiting for ACK
				else if(_RxMsg.MessageID==_rxID&&(_RxMsg.Data[0]&0xF0)==0x10)
				{
					_temp[i++]=_RxMsg.Data[1];
					_temp[i++]=_RxMsg.Data[2];
					_temp[i++]=_RxMsg.Data[3];
					_temp[i++]=_RxMsg.Data[4];
					_temp[i++]=_RxMsg.Data[5];
					_temp[i++]=_RxMsg.Data[6];
					_temp[i++]=_RxMsg.Data[7];

					// Send acknowledgement, sequence # = last packet sequence + 1
					// Example: 300 1 0xB4
					_TxMsg.MessageID=_txID; // VW TP2.0 outgoing message
					_TxMsg.Length=1;
					_TxMsg.Data[0]=0xB0+(_RxMsg.Data[0]&0x0F)+1; // ACK

					CAN_SendFrame(_TxMsg);

					*Value=_DecodeValue(_temp[3*Index+0+2], _temp[3*Index+1+2], _temp[3*Index+2+2]);

					return 0;
				}
				else
					return -2;
			}
		}
		else
			return -3;
	}
	else
		return -4;
}

// Sends KWP2000 channel test for keep alive
// Parameters:
//		None.
// Return values:
//		 0 = Ok
// 		-1 = CAN frame reading timeout
//		-2 = Unexpected error
int CAN_VWKWP_ChannelTest(void)
{
	// Example: 0x740 1 0xA3
	_TxMsg.MessageID=_txID;		// VW TP2.0, ID from opening channel
	_TxMsg.FrameType=CAN_DATA_FRAME;
	_TxMsg.Length=1;
	_TxMsg.Data[0]=0xA3;	// Operation (channel test)

	CAN_SendFrame(_TxMsg);

	if(!CAN_ReadFrame(&_RxMsg))
		return -1;

	// Example: 0x300 6 0xA1 0x0F 0x8A 0xFF 0x4A 0xFF 0x00 0x00
	if(_RxMsg.MessageID==_rxID&&_RxMsg.Data[0]==0xA1)
		return 0;

	return -2;
}
