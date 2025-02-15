#include "bonfire.h"
#include "py/runtime.h"
#include "mphalport.h"
#include "bonfire_spi.h"


#if (!defined (NO_FLASH))


/*
-- registers:
-- base+0   -- chip select control; bit 0 is slave_cs
-- base+4   -- status register; bit 0 indicates "transmitter busy"
-- base+8   -- transmitter: write a byte here, starts SPI bus transaction
-- base+0x0C   -- receiver: last byte received (updated on each transation)
-- base+0x10   -- clock divider: SPI CLK is clk_i/2*(1+n) ie for 96MHz clock, divisor 0 is 48MHz, 2 is 24MHz, 3 is 12MHz etc
*/

#define SPI_CHIPSELECT  0
#define SPI_STATUS      1
#define SPI_TX          2
#define SPI_RX          3
#define SPI_DIVISOR     4



#define FLASH_MAN      0xc2
#define FLASH_DEV1     0x20
#define FLASH_DEV2     0x17





typedef uint8_t t_flashid[3];

static volatile uint32_t *spiflash = (void*)SPIFLASH_BASE;

static inline void spiflash_init()
{
   spiflash[SPI_DIVISOR]=1;

}

static inline void spiflash_select()
{
  spiflash[SPI_CHIPSELECT]=0x0fe;
}

static inline void spiflash_deslect()
{
    spiflash[SPI_CHIPSELECT]=0x0ff;
}


static inline void spi_tx(uint8_t txbyte)
{
   spiflash[SPI_TX]=txbyte;
}

static inline uint8_t spi_rx()
{
  spiflash[SPI_TX]=0; // send dummy byte
  return (uint8_t) (spiflash[SPI_RX] & 0x0ff);
}




void spiflash_getid(t_flashid *pflashid)
{
  spiflash_select();
  spi_tx(0x9f);      //identify/RDID command

  spi_tx(0);
  spi_tx(0);
  spi_tx(0);
  (*pflashid)[0]=spi_rx();
  (*pflashid)[1]=spi_rx();
  (*pflashid)[2]=spi_rx();

  spiflash_deslect();

}



// spiflash_driver  hal


int impl_spiflash_spi_txrx(spiflash_t *spi, const uint8_t *tx_data,
      uint32_t tx_len, uint8_t *rx_data, uint32_t rx_len) {
  int res = SPIFLASH_OK;
  int i;



  if (tx_len > 0) {
    // first transmit tx_len bytes from tx_data if needed
    for(i=0;i<tx_len;i++) spi_tx(tx_data[i]);
  }

  if (rx_len > 0) {
    // then receive rx_len bytes into rx_data if needed
    for(i=0;i<rx_len;i++) rx_data[i]=spi_rx();
  }

  return res;
}


void impl_spiflash_spi_cs(spiflash_t *spi, uint8_t cs) {
  if (cs) {
    // assert cs pin
    spiflash_select();
  } else {
    // de assert cs pin
   spiflash_deslect();
  }
}


void impl_spiflash_wait(spiflash_t *spi, uint32_t ms) {
  mp_hal_delay_ms(ms);
}

static const spiflash_hal_t my_spiflash_hal = {
  ._spiflash_spi_txrx = impl_spiflash_spi_txrx,
  ._spiflash_spi_cs = impl_spiflash_spi_cs,
  ._spiflash_wait = impl_spiflash_wait
};


#define SPIFLASH_CMD_TBL_64K \
  (spiflash_cmd_tbl_t) { \
    .write_disable = 0x04, \
    .write_enable = 0x06, \
    .page_program = 0x02, \
    .read_data = 0x03, \
    .read_data_fast = 0x0b, \
    .write_sr = 0x01, \
    .read_sr = 0x05, \
    .block_erase_4 = 0x00, \
    .block_erase_8 = 0x00, \
    .block_erase_16 = 0x00, \
    .block_erase_32 = 0x00, \
    .block_erase_64 = 0xd8, \
    .chip_erase = 0xc7, \
    .device_id = 0x90, \
    .jedec_id = 0x9f, \
    .sr_busy_bit = 0x01, \
  }

#if (defined(NO_SUB_SECTOR_ERASE) && NO_SUB_SECTOR_ERASE==1 )
#pragma message "SPI Flash using SPIFLASH_CMD_TBL_64K"
const spiflash_cmd_tbl_t my_spiflash_cmds = SPIFLASH_CMD_TBL_64K;
#ifndef FLASH_ERASEBLOCK
#define FLASH_ERASEBLOCK 65536
#endif 
#else
const spiflash_cmd_tbl_t my_spiflash_cmds = SPIFLASH_CMD_TBL_STANDARD;
#ifndef FLASH_ERASEBLOCK
#define FLASH_ERASEBLOCK 4096
#endif 
#endif

const spiflash_config_t my_spiflash_config = {
  .sz = 1024*1024*8, // 8MB
  .page_sz = 256, // normally 256 byte pages
  .addr_sz = 3, // normally 3 byte addressing
  .addr_dummy_sz = 0, // using single line data, not quad or something
  .addr_endian = SPIFLASH_ENDIANNESS_BIG, // normally big endianess on addressing
  .sr_write_ms = 10,
  .page_program_ms = 2,
  .block_erase_4_ms = 100,
  .block_erase_8_ms = 0, // not supported
  .block_erase_16_ms = 0, // not supported
  
  .block_erase_32_ms = 0, // 175, 
  .block_erase_64_ms = 300,
  .chip_erase_ms = 30000
};

static spiflash_t spif;

spiflash_t* get_spiflash()
{
  return &spif;
}


spiflash_t* flash_init()
{
uint32_t  jedec_id;

   spiflash_init();
   //spiflash_test();
   SPIFLASH_init(&spif,
                &my_spiflash_config,
                &my_spiflash_cmds,
                &my_spiflash_hal,
                0,
                SPIFLASH_SYNCHRONOUS,
                NULL);

    SPIFLASH_read_jedec_id(&spif,&jedec_id);
    //printk("SPI Flash JEDEC ID: %x\n",jedec_id);
    return &spif;
}



#endif 

