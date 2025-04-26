#include "const.h"
ENTRY(_start)
SECTIONS{
    .= TEXT_OFFSET;
    img_start=ABSOLUTE(.);
    init :{
        ${init_objs}
    }
    . = ALIGN(SZ_16K);
    init_end=ABSOLUTE(.);
    .text KERNEL_VADDR + init_end: AT(init_end) {
        *(.text.*)
    }
    . = ALIGN(SZ_16K);

    .data:{
        *(.data.*)
    }

    . = ALIGN(SZ_16K);

    .rodata : {
        *(.rodata.*)
    }
    _edata=. - KERNEL_VADDR;
    _bss_start=. - KERNEL_VADDR;
    .bss : {
        *(.bss.*)
    }
    _bss_end=. - KERNEL_VADDR;
    . = ALIGN(SZ_16K);
    img_end=. - KERNEL_VADDR;
    /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}