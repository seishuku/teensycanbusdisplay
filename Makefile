PROJECT=main

OBJS = $(PROJECT).o vwtpkwp2k.o can.o spi.o startup.o

CFLAGS = -Wall -fno-common -mcpu=cortex-m4 -mthumb -O3 -I./include -I.
ASFLAGS = -mcpu=cortex-m4
LDFLAGS  = -mcpu=cortex-m4 -mthumb -nostartfiles -Tmk20dx256.ld

CC = arm-eabi-gcc
AS = arm-eabi-as
OBJCOPY = arm-eabi-objcopy

all:: $(PROJECT).hex

run: $(PROJECT).hex
	teensy_post_compile -file=$(PROJECT) -path=. -tools=\utils -board=TEENSY31 -reboot

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROJECT).elf

clean:
	rm -f $(OBJS) $(PROJECT).hex $(PROJECT).elf

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@    

.cpp.o :
	$(CC) $(CFLAGS) -c $< -o $@

.s.o :
	$(AS) $(ASFLAGS) -o $@ $<
