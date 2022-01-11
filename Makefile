TARGET = firmware
BUILDDIR = bin
PROJECT = cmsis

# Define chip architecture.
MCU  = cortex-m3

# things that can/should be set in the included Makefile
INCLUDES = 
DEFINES = 
LDSCRIPT = 
LDFLAGS = -nostdlib
CCFLAGS =

-include $(PROJECT)/Makefile

OBJS  = $(patsubst $(PROJECT)/%.s, $(BUILDDIR)/%.o, $(wildcard $(PROJECT)/*.s))
OBJS += $(patsubst $(PROJECT)/%.c, $(BUILDDIR)/%.o, $(wildcard $(PROJECT)/*.c))
OBJS += $(patsubst $(PROJECT)/%.cpp, $(BUILDDIR)/%.o, $(wildcard $(PROJECT)/*.cpp))

vpath %.s $(PROJECT)
vpath %.c $(PROJECT)
vpath %.cpp $(PROJECT)

# Toolchain definitions (ARM bare metal defaults)
TOOL=arm-none-eabi-

# Common flags, see: https://www.mikrocontroller.net/articles/ARM_GCC
CFCOMMON += -mcpu=$(MCU) -mthumb -Wall -fdata-sections -ffunction-sections

DEBUG = -O0 -g3 # runs as non optimized C code, with all debug info 
#DEBUG = -O1 -g # optimized, and minimal debug information

# C compilation directives
CFLAGS = $(CFCOMMON) $(DEBUG)
CPFLAGS = $(CFCOMMON) $(DEBUG) -fno-exceptions -fno-rtti

LDFLAGS += -Map=$(BUILDDIR)/$(TARGET).map --gc-sections

# default action: build all
all: $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).bin

# compiling assembler
$(BUILDDIR)/%.o: %.s Makefile
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(TOOL)gcc -x assembler-with-cpp -c $(CFLAGS) $< -o $@
	@echo ""

# compiling c
$(BUILDDIR)/%.o: %.c Makefile
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(TOOL)gcc -c $(CFLAGS) $(CCFLAGS) $(INCLUDES) $(DEFINES) $< -o $@
	@echo "" 

# linking
$(BUILDDIR)/$(TARGET).elf: $(OBJS) 
	$(TOOL)ld -T$(LDSCRIPT) $(LDFLAGS) -o $@ $^ 
	@echo ""

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	$(TOOL)objcopy -S -O ihex $< $@

$(BUILDDIR)/%.bin: $(BUILDDIR)/%.elf
	$(TOOL)objcopy -S -O binary $< $@	
	$(TOOL)size $< 

clean:
	clear
	rm -fr $(BUILDDIR)/*

dump:
	$(TOOL)objdump -ht $(BUILDDIR)/$(TARGET).elf

info:
	$(TOOL)size $(BUILDDIR)/$(TARGET).elf

flash: $(BUILDDIR)/$(TARGET).bin
	st-flash --reset write $(BUILDDIR)/$(TARGET).bin 0x8000000

device:
	st-info --probe
		
debug:
	@echo $(OBJS)