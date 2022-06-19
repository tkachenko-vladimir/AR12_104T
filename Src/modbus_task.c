#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"

bool RegPresent;

unsigned short int ReadInputReg(unsigned short int RegAdress)//04W
{
	RegPresent = true;
	switch(RegAdress)
	{
		default:
		{
			RegPresent = false;
			return 0;
		}
	}
}

unsigned short int ReadHoldingReg(unsigned short int RegAdress)//03W
{
	RegPresent = true;
	switch(RegAdress)
	{
		case 0x0020:
		{
			return VV_Block;
		}
		case 0x0021:
		{
			if(VV_Faults == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		case 0x0022:
		{
			return 0;
		}
		case 0x0023:
		{
			if(VV_Faults & 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0024:
		{
			if(VV_Faults & 32)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0025:
		{
			if(Inputs_State & INPUT_DOOR)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		case 0x0026:
		{
			if(Inputs_State & INPUT_POWER_FAULT)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0027:
		{
			if(Inputs_State & INPUT_RES_POWER)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		case 0x0028:
		{
			if(Inputs_State & INPUT_DV8)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		case 0x0029:
		{
			return 0;
		}
		case 0x002A:
		{
			return VV_Off_Sourse;
		}
		case 0x002B:
		{
			return VV_On_Sourse;
		}
		case 0x002C:
		{
			return 0;
		}
		case 0x002D:
		{
			return 0;
		}
		case 0x002E:
		{
			return 0;
		}
		case 0x002F:
		{
			return 0;
		}
		case 0x0030:
		{
			if(MB_PuskMSZ1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0031:
		{
			if(MB_VozvratMSZ1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0032:
		{
			if(MB_WorkMSZ1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0033:
		{
			if(MB_PuskMSZ2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0034:
		{
			if(MB_VozvratMSZ2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0035:
		{
			if(MB_WorkMSZ2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0036:
		{
			if(MB_PuskSV)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0037:
		{
			if(MB_WorkSV)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0038:
		{
			if(MB_GotovAPV)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0039:
		{
			if(MB_PuskAPV1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x003A:
		{
			if(MB_WorkAPV1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x003B:
		{
			if(MB_PuskAPV2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x003C:
		{
			if(MB_WorkAPV2)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x003D:
		{
			return 0;
		}
		case 0x003E:
		{
			return 0;
		}
		case 0x003F:
		{
			return 0;
		}
		case 0x0040:
		{
			if(MB_PuskZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0041:
		{
			if(MB_VozvratZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0042:
		{
			if(MB_WorkZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0043:
		{
			if(MB_PuskAPV_ZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0044:
		{
			if(MB_VozvratAPV_ZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0045:
		{
			if(MB_WorkAPV_ZMN)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0046:
		{
			return 0;
		}
		case 0x0047:
		{
			return 0;
		}
		case 0x0048:
		{
			return 0;
		}
		case 0x0049:
		{
			return 0;
		}
		case 0x004A:
		{
			return 0;
		}
		case 0x004B:
		{
			unsigned short int TmpRet = 0;
			
			if(VV_Block)
			{
				TmpRet |= 1;
			}
			if(VV_Faults != 0)
			{
				TmpRet |= 2;
			}
			if(VV_Faults & 1)
			{
				TmpRet |= 4;
			}
			if(VV_Faults & 32)
			{
				TmpRet |= 8;
			}
			if(!(Inputs_State & INPUT_DOOR))
			{
				TmpRet |= 16;
			}
			if(Inputs_State & INPUT_POWER_FAULT)
			{
				TmpRet |= 32;
			}
			if(!(Inputs_State & INPUT_RES_POWER))
			{
				TmpRet |= 64;
			}
			if(!(Inputs_State & INPUT_DV8))
			{
				TmpRet |= 128;
			}
			return TmpRet;
		}
		case 0x004C:
		{
			return VV_Off_Sourse;
		}
		case 0x004D:
		{
			return VV_On_Sourse;
		}
		case 0x004E:
		{
			unsigned short int TmpRet = 0;

			if(MB_PuskMSZ1)
			{
				TmpRet |= 1;
			}
			if(MB_VozvratMSZ1)
			{
				TmpRet |= 2;
			}
			if(MB_WorkMSZ1)
			{
				TmpRet |= 4;
			}
			if(MB_PuskMSZ2)
			{
				TmpRet |= 8;
			}
			if(MB_VozvratMSZ2)
			{
				TmpRet |= 16;
			}
			if(MB_WorkMSZ2)
			{
				TmpRet |= 32;
			}
			if(MB_PuskSV)
			{
				TmpRet |= 64;
			}
			if(MB_WorkSV)
			{
				TmpRet |= 128;
			}
			if(MB_GotovAPV)
			{
				TmpRet |= 256;
			}
			if(MB_PuskAPV1)
			{
				TmpRet |= 512;
			}
			if(MB_WorkAPV1)
			{
				TmpRet |= 1024;
			}
			if(MB_PuskAPV2)
			{
				TmpRet |= 2048;
			}
			if(MB_WorkAPV2)
			{
				TmpRet |= 4096;
			}
			return TmpRet;
		}
		case 0x004F:
		{
			unsigned short int TmpRet = 0;

			if(MB_PuskZMN)
			{
				TmpRet |= 1;
			}
			if(MB_VozvratZMN)
			{
				TmpRet |= 2;
			}
			if(MB_WorkZMN)
			{
				TmpRet |= 4;
			}
			if(MB_PuskAPV_ZMN)
			{
				TmpRet |= 8;
			}
			if(MB_VozvratAPV_ZMN)
			{
				TmpRet |= 16;
			}
			if(MB_WorkAPV_ZMN)
			{
				TmpRet |= 32;
			}
			return TmpRet;
		}
		case 0x0050:
		{
			return VV_State;
		}
		case 0x0051:
		{
			if(MSZ1vv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0052:
		{
			return Imsz1;
		}
		case 0x0053:
		{
			return Tmsz1;
		}
		case 0x0054:
		{
			return Imsz2;
		}
		case 0x0055:
		{
			return Tmsz2;
		}
		case 0x0056:
		{
			if(SVvv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0057:
		{
			return Isv;
		}
		case 0x0058:
		{
			return Tsv;
		}
		case 0x0059:
		{
			if(APVvv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x005A:
		{
			return APV_Tgot;
		}
		case 0x005B:
		{
			return APV1_T;
		}
		case 0x005C:
		{
			if(APV2vv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x005D:
		{
			return APV2_T;
		}
		case 0x005E:
		{
			if(ZMNvv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x005F:
		{
			if(ZMN_APVvv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0060:
		{
			return ZMN_level;
		}
		case 0x0061:
		{
			return ZMN_Tvidkl;
		}
		case 0x0062:
		{
			return ZMN_Tvkl;
		}
		case 0x0063:
		{
			if(DUvv)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0200:
		{
			return Ktt;
		}
		case 0x0201:
		{
			return (unsigned short int)(Ktn * 100);
		}
		case 0x0202:
		{
			return (((unsigned short int)Year) << 8) + Month;
		}
		case 0x0203:
		{
			return (((unsigned short int)Date) << 8) + Hour;
		}
		case 0x0204:
		{
			return (((unsigned short int)Minute) << 8) + Second;
		}
		case 0x0205:
		{
			if(SW_auto)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case 0x0300:
		{
			return (unsigned short int)Temp;
		}
		case 0x0301:
		{
			return IrmsA;
		}
		case 0x0302:
		{
			return IrmsC;
		}
		case 0x0303:
		{
			return VrmsA;
		}
		case 0x0304:
		{
			return IrmsA_poshk;
		}
		case 0x0305:
		{
			return IrmsC_poshk;
		}
		case 0x0306:
		{
			return VrmsA_poshk;
		}
		case 0x0400:
		{
			return VV_On_cnt;
		}
		case 0x0401:
		{
			return _SERIAL_NBR;
		}
		case 0x0402:
		{
			return _PO_VERSION;
		}
//////////////////////////////////////////////////////////////////////////////////
		case 0xF000:
		{
			unsigned short int TmpRet = 0;
			
			if(VV_Block)
			{
				TmpRet |= 1;
			}
			if(VV_Faults != 0)
			{
				TmpRet |= 2;
			}
			if(VV_Faults & 1)
			{
				TmpRet |= 4;
			}
			if(VV_Faults & 32)
			{
				TmpRet |= 8;
			}
			if(!(Inputs_State & INPUT_DOOR))
			{
				TmpRet |= 16;
			}
			if(Inputs_State & INPUT_POWER_FAULT)
			{
				TmpRet |= 32;
			}
			if(!(Inputs_State & INPUT_RES_POWER))
			{
				TmpRet |= 64;
			}
			if(!(Inputs_State & INPUT_DV8))
			{
				TmpRet |= 128;
			}
			if(MB_PuskMSZ1)
			{
				TmpRet |= 256;
			}
			if(MB_VozvratMSZ1)
			{
				TmpRet |= 512;
			}
			if(MB_WorkMSZ1)
			{
				TmpRet |= 1024;
			}
			if(MB_PuskMSZ2)
			{
				TmpRet |= 2048;
			}
			if(MB_VozvratMSZ2)
			{
				TmpRet |= 4096;
			}
			if(MB_WorkMSZ2)
			{
				TmpRet |= 8192;
			}
			if(MB_PuskSV)
			{
				TmpRet |= 16384;
			}
			if(MB_WorkSV)
			{
				TmpRet |= 32768;
			}
			return TmpRet;
		}
		case 0xF001:
		{
			unsigned short int TmpRet = 0;
			
			if(MB_GotovAPV)
			{
				TmpRet |= 1;
			}
			if(MB_PuskAPV1)
			{
				TmpRet |= 2;
			}
			if(MB_WorkAPV1)
			{
				TmpRet |= 4;
			}
			if(MB_PuskAPV2)
			{
				TmpRet |= 8;
			}
			if(MB_WorkAPV2)
			{
				TmpRet |= 16;
			}
			if(MB_PuskZMN)
			{
				TmpRet |= 32;
			}
			if(MB_VozvratZMN)
			{
				TmpRet |= 64;
			}
			if(MB_WorkZMN)
			{
				TmpRet |= 128;
			}
			if(MB_PuskAPV_ZMN)
			{
				TmpRet |= 256;
			}
			if(MB_VozvratAPV_ZMN)
			{
				TmpRet |= 512;
			}
			if(MB_WorkAPV_ZMN)
			{
				TmpRet |= 1024;
			}
			if(VV_State & 1)
			{
				TmpRet |= 2048;
			}
			if(VV_State & 2)
			{
				TmpRet |= 4096;
			}
			return TmpRet;
		}
		case 0xF002:
		{
			return Internal_Faults;
		}
		case 0x0206:
		{
			return LED_Brightness;
		}
		default:
		{
			RegPresent = false;
			return 0;
		}
	}
}

bool WriteHoldingReg(unsigned short int RegAdress, unsigned short int RegData)//06W
{
	bool ret = true;
	
	switch(RegAdress)
	{
		case 0x0050:
		{
			if(RegData == 0)
			{
				Command_VV_Off = true;
				VV_Off_Sourse_tmp = 2;
			}
			if(RegData == 1)
			{
				Command_VV_On = true;
				VV_On_Sourse_tmp = 2;
			}
			break;
		}
		case 0x0051:
		{
			if(RegData == 0)
			{
				MSZ1vv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				MSZ1vv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x0052:
		{
			if((RegData >= Imsz1_MIN) && (RegData <= Imsz1_MAX))
			{
				Imsz1 = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0053:
		{
			if(((RegData > Tmsz1_MIN) || (RegData == Tmsz1_MIN)) && (RegData <= Tmsz1_MAX))
			{
				Tmsz1 = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0054:
		{
			if(((RegData >= Imsz2_MIN)) && (RegData <= Imsz2_MAX))
			{
				Imsz2 = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0055:
		{
			if(((RegData > Tmsz2_MIN) || (RegData == Tmsz2_MIN)) && (RegData <= Tmsz2_MAX))
			{
				Tmsz2 = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0056:
		{
			if(RegData == 0)
			{
				SVvv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				SVvv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x0057:
		{
			if((RegData >= Isv_MIN) && (RegData <= Isv_MAX))
			{
				Isv = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0058:
		{
			if(((RegData > Tsv_MIN) || (RegData == Tsv_MIN)) && (RegData <= Tsv_MAX))
			{
				Tsv = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0059:
		{
			if(RegData == 0)
			{
				APVvv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				APVvv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x005A:
		{
			if(((RegData > APV_Tgot_MIN) || (RegData == APV_Tgot_MIN)) && (RegData <= APV_Tgot_MAX))
			{
				APV_Tgot = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x005B:
		{
			if(((RegData > APV1_T_MIN) || (RegData == APV1_T_MIN)) && (RegData <= APV1_T_MAX))
			{
				APV1_T = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x005C:
		{
			if(RegData == 0)
			{
				APV2vv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				APV2vv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x005D:
		{
			if((RegData >= APV2_T_MIN) && (RegData <= APV2_T_MAX))
			{
				APV2_T = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x005E:
		{
			if(RegData == 0)
			{
				ZMNvv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				ZMNvv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x005F:
		{
			if(RegData == 0)
			{
				ZMN_APVvv = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				ZMN_APVvv = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x0060:
		{
			if((RegData >= ZMN_level_MIN) && (RegData <= ZMN_level_MAX))
			{
				ZMN_level = (RegData / 10) * 10;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0061:
		{
			if((RegData >= ZMN_Tvidkl_MIN) && (RegData <= ZMN_Tvidkl_MAX))
			{
				ZMN_Tvidkl = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0062:
		{
			if((RegData >= ZMN_Tvkl_MIN) && (RegData <= ZMN_Tvkl_MAX))
			{
				ZMN_Tvkl = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x00064:
		{
			if(RegData == 1)
			{
				VV_Unblock_f = true;
			}
			break;
		}
		case 0x0200:
		{
			if((RegData == 10) || (RegData == 20) || (RegData == 30))
			{
				Ktt = RegData;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0201:
		{
			if((RegData == 4546) || (RegData == 2727))
			{
				Ktn = (float)RegData / 100;
				Save_Settings_Del_f = true;
				LED_Refresh_f = true;
			}
			break;
		}
		case 0x0202:
		{
			Year = (uint8_t)(RegData >> 8);
			Month = (uint8_t)RegData;
			Time_Save_Part_adress = 2;
			Time_Save_Part_f = true;
			break;
		}
		case 0x0203:
		{
			Date = (uint8_t)(RegData >> 8);
			Hour = (uint8_t)RegData;
			Time_Save_Part_adress = 1;
			Time_Save_Part_f = true;
			break;
		}
		case 0x0204:
		{
			Minute = (uint8_t)(RegData >> 8);
			Second = (uint8_t)RegData;
			Time_Save_Part_adress = 0;
			Time_Save_Part_f = true;
			break;
		}
		case 0x0205:
		{
			if(RegData == 0)
			{
				SW_auto = false;
				Save_Settings_Del_f = true;
			}
			if(RegData == 1)
			{
				SW_auto = true;
				Save_Settings_Del_f = true;
			}
			break;
		}
		case 0x0400:
		{
			if(RegData <= 20000)
			{
				VV_On_cnt = RegData;
				Save_State_f = true;
			}
			break;
		}
//////////////////////////////////////////////////////////////////////////////////
		case 0xF100:
		{
			if(RegData == 1)
			{
				HAL_NVIC_SystemReset();
			}
			break;
		}
		case 0x0206:
		{
			if(RegData <= 15)
			{
				LED_Brightness = RegData;
				Save_Settings_Del_f = true;
				LED_Init_f = true;
			}
			break;
		}
		default:
		{
			ret = false;
			break;
		}
	}
	return ret;
}

unsigned char ReadDiscreteInput(unsigned short int RegAdress)//02b
{
	RegPresent = true;
	switch(RegAdress)
	{
		default:
		{
			RegPresent = false;
			return 0;
		}
	}
}

unsigned char ReadCoil(unsigned short int RegAdress)//01b
{
	RegPresent = true;
	switch(RegAdress)
	{
		default:
		{
			RegPresent = false;
			return 0;
		}
	}
}

bool WriteCoil(unsigned short int RegAdress, unsigned char RegData)//05b
{
	bool ret = true;
	
	switch(RegAdress)
	{
		default:
		{
			ret = false;
			break;
		}
	}
	return ret;
}

void Modbus_Task(void const * argument)
{
  eMBErrorCode eStatus = eMBInit1(MB1_adress, 1, MB_BR[MB1_speed], MB_PAR_NONE);
  eStatus = eMBEnable1();
  eStatus = eMBInit2(MB2_adress, 1, MB_BR[MB2_speed], MB_PAR_NONE);
  eStatus = eMBEnable2();

	while(1)
	{
		Task_control &= 0xFFFF - 8;
		eMBPoll1();
		eMBPoll2();
		osDelay(50);
	}
}

eMBErrorCode
eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t RegCnt;
	unsigned short int RegTmp;
	eMBErrorCode  eStatus = MB_ENOERR;
	
	usAddress--;
	for(RegCnt = 0; RegCnt < usNRegs; RegCnt++)
	{
		RegTmp = ReadInputReg(usAddress + RegCnt);
		if(RegPresent)
		{
			*pucRegBuffer++ = (UCHAR)(RegTmp >> 8);
			*pucRegBuffer++ = (UCHAR)(RegTmp);
		}
		else
		{
			eStatus = MB_ENOREG;
		}
	}
  return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
	uint8_t RegCnt;
	unsigned short int RegTmp;
	eMBErrorCode  eStatus = MB_ENOERR;
	
	usAddress--;
	for(RegCnt = 0; RegCnt < usNRegs; RegCnt++)
	{
		if(eMode == MB_REG_READ)
		{
			RegTmp = ReadHoldingReg(usAddress + RegCnt);
			if(RegPresent)
			{
				*pucRegBuffer++ = (UCHAR)(RegTmp >> 8);
				*pucRegBuffer++ = (UCHAR)(RegTmp);
			}
			else
			{
				eStatus = MB_ENOREG;
			}
		}
		else
		{
			if(DUvv)
			{
				RegTmp = (unsigned short int)*pucRegBuffer++;
				RegTmp = RegTmp << 8;
				RegTmp = RegTmp + *pucRegBuffer++;
				if(!WriteHoldingReg((usAddress + RegCnt), RegTmp))
				{
					eStatus = MB_ENOREG;
				}
			}
		}
	}
  return eStatus;
}

eMBErrorCode//01
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
	uint8_t RegCnt;
	unsigned char RegTmp;
	eMBErrorCode  eStatus = MB_ENOERR;
	
	usAddress--;
	if(eMode == MB_REG_READ)
	{
		*pucRegBuffer = 0;
		for(RegCnt = 0; RegCnt < usNCoils; RegCnt++)
		{
			RegTmp = ReadCoil(usAddress + RegCnt);
			if(RegPresent)
			{
					xMBUtilSetBits(pucRegBuffer, RegCnt, 1, RegTmp);
			}
			else
			{
				eStatus = MB_ENOREG;
			}
		}
	}
	else
	{
		if(DUvv)
		{
			for(RegCnt = 0; RegCnt < usNCoils; RegCnt++)
			{
				RegTmp = xMBUtilGetBits(pucRegBuffer, RegCnt, 1);
				if(!WriteCoil((usAddress + RegCnt), RegTmp))
				{
					eStatus = MB_ENOREG;
				}
			}
		}
	}
  return eStatus;
}

eMBErrorCode//02
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	uint8_t RegCnt;
	unsigned char RegTmp;
	eMBErrorCode  eStatus = MB_ENOERR;
	
	*pucRegBuffer = 0;
	usAddress--;
	for(RegCnt = 0; RegCnt < usNDiscrete; RegCnt++)
	{
		RegTmp = ReadDiscreteInput(usAddress + RegCnt);
		if(RegPresent)
		{
				xMBUtilSetBits(pucRegBuffer, RegCnt, 1, RegTmp);
		}
		else
		{
			eStatus = MB_ENOREG;
		}
	}
  return eStatus;
}
