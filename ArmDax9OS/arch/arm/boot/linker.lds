#include "arch/arm/include/const.h"
ENTRY(_start)
SECTIONS{
    DWARF_DEBUG
    .= TEXT_OFFSET;
    init :{

    }
    . = ALIGN(4K);
    init_end=ABSOLUTE(.);
    
}