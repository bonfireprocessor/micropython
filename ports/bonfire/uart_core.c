#include <unistd.h>
#include "py/mpconfig.h"

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

  while (!(uartadr[UART_STATUS] & 0x01)); // Wait while receive buffer empty
  rx_data=uartadr[UART_RECV]; 
  return (int)rx_data;
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


void write_console(char *p)
{
   while (*p) {
    if (*p=='\n') writechar('\r');
    writechar(*p);
    p++;
  }

}