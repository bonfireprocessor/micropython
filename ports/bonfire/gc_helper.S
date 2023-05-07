.section .text

.global rv32_gc_helper_get_regs


#include "bits.h"
#include "platform.h"


rv32_gc_helper_get_regs:
    // a0 contains pointer to register readBuffer

    STORE  s0,0*REGBYTES(a0)
    STORE  s1,1*REGBYTES(a0)
    STORE  s2,2*REGBYTES(a0)
    STORE  s3,3*REGBYTES(a0)
    STORE  s4,4*REGBYTES(a0)
    STORE  s5,5*REGBYTES(a0)
    STORE  s6,6*REGBYTES(a0)
    STORE  s7,7*REGBYTES(a0)
    STORE  s8,8*REGBYTES(a0)
    STORE  s9,9*REGBYTES(a0)
    STORE  s10,10*REGBYTES(a0)
    STORE  s11,11*REGBYTES(a0)
    
    ret
