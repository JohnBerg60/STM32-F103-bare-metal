TARGET = firmware
BUILDDIR = bin

# Define the linker script location and chip architecture.
LDSCRIPT = FLASH.ld
MCU  = cortex-m3

OBJS  = $(patsubst %.s, $(BUILDDIR)/%.o, $(wildcard *.s))
OBJS += $(patsubst %.c, $(BUILDDIR)/%.o, $(wildcard *.c))

# Toolchain definitions (ARM bare metal defaults)
TOOL=arm-none-eabi-

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
	rm -fR $(BUILDDIR)

dump:
	$(TOOL)objdump -ht $(BUILDDIR)/$(TARGET).elf

info:
	$(TOOL)size $(BUILDDIR)/$(TARGET).elf

debug:
	@echo $(OBJS)