#ifndef __CAN_H__
#define __CAN_H__

// Extended CAN message ID flag
#define CAN_MESSAGE_ID_EXT			0x80000000

// Frame types
#define CAN_DATA_FRAME				0
#define CAN_RESPONSE_FRAME			1
#define CAN_REMOTE_FRAME			2

typedef struct
{
	uint32_t MessageID;
	uint16_t FrameType;
	uint8_t Length;
	uint8_t Data[8];
} CAN_Frame_t;

void CAN_Init(void);
int CAN_ReadFrame(CAN_Frame_t *Frame);
void CAN_SendFrame(CAN_Frame_t Frame);

#endif
