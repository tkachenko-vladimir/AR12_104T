#define _EEPROM_SIZE_KBIT 32
#define _EEPROM_I2C_ADRESS 0xA0
#define _EEPROM_FREERTOS_IS_ENABLE 1

#include "main.h"
#include "i2c.h"
#include "eeprom24xx.h"
#if (_EEPROM_FREERTOS_IS_ENABLE==1)
#include "cmsis_os.h"
#endif

bool EEPROM24XX_IsConnected(void)
{
	if(i2c_device_ready(_EEPROM_I2C_ADRESS) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EEPROM24XX_Save(unsigned short int Address, void *data, size_t size_of_data)
{
	#if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
	if(size_of_data > 8)
		return false;
	#endif
	#if ((_EEPROM_SIZE_KBIT==4) || (_EEPROM_SIZE_KBIT==8) || (_EEPROM_SIZE_KBIT==16))
	if(size_of_data > 16)
		return false;
	#endif
	#if ((_EEPROM_SIZE_KBIT==32) || (_EEPROM_SIZE_KBIT==64) || (_EEPROM_SIZE_KBIT==128))
	if(size_of_data > 32)
		return false;
	#endif
	
	#if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
	if(HAL_I2C_Mem_Write(&_EEPROM24XX_I2C,_EEPROM_I2C_ADRESS,Address,I2C_MEMADD_SIZE_8BIT,(uint8_t*)data,size_of_data,100) == HAL_OK)
	#else
	if(i2c_mem_write2(_EEPROM_I2C_ADRESS, Address, (uint8_t*)data, size_of_data) == 1)
	#endif
	{
		#if (_EEPROM_FREERTOS_IS_ENABLE==1)
		osDelay(7);
		#else
		HAL_Delay(7);
		#endif
		return true;
	}
	else
		return false;		
}

bool EEPROM24XX_Load(unsigned short int Address, void *data, size_t size_of_data)
{
	#if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
	if(HAL_I2C_Mem_Read(&_EEPROM24XX_I2C,_EEPROM_I2C_ADRESS,Address,I2C_MEMADD_SIZE_8BIT,(uint8_t*)data,size_of_data,100) == HAL_OK)
	#else
	if(i2c_mem_read2(_EEPROM_I2C_ADRESS, Address, (uint8_t*)data, size_of_data) == 1)
	#endif
	{
		return true;
	}
	else
		return false;		
}
