#ifndef __BONFIRE_MPHALPORT_H
#define __BONFIRE_MPHALPORT_H

#include "bonfire.h"
#include "stdint.h"


static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}


static inline void mp_hal_set_interrupt_char(char c) {
}

static inline long mp_hal_get_cpu_freq() {
    return SYSCLK;
}



static inline uint64_t mp_hal_sys_raw_read()
{
#if __riscv_xlen == 32
  while (1) {
    uint32_t hi = read_csr(mcycleh);
    uint32_t lo = read_csr(mcycle);
    if (hi == read_csr(mcycleh))
      return ((uint64_t)hi << 32) | lo;
  }
#else
  return read_csr(mcycle);
#endif

}

static inline void mp_hal_delay_ms(mp_uint_t delay) {
   uint64_t wait_cylces = (SYSCLK /1000 * delay);   
   uint64_t endcnt  = mp_hal_sys_raw_read() + wait_cylces;

   while (mp_hal_sys_raw_read() < endcnt) {
    ;
   }
}

static inline void mp_hal_enable_irq(int state) {
    
    if (state)
        set_csr(mstatus,MSTATUS_MIE); // Global Interrupt Enable
    else
        clear_csr(mstatus,MSTATUS_MIE); // Global Interrupt Diable
}

static inline int mp_hal_disable_irq() {

     uint32_t m = read_csr(mstatus);
     clear_csr(mstatus,MSTATUS_MIE);
     return  (m & MSTATUS_MIE); // Return old status
}





#endif