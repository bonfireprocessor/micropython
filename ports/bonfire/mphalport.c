
#include "mphalport.h"


void mp_hal_delay_ms(mp_uint_t delay) {
   uint64_t wait_cylces = (SYSCLK /1000 * delay);   
   uint64_t endcnt  = mp_hal_sys_raw_read() + wait_cylces;

   while (mp_hal_sys_raw_read() < endcnt) {
    ;
   }
}


void mp_hal_enable_irq(int state) {
    
    if (state)
        set_csr(mstatus,MSTATUS_MIE); // Global Interrupt Enable
    else
        clear_csr(mstatus,MSTATUS_MIE); // Global Interrupt Diable
}


int mp_hal_disable_irq() {

     uint32_t m = read_csr(mstatus);
     clear_csr(mstatus,MSTATUS_MIE);
     return  (m & MSTATUS_MIE); // Return old status
}


void mp_hal_reboot()
{
    write_csr(mie,0); // Disable all interrupts
    clear_csr(mstatus,MSTATUS_MIE);
     // Jump to Firmware
    void (*sram_base)() = (void*)SRAM_BASE;
    sram_base();   
}

uint64_t mp_hal_time_ns(void) {
  return 0;
}  


// Newlib overload
void __wrap__exit(int n) {
    mp_hal_reboot();
}
