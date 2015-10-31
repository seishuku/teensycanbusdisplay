#ifndef __CAN_H__
#define __CAN_H__

#define CAN_MESSAGE_ID_EXT			0x80000000

typedef struct
{
	uint32_t StdId;
	uint8_t DLC;
	uint8_t Data[8];
} CAN_Frame_t;

void CAN_Init(void);
int CAN_ReadFrame(CAN_Frame_t *Frame, uint16_t Timeout);
void CAN_SendFrame(CAN_Frame_t Frame);

#endif
