include ../../py/mkenv.mk



# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk

BASE_DIR=$(TOP)/lib/bonfire-software

include $(TOP)/lib/bonfire-software/bonfire_defs.Makefile.include


CROSS_COMPILE ?= $(TARGET_PREFIX)-


INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += $(INCLUDES)
#INC += $(TOP)/lib/bonfire-software/gdb-stub


DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py

CFLAGS += $(INC) $(TARGET_CFLAGS)
LDFLAGS += $(TARGET_LDFLAGS_SYSCALL)

CSUPEROPT = -Os # save some code space

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


MICROPY_ROM_TEXT_COMPRESSION = 0

LIBS += -lm

SRC_C = \
	main.c \
	uart_core.c \
	shared/libc/printf.c \
	shared/readline/readline.c \
	shared/runtime/pyexec.c \
	shared/runtime/stdout_helpers.c \
	lib/bonfire-software/gdb-stub/console.c \
	lib/bonfire-software/gdb-stub/riscv-gdb-stub.c \
	lib/bonfire-software/gdb-stub/trap.c \
	lib/bonfire-software/gdb-stub/gdb_interface.c \
	$(BUILD)/_frozen_mpy.c

SRC_S = \
	lib/bonfire-software/gdb-stub/gdb_traps.S \

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c



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
