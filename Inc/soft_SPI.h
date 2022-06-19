#include "main.h"

void _spi_init(void);
void _spi_start(void);
void _spi_stop(void);
void _spi_sendbyte(uint8_t d);
uint8_t _spi_readbyte(void);
void _spi_send(uint8_t* buff, uint16_t len);
void _spi_read(uint8_t* buff, uint16_t len);
