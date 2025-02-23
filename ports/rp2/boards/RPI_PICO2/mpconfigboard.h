// Board and hardware specific configuration
#define MICROPY_HW_BOARD_NAME                   "Raspberry Pi Pico2"
#define MICROPY_HW_FLASH_STORAGE_BYTES          (PICO_FLASH_SIZE_BYTES - 1024 * 1024)
#define MICROPY_HW_ENABLE_UART_REPL             (1)
#define MICROPY_MICROPY_RV32_UNCOMPRESSED       (1)
//#define MICROPY_HW_USB_CDC                      (0)
#define MICROPY_HW_ENABLE_USBDEV                (0)
