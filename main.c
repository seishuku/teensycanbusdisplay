#include "MK20D7.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi.h"
#include "can.h"
#include "vwtpkwp2k.h"

// Helpful bits
#define BIT0             (0x0001)
#define BIT1             (0x0002)
#define BIT2             (0x0004)
#define BIT3             (0x0008)
#define BIT4             (0x0010)
#define BIT5             (0x0020)
#define BIT6             (0x0040)
#define BIT7             (0x0080)
#define BIT8             (0x0100)
#define BIT9             (0x0200)
#define BIT10            (0x0400)
#define BIT11            (0x0800)
#define BIT12            (0x1000)
#define BIT13            (0x2000)
#define BIT14            (0x4000)
#define BIT15            (0x8000)

// Timing junk
uint32_t Time=0, Time2=0;
volatile uint32_t SysTick=0;

float prevBoost=9999.0f, prevEGT=9999.0f, prevCLT=9999.0f, prevSoot=9999.0f;
float Boost=9999.0f, EGT=9999.0f, CLT=9999.0f, Soot=9999.0f;

void delay(volatile unsigned i)
{
	while(i--);
}

// Display 4, 4 digit numbers across two display modules
uint8_t _display_buf[16]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t _digit[]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 }; // 0xBF = '-', 0x7F = '.', 0xFF = off

void _display(int16_t value, uint8_t disp, uint8_t dp)
{
	uint8_t neg=0, i;

	if(value>=9999)
		value=9999;

	if(value<=-999)
		value=-999;

	if(value<0)
	{
		neg=1;
		value=-value;
	}

	i=4*disp;

	if(value==0)
		_display_buf[i++]=_digit[0];
	else while(value)
	{
		_display_buf[i++]=_digit[value%10];
		value/=10;
	}

	if(neg)
		_display_buf[i++]=0xBF;

	while(i<=4*disp+4)
		_display_buf[i++]=0xFF;

	if(dp)
		_display_buf[4*disp+(dp-1)]&=0x7F;

	GPIOC_PCOR|=BIT0;
	SPI_WriteBuf(_display_buf, 16);
	GPIOC_PSOR|=BIT0;
}

// Display nothing on two display modules
void _displayOff(void)
{
	uint8_t buf[8]={ 0xFF, 0xFF, 0xFF, 0xFF };

	GPIOC_PCOR|=BIT0;
	SPI_WriteBuf(buf, 4);
	SPI_WriteBuf(buf, 4);
	SPI_WriteBuf(buf, 4);
	SPI_WriteBuf(buf, 4);
	GPIOC_PSOR|=BIT0;
}

int main(void)
{
	// Chip select for the displays
	PORTC_PCR0=PORT_PCR_MUX(1);
	GPIOC_PDDR|=BIT0;
	GPIOC_PSOR|=BIT0;

	// Initalize the SPI module (used for 74HC595 displays)
	SPI_Init();

	// Initalize the CANbus module
	CAN_Init();

	// Inital set to clear any garbage
	_displayOff();

	CAN_VWKWP_Init();

	// Inital time setting
	Time=SysTick;
	Time2=SysTick;

	while(1)
	{
		// Update every ~10ms
		if((SysTick-Time)>10)
		{
			Time=SysTick;

			prevSoot=Soot;
			Soot=(CAN_VWKWP_GetValue(0x72, 6)+prevSoot)*0.5f;

			prevCLT=CLT;
			CLT=((CAN_VWKWP_GetValue(0x01, 3)*1.8f+32.0f)+prevCLT)*0.5f;

			prevEGT=EGT;
			EGT=((CAN_VWKWP_GetValue(0x63, 1)*1.8f+32.0f)+prevEGT)*0.5f;

			prevBoost=Boost;
			Boost=((CAN_VWKWP_GetValue(0x0B, 2)-CAN_VWKWP_GetValue(0x0A, 1))+prevBoost)*0.5f;
		}

		// Channel keep alive
		if((SysTick-Time2)>100)
		{
			Time2=SysTick;
			CAN_VWKWP_ChannelTest();
		}

		_display((int16_t)EGT, 0, 0);
		_display((int16_t)(Soot*10.0f), 1, 2);
		_display((int16_t)CLT, 2, 0);
		_display((int16_t)(Boost*0.145f), 3, 2);
	}

	return 0;
}
