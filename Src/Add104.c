#include <stdlib.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "stdbool.h"
#include "Iec10x.h"
#include "Iec104.h"
#include "socket.h"

#define DHCP_SOCKET     0
#define IEC104_SOCKET     1

static int Linux_init(void)
{
	return 0;
}

void delay_ms(uint16_t ms)
{
	osDelay(ms);
}

static void *Linux_Malloc(unsigned char NumByte)
{
	return malloc(NumByte);
}

static void Linux_Free(void *pblock)
{
	free(pblock);
}

static void Linux_CloseLink(void)
{
//	GPRSFlag = SYSTEM_FLAG_GPRS_CLOSE;
	IEC104_STATE_FLAG_INIT = IEC104_FLAG_CLOSED;
}

uint8_t Linuxsend(int socketfd,char *buf, int len)
{
//	write(socketfd, buf, len))
	if(-1 == send(IEC104_SOCKET, (uint8_t*)buf, len))
	{
		LOG("-%s-,Send error \n",__FUNCTION__);
    return RET_ERROR;
  }
  printf("Send Ok!\r\n");
  return RET_SUCESS;
}

static uint32_t Linux_SetTime(PCP56Time2a_T time)
{
/*
	RTC_TimeTypeDef systemTime = {0};
	RTC_DateTypeDef systemDate = {0};
	
	systemTime.Minutes = time->_min._minutes;
	systemTime.Hours = time->_hour._hours;
	systemDate.WeekDay = time->_day._dayofweek;
	systemDate.Date = time->_day._dayofmonth;
	systemDate.Month = time->_month._month;
	systemDate.Year = time->_year._year;
	HAL_RTC_SetTime(&hrtc, &systemTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &systemDate, RTC_FORMAT_BIN);
*/
	return RET_SUCESS;
}

static uint32_t Linux_GetTime(PCP56Time2a_T time)
{
//	RTC_TimeTypeDef systemTime = {0};
//	RTC_DateTypeDef systemDate = {0};
	
//	HAL_RTC_GetTime(&hrtc, &systemTime, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &systemDate, RTC_FORMAT_BIN);
/*
	time->_milliseconds = systemTime.Seconds * 1000;
	time->_min._minutes = systemTime.Minutes;
	time->_hour._hours = systemTime.Hours;
	time->_day._dayofweek = systemDate.WeekDay;
	time->_day._dayofmonth = systemDate.Date;
	time->_month._month = systemDate.Month;
	time->_year._year = systemDate.Year;
*/	
	time->_milliseconds = 0;
	time->_min._minutes = 0;
	time->_hour._hours = 0;
	time->_day._dayofweek = 0;
	time->_day._dayofmonth = 0;
	time->_month._month = 0;
	time->_year._year = 0;
	return RET_SUCESS;
}

static int8_t Linux_GetStationState(uint16_t Addr, uint8_t DevType)
{//?
	switch(DevType)
	{
		case ENDDEVICE_TYPE_HXTM:
			if(Addr == 0)
				return 0;
			if(Addr == 1)
				return 0;
			break;
		case ENDDEVICE_TYPE_HXGF:
			if(Addr == 0)
				return 0;
			if(Addr == 1)
				return 0;
			break;
		default:
			return 0;
	}
	return 0;
}

static float Linux_GetStaValue(uint16_t Addr, uint8_t DevType)
{//?
	switch(DevType)
	{
		case ENDDEVICE_TYPE_HXTM:
			if(Addr == 0)
				return 0;
			if(Addr == 1)
				return 0;
			break;
		case ENDDEVICE_TYPE_HXGF:
			if(Addr == 0)
				return 0;
			if(Addr == 1)
				return 0;
			break;
		default:
			return 0;
	}
	return 0;
}

static uint16_t Linux_GetLinkAddr(void)
{
	return 0x0001;
}

int8_t Linux_GetInfoNum(uint8_t *InfoNum, uint8_t DevType)
{//?
	switch(DevType)
	{
		case ENDDEVICE_TYPE_HXTM:
			*InfoNum = 2;
			break;
		case ENDDEVICE_TYPE_HXGF:
			*InfoNum = 2;
			break;
		default:
			*InfoNum = 2;
	}
	return 1;
}

int8_t Linux_SetConfig(long Value, uint32_t addr)
{//?
	return 1;
}

int8_t Linux_SaveFirmware(uint8_t FirmLen, uint8_t *buf,uint32_t FirmwareType, uint32_t Iec10x_Update_SeekAddr)
{//?
	return 1;
}

int8_t Linux_CheckFirmware(uint32_t FirmwareType, uint32_t TotalLen)
{//?
	return 1;
}

int8_t Linux_UpdateFirmware(uint32_t FirmwareType)
{//?
	return 1;
}

int8_t Linux_BackoffFirmware(uint32_t FirmwareType)
{//?
	return 1;
}

void LinuxLock(void)
{
//	pthread_mutex_lock(&mutex);
}
void LinuxUnlock()
{
//	pthread_mutex_unlock(&mutex);
}

static IEC10X_T Linux = {
    "Linux",
    Linux_init,
    delay_ms,
    Linux_CloseLink,
    Linux_Malloc,
    Linux_Free,
    IEC10X_PrioEnQueue,
    IEC10X_PrioDeQueue,
    IEC10X_PrioFindQueueHead,
    IEC10X_HighestPrio,
    IEC10X_PrioInitQueue,
    IEC10X_Prio_ClearQueue,
    Linuxsend,
    Linux_SetTime,
    Linux_GetTime,
    Linux_GetStationState,
    Linux_GetStaValue,
    Linux_GetLinkAddr,
    Linux_GetInfoNum,
    Linux_SetConfig,
    Linux_SaveFirmware,
    Linux_CheckFirmware,
    Linux_UpdateFirmware,
    Linux_BackoffFirmware,
#ifdef IEC10XLOCK
    LinuxLock,
    LinuxUnlock
#endif
};
