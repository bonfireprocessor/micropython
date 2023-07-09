
#include "mphalport.h"
#include "bonfire.h"
#include "lib/bonfire-software/gdb-stub/trapframe.h"
#include "lib/bonfire-software/gdb-stub/riscv-gdb-stub.h"

#include "encoding.h"

#include "py/runtime.h"
#include "systick.h"
#include "pendsv.h"

volatile uint32_t *pmtime = (uint32_t*)MTIME_BASE; // Pointer to memory mapped RISC-V Timer registers

uint32_t tick_interval=0;

 extern void mp_handle_pending(bool);


void enable_irq(int state) {
    
    if (state)
        set_csr(mstatus,MSTATUS_MIE); // Global Interrupt Enable
    else
        clear_csr(mstatus,MSTATUS_MIE); // Global Interrupt Diable
}


int disable_irq() {

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

mp_uint_t  mp_hal_ticks_us(void)
{
    
    return mp_hal_sys_raw_read() / (SYSCLK/1000000) ;
}


mp_uint_t mp_hal_ticks_cpu(void)
{
  return mp_hal_sys_raw_read();
}


void mp_hal_delay_us(mp_uint_t delay)
{
  uint64_t wait_cylces = (SYSCLK / 1000000) * delay;   
  uint64_t endcnt  = mp_hal_sys_raw_read() + wait_cylces;

  while (mp_hal_sys_raw_read() < endcnt) {
    mp_handle_pending(true);
  }

}

void mp_hal_delay_ms(mp_uint_t delay) {
  uint64_t wait_cylces = (SYSCLK /1000 * delay);   
  uint64_t endcnt  = mp_hal_sys_raw_read() + wait_cylces;

  while (mp_hal_sys_raw_read() < endcnt) {
     mp_handle_pending(true);
  }
}



void _bonfire_poll_event_hook() {

}



uint32_t bonfire_mtime_setinterval(uint32_t interval)
{
// Implementation for 32 Bit timer in Bonfire. Need to be adapted in case of a 64Bit Timer

   tick_interval=interval;

   if (interval >0) {
     pmtime[2]=pmtime[0]+interval;
     set_csr(mie,MIP_MTIP); // Enable Timer Interrupt
   } else {
     clear_csr(mie,MIP_MTIP); // Disable Timer Interrupt

   }
   return tick_interval;
}


// implemented in gdbstub 
extern void __trap();

void bonfire_init_interrupts() {
    write_csr(mtvec,__trap);
    bonfire_mtime_setinterval(SYSCLK/1000); // Systick every 1us
    enable_irq(1);
}

// Overload weak Trap Handler in gdb_stub

trapframe_t* trap_handler(trapframe_t *ptf)
{

    if (ptf->cause & 0x80000000) {
        switch (ptf->cause & 0x0ff) {
             case 0x07:               
               SysTick_Handler();
               pmtime[2]=pmtime[0]+tick_interval;
               break;
            //  case 16+6: // Local Interrupt 6 (lxp irq_i(7))
            //  case 16+5: // Local Interrupt 5 (lxp irq_i(6))
            //    uart_irq_handler(ptf->cause & 0x0ff);
            //    break;
   

   
            //  case 0x0b:
            //    ext_irq_handler();
            //    break;
   
             default:
                 mp_printf(&mp_plat_print,"Unexepted Interrupt cause %x\n",ptf->cause);
          }
          pendsv_emulation();  
          return ptf;
    }  else {
       return handle_exception(ptf);
    }
}