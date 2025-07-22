#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/builtin.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/stackctrl.h"
#include "py/mperrno.h"
#include "shared/runtime/pyexec.h"
#include "shared/runtime/gchelper.h"
#include "lib/bonfire-software/gdb-stub/riscv-gdb-stub.h"
#include  "lib/bonfire-software/gdb-stub/console.h"
#include "mphalport.h"
#include "pendsv.h"
#include "shared/runtime/softtimer.h"


#include <stdarg.h>

int  DEBUG_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}


#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif


void write_console(char *p);
 
extern uint32_t _endofheap;
extern uint32_t _end;

extern pyexec_mode_kind_t pyexec_mode_kind;

#define BAUDRATE (500000)

void start_debugger() {
   printf("Run with baudrate %d\n",BAUDRATE);
   gdb_setup_interface(BAUDRATE);
   gdb_initDebugger(1);
   gdb_breakpoint();
}


int main(int argc, char **argv) {

    #if BONFIRE_DEBUG_MAIN
    start_debugger();
    #endif

    int stack_dummy;
    void *stack_top = (void *)&stack_dummy;
  
    void * start_heap = (void*) &_end;

    #ifdef MICROPY_HEAP_SIZE
    void * end_heap = start_heap + MICROPY_HEAP_SIZE;
    #else
    void * end_heap = ((void*) stack_top) - MICROPY_STACK_SIZE;
    #endif 

soft_reset: 
    printf("Stack top %p\n",stack_top);
    mp_stack_set_top(stack_top);
    mp_stack_set_limit(stack_top  - end_heap);

    // GC init
    printf("Set heap  %p..%p\n",start_heap,end_heap);
    gc_init(start_heap,end_heap);

    mp_init();
    pendsv_init();
    bonfire_init_interrupts();

    #ifdef MICROPY_BOARD_FROZEN_BOOT_FILE
    pyexec_frozen_module(MICROPY_BOARD_FROZEN_BOOT_FILE, false);
    #endif 

    #if MICROPY_ENABLE_COMPILER
    #if MICROPY_REPL_EVENT_DRIVEN
   
    pyexec_event_repl_init();
    for (;;) {
        int c = mp_hal_stdin_rx_chr();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
    #else
    while(1) {
        int exitcode;
        if (pyexec_mode_kind==PYEXEC_MODE_FRIENDLY_REPL) {
            // mp_hal_bonfire_setBaudRate(500000);
            exitcode = pyexec_friendly_repl();
        } else {    
            //mp_hal_bonfire_setBaudRate(115200);
            exitcode = pyexec_raw_repl();
        }
         if (exitcode==PYEXEC_FORCED_EXIT) {            
            break; // Leave loop to prepare for Soft-Reboot
        }
    }    
    #endif
  
    #endif
    mp_printf(&mp_plat_print, "MPY: soft reboot\n");
    soft_timer_gc_mark_all();
    gc_sweep_all();
    soft_timer_deinit();
    mp_deinit();
    goto soft_reset;
    return 0;
}

#if MICROPY_ENABLE_GC


typedef struct  {
    uint32_t s_reg[12]; // Save S0..S11

} rv32_gc_helper_regs_t;


void rv32_gc_helper_get_regs(rv32_gc_helper_regs_t *regs); // Implemented in gc_helper.S

MP_NOINLINE void rv32_gc_helper_collect_regs_and_stack(void) {
    rv32_gc_helper_regs_t regs;
    rv32_gc_helper_get_regs(&regs);
    // GC stack (and regs because we captured them)
    void **regs_ptr = (void **)(void *)&regs;
    gc_collect_root(regs_ptr, ((uintptr_t)MP_STATE_THREAD(stack_top) - (uintptr_t)&regs)  / sizeof(uintptr_t));
}



void gc_collect(void) {
   
    gc_collect_start();
    rv32_gc_helper_collect_regs_and_stack();    
    soft_timer_gc_mark_all();
    gc_collect_end();
    
    //gc_dump_info(&mp_plat_print);
}
#endif





#if  !(MICROPY_VFS)

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_vfs_import_stat(const char *path)
{
    return MP_IMPORT_STAT_NO_EXIST;
}


static inline mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

#endif 

void nlr_jump_fail(void *val) {
    do_panic("nlr_jump_fail %lx\n",(uint32_t)val);
}

void NORETURN __fatal_error(const char *msg) {
    do_panic(msg);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif




#if MICROPY_MIN_USE_CORTEX_CPU

// this is a minimal IRQ and reset framework for any Cortex-M CPU

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) __attribute__((naked));
void Reset_Handler(void) {
    // set stack pointer
    __asm volatile ("ldr sp, =_estack");
    // copy .data section from flash to RAM
    for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }
    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }
    // jump to board initialisation
    void _start(void);
    _start();
}

void Default_Handler(void) {
    for (;;) {
    }
}

const uint32_t isr_vector[] __attribute__((section(".isr_vector"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&Default_Handler, // NMI_Handler
    (uint32_t)&Default_Handler, // HardFault_Handler
    (uint32_t)&Default_Handler, // MemManage_Handler
    (uint32_t)&Default_Handler, // BusFault_Handler
    (uint32_t)&Default_Handler, // UsageFault_Handler
    0,
    0,
    0,
    0,
    (uint32_t)&Default_Handler, // SVC_Handler
    (uint32_t)&Default_Handler, // DebugMon_Handler
    0,
    (uint32_t)&Default_Handler, // PendSV_Handler
    (uint32_t)&Default_Handler, // SysTick_Handler
};

void _start(void) {
    // when we get here: stack is initialised, bss is clear, data is copied

    // SCB->CCR: enable 8-byte stack alignment for IRQ handlers, in accord with EABI
    *((volatile uint32_t *)0xe000ed14) |= 1 << 9;

    // initialise the cpu and peripherals
    #if MICROPY_MIN_USE_STM32_MCU
    void stm32_init(void);
    stm32_init();
    #endif

    // now that we have a basic system up and running we can call main
    main(0, NULL);

    // we must not return
    for (;;) {
    }
}

#endif

