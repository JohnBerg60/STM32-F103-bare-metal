MEMORY
{
    ram (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K
    flash (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
}


SECTIONS 
{
    .vectors : { *(.vectors) } > flash
    .text    : { *(.text)    } > flash
    .rodata  : { *(.rodata)  } > flash
    .bss     : { *(.bss)     } > ram
}