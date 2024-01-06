#include <stdint.h>
#include "mpconfigboard.h"

#define MICROPY_PY_SYS_PLATFORM ("pyboard")

#define MICROPY_DEBUG_VERBOSE (0)

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)

#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)
#define MICROPY_USE_READLINE_HISTORY (1)

#ifndef MICROPY_READLINE_HISTORY_SIZE
#define MICROPY_READLINE_HISTORY_SIZE  (16)
#endif

#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
#define MICROPY_ENABLE_GC                 (1)
//#define MICROPY_HELPER_REPL               (1)
//#define MICROPY_MODULE_FROZEN_MPY         (1)
#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)
#define MICROPY_FLOAT_IMPL       (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_LONGINT_IMPL     (MICROPY_LONGINT_IMPL_MPZ)
//#define MICROPY_LONGINT_IMPL     (MICROPY_LONGINT_IMPL_NONE)
#define MICROPY_HELPER_REPL (1) 
#define MICROPY_REPL_AUTO_INDENT (1)
#define MICROPY_ENABLE_SOURCE_LINE (1)
#define MICROPY_ERROR_REPORTING (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_PY_BUILTINS_HELP  (1)
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY_MICROPYTHON_MEM_INFO  (1)
#define MICROPY_PY_MICROPYTHON_STACK_USE (1)
#define MICROPY_PY_MACHINE               (1)
#define MICROPY_PY_MACHINE_BARE_METAL_FUNCS (1)
#define MICROPY_PY_MACHINE_DISABLE_IRQ_ENABLE_IRQ (1)
#define MICROPY_PY_MACHINE_TIMER (1)
#define MICROPY_ENABLE_SCHEDULER (1)
#define MICROPY_VFS                             (1)
#define MICROPY_VFS_LFS2                        (1)
#define MICROPY_READER_VFS              (1)
#define MICROPY_ENABLE_FINALISER (1)
#define MICROPY_PY_UOS (1)
#define MICROPY_PY_UOS_UNAME (1)
//#define MICROPY_PY_UOS_SYSTEM (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)
//#define MICROPY_VFS_FAT                         (1)


#define MICROPY_ALLOC_PATH_MAX            (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define MICROPY_SOFT_TIMER_TICKS_MS uwTick

#include <limits.h>
#define SSIZE_MAX INT_MAX

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#ifndef MICROPY_HW_BOARD_NAME
#define MICROPY_HW_BOARD_NAME "Bonfire Generic"
#endif
#ifndef MICROPY_HW_MCU_NAME
#define MICROPY_HW_MCU_NAME "Bonfire CPU"
#endif

#ifndef MICROPY_HEAP_SIZE
#define MICROPY_HEAP_SIZE (16384*1024)
//#define MICROPY_STACK_SIZE (256*1024) // 256K Stack
#endif 

#include "mphalport.h"

#define MICROPY_BEGIN_ATOMIC_SECTION()     disable_irq()
#define MICROPY_END_ATOMIC_SECTION(state)  enable_irq(state)

// PENDSV Emulation
#define IRQ_PRI_PENDSV          (1) // Dummy value
#define MICROPY_PY_PENDSV_ENTER     uint32_t atomic_state = disable_irq();
#define MICROPY_PY_PENDSV_REENTER  atomic_state = disable_irq());
#define MICROPY_PY_PENDSV_EXIT     enable_irq(atomic_state);


#define MP_STATE_PORT MP_STATE_VM

// LFS Flash Address and size -- don't use the same lfs file system as the boot loeader 
#define MICROPY_HW_FLASH_STORAGE_BASE (FLASH_IMAGEBASE+MAX_FLASH_IMAGESIZE) // Place after Boot FLASH_IMAGEBASE
#define MICROPY_HW_FLASH_STORAGE_BYTES (FLASH_FSBASE-MICROPY_HW_FLASH_STORAGE_BASE) // Size is limited by boot loader fls

#define LFS2_MULTIVERSION

// Extra Machine Methods 


#define MICROPY_PY_MACHINE_EXTRA_GLOBALS  { MP_ROM_QSTR(MP_QSTR_Timer),               MP_ROM_PTR(&machine_timer_type) }
