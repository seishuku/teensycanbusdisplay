#include "mk20d7.h"
#include "can.h"

// MessageBox codes for RX
#define CAN_MB_RX_NOT_ACTIVE		0x0		// MB is not active
#define CAN_MB_RX_FULL				0x2		// MB is full
#define CAN_MB_RX_EMPTY				0x4		// MB is active and empty
#define CAN_MB_RX_OVERRUN			0x6		// MB is being overwritten into a full buffer
#define CAN_MB_RX_BUSY				0x1		// FlexCAN is updating the contents of the MB
#define CAN_MB_RX_RANSWER			0xA		// A frame was configured to recognize a Remote Request Frame and transmit a ResponseFrame in return

// MessageBox codes for TX
#define CAN_MB_TX_NOT_ACTIVE		0x8		// MB is not active
#define CAN_MB_TX_ABORT				0x9		// MB is aborted
#define CAN_MB_TX_DATA_FRAME		0xC		// MB is a Tx Data Frame
#define CAN_MB_TX_REMOTE_FRAME		0xC		// MB is a Tx Remote Request Frame
#define CAN_MB_TX_RESPONSE_FRAME	0xA		// MB is a Tx Response Frame from an incoming Remote Request Frame

void CAN_Init(void)
{
	// Enable CAN module clock
	SIM_SCGC6|=SIM_SCGC6_FLEXCAN0_MASK;

	// initialization of CAN RX pin
	PORTA_PCR13=(PORTA_PCR13&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x05)))|PORT_PCR_MUX(0x02);
    PORTA_PCR13=(PORTA_PCR13&~(PORT_PCR_PS_MASK|PORT_PCR_ISF_MASK))|0x1;
    PORTA_PCR13=(PORTA_PCR13&~(PORT_PCR_PE_MASK|PORT_PCR_ISF_MASK))|0x2;

	// initialization of CAN TX pin
	PORTA_PCR12=(PORTA_PCR12&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x05)))|PORT_PCR_MUX(0x02);
    PORTA_PCR12=(PORTA_PCR12&~(PORT_PCR_PS_MASK|PORT_PCR_ISF_MASK))|0x1;
    PORTA_PCR12=(PORTA_PCR12&~(PORT_PCR_PE_MASK|PORT_PCR_ISF_MASK))|0x2;

	// Disable CAN module
	CAN0_MCR|=CAN_MCR_MDIS_MASK;

	// Bus Clock selected
	CAN0_CTRL1=CAN_CTRL1_PRESDIV(0x00)|CAN_CTRL1_RJW(0x00)|CAN_CTRL1_PSEG1(0x00)|CAN_CTRL1_PSEG2(0x00)|CAN_CTRL1_CLKSRC_MASK|CAN_CTRL1_PROPSEG(0x00);

	// Soft Reset
	CAN0_MCR=(CAN0_MCR&~CAN_MCR_MDIS_MASK)|CAN_MCR_SOFTRST_MASK;

	/* Wait for Soft reset Acknowledge */
	while(CAN0_MCR&CAN_MCR_SOFTRST_MASK);

	/* Wait for entering the freeze mode */
	while(!(CAN0_MCR&CAN_MCR_FRZACK_MASK));

	/* MCR reg. Settings */
	CAN0_MCR=(CAN0_MCR&~CAN_MCR_MAXMB(0x0E))|(CAN_MCR_WRNEN_MASK|CAN_MCR_SRXDIS_MASK|CAN_MCR_MAXMB(0x01));

	/* Setting CTRL1 register, 500kbps*/
	CAN0_CTRL1|=CAN_CTRL1_PRESDIV(0x02)|CAN_CTRL1_PSEG1(0x06)|CAN_CTRL1_PSEG2(0x07)|CAN_CTRL1_LBUF_MASK|CAN_CTRL1_PROPSEG(0x07)|CAN_CTRL1_LPB_MASK;

	/* Setting CTRL1 register, 100kbps */
//	CAN0_CTRL1|=CAN_CTRL1_PRESDIV(0x11)|CAN_CTRL1_PSEG1(0x02)|CAN_CTRL1_PSEG2(0x07)|CAN_CTRL1_LBUF_MASK|CAN_CTRL1_PROPSEG(0x07);

	/* Setting CTRL2 register */
	CAN0_CTRL2|=CAN_CTRL2_RRS_MASK;
	/* Set the Global acceptance mask register */
	CAN0_RXMGMASK=CAN_RXMGMASK_MG(0x1FFFFFFF);
	/* Set the acceptance mask register for buffers 14 */
	CAN0_RX14MASK=CAN_RX14MASK_RX14M(0x1FFFFFFF);
	/* Set the acceptance mask register for buffers 15 */
	CAN0_RX15MASK=CAN_RX15MASK_RX15M(0x1FFFFFFF);

	/* Initialize the message buffer 0 - Rx */
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_CODE_MASK)|(CAN_MB_RX_NOT_ACTIVE<<CAN_CS_CODE_SHIFT);
	/* Extended Frame bit IDE clear*/
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_IDE_MASK)|(0<<CAN_CS_IDE_SHIFT);
	/* Set standard buffer ID */
	CAN0_ID(0)=(CAN0_ID(0)&~CAN_ID_STD_MASK)|(0x07FF<<CAN_ID_STD_SHIFT);
	/* Empty Frame*/
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_CODE_MASK)|(CAN_MB_RX_EMPTY<<CAN_CS_CODE_SHIFT);
	/* SRR set to 0 */
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_SRR_MASK)|(0<<CAN_CS_SRR_SHIFT);
	/* RTR set to 0*/
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_RTR_MASK)|(0<<CAN_CS_RTR_SHIFT);
	/*Clear Data field*/
	CAN0_WORD0(0)=0x00000000;
	CAN0_WORD1(0)=0x00000000;
	/* Clear timestamp */
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_TIME_STAMP_MASK)|0x00;

	/* Initialize the message buffer 1 - Tx */
	/* Control/status word to hold Tx message buffer 1 inactive */
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_CODE_MASK)|(CAN_MB_TX_NOT_ACTIVE<<CAN_CS_CODE_SHIFT);
	/* SRR set to 0 */
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_SRR_MASK)|(0<<CAN_CS_SRR_SHIFT);
	/* RTR set to 0 */
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_RTR_MASK)|(0<<CAN_CS_RTR_SHIFT);
	/* Clear Data field */
	CAN0_WORD0(1)=0x00000000;
	CAN0_WORD1(1)=0x00000000;
	/* Clear timestamp */
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_TIME_STAMP_MASK)|0x00;

	/* Enable Tx/Rx interrupt for message buffer/s */
	CAN0_IMASK1=CAN_IMASK1_BUFLM(0x03);

	/* Exit Freeze Mode */
	CAN0_MCR&=~CAN_MCR_HALT_MASK;
    CAN0_MCR&=~CAN_MCR_FRZ_MASK;

	/* Wait for exit the freeze mode */
	while(CAN0_MCR&CAN_MCR_FRZACK_MASK);
	while(CAN0_MCR&CAN_MCR_NOTRDY_MASK);
}

void CAN_SendFrame(CAN_Frame_t Frame)
{
	uint8_t TxMBCode=0x00, i;

	// Is number of data greater than MaxDataLen?
	if(Frame.Length>8)
		return;

	// Is FrameType other than LDD_CAN_DATA_FRAME_STD, LDD_CAN_DATA_FRAME_EXT or LDD_CAN_REMOTE_FRAME?
	if(Frame.FrameType>CAN_RESPONSE_FRAME)
		return;

	// Are both IDLE and TX/RX bits of the status register cleared?
	if(CAN0_ESR1&(CAN_ESR1_RX_MASK|CAN_ESR1_TX_MASK))
		return;

	// Set TX Buffer Inactive
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_CODE_MASK)|(CAN_MB_TX_NOT_ACTIVE<<CAN_CS_CODE_SHIFT);

	// Is the frame configured as Extended ID?
	if((Frame.MessageID&CAN_MESSAGE_ID_EXT)!=0x00)
	{
		// Assign extended ID to buffer
		CAN0_ID(1)=(CAN0_ID(1)&~(CAN_ID_STD_MASK|CAN_ID_EXT_MASK))|(Frame.MessageID&~CAN_MESSAGE_ID_EXT);
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_IDE_MASK)|(1<<CAN_CS_IDE_SHIFT);
	}
	else
	{
		// Assign Standard ID to buffer
		CAN0_ID(1)=(CAN0_ID(1)&~CAN_ID_STD_MASK)|(Frame.MessageID<<CAN_ID_STD_SHIFT);
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_IDE_MASK)|(0<<CAN_CS_IDE_SHIFT);
	}

	// Is it a data or WaitOnRemote frame?
	if((Frame.FrameType==CAN_DATA_FRAME)||(Frame.FrameType==CAN_RESPONSE_FRAME))
	{
		// Fill message buffer data array
		for(i=0;i<Frame.Length;i++)
		{
			if(!(i&0xFC))
				CAN0_WORD0(1)=(CAN0_WORD0(1)&~(0xFF<<(24-((i&0x3)<<3))))|(Frame.Data[i]<<(24-((i&0x3)<<3)));
			else
				CAN0_WORD1(1)=(CAN0_WORD1(1)&~(0xFF<<(24-((i&0x3)<<3))))|(Frame.Data[i]<<(24-((i&0x3)<<3)));
		}

		// Set the length of the message
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_DLC_MASK)|(Frame.Length<<CAN_CS_DLC_SHIFT);
		// Clear RTR bit
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_RTR_MASK)|(0<<CAN_CS_RTR_SHIFT);
		// Clear SRR bit
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_SRR_MASK)|(0<<CAN_CS_SRR_SHIFT);

		if(Frame.FrameType==CAN_DATA_FRAME)
			// Set bufer as a transmit buffer
			TxMBCode=CAN_MB_TX_DATA_FRAME;
		else
			// Set bufer as a response transmit buffer for remote frames
			TxMBCode=CAN_MB_TX_RESPONSE_FRAME;
	}
	else
	{
		// Remote frame, set Tx bufer for remote frames
		TxMBCode=CAN_MB_TX_REMOTE_FRAME;

		// Set the length of the message
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_DLC_MASK)|(0<<CAN_CS_DLC_SHIFT);
		// Set the message as a remote frame
		CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_RTR_MASK)|(1<<CAN_CS_RTR_SHIFT);

		if(Frame.MessageID&CAN_MESSAGE_ID_EXT)
			// Extended frame, set SRR bit
			CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_SRR_MASK)|(1<<CAN_CS_SRR_SHIFT);
		else
			// Standard frame, clear SRR bit
			CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_SRR_MASK)|(0<<CAN_CS_SRR_SHIFT);
	}

	// Set code for Tx buffer of the message
	CAN0_CS(1)=(CAN0_CS(1)&~CAN_CS_CODE_MASK)|(TxMBCode<<CAN_CS_CODE_SHIFT);
}

int CAN_RxTxIsr(void)
{
	uint32_t RxBufferMask, BufferMask;
	uint32_t StatusReg=CAN0_IFLAG1;

	if(StatusReg!=0x00)
	{
		CAN0_IFLAG1=StatusReg;

		RxBufferMask=0x01&StatusReg;

		if(RxBufferMask!=0x00)
		{
			BufferMask=0x01;

			if((RxBufferMask&BufferMask)!=0x00)
				return 1;
		}
	}

	return 0;
}

int CAN_ReadFrame(CAN_Frame_t *Frame)
{
	uint8_t RxMBCode, i;

	// Wait for a message to arrive
//	while(!(CAN0_IFLAG1&CAN_IFLAG1_BUF5I_MASK));
	while(!CAN_RxTxIsr());

	// Read the code of the given message buffer
	RxMBCode=(CAN0_CS(0)&CAN_CS_CODE_MASK)>>CAN_CS_CODE_SHIFT;

	// Is the receive buffer busy?
	if(RxMBCode==CAN_MB_RX_BUSY)
		return 0;

	// Is the receive buffer empty?
	if(RxMBCode==CAN_MB_RX_EMPTY)
	{
		// Dummy read of Free running timer register release buffer lock
//		(void)CAN0_TIMER&CAN_TIMER_TIMER_MASK;
		return 0;
	}

	// Hold inactive Rx buffer
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_CODE_MASK)|(CAN_MB_RX_NOT_ACTIVE<<CAN_CS_CODE_SHIFT);

	// Extended ID?
	if(CAN0_CS(0)&CAN_CS_IDE_MASK)
		Frame->MessageID=(CAN0_ID(0)&(CAN_ID_STD_MASK|CAN_ID_EXT_MASK))|CAN_MESSAGE_ID_EXT;
	else
		Frame->MessageID=(CAN0_ID(0)&CAN_ID_STD_MASK)>>CAN_ID_STD_SHIFT;

	// Remote Frame type?
	if((CAN0_CS(0)&CAN_CS_RTR_MASK)>>CAN_CS_RTR_SHIFT)
		Frame->FrameType=CAN_REMOTE_FRAME;
	else
		Frame->FrameType=CAN_DATA_FRAME;

	// Result length of the message
	Frame->Length=(CAN0_CS(0)&CAN_CS_DLC_MASK)>>CAN_CS_DLC_SHIFT;
	// Value time stamp of the message
//	Frame->TimeStamp=CAN0_CS(0)&CAN_CS_TIME_STAMP_MASK;

	// Is it a data frame?
	if(Frame->FrameType!=CAN_REMOTE_FRAME)
	{
		// Read data bytes
		for(i=0;i<Frame->Length;i++)
		{
			if(!(i&0xFC))
				Frame->Data[i]=CAN0_WORD0(0)>>(24-((i&0x3)<<3));
			else
				Frame->Data[i]=CAN0_WORD1(0)>>(24-((i&0x3)<<3));
		}
	}

	// Set the message buffer code
	CAN0_CS(0)=(CAN0_CS(0)&~CAN_CS_CODE_MASK)|(CAN_MB_RX_EMPTY<<CAN_CS_CODE_SHIFT);

	// Dummy read of Free running timer register release buffer lock
//	CAN0_TIMER&CAN_TIMER_TIMER_MASK;

	// Is the overrun flag set?
	if(RxMBCode==CAN_MB_RX_OVERRUN)
		return 0;

	return 1;
}
