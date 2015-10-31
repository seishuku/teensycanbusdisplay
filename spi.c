/*
	Copyright 2015 Matt Williams
	Simple K20 SPI driver implementation
	Needs work.
*/

#include "MK20D7.h"
#include <sys/types.h>

void SPI_Init(void)
{
	uint32_t mcr;

	SIM_SCGC6|=SIM_SCGC6_SPI0_MASK;

	// MOSI
	PORTD_PCR2=PORT_PCR_DSE_MASK|PORT_PCR_MUX(2);

	//MISO
	PORTD_PCR3=PORT_PCR_MUX(2);

	// SCLK
	PORTD_PCR1=PORT_PCR_DSE_MASK|PORT_PCR_MUX(2);

	SPI0_MCR=SPI_MCR_MSTR_MASK|SPI_MCR_CLR_RXF_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS(0x1F);

	mcr=SPI0_MCR;

	if(mcr&SPI_MCR_MDIS_MASK)
	{
		SPI0_CTAR0=SPI_CTAR_DBR_MASK|SPI_CTAR_FMSZ(7);
		SPI0_CTAR1=SPI_CTAR_DBR_MASK|SPI_CTAR_FMSZ(7)|SPI_CTAR_FMSZ(15);
	}
	else
	{
		SPI0_MCR=mcr|SPI_MCR_MDIS_MASK|SPI_MCR_HALT_MASK;
		SPI0_CTAR0=SPI_CTAR_DBR_MASK|SPI_CTAR_FMSZ(7);
		SPI0_CTAR1=SPI_CTAR_DBR_MASK|SPI_CTAR_FMSZ(7)|SPI_CTAR_FMSZ(15);
		SPI0_MCR=mcr;
	}
}

void SPI_WriteByte(uint8_t b)
{
	SPI0_MCR=SPI_MCR_MSTR_MASK|SPI_MCR_CLR_RXF_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS(0x1F);
	SPI0_SR=SPI_SR_TCF_MASK;
	SPI0_PUSHR=SPI_PUSHR_CONT_MASK|b;

	while(!(SPI0_SR&SPI_SR_TCF_MASK));
}

void SPI_WriteBuf(void *bufr, size_t n)
{
	int i, nf;
	uint8_t *buf=(uint8_t *)bufr, *limit;
	uint16_t w;

	if(n&1)
	{
		SPI_WriteByte(*buf++);
		n--;
	}

	SPI0_MCR=SPI_MCR_MSTR_MASK|SPI_MCR_CLR_RXF_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS(0x1F);

	nf=n/2<3?n/2:3;
	limit=buf+n;

	for(i=0;i<nf;i++)
	{
		w=*buf++<<8;
		w|=*buf++;
		SPI0_PUSHR=SPI_PUSHR_CONT_MASK|SPI_PUSHR_CTAS(1)|w;
	}

	while(buf<limit)
	{
		while(!(SPI0_SR&SPI_SR_RXCTR_MASK));

		w=*buf++<<8;
		w|=*buf++;
		SPI0_PUSHR=SPI_PUSHR_CONT_MASK|SPI_PUSHR_CTAS(1)|w;
		SPI0_POPR;
	}

	while(nf)
	{
		while(!(SPI0_SR&SPI_SR_RXCTR_MASK));

		SPI0_POPR;
		nf--;
	}
}

uint8_t SPI_ReadByte(void)
{
	SPI0_MCR=SPI_MCR_MSTR_MASK|SPI_MCR_CLR_RXF_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS(0x1F);
	SPI0_SR=SPI_SR_TCF_MASK;
	SPI0_PUSHR=SPI_PUSHR_CONT_MASK|0xFF;

	while(!(SPI0_SR&SPI_SR_TCF_MASK));

	return SPI0_POPR;
}

void SPI_ReadBuf(void *bufr, size_t n)
{
	int i, nf;
	uint8_t *buf=(uint8_t *)bufr, *limit;
	uint16_t w;

	if(n&1)
	{
		*buf++=SPI_ReadByte();
		n--;
	}

	SPI0_MCR=SPI_MCR_MSTR_MASK|SPI_MCR_CLR_RXF_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS(0x1F);
	nf=n/2<3?n/2:3;

	for(i=0;i<nf;i++)
		SPI0_PUSHR=SPI_PUSHR_CONT_MASK|SPI_PUSHR_CTAS(1)|0xFFFF;

	limit=buf+n-2*nf;

	while(buf<limit)
	{
		while(!(SPI0_SR&SPI_SR_RXCTR_MASK));

		SPI0_PUSHR=SPI_PUSHR_CONT_MASK|SPI_PUSHR_CTAS(1)|0xFFFF;
		w=SPI0_POPR;
		*buf++=w>>8;
		*buf++=w&0xFF;
	}

	limit+=2*nf;

	while(buf<limit)
	{
		while(!(SPI0_SR&SPI_SR_RXCTR_MASK));

		w=SPI0_POPR;
		*buf++=w>>8;
		*buf++=w&0xFF;
	}
}
