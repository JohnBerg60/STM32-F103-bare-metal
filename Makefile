TARGET = firmware
BUILDDIR = bin
PROJECT = minimum

# Define the linker script location and chip architecture.
LDSCRIPT = STM32-F103.ld
MCU  = cortex-m3

OBJS  = $(patsubst %.s, $(BUILDDIR)/%.o, $(wildcard *.s))
OBJS += $(patsubst $(PROJECT)/%.c, $(BUILDDIR)/%.o, $(wildcard $(PROJECT)/*.c))
OBJS += $(patsubst $(PROJECT)/%.cpp, $(BUILDDIR)/%.o, $(wildcard $(PROJECT)/*.cpp))

vpath %.s $(PROJECT)
vpath %.c $(PROJECT)
vpath %.cpp $(PROJECT)

# Toolchain definitions (ARM bare metal defaults)
TOOL=arm-none-eabi-

# Common flags
CFCOMMON = -mcpu=$(MCU) -Wall -mthumb

DEBUG = -Os -g3

# C compilation directives
CFLAGS = $(CFCOMMON) $(DEBUG)

# Linker flags
LDFLAGS = -T $(LDSCRIPT) 

INCLUDE  =

DEFINES = 

# default action: build all
all: $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).bin

# compiling c
$(BUILDDIR)/%.o: %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(TOOL)gcc -c $(CFLAGS) $(INCLUDE) $(DEFINES) $< -o $@
	@echo ""

# linking
$(BUILDDIR)/$(TARGET).elf: $(OBJS)
	$(TOOL)ld $^ $(LDFLAGS) -o $@
	@echo ""

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	$(TOOL)objcopy -S -O ihex $< $@

$(BUILDDIR)/%.bin: $(BUILDDIR)/%.elf
	$(TOOL)objcopy  -S -O binary $< $@	
	$(TOOL)size $< 

clean:
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