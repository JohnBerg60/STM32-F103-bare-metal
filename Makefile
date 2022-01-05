TARGET = firmware
BUILDDIR = bin

# Define the linker script location and chip architecture.
LDSCRIPT = FLASH.ld
MCU  = cortex-m3

OBJS  = $(patsubst %.s, $(BUILDDIR)/%.o, $(wildcard *.s))
OBJS += $(patsubst %.c, $(BUILDDIR)/%.o, $(wildcard *.c))

# Toolchain definitions (ARM bare metal defaults)
CC = arm-none-eabi-gcc
CX = arm-none-eabi-g++
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

# Common flags
CFCOMMON = -mcpu=$(MCU)

# C compilation directives
CFLAGS = $(CFCOMMON)

# Linker flags
LDFLAGS = -T $(LDSCRIPT)

INCLUDE  =

DEFINES = 

# default action: build all
all: $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).bin

# compiling c
$(BUILDDIR)/%.o: %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDE) $(DEFINES) $< -o $@
	@echo ""

# linking
$(BUILDDIR)/$(TARGET).elf: $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@
	@echo ""

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	$(OC) -S -O ihex $< $@

$(BUILDDIR)/%.bin: $(BUILDDIR)/%.elf
	$(OC)  -S -O binary $< $@	
	$(OS) $< 

clean:
	rm -fR $(BUILDDIR)

dump:
	$(OD) -ht $(BUILDDIR)/$(TARGET).elf

debug:
	@echo $(OBJS)