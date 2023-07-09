#include <unistd.h>
#include "py/mpconfig.h"
#include "py/runtime.h"
#include "py/stream.h"

#include <bonfire.h>


#define UART_TX 0
#define UART_RECV 0
#define UART_STATUS 1
#define UART_CONTROL 2


//volatile uint32_t *uartadr=(uint32_t *)UART_BASE;

#define uartadr ((volatile uint32_t*)UART_BASE)

// Receive single character
int mp_hal_stdin_rx_chr(void) {
uint32_t rx_data;

  while (!(uartadr[UART_STATUS] & 0x01)) {
        extern void mp_handle_pending(bool);
        mp_handle_pending(true); 
  }; // Wait while receive buffer empty
  rx_data=uartadr[UART_RECV]; 
  return (int)rx_data;
}


int mp_hal_stdio_poll(uint32_t poll_flags) {
    uint32_t ret = 0;
    if ((poll_flags & MP_STREAM_POLL_RD) && (uartadr[UART_STATUS] & 0x01)) {
        
        ret |= MP_STREAM_POLL_RD;
    }
    if ((poll_flags & MP_STREAM_POLL_WR) && (uartadr[UART_STATUS] & 0x2)) {
        ret |= MP_STREAM_POLL_WR;
    }
    return ret;

}


static inline void writechar(char c)
{

  while (!(uartadr[UART_STATUS] & 0x2)); // Wait while transmit buffer full
  uartadr[UART_TX]=(uint32_t)c;

}


// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
   for(int i=0;i<len;i++) {
      writechar(str[i]);
   }
}

static uint16_t l_divisor=0;

void _setDivisor(uint32_t divisor){

   l_divisor = divisor;
   uartadr[UART_CONTROL]= 0x030000L | (uint16_t)divisor; // Set Baudrate divisor and enable port and set extended mode
}

void setDivisor(uint32_t divisor)
{
    _setDivisor(divisor);
}

uint32_t getDivisor()
{
  return  uartadr[UART_CONTROL] & 0x0ffff ;
}

void mp_hal_bonfire_setBaudRate(int baudrate) {


   setDivisor(SYSCLK / baudrate -1);
}


