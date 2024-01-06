#ifndef __BONFIRE_MPHALPORT_H
#define __BONFIRE_MPHALPORT_H

#include "bonfire.h"
#include "stdint.h"
#include "mpconfigport.h"


// static inline mp_uint_t mp_hal_ticks_ms(void) {
//     return 0;
// }


static inline void mp_hal_set_interrupt_char(int c) {
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

void mp_hal_delay_ms(mp_uint_t delay);
void enable_irq(int state);
int  disable_irq();
uint64_t mp_hal_time_ns(void);

mp_uint_t mp_hal_ticks_us(void);
mp_uint_t mp_hal_ticks_cpu(void);
void mp_hal_delay_us(mp_uint_t delay);


void mp_hal_bonfire_setBaudRate(int b);

void mp_hal_reboot();

void _bonfire_poll_event_hook();

void  bonfire_init_interrupts();

#define MICROPY_EVENT_POLL_HOOK _bonfire_poll_event_hook();

// // Aliases 
// #define enable_irq(state) (mp_hal_enable_irq(state))
// #define disable_irq() (mp_hal_disable_irq())

#define raise_irq_pri(p) (1)
#define restore_irq_pri(state) 

#define __IO volatile

#endif
