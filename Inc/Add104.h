#include "stm32f1xx_hal.h"
#include "Iec10x.h"
#include "Iec104.h"

static int Linux_init(void);
void delay_ms(uint16_t ms);
static void *Linux_Malloc(unsigned char NumByte);
static void Linux_Free(void *pblock);
static void Linux_CloseLink(void);
uint8_t Linuxsend(int socketfd,char *buf, int len);
static uint32_t Linux_SetTime(PCP56Time2a_T time);
static uint32_t Linux_GetTime(PCP56Time2a_T time);
static int8_t Linux_GetStationState(uint16_t Addr, uint8_t DevType);
static float Linux_GetStaValue(uint16_t Addr, uint8_t DevType);
static uint16_t Linux_GetLinkAddr(void);
int8_t Linux_GetInfoNum(uint8_t *InfoNum, uint8_t DevType);
int8_t Linux_SetConfig(long Value, uint32_t addr);
int8_t Linux_SaveFirmware(uint8_t FirmLen, uint8_t *buf,uint32_t FirmwareType, uint32_t Iec10x_Update_SeekAddr);
int8_t Linux_CheckFirmware(uint32_t FirmwareType, uint32_t TotalLen);
int8_t Linux_UpdateFirmware(uint32_t FirmwareType);
int8_t Linux_BackoffFirmware(uint32_t FirmwareType);
void LinuxLock(void);
void LinuxUnlock();
extern IEC10X_T Linux;
