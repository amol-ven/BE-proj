#####################################################
######            LPC1768  Makefile            ######
######                                         ######
######  Copyright (C) 2012, Amol Vengurlekar   ######
######                                         ######
######          All Rights Reserved            ######
######                                         ######
###### You are free to use this code as part   ######
###### of your own applications provided       ######
###### you keep this copyright notice intact   ######
###### and acknowledge its authorship with     ######
###### the words:                              ######
######                                         ######
###### "Contains software by Amol Vengurlekar" ######
######                                         ######
######                                         ######
#####################################################

#####################################################
##  This makefile uses the CodeSourcery toolchain  ##
##  which uses the GCC compiler.                   ##
##  To flash the microcontroller it uses the       ##
##  lpc21isp program.                              ##
#####################################################


#####################################################
############## GCC Compiler details #################
#####################################################

# The include folder in which the ARM LPC1768 header files and linker script are placed.
IDIR = /home/amol/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/arm-none-eabi/include/LPC17xx

# The compiler directory
CD = /home/amol/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin

# The C compiler
CC = arm-none-eabi-gcc

# The object copy command
ObjCopy = arm-none-eabi-objcopy

# The compiler flags for creating object files
FlagsForOBJ = -mcpu=cortex-m3 -mthumb -c

# The compiler flags for creating elf the file 
FlagsForELF = -mcpu=cortex-m3 -mthumb -Os -nostartfiles -Wl,-Map=lpc1768.map

# The filename of the main source file (without file extenssion)
TRGT = application

# The file extenssion of the main source file
 EXT = c
# EXT = cpp
# EXT = S
#####################################################
#####################################################




#####################################################
############## lpc21isp details #####################
#####################################################

# Serial Port
SerialPort = /dev/ttyUSB0

# lpc21isp baud rate
BaudRate = 115200

# Crystal Fequency (in KHz)
XTAL = 12000
#####################################################
#####################################################


all: 
	make obj #|| make finde
	make elf
	make hex
	make bin



obj: $(IDIR)/LPC17xx.h $(IDIR)/startup_LPC17xx.c $(IDIR)/core_cm3.c $(IDIR)/system_LPC17xx.c $(IDIR)/syscalls.c $(TRGT).$(EXT)
	 $(CD)/$(CC) $(FlagsForOBJ) $(IDIR)/startup_LPC17xx.c $(IDIR)/core_cm3.c $(IDIR)/system_LPC17xx.c $(IDIR)/syscalls.c $(IDIR)/LPC17xx.h $(TRGT).$(EXT)


elf: $(IDIR)/LPC17xx.ld startup_LPC17xx.o core_cm3.o system_LPC17xx.o syscalls.o $(TRGT).o
	 $(CD)/$(CC) $(FlagsForELF) -T$(IDIR)/LPC17xx.ld startup_LPC17xx.o core_cm3.o system_LPC17xx.o syscalls.o $(TRGT).o -o $(TRGT).elf

hex: $(TRGT).elf
	$(CD)/$(ObjCopy) -R .stack -R .bss -O ihex $(TRGT).elf $(TRGT).hex
#	 $(CD)/arm-none-eabi-objcopy -R .stack -R .bss -O ihex $(TRGT).elf $(TRGT).hex
	

bin: $(TRGT).elf
	$(CD)/$(ObjCopy) -O binary -j .text -j .data $(TRGT).elf $(TRGT).bin
clean:
	@-rm -f *.elf
	@-\
for D in "." "**"; do \
  rm -f $$D/*.o $$D/*.d $$D/*.lst $$D/*.dump $$D/*.map; \
done

elfhex: $(IDIR)/LPC17xx.ld startup_LPC17xx.o core_cm3.o system_LPC17xx.o $(TRGT).o
		make elf
		make hex
		

elfbin: $(IDIR)/LPC17xx.ld startup_LPC17xx.o core_cm3.o system_LPC17xx.o $(TRGT).o
		make elf
		make bin

dis:$(TRGT).elf
	$(CD)/arm-none-eabi-objdump -h -S $(TRGT).elf > $(TRGT).lst


flash:$(TRGT).hex
		lpc21isp $(TRGT).hex $(SerialPort) $(BaudRate) $(XTAL)

nuke: clean
	-rm -f *.hex *.bin
	

finde:finde_obj

finde_obj:$(IDIR)/LPC17xx.h $(IDIR)/startup_LPC17xx.c $(IDIR)/core_cm3.c $(IDIR)/system_LPC17xx.c $(IDIR)/syscalls.c $(TRGT).$(EXT)
	 $(CD)/$(CC) $(FlagsForOBJ) $(IDIR)/startup_LPC17xx.c $(IDIR)/core_cm3.c $(IDIR)/system_LPC17xx.c $(IDIR)/syscalls.c $(IDIR)/LPC17xx.h $(TRGT).$(EXT) 2>err_obj.log   || make grepe_obj

grepe_obj: err_obj.log
		grep --color 'error' err_obj.log  &&  grep --color 'note' err_obj.log
