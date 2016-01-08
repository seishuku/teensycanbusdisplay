#ifndef __VWTPKWP2K_H__
#define __VWTPKWP2K_H__

int CAN_VWKWP_Init(void);
int CAN_VWKWP_GetValue(uint8_t LocalID, uint8_t Index, float *Value);
int CAN_VWKWP_ChannelTest(void);

#endif
