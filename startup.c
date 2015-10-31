#include "mk20d7.h"

extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;

extern uint32_t SysTick;

extern int main (void);
void ResetHandler(void);

void fault_isr(void)
{
	while(1);
}

void unused_isr(void)
{
	while(1);
}

void systick_default_isr(void)
{
	SysTick++;
}

void nmi_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void hard_fault_isr(void)		__attribute__ ((weak, alias("fault_isr")));
void memmanage_fault_isr(void)	__attribute__ ((weak, alias("fault_isr")));
void bus_fault_isr(void)		__attribute__ ((weak, alias("fault_isr")));
void usage_fault_isr(void)		__attribute__ ((weak, alias("fault_isr")));
void svcall_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void debugmonitor_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void pendablesrvreq_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void systick_isr(void)		__attribute__ ((weak, alias("systick_default_isr")));

void dma_ch0_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch1_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch2_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch3_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch4_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch5_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch6_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch7_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch8_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch9_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch10_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch11_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch12_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch13_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch14_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_ch15_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dma_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void mcm_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void flash_cmd_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void flash_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void low_voltage_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void wakeup_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void watchdog_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void i2c0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void i2c1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void i2c2_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void spi0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void spi1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void spi2_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void sdhc_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void can0_message_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void can0_bus_off_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void can0_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void can0_tx_warn_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void can0_rx_warn_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void can0_wakeup_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void i2s0_tx_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void i2s0_rx_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void i2s0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void uart0_lon_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart0_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart0_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart1_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart1_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart2_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart2_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart3_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart3_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart4_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart4_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void uart5_status_isr(void)	__attribute__ ((weak, alias("unused_isr")));
void uart5_error_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void adc0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void adc1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void cmp0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void cmp1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void cmp2_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void ftm0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void ftm1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void ftm2_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void ftm3_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void cmt_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void rtc_alarm_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void rtc_seconds_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void pit_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void pit0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void pit1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void pit2_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void pit3_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void pdb_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void usb_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void usb_charge_isr(void)		__attribute__ ((weak, alias("unused_isr")));
void dac0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void dac1_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void tsi0_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void mcg_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void lptmr_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void porta_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void portb_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void portc_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void portd_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void porte_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void portcd_isr(void)			__attribute__ ((weak, alias("unused_isr")));
void software_isr(void)		__attribute__ ((weak, alias("unused_isr")));

__attribute__ ((section(".dmabuffers"), used, aligned(512))) void (* _VectorsRam[111])(void);

__attribute__ ((section(".vectors"), used)) void (* const _VectorsFlash[111])(void)=
{
	(void (*)(void))((unsigned long)&_estack),	//  0 ARM: Initial Stack Pointer
	ResetHandler,						//  1 ARM: Initial Program Counter
	nmi_isr,							//  2 ARM: Non-maskable Interrupt (NMI)
	hard_fault_isr,						//  3 ARM: Hard Fault
	memmanage_fault_isr,					//  4 ARM: MemManage Fault
	bus_fault_isr,						//  5 ARM: Bus Fault
	usage_fault_isr,					//  6 ARM: Usage Fault
	fault_isr,							//  7 --
	fault_isr,							//  8 --
	fault_isr,							//  9 --
	fault_isr,							// 10 --
	svcall_isr,						// 11 ARM: Supervisor call (SVCall)
	debugmonitor_isr,					// 12 ARM: Debug Monitor
	fault_isr,							// 13 --
	pendablesrvreq_isr,					// 14 ARM: Pendable req serv(PendableSrvReq)
	systick_isr,						// 15 ARM: System tick timer (SysTick)
	dma_ch0_isr,						// 16 DMA channel 0 transfer complete
	dma_ch1_isr,						// 17 DMA channel 1 transfer complete
	dma_ch2_isr,						// 18 DMA channel 2 transfer complete
	dma_ch3_isr,						// 19 DMA channel 3 transfer complete
	dma_ch4_isr,						// 20 DMA channel 4 transfer complete
	dma_ch5_isr,						// 21 DMA channel 5 transfer complete
	dma_ch6_isr,						// 22 DMA channel 6 transfer complete
	dma_ch7_isr,						// 23 DMA channel 7 transfer complete
	dma_ch8_isr,						// 24 DMA channel 8 transfer complete
	dma_ch9_isr,						// 25 DMA channel 9 transfer complete
	dma_ch10_isr,						// 26 DMA channel 10 transfer complete
	dma_ch11_isr,						// 27 DMA channel 10 transfer complete
	dma_ch12_isr,						// 28 DMA channel 10 transfer complete
	dma_ch13_isr,						// 29 DMA channel 10 transfer complete
	dma_ch14_isr,						// 30 DMA channel 10 transfer complete
	dma_ch15_isr,						// 31 DMA channel 10 transfer complete
	dma_error_isr,						// 32 DMA error interrupt channel
	unused_isr,						// 33 --
	flash_cmd_isr,						// 34 Flash Memory Command complete
	flash_error_isr,					// 35 Flash Read collision
	low_voltage_isr,					// 36 Low-voltage detect/warning
	wakeup_isr,						// 37 Low Leakage Wakeup
	watchdog_isr,						// 38 Both EWM and WDOG interrupt
	unused_isr,						// 39 --
	i2c0_isr,							// 40 I2C0
	i2c1_isr,							// 41 I2C1
	spi0_isr,							// 42 SPI0
	spi1_isr,							// 43 SPI1
	unused_isr,						// 44 --
	can0_message_isr,					// 45 CAN OR'ed Message buffer (0-15)
	can0_bus_off_isr,					// 46 CAN Bus Off
	can0_error_isr,						// 47 CAN Error
	can0_tx_warn_isr,					// 48 CAN Transmit Warning
	can0_rx_warn_isr,					// 49 CAN Receive Warning
	can0_wakeup_isr,					// 50 CAN Wake Up
	i2s0_tx_isr,						// 51 I2S0 Transmit
	i2s0_rx_isr,						// 52 I2S0 Receive
	unused_isr,						// 53 --
	unused_isr,						// 54 --
	unused_isr,						// 55 --
	unused_isr,						// 56 --
	unused_isr,						// 57 --
	unused_isr,						// 58 --
	unused_isr,						// 59 --
	uart0_lon_isr,						// 60 UART0 CEA709.1-B (LON) status
	uart0_status_isr,					// 61 UART0 status
	uart0_error_isr,					// 62 UART0 error
	uart1_status_isr,					// 63 UART1 status
	uart1_error_isr,					// 64 UART1 error
	uart2_status_isr,					// 65 UART2 status
	uart2_error_isr,					// 66 UART2 error
	unused_isr,						// 67 --
	unused_isr,						// 68 --
	unused_isr,						// 69 --
	unused_isr,						// 70 --
	unused_isr,						// 71 --
	unused_isr,						// 72 --
	adc0_isr,							// 73 ADC0
	adc1_isr,							// 74 ADC1
	cmp0_isr,							// 75 CMP0
	cmp1_isr,							// 76 CMP1
	cmp2_isr,							// 77 CMP2
	ftm0_isr,							// 78 FTM0
	ftm1_isr,							// 79 FTM1
	ftm2_isr,							// 80 FTM2
	cmt_isr,							// 81 CMT
	rtc_alarm_isr,						// 82 RTC Alarm interrupt
	rtc_seconds_isr,					// 83 RTC Seconds interrupt
	pit0_isr,							// 84 PIT Channel 0
	pit1_isr,							// 85 PIT Channel 1
	pit2_isr,							// 86 PIT Channel 2
	pit3_isr,							// 87 PIT Channel 3
	pdb_isr,							// 88 PDB Programmable Delay Block
	usb_isr,							// 89 USB OTG
	usb_charge_isr,						// 90 USB Charger Detect
	unused_isr,						// 91 --
	unused_isr,						// 92 --
	unused_isr,						// 93 --
	unused_isr,						// 94 --
	unused_isr,						// 95 --
	unused_isr,						// 96 --
	dac0_isr,							// 97 DAC0
	unused_isr,						// 98 --
	tsi0_isr,							// 99 TSI0
	mcg_isr,							// 100 MCG
	lptmr_isr,							// 101 Low Power Timer
	unused_isr,						// 102 --
	porta_isr,							// 103 Pin detect (Port A)
	portb_isr,							// 104 Pin detect (Port B)
	portc_isr,							// 105 Pin detect (Port C)
	portd_isr,							// 106 Pin detect (Port D)
	porte_isr,							// 107 Pin detect (Port E)
	unused_isr,						// 108 --
	unused_isr,						// 109 --
	software_isr,						// 110 Software interrupt
};


__attribute__ ((section(".flashconfig"), used)) const uint8_t flashconfigbytes[16]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF };

__attribute__ ((section(".startup"))) void ResetHandler(void)
{
	uint32_t *src=&_etext;
	uint32_t *dest=&_sdata;
	unsigned int i;

	WDOG_UNLOCK=0xC520;
	WDOG_UNLOCK=0xD928;
	__asm__ volatile ("nop");
	__asm__ volatile ("nop");
	WDOG_STCTRLH=WDOG_STCTRLH_ALLOWUPDATE_MASK;

	SIM_SCGC3=SIM_SCGC3_ADC1_MASK|SIM_SCGC3_FTM2_MASK;
	SIM_SCGC5=0x00043F82;
	SIM_SCGC6=SIM_SCGC6_RTC_MASK|SIM_SCGC6_FTM0_MASK|SIM_SCGC6_FTM1_MASK|SIM_SCGC6_ADC0_MASK|SIM_SCGC6_FTFL_MASK;

	if (!(RTC_CR & RTC_CR_OSCE_MASK)) {
		RTC_SR = 0;
		RTC_CR = RTC_CR_SC16P_MASK | RTC_CR_SC4P_MASK | RTC_CR_OSCE_MASK;
	}

	if(PMC_REGSC&PMC_REGSC_ACKISO_MASK)
		PMC_REGSC|=PMC_REGSC_ACKISO_MASK;

	SMC_PMPROT=SMC_PMPROT_AVLP_MASK|SMC_PMPROT_ALLS_MASK|SMC_PMPROT_AVLLS_MASK;

	while(dest<&_edata)
		*dest++=*src++;

	dest=&_sbss;

	while(dest<&_ebss)
		*dest++=0;

	for(i=0;i<111;i++)
		_VectorsRam[i]=_VectorsFlash[i];

	for(i=0;i<95;i++)
		(*((volatile uint8_t *)0xE000E400+i)=128);

	SCB_VTOR=(uint32_t)_VectorsRam;

	OSC_CR=OSC_CR_SC8P_MASK|OSC_CR_SC2P_MASK|OSC_CR_ERCLKEN_MASK;

	MCG_C2&=~(MCG_C2_RANGE0_MASK|MCG_C2_HGO0_MASK|MCG_C2_EREFS0_MASK);
	MCG_C2|=(MCG_C2_RANGE0(2)|(1<<MCG_C2_EREFS0_SHIFT));

	MCG_C1&=~(MCG_C1_CLKS_MASK|MCG_C1_FRDIV_MASK|MCG_C1_IREFS_MASK);
	MCG_C1|=MCG_C1_CLKS(2)|MCG_C1_FRDIV(4);

	while((MCG_S&MCG_S_OSCINIT0_MASK)==0);
	while(((MCG_S&MCG_S_CLKST_MASK)>>MCG_S_CLKST_SHIFT)!=0x2);

	MCG_C6|=MCG_C6_CME0_MASK;

	MCG_C5&=~MCG_C5_PRDIV0_MASK;
	MCG_C5|=MCG_C5_PRDIV0(5);
//	MCG_C5|=MCG_C5_PRDIV0(7);

	MCG_C6&=~MCG_C6_VDIV0_MASK;
	MCG_C6|=MCG_C6_PLLS_MASK|MCG_C6_VDIV0(3);
//	MCG_C6|=MCG_C6_PLLS_MASK|MCG_C6_VDIV0(12);

	while(!(MCG_S&MCG_S_PLLST_MASK));
	while(!(MCG_S&MCG_S_LOCK0_MASK));

	MCG_C1&=~MCG_C1_CLKS_MASK;

	while(((MCG_S&MCG_S_CLKST_MASK)>>MCG_S_CLKST_SHIFT)!=0x3);

	SIM_CLKDIV1=SIM_CLKDIV1_OUTDIV1(0)|SIM_CLKDIV1_OUTDIV2(1)|SIM_CLKDIV1_OUTDIV4(2);
	SIM_CLKDIV2=SIM_CLKDIV2_USBDIV(2)|SIM_CLKDIV2_USBFRAC_MASK;

	MCG_C1=MCG_C1_CLKS(0)|MCG_C1_FRDIV(4);
	while((MCG_S&MCG_S_CLKST_MASK)!=MCG_S_CLKST(3));

	SIM_SOPT2=SIM_SOPT2_USBSRC_MASK|SIM_SOPT2_PLLFLLSEL_MASK|SIM_SOPT2_TRACECLKSEL_MASK|SIM_SOPT2_CLKOUTSEL(6);

	SYST_RVR=(72000000/1000)-1;
	SYST_CSR=SysTick_CSR_CLKSOURCE_MASK|SysTick_CSR_TICKINT_MASK|SysTick_CSR_ENABLE_MASK;

	__asm__ volatile("CPSIE i");

	main();

	while(1);
}

char *__brkval=(char *)&_ebss;

void *_sbrk(int incr)
{
	char *prev=__brkval;

	__brkval+=incr;

	return prev;
}

__attribute__((weak)) int _read(int file, char *ptr, int len)
{
	return 0;
}

__attribute__((weak)) int _close(int fd)
{
	return -1;
}

#include <sys/stat.h>

__attribute__((weak)) int _fstat(int fd, struct stat *st)
{
	st->st_mode=S_IFCHR;

	return 0;
}

__attribute__((weak)) int _isatty(int fd)
{
	return 1;
}

__attribute__((weak)) int _lseek(int fd, long long offset, int whence)
{
	return -1;
}

__attribute__((weak)) void _exit(int status)
{
	while(1);
}

__attribute__((weak)) void __cxa_pure_virtual()
{
	while(1);
}

__attribute__((weak)) int __cxa_guard_acquire (char *g) 
{
	return !(*g);
}

__attribute__((weak)) void __cxa_guard_release(char *g)
{
	*g=1;
}
