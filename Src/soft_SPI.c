#include "soft_SPI.h"
#include "main.h"

void _spi_init(void)
{
	HAL_GPIO_WritePin(NRF_CS_GPIO_Port, NRF_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NRF_MOSI_GPIO_Port, NRF_MOSI_Pin, GPIO_PIN_RESET);
}

void _spi_start(void)
{
   HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(NRF_CS_GPIO_Port, NRF_CS_Pin, GPIO_PIN_RESET);
}

void _spi_stop(void)
{
   HAL_GPIO_WritePin(NRF_CS_GPIO_Port, NRF_CS_Pin, GPIO_PIN_SET);
   HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_RESET);
}

void _spi_sendbyte(uint8_t d)
{
   for(uint8_t i = 0; i < 8; i++)
   {
      if(d & 0x80)
      {
         HAL_GPIO_WritePin(NRF_MOSI_GPIO_Port, NRF_MOSI_Pin, GPIO_PIN_SET);
      }
      else
      {
         HAL_GPIO_WritePin(NRF_MOSI_GPIO_Port, NRF_MOSI_Pin, GPIO_PIN_RESET);
      }
      HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_SET);
      d <<= 1;
      __nop();
      HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_RESET);
   }
}

uint8_t _spi_readbyte(void)
{
   uint8_t i, spiReadData = 0;
 
   for(i = 0; i < 8; i++)
   {
      spiReadData <<= 1;
      HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_SET);
      __nop();
      if(HAL_GPIO_ReadPin(NRF_MISO_GPIO_Port, NRF_MISO_Pin))
			{
				spiReadData |= 0x01;
			}
      __nop();
      HAL_GPIO_WritePin(NRF_SCK_GPIO_Port, NRF_SCK_Pin, GPIO_PIN_RESET);
   }
   return spiReadData;
}

void _spi_send(uint8_t* buff, uint16_t len)
{
	for(uint8_t cnt = 0; cnt < len; cnt++)
	{
		_spi_sendbyte(*buff);
		buff++;
	}
}

void _spi_read(uint8_t* buff, uint16_t len)
{
	for(uint8_t cnt = 0; cnt < len; cnt++)
	{
		*buff = _spi_readbyte();
		buff++;
	}
}
