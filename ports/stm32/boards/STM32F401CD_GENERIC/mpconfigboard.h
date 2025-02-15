#define MICROPY_HW_BOARD_NAME       "STM32F401CD"
#define MICROPY_HW_MCU_NAME         "STM32F401xD"


#define MICROPY_CONFIG_ROM_LEVEL MICROPY_CONFIG_ROM_LEVEL_BASIC_FEATURES
#define MICROPY_ENABLE_FINALISER (1)
#define MICROPY_HELPER_REPL (1)
#define MICROPY_PY_SYS_PS1_PS2 (1)
#define MICROPY_PY_MACHINE_TIMER (1)
#define MICROPY_ENABLE_SCHEDULER (1)
#define MICROPY_KBD_EXCEPTION (1)
#define  MICROPY_PY_NETWORK (0)

#define  MICROPY_PY_IO (1)
#define MICROPY_PY_IO_IOBASE (1)
#define MICROPY_PY_SYS_STDFILES (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW (1)

#define MICROPY_PY_MICROPYTHON_MEM_INFO  (1)
#define MICROPY_PY_MICROPYTHON_STACK_USE (1)
//#define MICROPY_PY_MACHINE               (1)
#define MICROPY_PY_OS_SYNC (1)
#define MICROPY_PY_OS (1)


#define MICROPY_HW_HAS_SWITCH       (1)
#define MICROPY_HW_HAS_FLASH        (1)
#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_SERVO     (1)
#define MICROPY_HW_ENABLE_USB       (1)
#define MICROPY_HW_USB_FS (1)

// HSE is 25MHz
// Default source for the clock is HSE.
// For revisions of the board greater than C-01, HSE can be used as a
// clock source by removing the #define MICROPY_HW_CLK_USE_HSE line
#define MICROPY_HW_CLK_USE_HSI (0)

#if MICROPY_HW_CLK_USE_HSI
#define MICROPY_HW_CLK_PLLM (16)
#else
#define MICROPY_HW_CLK_PLLM (15)
#endif
#define MICROPY_HW_CLK_PLLN (144)
#define MICROPY_HW_CLK_PLLP (RCC_PLLP_DIV4)
#define MICROPY_HW_CLK_PLLQ (5)

// UART config
#define MICROPY_HW_UART2_TX     (pin_A2)
#define MICROPY_HW_UART2_RX     (pin_A3)
//#define MICROPY_HW_UART6_TX     (pin_C6)
//#define MICROPY_HW_UART6_RX     (pin_C7)
// UART 2 connects to the STM32F103 (STLINK) on the Nucleo board
// and this is exposed as a USB Serial port.
#define MICROPY_HW_UART_REPL        PYB_UART_2
#define MICROPY_HW_UART_REPL_BAUD   115200

// I2C buses
#define MICROPY_HW_I2C1_SCL (pin_B8)        // Arduino D15, pin 3 on CN10
#define MICROPY_HW_I2C1_SDA (pin_B9)        //         D14, pin 5 on CN10
#define MICROPY_HW_I2C2_SCL (pin_B10)       // Arduino D6,  pin 25 on CN10
#define MICROPY_HW_I2C2_SDA (pin_B3)        // Arduino D3,  pin 31 on CN10
#define MICROPY_HW_I2C3_SCL (pin_A8)        // Arduino D7,  pin 23 on CN10
#define MICROPY_HW_I2C3_SDA (pin_C9)        //              pin  1 on CN10

// SPI buses
#define MICROPY_HW_SPI1_NSS     (pin_A15)   //              pin 17 on CN7
#define MICROPY_HW_SPI1_SCK     (pin_A5)    // Arduino D13, pin 11 on CN10
#define MICROPY_HW_SPI1_MISO    (pin_A6)    // Arduino D12, pin 13 on CN10
#define MICROPY_HW_SPI1_MOSI    (pin_A7)    // Arduino D11, pin 15 on CN10

#define MICROPY_HW_SPI2_NSS     (pin_B12)   //              pin 16 on CN10
#define MICROPY_HW_SPI2_SCK     (pin_B13)   //              pin 30 on CN10
#define MICROPY_HW_SPI2_MISO    (pin_B14)   //              pin 28 on CN10
#define MICROPY_HW_SPI2_MOSI    (pin_B15)   //              pin 26 on CN10

#define MICROPY_HW_SPI3_NSS     (pin_A4)    // Arduino A2,  pin 32 on CN7
#define MICROPY_HW_SPI3_SCK     (pin_B3)    // Arduino D3,  pin 31 on CN10
#define MICROPY_HW_SPI3_MISO    (pin_B4)    // Arduino D5,  pin 27 on CN10
#define MICROPY_HW_SPI3_MOSI    (pin_B5)    // Arduino D4,  pin 29 on CN10

// USRSW is pulled low. Pressing the button makes the input go high.
#define MICROPY_HW_USRSW_PIN        (pin_A0)
#define MICROPY_HW_USRSW_PULL       (GPIO_NOPULL)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_FALLING)
#define MICROPY_HW_USRSW_PRESSED    (0)

// LEDs
#define MICROPY_HW_LED1             (pin_C13) // Green LD2 LED on Nucleo
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_high(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_low(pin))
