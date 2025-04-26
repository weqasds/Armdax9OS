/*
   MIT License

   Copyright (c) 2018 Sergey Matyukevich

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#include <common/asm.h>
#include <arch/arm/machine/registers.h>
ASM_FUNC_BEGIN(switch_elx_to_el1)

ASM_FUNC_END(switch_elx_to_el1)

ASM_FUNC_BEGIN(early_put32)
    str w1, [x0]
    ret
ASM_FUNC_END(early_put32)

ASM_FUNC_BEGIN(early_get32)
    ldr w0, [x0]
    ret
ASM_FUNC_END(early_get32)

ASM_FUNC_BEGIN(delay)
    subs x0, x0, #1
    bne delay
    ret
ASM_FUNC_END(delay)
