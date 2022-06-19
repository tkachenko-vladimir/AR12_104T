#define _DS3231_I2C_ADRESS 0xD0
#define _DS3231_FREERTOS_IS_ENABLE 1

#include "main.h"
#include "i2c.h"
#include "ds3231.h"
#if (_DS3231_FREERTOS_IS_ENABLE==1)
#include "cmsis_os.h"
#endif

const unsigned char MonthDayTab[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

bool IsLeapYear(void)
{
	return ((Year % 400) == 0) || (((Year % 4) == 0) && ((Year % 100) != 0));
}

unsigned char MonthDay(unsigned char month)
{
	if(IsLeapYear() && (month == 2))
	{
		return 29;
	}
	else
	{
		return MonthDayTab[month-1];
	}
}

unsigned char DayOfWeek(unsigned char day, unsigned char month, unsigned char year)
{
	year += 2000;
  uint16_t a = (14 - month) / 12;
  uint16_t y = year - a;
  uint16_t m = month + 12 * a - 2;

  return (7000 + (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12)) % 7;
}

bool SunTime(unsigned char hour, unsigned char day, unsigned char month, unsigned char year)
{
	unsigned char M, O;

	year += 2000;
	M = 31 - DayOfWeek(31, 3, Year);
	O = 31 - DayOfWeek(31, 10, Year);
	if((((Month > 3) && (Month < 10)) || ((Month == 3) && (Date >= M)) || ((Month == 10) && (Date <= O))) && (hour >= 3))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DS3231_IsConnected(void)
{
	if(i2c_device_ready(_DS3231_I2C_ADRESS) == 1)
	{
		return true;
	}
	else
	{
		return false;	
	}
}

bool DS3231_Save(unsigned short int Address, void *data, unsigned char DataCount)
{
	if(i2c_mem_write(_DS3231_I2C_ADRESS, Address, (uint8_t*)data, DataCount) == 1)
	{
		#if (_DS3231_FREERTOS_IS_ENABLE==1)
		osDelay(7);
		#else
		HAL_Delay(7);
		#endif
		return true;
	}
	else
		return false;		
}

bool DS3231_Load(unsigned short int Address, void *data)
{
	if(i2c_mem_read(_DS3231_I2C_ADRESS, Address, (uint8_t*)data, 7) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DS3231_Load_Temp(void *data)
{
	if(i2c_mem_read(_DS3231_I2C_ADRESS, 0x11, (uint8_t*)data, 2) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Save_Time_All(void)
{
	unsigned char Data[7];

	Data[0] = ((Second / 10) << 4) | (Second % 10);
	Data[1] = ((Minute / 10) << 4) | (Minute % 10);
	Data[2] = ((Hour / 10) << 4) | (Hour % 10);
	Data[4] = ((Date / 10) << 4) | (Date % 10);
	Data[5] = ((Month / 10) << 4) | (Month % 10);
	Data[6] = ((Year / 10) << 4) | (Year % 10);
	DS3231_Save(0, Data, 7);
}

void Save_Time(unsigned char adress)
{
	unsigned char Data[3];

	switch(adress)
	{
		case 0:
		{
			Data[0] = ((Second / 10) << 4) | (Second % 10);
			Data[1] = ((Minute / 10) << 4) | (Minute % 10);
			DS3231_Save(0, &Data, 2);
			break;
		}
		case 1:
		{
			Data[0] = ((Hour / 10) << 4) | (Hour % 10);
			Data[2] = ((Date / 10) << 4) | (Date % 10);
			DS3231_Save(2, &Data, 3);
			break;
		}
		case 2:
		{
			Data[0] = ((Month / 10) << 4) | (Month % 10);
			Data[1] = ((Year / 10) << 4) | (Year % 10);
			DS3231_Save(5, &Data, 2);
			break;
		}
	}
}
