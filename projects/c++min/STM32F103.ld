/* idea: https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware */

MEMORY
{
	ram (rw)        : ORIGIN = 0x20000000, LENGTH = 20K
	flash (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
}

_estack = ORIGIN(ram) + LENGTH(ram);

SECTIONS 
{
	.text :
	{
		KEEP(*(.vectors .vectors.*))
		*(.text .text*)
		*(.rodata .rodata*)
		_etext = .; 
	} > flash

	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} > flash

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > flash
	
	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > flash

	.bss (NOLOAD) :
	{
		_sbss = . ;
		*(.bss .bss*)
		*(COMMON)
		_ebss = . ;
	} > ram

	.data :
	{
		_sdata = .;
		*(.data .data.*);
		_edata = .;
	} > ram AT >flash
}