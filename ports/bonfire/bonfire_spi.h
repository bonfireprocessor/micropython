#ifndef MICROPY_INCLUDED_BONFIRE_BONFIRE_SPI_H
#define MICROPY_INCLUDED_BONFIRE_BONFIRE_SPI_H

#include <stdbool.h>
#include "lib/bonfire-software/bonfire-boot/spiflash_driver/src/spiflash.h"


spiflash_t* flash_init();
spiflash_t* get_spiflash();

#endif
