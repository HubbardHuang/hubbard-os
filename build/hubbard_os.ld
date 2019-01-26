ENTRY(start)

SECTIONS
{
    . = 0x100000;
    PROVIDE( start_address = . );
    .text :
    {
        *(.text)
        . = ALIGN(4096);
    }
    .data :
    {
        *(.data)
        . = ALIGN(4096);
    }
    .rodata :
    {
        *(.rodata)
        . = ALIGN(4096);
    }
    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
    }
    .stab :
    {
        *(.stab)
        . = ALIGN(4096);
    }
    .stabstr :
    {
        *(.stabstr)
        . = ALIGN(4096);
    }
    PROVIDE( end_address = . );

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}