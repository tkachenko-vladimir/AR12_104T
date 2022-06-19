/* USER CODE BEGIN Header */
#define DHCP_SOCKET     0
#define IEC104_SOCKET     1

const unsigned short int _PO_VERSION = 3;
const unsigned char _SETTINGS_VER_H = 0xAA;
const unsigned char _SETTINGS_VER_L = 0xAB;

unsigned short int _SERIAL_NBR = 4;

float _K_IA = 0.09;
float _K_IC = 0.09;
float _K_UA = 0.261;
short int _B_IA = 0;
short int _B_IC = 0;
short int _B_UA = 0;

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "math.h"
#include "LED.h"
#include "eeprom24xx.h"
#include "ds3231.h"
#include "usbd_cdc_if.h"
#include "mb.h"
#include "i2c.h"
#include "soft_SPI.h"
#include "socket.h"
#include "Iec10x.h"
#include "Iec104.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osTimerId myTimer01Handle;
osTimerId myTimer02Handle;
/* USER CODE BEGIN PV */

uint8_t dhcp_buffer[1024];

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
                            .ip = {192, 168, 0, 10},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 0, 1},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_STATIC };

wiz_NetInfo net_info = {
	.mac  = { 0xEA, 0x11, 0x22, 0x33, 0x44, 0xEA },
  .dhcp = NETINFO_DHCP
};

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
    return RET_ERROR;
  }
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
	Hour = (unsigned short int)time->_hour._hours;
	Minute = (unsigned short int)time->_min._minutes;
	Second = (unsigned short int)time->_milliseconds / 1000;
	Date = (unsigned short int)time->_day._dayofmonth;
	Month = (unsigned short int)time->_month._month;
	Year = (unsigned short int)time->_year._year;
	Time_Save_f = true;
	Save_Settings_f = true;
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
	time->_milliseconds = Second * 1000;
	time->_min._minutes = Minute;
	time->_hour._hours = Hour;
	time->_day._dayofweek = 0;
	time->_day._dayofmonth = Date;
	time->_month._month = Month;
	time->_year._year = Year;
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
			{
				if(VV_State == 0)
				{
					return 0;
				}
				else
				{
					if(VV_State == 1)
					{
						return 1;
					}
					else
					{
						return 0x80;
					}
				}
			}
			if(Addr == 1)
			{
				if(!(Inputs_State & INPUT_DOOR))
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			if(Addr == 2)
				return 0;
			if(Addr == 3)
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
				return IrmsA;
			if(Addr == 1)
				return IrmsC;
			if(Addr == 2)
				return VrmsA;
			if(Addr == 3)
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
			*InfoNum = 4;
			break;
		default:
			*InfoNum = 2;
	}
	return 1;
}

int8_t Linux_SetConfig(long Value, uint32_t addr)
{//?
	switch(addr)
	{
		case 401:
			if(Value == 1)
			{
				Command_VV_Off = true;
				VV_Off_Sourse_tmp = 2;
			}
			if(Value == 2)
			{
				Command_VV_On = true;
				VV_Off_Sourse_tmp = 2;
			}
			break;
		case 402:
			if(Value <= 15)
			{
				LED_Brightness = (unsigned char)Value;
				Save_Settings_Del_f = true;
				LED_Init_f = true;
			}
			break;

		default:
			return 0;
	}
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

void W5500_Select(void)
{
    _spi_start();
}

void W5500_Unselect(void) 
{
    _spi_stop();
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) 
{
    _spi_read(buff, len);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) 
{
    _spi_send(buff, len);
}

uint8_t W5500_ReadByte(void) 
{
    uint8_t byte;
	byte = _spi_readbyte();
//    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) 
{
//    W5500_WriteBuff(&byte, sizeof(byte));
	_spi_sendbyte(byte);
}

void W5500_Init(void)
{
	HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	
	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
  reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	
	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
	
  wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	HAL_Delay(100);
	
	setSHAR(net_info.mac);
/*	DHCP_init(DHCP_SOCKET, dhcp_buffer);
	reg_dhcp_cbfunc(
			Callback_IPAssigned,
			Callback_IPAssigned,
			Callback_IPConflict
	);
	wizchip_setinterruptmask(IK_SOCK_1);
*/
  wizchip_setnetinfo(&gWIZNETINFO);
	
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
	HAL_Delay(1000);

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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_IWDG_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void Callback01(void const * argument);
void Callback02(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const unsigned long MB_BR[5] = {9600, 19200, 38400, 57600, 115200};

short int Imsz1 = 5, Tmsz1 = 60, Imsz2 = 50, Tmsz2 = 10, Isv = 100, Tsv = 1, APV_Tgot = 9, APV1_T = 20, APV2_T = 10;
short int ZMN_level = 80, ZMN_Tvidkl = 10, ZMN_Tvkl = 20;
bool MSZ1vv = true, MSZ1vv_tmp = true, SVvv = true, SVvv_tmp = true, APV2vv = false, APV2vv_tmp = false;
bool ZMN_APVvv = false, ZMN_APVvv_tmp = false;
short int Imsz1_tmp, Tmsz1_tmp, Imsz2_tmp, Tmsz2_tmp, Isv_tmp, Tsv_tmp, APV_Tgot_tmp, APV1_T_tmp, APV2_T_tmp;
short int ZMN_level_tmp, ZMN_Tvidkl_tmp, ZMN_Tvkl_tmp;
unsigned short int Ubazovoe = 220;
bool APVvv = true, ZMNvv = false, DUvv = true;
unsigned char MB1_adress = 1, MB1_speed = 1, MB2_adress = 1, MB2_speed = 1;
bool LED_1 = false, LED_NesprVimik = false, LED_3 = false, LED_4 = false, LED_5 = false;
bool SW_auto = false;
unsigned char LED_Brightness = 13;
unsigned char Ktt = 10;
float Ktn = 45.46;
unsigned char Year, Month, Date, Hour, Minute, Second, Temp;
bool Summer_Time = false;
short int VV_On_cnt = 0;

unsigned short int Setup2_View_cnt = 0, Save_Settings_Del_cnt = 0, Blink_cnt = 0, Sec_cnt = 0, Min_cnt = 0;
unsigned short int Setup2_En_cnt = 0, DUvv_cnt = 0, Setup1Error_cnt = 0;
unsigned char Time_Save_Part_adress = 0, Setup2_View  = 0, MSZ_state = 0, APV_cikl = 0;
unsigned int Setup_En_cnt = 0;

bool LED_Clear_f = false, LED_Init_f = false, LED_Chk_On_f = false, LED_Chk_Off_f = false, LED_Refresh_f = false;
bool LED_Off_f = false, LED_On_f = false, VV_Unblock_f = false, ADC_RST_f = false;
bool Save_Settings_f = false, Load_Settings_f = false, Setup_En = false;
bool Setup2_En = false;
bool Save_Settings_Del_f = false, Time_Load_f = false, Time_Save_f = false, Time_Save_Part_f = false;
bool Second_f = false, USB_Read_f = false, LED_check = false;
bool LED1b = false, LED2b = false, LED3b = false, LED4b = false, LED5b = false, LED6b = false, LED7b = false;
bool LED8b = false, LED9b = false, LED10b = false, LED11b = false, LED12b = false, LED13b = false;
unsigned char Setup1Error_flags = 0, Inputs_State = 0;

char USB_Send_buf[200], USB_Read_buf[200], USB_Cmd_buf[200];
unsigned short int ADC_Read_Buf[4];
unsigned char USB_Cmd_pos = 0;
unsigned int USB_Menu = 0;

unsigned int VrmsA = 0, IrmsA = 0, IrmsC = 0, VrmsA_d, IrmsA_d, IrmsC_d;
unsigned int VrmsA_poshk = 0, IrmsA_poshk = 0, IrmsC_poshk = 0;
short int VrmsAsend = 0, IrmsAsend = 0, IrmsCsend = 0;
bool RMS_process = false, Debug_mode = false;

bool VV_Block = false, Command_VV_On = false, Command_VV_Off = false;
unsigned int ChasGotovn_timer = 0, VV_Ready_Timer = 0;
unsigned int Timer_OZZ = 0, MSZ_APV_timer = 0;
unsigned int MSZ1A_timer = 0, MSZ2A_timer = 0, MSZsvA_timer = 0, MSZ1B_timer = 0, MSZ2B_timer = 0, MSZsvB_timer = 0;
unsigned char MSZ_APV_cikl = 0, OZZ_state = 0, MSZ_Worked_flags = 0, VV_Faults = 0;
unsigned int Imsz1A = 0, Imsz1B = 0, Tmsz1A = 0, Tmsz1B = 0;

bool VV_Off_State = false, VV_On_State = false, VV_Ready_State = false;
unsigned char VV_On_Sourse = 0, VV_Off_Sourse = 0, VV_On_Sourse_tmp = 0, VV_Off_Sourse_tmp = 0, VV_State = 0;

unsigned int Control_VV_Off_timer = 0, Control_VV_On_timer = 0, ADC_Start_Timer = 200, MB_CoilFault_timer = 0;
unsigned short int ADC_BUSY_timer = 10, Internal_Faults = 0, Task_control = 0, Int_Flts_cnt = 0;
bool LED_state = true, Save_State_f = false;

bool MB_PuskMSZ1 = false, MB_VozvratMSZ1 = false, MB_WorkMSZ1 = false;
bool MB_PuskMSZ2 = false, MB_VozvratMSZ2 = false, MB_WorkMSZ2 = false;
bool MB_PuskSV = false, MB_WorkSV = false, MB_GotovAPV = false;
bool MB_PuskAPV1 = false, MB_WorkAPV1 = false, MB_PuskAPV2 = false, MB_WorkAPV2 = false;
bool MB_PuskZMN = false, MB_VozvratZMN = false, MB_WorkZMN = false, MB_PuskAPV_ZMN = false;
bool MB_VozvratAPV_ZMN = false, MB_WorkAPV_ZMN = false;

extern void Keyboard_Task(void const * argument);
extern void LED_Task(void const * argument);
extern void ADC_Task(void const * argument);
extern void Modbus_Task(void const * argument);
extern void Protection_Task(void const * argument);
extern void I2C_Task(void const * argument);
extern unsigned short int ReadHoldingReg(unsigned short int RegAdress);
extern bool WriteHoldingReg(unsigned short int RegAdress, unsigned short int RegData);

void USART1_Init(uint32_t BaudRate)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = BaudRate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void USART3_Init(uint32_t BaudRate)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = BaudRate;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

void Setup1End(void)
{
	if(((Imsz1_tmp < Imsz2_tmp) || (!MSZ1vv_tmp)) && ((Imsz2_tmp < Isv_tmp) || (!SVvv_tmp)) && (((Tmsz1_tmp * 100) >= (Tmsz2_tmp * 10)) || (!MSZ1vv_tmp)) && (((Tmsz2_tmp * 10) >= Tsv_tmp)  || (!SVvv_tmp)))
	{
		Setup_En = false;
		Setup1Error_flags = 0;
		if(MSZ1vv_tmp)
		{
			Imsz1 = Imsz1_tmp;
		}
		if(SVvv_tmp)
		{
			Isv = Isv_tmp;
		}
		MSZ1vv = MSZ1vv_tmp;
		SVvv = SVvv_tmp;
		APV2vv = APV2vv_tmp;
		ZMN_APVvv = ZMN_APVvv_tmp;
		Tmsz1 = Tmsz1_tmp; Imsz2 = Imsz2_tmp; Tmsz2 = Tmsz2_tmp; Tsv = Tsv_tmp;
		APV_Tgot = APV_Tgot_tmp; APV1_T = APV1_T_tmp; APV2_T = APV2_T_tmp; ZMN_level = ZMN_level_tmp; ZMN_Tvidkl = ZMN_Tvidkl_tmp; ZMN_Tvkl = ZMN_Tvkl_tmp;
		LED_Refresh_f = true;
		Save_Settings_f = true;
	}
	else
	{
		Setup1Error_cnt = 20;
		if(Imsz1_tmp >= Imsz2_tmp)
		{
			Setup1Error_flags |= 1;
		}
		if(Imsz2_tmp >= Isv_tmp)
		{
			Setup1Error_flags |= 2;
		}
		if((Tmsz1_tmp * 100) < (Tmsz2_tmp * 10))
		{
			Setup1Error_flags |= 4;
		}
		if((Tmsz2_tmp * 10) < Tsv_tmp)
		{
			Setup1Error_flags |= 8;
		}
	}
}

void Setup2End(void)
{
	Setup2_En = false;
	if(Setup2_View != 0)
	{
		Setup2_View_cnt = 30000;
	}
	LED_Refresh_f = true;
	Save_Settings_f = true;
	USART1_Init(MB_BR[MB1_speed]);
	USART3_Init(MB_BR[MB2_speed]);
	eMBErrorCode eStatus = eMBInit1(MB1_adress, 1, MB_BR[MB1_speed], MB_PAR_NONE);
	eStatus = eMBInit2(MB2_adress, 1, MB_BR[MB2_speed], MB_PAR_NONE);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
}

void HAL_GPIO_EXTI_Callback(unsigned short int GPIO_Pin)
{
  if(GPIO_Pin == ADC_BUSY_Pin)
  {
		HAL_GPIO_WritePin(ADC_CONVRS_GPIO_Port, ADC_CONVRS_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Receive_DMA(&hspi1, (uint8_t*)ADC_Read_Buf, 4);
  } 
}

short int adc_tmp;

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);
	ADC_BUSY_timer = 10;
	Internal_Faults &= 0xFFFF - 1;
	if((ADC_Read_Buf[0] & 0x8000) != 0)
	{
		IrmsAsend = 0 - (((0x8000 - (ADC_Read_Buf[0] & 0x7FFF)) >> 4) + _B_IA);
	}
	else
	{
		IrmsAsend = (short int)(ADC_Read_Buf[0] >> 4) + _B_IA;
	}
	if((ADC_Read_Buf[2] & 0x8000) != 0)
	{
		IrmsCsend = 0 - (((0x8000 - (ADC_Read_Buf[2] & 0x7FFF)) >> 4) + _B_IC);
	}
	else
	{
		IrmsCsend = (short int)(ADC_Read_Buf[2] >> 4) + _B_IC;
	}
	if((ADC_Read_Buf[3] & 0x8000) != 0)
	{
		VrmsAsend = 0 - (((0x8000 - (ADC_Read_Buf[3] & 0x7FFF)) >> 4) + _B_UA);
	}
	else
	{
		VrmsAsend = (short int)(ADC_Read_Buf[3] >> 4) + _B_UA;
	}
	RMS_process = true;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	RegisterIEC10XMoudle(&Linux);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
//  MX_IWDG_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	i2c_init();
	_spi_init();
	W5500_Init();
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, Callback02);
  myTimer02Handle = osTimerCreate(osTimer(myTimer02), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 71;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */
	HAL_TIM_Base_Start_IT(&htim5);
  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 71;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 499;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
	HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 19200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RELE_3_Pin|RELE_4_Pin|RELE_5_Pin|RELE_6_Pin 
                          |RELE_7_Pin|D_14_Pin|D_15_Pin|NRF_CE_Pin 
                          |NRF_CS_Pin|NRF_SCK_Pin|NRF_MOSI_Pin|RELE_1_Pin 
                          |RELE_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_TEST_Pin|SIM800_ON_Pin|IRLZ_ON_Pin|IRLZ_OFF_Pin 
                          |ADC_RESET_Pin|ADC_CONVRS_Pin|ADC_OS0_Pin|ADC_OS1_Pin 
                          |ADC_OS2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, KBRD_O1_Pin|KBRD_O2_Pin|KBRD_O3_Pin|LED_CS_Pin 
                          |KBRD_O4_Pin|KBRD_O5_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DEBUG_OUT_Pin|SCL_Pin|SDA_Pin|MODBUS2_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MODBUS1_DIR_GPIO_Port, MODBUS1_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RELE_3_Pin RELE_4_Pin RELE_5_Pin RELE_6_Pin 
                           RELE_7_Pin D_14_Pin D_15_Pin NRF_CE_Pin 
                           RELE_1_Pin RELE_2_Pin */
  GPIO_InitStruct.Pin = RELE_3_Pin|RELE_4_Pin|RELE_5_Pin|RELE_6_Pin 
                          |RELE_7_Pin|D_14_Pin|D_15_Pin|NRF_CE_Pin 
                          |RELE_1_Pin|RELE_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_TEST_Pin SIM800_ON_Pin IRLZ_ON_Pin IRLZ_OFF_Pin 
                           ADC_RESET_Pin ADC_CONVRS_Pin ADC_OS0_Pin ADC_OS1_Pin 
                           ADC_OS2_Pin */
  GPIO_InitStruct.Pin = LED_TEST_Pin|SIM800_ON_Pin|IRLZ_ON_Pin|IRLZ_OFF_Pin 
                          |ADC_RESET_Pin|ADC_CONVRS_Pin|ADC_OS0_Pin|ADC_OS1_Pin 
                          |ADC_OS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_CS_Pin MODBUS1_DIR_Pin */
  GPIO_InitStruct.Pin = ADC_CS_Pin|MODBUS1_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ADC_BUSY_Pin */
  GPIO_InitStruct.Pin = ADC_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KBRD_O1_Pin KBRD_O2_Pin KBRD_O3_Pin LED_CS_Pin 
                           DEBUG_OUT_Pin KBRD_O4_Pin KBRD_O5_Pin SCL_Pin 
                           SDA_Pin MODBUS2_DIR_Pin */
  GPIO_InitStruct.Pin = KBRD_O1_Pin|KBRD_O2_Pin|KBRD_O3_Pin|LED_CS_Pin 
                          |DEBUG_OUT_Pin|KBRD_O4_Pin|KBRD_O5_Pin|SCL_Pin 
                          |SDA_Pin|MODBUS2_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_34_Pin KEY_35_Pin */
  GPIO_InitStruct.Pin = KEY_34_Pin|KEY_35_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : NRF_CS_Pin NRF_SCK_Pin NRF_MOSI_Pin */
  GPIO_InitStruct.Pin = NRF_CS_Pin|NRF_SCK_Pin|NRF_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : NRF_MISO_Pin */
  GPIO_InitStruct.Pin = NRF_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NRF_MISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KBRD_I1_Pin KBRD_I2_Pin KBRD_I3_Pin KBRD_I4_Pin 
                           KBRD_I5_Pin KBRD_I6_Pin KBRD_I7_Pin */
  GPIO_InitStruct.Pin = KBRD_I1_Pin|KBRD_I2_Pin|KBRD_I3_Pin|KBRD_I4_Pin 
                          |KBRD_I5_Pin|KBRD_I6_Pin|KBRD_I7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : INPUT_1_Pin */
  GPIO_InitStruct.Pin = INPUT_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INPUT_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : INPUT_2_Pin INPUT_3_Pin INPUT_4_Pin INPUT_5_Pin 
                           INPUT_6_Pin INPUT_7_Pin INPUT_8_Pin */
  GPIO_InitStruct.Pin = INPUT_2_Pin|INPUT_3_Pin|INPUT_4_Pin|INPUT_5_Pin 
                          |INPUT_6_Pin|INPUT_7_Pin|INPUT_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 5 */
	osThreadDef(LED_Task, LED_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(LED_Task), NULL); 
	LED_Init_f = true;
	LED_Clear_f = true;
	LED_Refresh_f = true;
	while(LED_Refresh_f)
	{
		osThreadYield();
	}
	LED_Chk_On_f = true;
	while(LED_Chk_On_f)
	{
		osThreadYield();
	}
	osDelay(1000);
	LED_Chk_Off_f = true;
	while(LED_Chk_Off_f)
	{
		osThreadYield();
	}

	osThreadDef(I2C_Task, I2C_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(I2C_Task), NULL);

	Load_Settings_f = true;
	while(Load_Settings_f)
	{
		osThreadYield();
	}

	Time_Load_f = true;
	while(Time_Load_f)
	{
		osThreadYield();
	}

	osThreadDef(Keyboard_Task, Keyboard_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Keyboard_Task), NULL); 
	osThreadDef(ADC_Task, ADC_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(ADC_Task), NULL); 
//	osThreadDef(Modbus_Task, Modbus_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
//  osThreadCreate(osThread(Modbus_Task), NULL); 
	osThreadDef(Protection_Task, Protection_Task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Protection_Task), NULL);

//  MX_IWDG_Init();
  /* Infinite loop */
  for(;;)
  {
//		if(Task_control == 0)
		{
//			HAL_IWDG_Refresh(&hiwdg);
			Task_control |= 1 + 2 + 4 + 8 + 16;
		}
		if((Internal_Faults & 1) && (Internal_Faults & 4))
		{
			Int_Flts_cnt++;
			if(Int_Flts_cnt == 200)
			{
				HAL_NVIC_SystemReset();
			}
		}
		else
		{
			Int_Flts_cnt = 0;
		}
		if(Save_Settings_Del_f)
		{
			Save_Settings_Del_f = false;
			Save_Settings_Del_cnt = 3000;
		}
		if(Second_f)
		{
			Second_f = false;
			if(!LED_check)
			{
				LED_Refresh_f = true;
			}
			HAL_GPIO_TogglePin(LED_TEST_GPIO_Port, LED_TEST_Pin);
		}
		if(USB_Read_f)
		{
			USB_Read_f = false;
			if(!memcmp(USB_Read_buf, "smb", 3))
			{
				unsigned short int Adress, Data;
				sscanf(&USB_Read_buf[4], "%hu,%hu", &Adress, &Data);
				WriteHoldingReg(Adress, Data);
			}
			if(!memcmp(USB_Read_buf, "gmb", 3))
			{
				sprintf(USB_Send_buf, "50,%u", ReadHoldingReg(atoi(&USB_Read_buf[4])));
				CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
			}
			if(!memcmp(USB_Read_buf, "save", 4))
			{
				switch(USB_Read_buf[5])
				{
					case '0':
					{
						sscanf(&USB_Read_buf[7], "%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd", &Imsz1, &Tmsz1, &Imsz2, &Tmsz2, &Isv, &Tsv, &APV_Tgot, &APV1_T, &APV2_T, &ZMN_level, &ZMN_Tvidkl, &ZMN_Tvkl);
						Save_Settings_f = true;
						LED_Refresh_f = true;
						break;
					}
					case '1':
					{
//						sscanf(&USB_Read_buf[7], "%f", &TMP);
						break;
					}
					case '2':
					{
						unsigned int _K_IAt, _K_IBt, _K_UAt;
						sscanf(&USB_Read_buf[7], "%u,%u,%u,%hd,%hd,%hd,%hd", &_K_IAt, &_K_IBt, &_K_UAt, &_B_IA, &_B_IC, &_B_UA, &_SERIAL_NBR);
						_K_IA = (float)_K_IAt / 10000;
						_K_IC = (float)_K_IBt / 10000;
						_K_UA = (float)_K_UAt / 10000;
						Save_Settings_f = true;
						break;
					}
					case '3':
					{
						unsigned int Tscf1, Tscf2, Tscf3, Tscf4, Tscf5, Tscf6, Tscf7;
						sscanf(&USB_Read_buf[7],"%u:%u:%u %u/%u/%u,%u", &Tscf1, &Tscf2, &Tscf3, &Tscf4, &Tscf5, &Tscf6, &Tscf7);
						Hour = (unsigned short int)Tscf1;
						Minute = (unsigned short int)Tscf2;
						Second = (unsigned short int)Tscf3;
						Date = (unsigned short int)Tscf4;
						Month = (unsigned short int)Tscf5;
						Year = (unsigned short int)Tscf6;
						if(Tscf7 == 1)
						{
							SW_auto = true;
						}
						else
						{
							SW_auto = false;
						}
						Time_Save_f = true;
						Save_Settings_f = true;
						break;
					}
					case '4':
					{
						unsigned int Tscf1;
						sscanf(&USB_Read_buf[7], "%d,%d,%d,%d", &Tscf1, &IrmsA_d, &IrmsC_d, &VrmsA_d);
						if(Tscf1 == 1)
						{
							Debug_mode = true;
						}
						else
						{
							Debug_mode = false;
						}
						break;
					}
				}
			}
			if(!memcmp(USB_Read_buf, "get", 3))
			{
				switch(USB_Read_buf[4])
				{
					case '0':
					{
						sprintf(USB_Send_buf, "0,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", Imsz1, Tmsz1, Imsz2, Tmsz2, Isv, Tsv, APV_Tgot, APV1_T, APV2_T, ZMN_level, ZMN_Tvidkl, ZMN_Tvkl);
						CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
						break;
					}
					case '1':
					{
						sprintf(USB_Send_buf, "1,%f", 0.01);
						CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
						break;
					}
					case '2':
					{
						sprintf(USB_Send_buf, "2,%u,%u,%u,%d,%d,%d,%d,%d,%d,%d", (unsigned int)(_K_IA*10000), (unsigned int)(_K_IC*10000), (unsigned int)(_K_UA*10000), _B_IA, _B_IC, _B_UA, _SERIAL_NBR, _PO_VERSION, _SETTINGS_VER_H, _SETTINGS_VER_L);
						CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
						break;
					}
					case '3':
					{
						sprintf(USB_Send_buf, "3,%02u:%02u:%02u %02u/%02u/%02u,%u", Hour, Minute, Second, Date, Month, Year, SW_auto);
						CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
						break;
					}
					case '4':
					{
						sprintf(USB_Send_buf, "4,%u,%u,%u,%u", Debug_mode, IrmsA_d, IrmsC_d, VrmsA_d);
						CDC_Transmit_FS((unsigned char*)USB_Send_buf, strlen(USB_Send_buf));
						break;
					}
				}
			}
			if(!memcmp(USB_Read_buf, "cmd", 3))
			{
				switch(USB_Read_buf[4])
				{
					case '0':
					{
						break;
					}
				}
			}
		}
		osDelay(10);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
char Iec104_SendBuf[512];
uint16_t Iec104_Lenght = 0;
char Iec104_RecvBuf[512];
uint16_t Iec104_RecvLen;
uint8_t val1 = 0, val2 = 0, val3 = 0, val4 = 0, b1 = 0, b2 = 1, b3 = 0, b4 = 1;
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
//	osTimerStart(myTimer01Handle, 500);
//	osTimerStart(myTimer02Handle, 1000);

  /* Infinite loop */
  for(;;)
  {
		socket(IEC104_SOCKET, Sn_MR_TCP, 2404, 0);
		listen(IEC104_SOCKET);
		while(getSn_SR(IEC104_SOCKET) != SOCK_ESTABLISHED) {
//			osThreadYield();
			osDelay(100);
		}

//		printf("Connected\r\n");
		IEC104_STATE_FLAG_INIT = IEC104_FLAG_SEND_CLOSED;
		while(getSn_SR(IEC104_SOCKET) == SOCK_ESTABLISHED) {

			
		uint16_t tmp = getSn_RX_RSR(IEC104_SOCKET);
		if((tmp > 0) && (tmp != 65535))
		{
//			printf("getSn_RX_RSR:%u\r\n", tmp);
			Iec104_RecvLen = recv(IEC104_SOCKET, (uint8_t*)Iec104_RecvBuf, 512);
			if(Iec104_RecvLen < 512)
			{
				Iex104_Receive((uint8_t*)Iec104_RecvBuf, Iec104_RecvLen);
			}
		}
			//			char Message[128];
			
//			HAL_RTC_GetTime(&hrtc, &globalTime, RTC_FORMAT_BIN);
//			sprintf(Message, "%02u:%02u:%02u\r", globalTime.Hours, globalTime.Minutes, globalTime.Seconds);
//			send(IEC104_SOCKET, (uint8_t*)Message, strlen(Message));
				Iec10x_Scheduled(IEC104_SOCKET);      
    Iec104_StateMachine();

			osDelay(100);
		}
//		printf("Disconnected\r\n");
		disconnect(IEC104_SOCKET);
		close(IEC104_SOCKET);
   }
  /* USER CODE END StartTask02 */
}

/* Callback01 function */
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */
		uint16_t tmp = getSn_RX_RSR(IEC104_SOCKET);
		if((tmp > 0) && (tmp != 65535))
		{
//			printf("getSn_RX_RSR:%u\r\n", tmp);
			Iec104_RecvLen = recv(IEC104_SOCKET, (uint8_t*)Iec104_RecvBuf, 512);
			if(Iec104_RecvLen < 512)
			{
				__nop();
			}
		}
  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void const * argument)
{
  /* USER CODE BEGIN Callback02 */
  
  /* USER CODE END Callback02 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM5)
	{
//		HAL_GPIO_TogglePin(DEBUG_OUT_GPIO_Port, DEBUG_OUT_Pin);
		if(ADC_BUSY_timer != 0)
		{
			ADC_BUSY_timer--;
			if(ADC_BUSY_timer == 0)
			{
				Internal_Faults |= 1;
				ADC_RST_f = true;
			}
		}
		if(MB_CoilFault_timer != 0)
		{
			MB_CoilFault_timer--;
			if(MB_CoilFault_timer == 0)
			{
				VV_Faults |= 32;
			}
		}
		if(ADC_Start_Timer != 0)
		{
			ADC_Start_Timer--;
		}
		if(Control_VV_Off_timer != 0)
		{
			Control_VV_Off_timer--;
		}
		if(Control_VV_On_timer != 0)
		{
			Control_VV_On_timer--;
		}
		if(ChasGotovn_timer != 0)
		{
			ChasGotovn_timer--;
			if(ChasGotovn_timer == 0)
			{
				MSZ_APV_cikl = 0;
				MSZ_Worked_flags = 0;
			}
		}
		if(VV_Ready_Timer != 0)
		{
			VV_Ready_Timer--;
		}
		if(MSZ_APV_timer != 0)
		{
			MSZ_APV_timer--;
		}

		if((MSZ2A_timer == 0) && (MSZ2A_timer != 0xFFFFFFFF))
		{
			MSZ1A_timer++;
		}
		if((MSZ2B_timer == 0) && (MSZ2B_timer != 0xFFFFFFFF))
		{
			MSZ1B_timer++;
		}
		if(MSZ2A_timer != 0)
		{
			MSZ2A_timer--;
			if(MSZ2A_timer == 0)
			{
				MSZ_state = 1;
				MSZ_Worked_flags |= 2;
			}
		}
		if(MSZsvA_timer != 0)
		{
			MSZsvA_timer--;
			if(MSZsvA_timer == 0)
			{
				MSZ_state = 1;
				MSZ_Worked_flags |= 4;
			}
		}
		if(MSZ2B_timer != 0)
		{
			MSZ2B_timer--;
			if(MSZ2B_timer == 0)
			{
				MSZ_state = 1;
				MSZ_Worked_flags |= 2;
			}
		}
		if(MSZsvB_timer != 0)
		{
			MSZsvB_timer--;
			if(MSZsvB_timer == 0)
			{
				MSZ_state = 1;
				MSZ_Worked_flags |= 4;
			}
		}
		if(Timer_OZZ != 0)
		{
			Timer_OZZ--;
			if(Timer_OZZ == 0)
			{
				OZZ_state++;
			}
		}
		Sec_cnt++;
		if(Sec_cnt == 1000)
		{
			Sec_cnt = 0;
			Min_cnt++;
			if(Min_cnt == 60)
			{
				Min_cnt = 0;
				
			}
			Time_Load_f = true;
			Second_f = true;
			if(DUvv_cnt != 0)
			{
				DUvv_cnt--;
				if(DUvv_cnt == 0)
				{
					DUvv = true;
				}
			}
			if(Setup1Error_cnt != 0)
			{
				Setup1Error_cnt--;
				if(Setup1Error_cnt == 0)
				{
					Setup_En = false;
					Setup1Error_flags = 0;
					if(MSZ1vv_tmp)
					{
						Imsz1 = Imsz1_tmp;
					}
					if(SVvv_tmp)
					{
						Isv = Isv_tmp;
					}
					if(APV2vv_tmp)
					{
						APV2_T = APV2_T_tmp;
					}
					MSZ1vv = MSZ1vv_tmp;
					SVvv = SVvv_tmp;
					APV2vv = APV2vv_tmp;
					ZMN_APVvv = ZMN_APVvv_tmp;
					Tmsz1 = Tmsz1_tmp; Imsz2 = Imsz2_tmp; Tmsz2 = Tmsz2_tmp; Tsv = Tsv_tmp;
					APV_Tgot = APV_Tgot_tmp; APV1_T = APV1_T_tmp; ZMN_level = ZMN_level_tmp; ZMN_Tvidkl = ZMN_Tvidkl_tmp; ZMN_Tvkl = ZMN_Tvkl_tmp;
					LED_Refresh_f = true;
					Save_Settings_f = true;
				}
			}
		}
		Blink_cnt++;
		if((Blink_cnt == 201) || (Blink_cnt == 501))
		{
			LED_Refresh_f = true;
		}
		if(Blink_cnt == 1000)
		{
			LED_Refresh_f = true;
			Blink_cnt = 0;
		}
		if(Setup_En_cnt != 0)
		{
			Setup_En_cnt--;
			if(Setup_En_cnt == 0)
			{
				Setup1End();
			}
		}
		if(Setup2_En_cnt != 0)
		{
			Setup2_En_cnt--;
			if(Setup2_En_cnt == 0)
			{
				Setup2End();
			}
		}
		if(Setup2_View_cnt != 0)
		{
			Setup2_View_cnt--;
			if(Setup2_View_cnt == 0)
			{
				Setup2_View = 0;
				LED_Refresh_f = true;
			}
		}
		if(Save_Settings_Del_cnt != 0)
		{
			Save_Settings_Del_cnt--;
			if(Save_Settings_Del_cnt == 0)
			{
				Save_Settings_f = true;
			}
		}
  }
  if (htim->Instance == TIM7)
	{
		HAL_GPIO_WritePin(ADC_CONVRS_GPIO_Port, ADC_CONVRS_Pin, GPIO_PIN_SET);
	}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM8) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
