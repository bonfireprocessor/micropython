#include "py/runtime.h"
#include "modbonfire.h"

#include "lib/bonfire-software/gdb-stub/riscv-gdb-stub.h"

extern void start_debugger();


STATIC mp_obj_t bonfire_info(void) {
    mp_printf(&mp_plat_print, "info about my port\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(bonfire_info_obj, bonfire_info);


STATIC mp_obj_t bonfire_start_debugger(void) {
    start_debugger();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(bonfire_start_debugger_obj, bonfire_start_debugger);


STATIC mp_obj_t bonfire_breakpoint(void) {
    gdb_breakpoint();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(bonfire_breakpoint_obj, bonfire_breakpoint);


STATIC const mp_rom_map_elem_t bonfire_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_bonfire) },
    { MP_ROM_QSTR(MP_QSTR_info), MP_ROM_PTR(&bonfire_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_start_debugger), MP_ROM_PTR(&bonfire_start_debugger_obj) },
    { MP_ROM_QSTR(MP_QSTR_breakpoint), MP_ROM_PTR(&bonfire_breakpoint_obj) },
    { MP_ROM_QSTR(MP_QSTR_Flash),  MP_ROM_PTR(&bonfire_flash_type) }
};
STATIC MP_DEFINE_CONST_DICT(bonfire_module_globals, bonfire_module_globals_table);

const mp_obj_module_t bonfire_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&bonfire_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_bonfire, bonfire_module);
