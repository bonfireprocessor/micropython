#include <stdint.h>
#include "mpconfigboard.h"


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
#define MICROPY_VFS                             (1)
#define MICROPY_VFS_LFS2                        (1)
#define MICROPY_READER_VFS              (1)
#define MICROPY_ENABLE_FINALISER (1)
#define MICROPY_PY_UOS (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)
//#define MICROPY_VFS_FAT                         (1)

#define MICROPY_ALLOC_PATH_MAX            (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

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

#define MICROPY_BEGIN_ATOMIC_SECTION()     mp_hal_disable_irq()
#define MICROPY_END_ATOMIC_SECTION(state)  mp_hal_enable_irq(state)


#define MP_STATE_PORT MP_STATE_VM
