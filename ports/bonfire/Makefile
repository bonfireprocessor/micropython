BOARD ?= ARTY
BOARD_DIR ?= boards/$(BOARD)
ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)

include ../../py/mkenv.mk

include $(BOARD_DIR)/mpconfigboard.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 0


# include py core make definitions
include $(TOP)/py/py.mk

BASE_DIR=$(TOP)/lib/bonfire-software
include $(TOP)/lib/bonfire-software/bonfire_defs.Makefile.include


CROSS_COMPILE ?= $(TARGET_PREFIX)-


INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += $(INCLUDES)
INC += -I$(BOARD_DIR)
#INC += $(TOP)/lib/bonfire-software/gdb-stub


DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py

CSUPEROPT = -Os # save some code space - is used for some files in py.mk

CFLAGS += $(INC) $(TARGET_CFLAGS)
LDFLAGS += $(TARGET_LDFLAGS_SYSCALL)  -Wl,-wrap=_exit

CFLAGS += -DMICROPY_MODULE_FROZEN_MPY
CFLAGS += -DMICROPY_QSTR_EXTRA_POOL=mp_qstr_frozen_const_pool

ifneq ($(FROZEN_MANIFEST),)
$(info Frozen manifest found $(FROZEN_MANIFEST) )
CFLAGS += -DMICROPY_MODULE_FROZEN_STR
endif

# Tune for Debugging or Optimization
# CFLAGS += -g  # always include debug info in the ELF
# ifeq ($(DEBUG), 1)
# CFLAGS += -Og
# else
# CFLAGS += -Os -DNDEBUG
# CFLAGS += -fdata-sections -ffunction-sections
# endif

# Flags for optional C++ source code
CXXFLAGS += $(filter-out -std=c99,$(CFLAGS))

LIBS += -lm




SRC_C = \
	main.c \
	uart_core.c \
	shared/libc/printf.c \
	shared/readline/readline.c \
	shared/runtime/pyexec.c \
	shared/runtime/stdout_helpers.c \
	shared/runtime/sys_stdio_mphal.c \
	shared/runtime/softtimer.c \
	extmod/moduos.c \
	lib/bonfire-software/gdb-stub/console.c \
	lib/bonfire-software/gdb-stub/riscv-gdb-stub.c \
	lib/bonfire-software/gdb-stub/trap.c \
	lib/bonfire-software/gdb-stub/gdb_interface.c \
	lib/bonfire-software/bonfire-boot/spiflash_driver/src/spiflash.c \
	modbonfire.c \
	modmachine.c \
	mphalport.c \
	bonfire_flash.c \
	bonfire_spi.c \
	systick.c \
	pendsv.c \
	lib/littlefs/lfs2.c \
	lib/littlefs/lfs2_util.c \
	$(BUILD)/frozen_content.c



include $(TOP)/extmod/extmod.mk

SRC_C += $(SRC_EXTMOD_C)

SRC_S = \
	lib/bonfire-software/gdb-stub/gdb_traps.S \
	gc_helper.S

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c \
			shared/runtime/sys_stdio_mphal.c \
            modbonfire.c modmachine.c bonfire_flash.c \
			extmod/moduos.c

SRC_QSTR += $(SRC_VFS)


OBJ += $(PY_CORE_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.S=.o))


all: $(BUILD)/firmware.elf $(BUILD)/firmware.bin


$(BUILD)/_frozen_mpy.c: $(TOP)/tests/frozen/frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(TARGET_LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(TARGET_SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(TARGET_OBJCOPY) -S -O binary $<  $@
	$(TARGET_PREFIX)-objdump -S -d $< >$(basename $@).lst
	$(TARGET_PREFIX)-objdump -s $< >$(basename $@).dmp



include $(TOP)/py/mkrules.mk
