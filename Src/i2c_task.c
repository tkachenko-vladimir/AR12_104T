#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "ds3231.h"
#include "eeprom24xx.h"

unsigned char I2C_buf[32];
short int Read_pos = 0;

void I2C_Task(void const * argument)
{
	while(1)
	{
		Task_control &= 0xFFFF - 16;
		if(Save_State_f)
		{
			I2C_buf[0] = OZZ_state;
			I2C_buf[1] = (unsigned char)(VV_On_cnt >> 8);
			I2C_buf[2] = (unsigned char)(VV_On_cnt);
			if(VV_Block)
			{
				I2C_buf[3] = 1;
			}
			else
			{
				I2C_buf[3] = 0;
			}
			I2C_buf[4] = VV_Off_State;
			I2C_buf[5] = VV_On_State;
			I2C_buf[6] = IrmsA_poshk;
			I2C_buf[7] = IrmsC_poshk;
			I2C_buf[8] = VrmsA_poshk;
			EEPROM24XX_Save(176, I2C_buf, 16);
			Save_State_f = false;
		}
		if(Load_Settings_f)
		{
			if(!EEPROM24XX_Load(0, I2C_buf, 2))
			{
				Internal_Faults |= 8;
			}
			if((I2C_buf[0] == _SETTINGS_VER_H) && (I2C_buf[1] == _SETTINGS_VER_L))
			{
				EEPROM24XX_Load(2, I2C_buf, 30);
				_SERIAL_NBR = (short int)(((unsigned int)I2C_buf[0] << 8) + I2C_buf[1]);
				_K_IA = (float)(((unsigned int)I2C_buf[2] << 8) + I2C_buf[3]) / 100000;
				_K_IC = (float)(((unsigned int)I2C_buf[4] << 8) + I2C_buf[5]) / 100000;
				_K_UA = (float)(((unsigned int)I2C_buf[6] << 8) + I2C_buf[7]) / 100000;
				_B_IA = (short int)(((unsigned int)I2C_buf[8] << 8) + I2C_buf[9]) - 32768;
				_B_IC = (short int)(((unsigned int)I2C_buf[10] << 8) + I2C_buf[11]) - 32768;
				_B_UA = (short int)(((unsigned int)I2C_buf[12] << 8) + I2C_buf[13]) - 32768;
				EEPROM24XX_Load(32, I2C_buf, 32);
				Imsz1 = ((unsigned int)I2C_buf[0] << 8) + I2C_buf[1];
				Tmsz1 = ((unsigned int)I2C_buf[2] << 8) + I2C_buf[3];
				Imsz2 = ((unsigned int)I2C_buf[4] << 8) + I2C_buf[5];
				Tmsz2 = ((unsigned int)I2C_buf[6] << 8) + I2C_buf[7];
				Isv = ((unsigned int)I2C_buf[8] << 8) + I2C_buf[9];
				Tsv = ((unsigned int)I2C_buf[10] << 8) + I2C_buf[11];
				APV_Tgot = ((unsigned int)I2C_buf[12] << 8) + I2C_buf[13];
				APV1_T = ((unsigned int)I2C_buf[14] << 8) + I2C_buf[15];
				APV2_T = ((unsigned int)I2C_buf[16] << 8) + I2C_buf[17];
				ZMN_level = ((unsigned int)I2C_buf[18] << 8) + I2C_buf[19];
				ZMN_Tvidkl = ((unsigned int)I2C_buf[20] << 8) + I2C_buf[21];
				ZMN_Tvkl = ((unsigned int)I2C_buf[22] << 8) + I2C_buf[23];
				LED_Brightness = I2C_buf[24];
				Ktt = I2C_buf[25];
				unsigned short int Ktn_tmp =( (unsigned int)I2C_buf[26] << 8) + I2C_buf[27];
				Ktn = (float)Ktn_tmp / 100;
				EEPROM24XX_Load(96, I2C_buf, 32);
				Ubazovoe = ((unsigned int)I2C_buf[0] << 8) + I2C_buf[1];
				if(I2C_buf[2] & 1)
				{
					SW_auto = true;
				}
				else
				{
					SW_auto = false;
				}
				APVvv = false;
				ZMNvv = false;
				DUvv = false;
				MSZ1vv = false;
				SVvv = false;
				APV2vv = false;
				if(I2C_buf[7] & 1)
				{
					APVvv = true;
				}
				if(I2C_buf[7] & 2)
				{
					ZMNvv = true;
				}
				if(I2C_buf[7] & 4)
				{
					DUvv = true;
				}
				if(I2C_buf[7] & 8)
				{
					MSZ1vv = true;
				}
				if(I2C_buf[7] & 16)
				{
					SVvv = true;
				}
				if(I2C_buf[7] & 32)
				{
					APV2vv = true;
				}
				if(I2C_buf[7] & 64)
				{
					ZMN_APVvv = true;
				}
				MB1_adress = (unsigned char)I2C_buf[8];
				MB1_speed = (unsigned char)I2C_buf[9];
				MB2_adress = (unsigned char)I2C_buf[10];
				MB2_speed = (unsigned char)I2C_buf[11];
				EEPROM24XX_Load(176, I2C_buf, 16);
				OZZ_state = I2C_buf[0];
				VV_On_cnt = ((unsigned int)I2C_buf[1] << 8) + I2C_buf[2];
				if(I2C_buf[3] == 1)
				{
					VV_Block = true;
				}
				else
				{
					VV_Block = false;
				}
				VV_Off_State = I2C_buf[4];
				VV_On_State = I2C_buf[5];
				IrmsA_poshk = I2C_buf[6];
				IrmsC_poshk = I2C_buf[7];
				VrmsA_poshk = I2C_buf[8];
			}
			else
			{
				Internal_Faults |= 2;
				Save_Settings_f = true;
			}

			Load_Settings_f = false;

			if(VV_On_State && !VV_Off_State && APVvv)
			{
				ChasGotovn_timer = APV_Tgot * 1000;
			}
		}
		if(Save_Settings_f)
		{
			I2C_buf[0] = _SETTINGS_VER_H;
			I2C_buf[1] = _SETTINGS_VER_L;
			I2C_buf[2] = (unsigned char)((_SERIAL_NBR) >> 8);
			I2C_buf[3] = (unsigned char)_SERIAL_NBR;
			I2C_buf[4] = (unsigned char)((short int)(_K_IA * 100000) >> 8);
			I2C_buf[5] = (unsigned char)((short int)(_K_IA * 100000));
			I2C_buf[6] = (unsigned char)((short int)(_K_IC * 100000) >> 8);
			I2C_buf[7] = (unsigned char)((short int)(_K_IC * 100000));
			I2C_buf[8] = (unsigned char)((short int)(_K_UA * 100000) >> 8);
			I2C_buf[9] = (unsigned char)((short int)(_K_UA * 100000));
			I2C_buf[10] = (unsigned char)((_B_IA + 32768) >> 8);
			I2C_buf[11] = (unsigned char)(_B_IA + 32768);
			I2C_buf[12] = (unsigned char)((_B_IC + 32768) >> 8);
			I2C_buf[13] = (unsigned char)(_B_IC + 32768);
			I2C_buf[14] = (unsigned char)((_B_UA + 32768) >> 8);
			I2C_buf[15] = (unsigned char)(_B_UA + 32768);
			EEPROM24XX_Save(0, I2C_buf, 32);
			I2C_buf[0] = (unsigned char)(Imsz1 >> 8);
			I2C_buf[1] = (unsigned char)(Imsz1);
			I2C_buf[2] = (unsigned char)(Tmsz1 >> 8);
			I2C_buf[3] = (unsigned char)(Tmsz1);
			I2C_buf[4] = (unsigned char)(Imsz2 >> 8);
			I2C_buf[5] = (unsigned char)(Imsz2);
			I2C_buf[6] = (unsigned char)(Tmsz2 >> 8);
			I2C_buf[7] = (unsigned char)(Tmsz2);
			I2C_buf[8] = (unsigned char)(Isv >> 8);
			I2C_buf[9] = (unsigned char)(Isv);
			I2C_buf[10] = (unsigned char)(Tsv >> 8);
			I2C_buf[11] = (unsigned char)(Tsv);
			I2C_buf[12] = (unsigned char)(APV_Tgot >> 8);
			I2C_buf[13] = (unsigned char)(APV_Tgot);
			I2C_buf[14] = (unsigned char)(APV1_T >> 8);
			I2C_buf[15] = (unsigned char)(APV1_T);
			I2C_buf[16] = (unsigned char)(APV2_T >> 8);
			I2C_buf[17] = (unsigned char)(APV2_T);
			I2C_buf[18] = (unsigned char)(ZMN_level >> 8);
			I2C_buf[19] = (unsigned char)(ZMN_level);
			I2C_buf[20] = (unsigned char)(ZMN_Tvidkl >> 8);
			I2C_buf[21] = (unsigned char)(ZMN_Tvidkl);
			I2C_buf[22] = (unsigned char)(ZMN_Tvkl >> 8);
			I2C_buf[23] = (unsigned char)(ZMN_Tvkl);
			I2C_buf[24] = LED_Brightness;
			I2C_buf[25] = Ktt;
			unsigned short int Ktn_tmp = (unsigned short int)(Ktn * 100);
			I2C_buf[26] = (unsigned char)(Ktn_tmp >> 8);
			I2C_buf[27] = (unsigned char)(Ktn_tmp);
			EEPROM24XX_Save(32, I2C_buf, 32);
			I2C_buf[0] = (unsigned char)(Ubazovoe >> 8);
			I2C_buf[1] = (unsigned char)(Ubazovoe);
			I2C_buf[2] = 0;
			if(SW_auto)
			{
				I2C_buf[2] = I2C_buf[2] | 1;
			}
			I2C_buf[3] = 0;
			I2C_buf[4] = 0;
			I2C_buf[5] = 0;
			I2C_buf[6] = 0;
			I2C_buf[7] = 0;
			if(APVvv)
			{
				I2C_buf[7] |= 1;
			}
			if(ZMNvv)
			{
				I2C_buf[7] |= 2;
			}
			if(DUvv)
			{
				I2C_buf[7] |= 4;
			}
			if(MSZ1vv)
			{
				I2C_buf[7] |= 8;
			}
			if(SVvv)
			{
				I2C_buf[7] |= 16;
			}
			if(APV2vv)
			{
				I2C_buf[7] |= 32;
			}
			if(ZMN_APVvv)
			{
				I2C_buf[7] |= 64;
			}
			I2C_buf[8] = MB1_adress;
			I2C_buf[9] = MB1_speed;
			I2C_buf[10] = MB2_adress;
			I2C_buf[11] = MB2_speed;
			I2C_buf[12] = IrmsA_poshk;
			I2C_buf[13] = IrmsC_poshk;
			I2C_buf[14] = VrmsA_poshk;
			EEPROM24XX_Save(96, I2C_buf, 32);
			Save_Settings_f = false;
		}
		if(Time_Load_f)
		{
			unsigned char		Data[7];

			if(DS3231_Load_Temp(Data))
			{
				Internal_Faults &= 0xFFFF - 4;
			}
			else
			{
				Internal_Faults |= 4;
			}
			Temp = Data[0];

			DS3231_Load(0, Data);
			Second =  ((Data[0] >> 4) * 10 + (0x0F & Data[0]));
			Minute = ((Data[1] >> 4) * 10 + (0x0F & Data[1]));
			Hour = ((Data[2] >> 4) * 10 + (0x0F & Data[2]));
			Date = ((Data[4] >> 4) * 10 + (0x0F & Data[4]));
			Month = ((Data[5] >> 4) * 10 + (0x0F & Data[5]));
			Year = ((Data[6] >> 4) * 10 + (0x0F & Data[6]));

			if(SW_auto)
			{
				if(SunTime(Hour, Date, Month, Year))
				{
					Hour++;
					Summer_Time = true;
					if(Hour == 24)
					{
						Hour = 0;
						Date++;
						if(Date > MonthDay(Month))
						{
							Date = 1;
							Month++;
							if(Month == 13)
							{
								Month = 1;
								Year++;
							}
						}
					}
				}
				else
				{
					Summer_Time = false;
				}
			}
			Time_Load_f = false;
		}
		if(Time_Save_f)
		{
			unsigned char Data[7];

			if(SW_auto)
			{
				if(SunTime(Hour, Date, Month, Year))
				{
					if(Hour == 0)
					{
						Hour = 23;
						Date--;
						if(Date == 0)
						{
							Month--;
							if(Month == 0)
							{
								Month = 12;
								Year--;
							}
							Date = MonthDay(Month);
						}
					}
					else
					{
						Hour--;
					}
				}
			}
			Data[0] = ((Second / 10) << 4) | (Second % 10);
			Data[1] = ((Minute / 10) << 4) | (Minute % 10);
			Data[2] = ((Hour / 10) << 4) | (Hour % 10);
			Data[4] = ((Date / 10) << 4) | (Date % 10);
			Data[5] = ((Month / 10) << 4) | (Month % 10);
			Data[6] = ((Year / 10) << 4) | (Year % 10);
			DS3231_Save(0, Data, 7);
			Time_Save_f = false;
		}
		if(Time_Save_Part_f)
		{
			unsigned char Data[3];
			switch(Time_Save_Part_adress)
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
			Time_Save_Part_f = false;
		}
    osDelay(1000);
	}
}
