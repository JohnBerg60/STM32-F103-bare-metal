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
        *(.text.*)
        *(.rodata.*)
        _etext = .;
    } > flash

    .bss (NOLOAD) :
    {
        _sbss = . ;
        *(.bss .bss.*)
        *(COMMON)
        _ebss = . ;
    } > ram

    .data :
    {
        _sdata = .;
        *(.data*);
        _edata = .;
    } > ram AT >flash
}