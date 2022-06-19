#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "LED.h"
#include "Iec104.h"

const unsigned short int cb_rep_spd_tab[4] = {15, 7, 4, 1};
unsigned char kbrd_state = 0, kbrd_code = 0, cb_rep_spd = 0;
unsigned short int cb_rep = 0, cb_rep_cnt = 0;
unsigned short int cb32 = 0, cb34 = 0;
unsigned short int cb[49] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void VV_on_event(void)
{
	VV_Ready_Timer = 10000;
	if(APVvv)
	{
		ChasGotovn_timer = APV_Tgot * 1000;
	}
	OZZ_state = 0;
	Timer_OZZ = 0;
	LED11b = false;
	VV_On_cnt++;
	Save_State_f = true;
}

void VV_off_event(void)
{
	ChasGotovn_timer = 0;
}

void btn_process(unsigned char* ret_value, unsigned char btn_nbr, bool btn_state, unsigned char mode)
{
	if(!VV_Block || (btn_nbr == 30) || (btn_nbr == 35) || (btn_nbr > 40))
	{
		switch(mode)
		{
			case 0:
			{
				if(btn_state)
				{
					if(cb[btn_nbr] != 3)
					{
						cb[btn_nbr]++;
						if(cb[btn_nbr] == 3)
						{
							*ret_value = btn_nbr;
						}
					}
				}
				else
				{
					if(cb[btn_nbr] != 0)
					{
						cb[btn_nbr]--;
						if(cb[btn_nbr] == 0)
						{
							*ret_value = btn_nbr + 0x80;
						}
					}
				}
				break;
			}
			case 1:
			{
				if(btn_state)
				{
					cb[btn_nbr]++;
					if(cb[btn_nbr] == 3)
					{
						cb_rep = 0;
						cb_rep_spd = 0;
						cb_rep_cnt = 0;
						*ret_value = btn_nbr;
					}
					else
					{
						cb_rep++;
						if(cb_rep >= cb_rep_spd_tab[cb_rep_spd])
						{
							cb_rep = 0;
							*ret_value = btn_nbr;
						}
					}
				}
				else
				{
					cb[btn_nbr] = 0;
				}
				break;
			}
		}
	}
}

void Keyboard_Task(void const * argument)
{
	if(HAL_GPIO_ReadPin(INPUT_1_GPIO_Port, INPUT_1_Pin))
	{
		cb[41] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_2_GPIO_Port, INPUT_2_Pin))
	{
		cb[42] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_3_GPIO_Port, INPUT_3_Pin))
	{
		cb[43] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_4_GPIO_Port, INPUT_4_Pin))
	{
		cb[44] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_5_GPIO_Port, INPUT_5_Pin))
	{
		cb[45] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_6_GPIO_Port, INPUT_6_Pin))
	{
		cb[46] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_7_GPIO_Port, INPUT_7_Pin))
	{
		cb[47] = 3;
	}
	if(HAL_GPIO_ReadPin(INPUT_8_GPIO_Port, INPUT_8_Pin))
	{
		cb[48] = 3;
	}
	while(1)
	{
		Task_control &= 0xFFFF - 2;
		switch(kbrd_state)
		{
			case 0:
			{
				HAL_GPIO_WritePin(KBRD_O1_GPIO_Port, KBRD_O1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(KBRD_O2_GPIO_Port, KBRD_O2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O4_GPIO_Port, KBRD_O4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O5_GPIO_Port, KBRD_O5_Pin, GPIO_PIN_SET);

				btn_process(&kbrd_code, 1, !HAL_GPIO_ReadPin(KBRD_I1_GPIO_Port, KBRD_I1_Pin), 1);
				btn_process(&kbrd_code, 6, !HAL_GPIO_ReadPin(KBRD_I2_GPIO_Port, KBRD_I2_Pin), 1);
				btn_process(&kbrd_code, 11, !HAL_GPIO_ReadPin(KBRD_I3_GPIO_Port, KBRD_I3_Pin), 1);
				btn_process(&kbrd_code, 16, !HAL_GPIO_ReadPin(KBRD_I4_GPIO_Port, KBRD_I4_Pin), 1);
				btn_process(&kbrd_code, 21, !HAL_GPIO_ReadPin(KBRD_I5_GPIO_Port, KBRD_I5_Pin), 1);
				btn_process(&kbrd_code, 26, !HAL_GPIO_ReadPin(KBRD_I6_GPIO_Port, KBRD_I6_Pin), 1);
				btn_process(&kbrd_code, 31, !HAL_GPIO_ReadPin(KBRD_I7_GPIO_Port, KBRD_I7_Pin), 0);
				break;
			}
			case 1:
			{
				HAL_GPIO_WritePin(KBRD_O1_GPIO_Port, KBRD_O1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O4_GPIO_Port, KBRD_O4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O5_GPIO_Port, KBRD_O5_Pin, GPIO_PIN_SET);

				btn_process(&kbrd_code, 2, !HAL_GPIO_ReadPin(KBRD_I1_GPIO_Port, KBRD_I1_Pin), 1);
				btn_process(&kbrd_code, 7, !HAL_GPIO_ReadPin(KBRD_I2_GPIO_Port, KBRD_I2_Pin), 1);
				btn_process(&kbrd_code, 12, !HAL_GPIO_ReadPin(KBRD_I3_GPIO_Port, KBRD_I3_Pin), 1);
				btn_process(&kbrd_code, 17, !HAL_GPIO_ReadPin(KBRD_I4_GPIO_Port, KBRD_I4_Pin), 1);
				btn_process(&kbrd_code, 22, !HAL_GPIO_ReadPin(KBRD_I5_GPIO_Port, KBRD_I5_Pin), 1);
				if(!VV_Block)
				{
					if(!HAL_GPIO_ReadPin(KBRD_I7_GPIO_Port, KBRD_I7_Pin))
					{
						if(cb32 != 400)
						{
							cb32++;
							if(cb32 == 400)
							{
								if((Setup2_View != 0) && (Setup2_View != 1))
								{
									Setup2_En = true;
									Setup2_View_cnt = 0;
									Setup2_En_cnt = 15000;
								}
							}
						}
					}
					else
					{
						if(cb32 != 400)
						{
							if(cb32 > 3)
							{
								if(Setup2_En)
								{
									Setup2End();
									Setup2_En_cnt = 0;
								}
								else
								{
									if(Setup_En)
									{
										Setup1End();
										Setup_En_cnt = 0;
									}
									else
									{
										Setup_En = true;
										Setup_En_cnt = 120000;
										MSZ1vv_tmp = MSZ1vv;
										SVvv_tmp = SVvv;
										APV2vv_tmp = APV2vv;
										ZMN_APVvv_tmp = ZMN_APVvv;
										Imsz1_tmp = Imsz1; Tmsz1_tmp = Tmsz1; Imsz2_tmp = Imsz2; Tmsz2_tmp = Tmsz2; Isv_tmp = Isv; Tsv_tmp = Tsv;
										APV_Tgot_tmp = APV_Tgot; APV1_T_tmp = APV1_T; APV2_T_tmp = APV2_T; ZMN_level_tmp = ZMN_level; ZMN_Tvidkl_tmp = ZMN_Tvidkl; ZMN_Tvkl_tmp = ZMN_Tvkl;
									}
								}
							}
						}
						cb32 = 0;
					}
				}
				if(!VV_Block)
				{
					if(!HAL_GPIO_ReadPin(KEY_34_GPIO_Port, KEY_34_Pin))
					{
						if(cb34 != 100)
						{
							cb34++;
							if(cb34 == 100)
							{
								kbrd_code = 34;
							}
						}
					}
					else
					{
						cb34 = 0;
					}
				}
				break;
			}
			case 2:
			{
				HAL_GPIO_WritePin(KBRD_O1_GPIO_Port, KBRD_O1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O3_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(KBRD_O4_GPIO_Port, KBRD_O4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O5_GPIO_Port, KBRD_O5_Pin, GPIO_PIN_SET);

				btn_process(&kbrd_code, 3, !HAL_GPIO_ReadPin(KBRD_I1_GPIO_Port, KBRD_I1_Pin), 1);
				btn_process(&kbrd_code, 8, !HAL_GPIO_ReadPin(KBRD_I2_GPIO_Port, KBRD_I2_Pin), 1);
				btn_process(&kbrd_code, 13, !HAL_GPIO_ReadPin(KBRD_I3_GPIO_Port, KBRD_I3_Pin), 1);
				btn_process(&kbrd_code, 18, !HAL_GPIO_ReadPin(KBRD_I4_GPIO_Port, KBRD_I4_Pin), 1);
				btn_process(&kbrd_code, 23, !HAL_GPIO_ReadPin(KBRD_I5_GPIO_Port, KBRD_I5_Pin), 1);
				btn_process(&kbrd_code, 28, !HAL_GPIO_ReadPin(KBRD_I6_GPIO_Port, KBRD_I6_Pin), 0);
				btn_process(&kbrd_code, 33, !HAL_GPIO_ReadPin(KBRD_I7_GPIO_Port, KBRD_I7_Pin), 0);
				break;
			}
			case 3:
			{
				HAL_GPIO_WritePin(KBRD_O1_GPIO_Port, KBRD_O1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O4_GPIO_Port, KBRD_O4_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(KBRD_O5_GPIO_Port, KBRD_O5_Pin, GPIO_PIN_SET);

				btn_process(&kbrd_code, 4, !HAL_GPIO_ReadPin(KBRD_I1_GPIO_Port, KBRD_I1_Pin), 1);
				btn_process(&kbrd_code, 9, !HAL_GPIO_ReadPin(KBRD_I2_GPIO_Port, KBRD_I2_Pin), 1);
				btn_process(&kbrd_code, 14, !HAL_GPIO_ReadPin(KBRD_I3_GPIO_Port, KBRD_I3_Pin), 1);
				btn_process(&kbrd_code, 19, !HAL_GPIO_ReadPin(KBRD_I4_GPIO_Port, KBRD_I4_Pin), 1);
				btn_process(&kbrd_code, 24, !HAL_GPIO_ReadPin(KBRD_I5_GPIO_Port, KBRD_I5_Pin), 1);
				btn_process(&kbrd_code, 29, !HAL_GPIO_ReadPin(KBRD_I6_GPIO_Port, KBRD_I6_Pin), 1);
				break;
			}
			case 4:
			{
				HAL_GPIO_WritePin(KBRD_O1_GPIO_Port, KBRD_O1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O3_GPIO_Port, KBRD_O3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O4_GPIO_Port, KBRD_O4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(KBRD_O5_GPIO_Port, KBRD_O5_Pin, GPIO_PIN_RESET);

				btn_process(&kbrd_code, 5, !HAL_GPIO_ReadPin(KBRD_I1_GPIO_Port, KBRD_I1_Pin), 1);
				btn_process(&kbrd_code, 10, !HAL_GPIO_ReadPin(KBRD_I2_GPIO_Port, KBRD_I2_Pin), 1);
				btn_process(&kbrd_code, 15, !HAL_GPIO_ReadPin(KBRD_I3_GPIO_Port, KBRD_I3_Pin), 1);
				btn_process(&kbrd_code, 20, !HAL_GPIO_ReadPin(KBRD_I4_GPIO_Port, KBRD_I4_Pin), 1);
				btn_process(&kbrd_code, 25, !HAL_GPIO_ReadPin(KBRD_I5_GPIO_Port, KBRD_I5_Pin), 1);
				btn_process(&kbrd_code, 30, !HAL_GPIO_ReadPin(KBRD_I6_GPIO_Port, KBRD_I6_Pin), 1);

				btn_process(&kbrd_code, 35, !HAL_GPIO_ReadPin(KEY_35_GPIO_Port, KEY_35_Pin), 0);
				btn_process(&kbrd_code, 41, !HAL_GPIO_ReadPin(INPUT_1_GPIO_Port, INPUT_1_Pin), 0);
				if(kbrd_code == 41)
				{
					VV_Off_State = true;
					VV_off_event();
					kbrd_code = 0;
					Inputs_State &= 0xFF - 1;
				}
				if(kbrd_code == (41 + 0x80))
				{
					VV_Off_State = false;
					kbrd_code = 0;
					Inputs_State |= 1;
				}
				btn_process(&kbrd_code, 42, !HAL_GPIO_ReadPin(INPUT_2_GPIO_Port, INPUT_2_Pin), 0);
				if(kbrd_code == 42)
				{
					VV_On_State = true;
					VV_on_event();
					kbrd_code = 0;
					Inputs_State &= 0xFF - 2;
				}
				if(kbrd_code == (42 + 0x80))
				{
					VV_On_State = false;
					kbrd_code = 0;
					Inputs_State |= 2;
				}
				btn_process(&kbrd_code, 43, !HAL_GPIO_ReadPin(INPUT_3_GPIO_Port, INPUT_3_Pin), 0);
				if(kbrd_code == 43)
				{
					VV_Ready_State = true;
					kbrd_code = 0;
					Inputs_State &= 0xFF - 4;
				}
				if(kbrd_code == (43 + 0x80))
				{
					VV_Ready_State = false;
					kbrd_code = 0;
					Inputs_State |= 4;
				}
				btn_process(&kbrd_code, 44, !HAL_GPIO_ReadPin(INPUT_4_GPIO_Port, INPUT_4_Pin), 0);
				if(kbrd_code == 44)
				{
					LED_5 = true;
					kbrd_code = 0;
					Inputs_State &= 0xFF - 8;
				}
				if(kbrd_code == (44 + 0x80))
				{
					LED_5 = false;
					kbrd_code = 0;
					Inputs_State |= 8;
				}
				btn_process(&kbrd_code, 45, !HAL_GPIO_ReadPin(INPUT_5_GPIO_Port, INPUT_5_Pin), 0);
				if(kbrd_code == 45)
				{
					LED_1 = true;
					kbrd_code = 0;
					Inputs_State &= 0xFF - 16;
				}
				if(kbrd_code == (45 + 0x80))
				{
					LED_1 = false;
					kbrd_code = 0;
					Inputs_State |= 16;
				}
				btn_process(&kbrd_code, 46, !HAL_GPIO_ReadPin(INPUT_6_GPIO_Port, INPUT_6_Pin), 0);
				if(kbrd_code == 46)
				{
					LED_On_f = true;
					kbrd_code = 0;
					Inputs_State &= 0xFF - 32;
					IEC104_BuildSignal_Spon(0, 1, ENDDEVICE_TYPE_HXTM+1);

				}
				if(kbrd_code == (46 + 0x80))
				{
					LED_Off_f = true;
					HAL_GPIO_WritePin(D_14_GPIO_Port, D_14_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(D_15_GPIO_Port, D_15_Pin, GPIO_PIN_RESET);
					kbrd_code = 0;
					Inputs_State |= 32;
					IEC104_BuildSignal_Spon(0, 0, ENDDEVICE_TYPE_HXTM+1);
				}
				btn_process(&kbrd_code, 47, !HAL_GPIO_ReadPin(INPUT_7_GPIO_Port, INPUT_7_Pin), 0);
				if(kbrd_code == 47)
				{
					kbrd_code = 0;
					Inputs_State &= 0xFF - 64;
				}
				if(kbrd_code == (47 + 0x80))
				{
					kbrd_code = 0;
					Inputs_State |= 64;
				}
				btn_process(&kbrd_code, 48, !HAL_GPIO_ReadPin(INPUT_8_GPIO_Port, INPUT_8_Pin), 0);
				if(kbrd_code == 48)
				{
					LED_4 = true;
					kbrd_code = 0;
					Inputs_State &= 0xFF - 128;
				}
				if(kbrd_code == (48 + 0x80))
				{
					LED_4 = false;
					kbrd_code = 0;
					Inputs_State |= 128;
				}
				break;
			}
		}
		
		if(kbrd_code != 0)
		{
			if((!DUvv) && (kbrd_code != 33))
			{
				DUvv_cnt = 43200;
			}

			if(Setup_En)
			{
				switch(kbrd_code)
				{
					case 1:
					{
						if(Imsz1_tmp != Imsz1_MIN)
						{
							Imsz1_tmp--;
						}
						else
						{
							MSZ1vv_tmp = false;
						}
						break;
					}
					case 2:
					{
						if(Imsz1_tmp != Imsz1_MAX)
						{
							Imsz1_tmp++;
							MSZ1vv_tmp = true;
						}
						break;
					}
					case 3:
					{
						if((Tmsz1_tmp != Tmsz1_MIN) && (MSZ1vv_tmp))
						{
							Tmsz1_tmp--;
						}
						break;
					}
					case 4:
					{
						if((Tmsz1_tmp != Tmsz1_MAX) && (MSZ1vv_tmp))
						{
							Tmsz1_tmp++;
						}
						break;
					}
					case 5:
					{
						if(Imsz2_tmp != Imsz2_MIN)
						{
							Imsz2_tmp--;
						}
						break;
					}
					case 6:
					{
						if(Imsz2_tmp != Imsz2_MAX)
						{
							Imsz2_tmp++;
						}
						break;
					}
					case 7:
					{
						if(Tmsz2_tmp != Tmsz2_MIN)
						{
							Tmsz2_tmp--;
						}
						break;
					}
					case 8:
					{
						if(Tmsz2_tmp != Tmsz2_MAX)
						{
							Tmsz2_tmp++;
						}
						break;
					}
					case 9:
					{
						if(Isv_tmp != Isv_MIN)
						{
							Isv_tmp--;
						}
						else
						{
							SVvv_tmp = false;
						}
						break;
					}
					case 10:
					{
						if(Isv_tmp != Isv_MAX)
						{
							Isv_tmp++;
							SVvv_tmp = true;
						}
					}
					case 11:
					{
						if((Tsv_tmp != Tsv_MIN) && (SVvv_tmp))
						{
							Tsv_tmp--;
						}
						break;
					}
					case 12:
					{
						if((Tsv_tmp != Tsv_MAX) && (SVvv_tmp))
						{
							Tsv_tmp++;
						}
						break;
					}
					case 15:
					{
						if(APV_Tgot_tmp != APV_Tgot_MIN)
						{
							APV_Tgot_tmp--;
						}
						break;
					}
					case 16:
					{
						if(APV_Tgot_tmp != APV_Tgot_MAX)
						{
							APV_Tgot_tmp++;
						}
						break;
					}
					case 17:
					{
						if(APV1_T_tmp != APV1_T_MIN)
						{
							APV1_T_tmp--;
						}
						break;
					}
					case 18:
					{
						if(APV1_T_tmp != APV1_T_MAX)
						{
							APV1_T_tmp++;
						}
						break;
					}
					case 19:
					{
						if(APV2_T_tmp != APV2_T_MIN)
						{
							APV2_T_tmp--;
						}
						else
						{
							APV2vv_tmp = false;
						}
						break;
					}
					case 20:
					{
						if(APV2_T_tmp != APV2_T_MAX)
						{
							APV2_T_tmp++;
							APV2vv_tmp = true;
						}
						break;
					}
					case 21:
					{
						if(ZMN_level_tmp != ZMN_level_MIN)
						{
							ZMN_level_tmp -= 10;
						}
						break;
					}
					case 22:
					{
						if(ZMN_level_tmp != ZMN_level_MAX)
						{
							ZMN_level_tmp += 10;
						}
						break;
					}
					case 23:
					{
						if(ZMN_Tvidkl_tmp != ZMN_Tvidkl_MIN)
						{
							ZMN_Tvidkl_tmp--;
						}
						break;
					}
					case 24:
					{
						if(ZMN_Tvidkl_tmp != ZMN_Tvidkl_MAX)
						{
							ZMN_Tvidkl_tmp++;
						}
						break;
					}
					case 25:
					{
						if(ZMN_Tvkl_tmp != ZMN_Tvkl_MIN)
						{
							ZMN_Tvkl_tmp--;
						}
						else
						{
							ZMN_APVvv_tmp = false;
						}
						break;
					}
					case 26:
					{
						if(ZMN_Tvkl_tmp != ZMN_Tvkl_MAX)
						{
							ZMN_Tvkl_tmp++;
							ZMN_APVvv_tmp = true;
						}
						break;
					}
				}
				LED_Refresh_f = true;
				Setup_En_cnt = 120000;
			}
			if(Setup2_En)
			{
				switch(kbrd_code)
				{
					case 13:
					{
						Setup2_En_cnt = 15000;
						if(Setup2_View == 2)
						{
							Ktt--;
							if(Ktt == 255)
							{
								Ktt = 5;
							}
						}
						if(Setup2_View == 3)
						{
							MB1_adress--;
							if(MB1_adress == 0)
							{
								MB1_adress = 255;
							}
						}
						if(Setup2_View == 4)
						{
							MB1_speed--;
							if(MB1_speed == 255)
							{
								MB1_speed = 4;
							}
						}
						break;
					}
					case 14:
					{
						Setup2_En_cnt = 15000;
						if(Setup2_View == 2)
						{
							Ktt++;
							if(Ktt == 6)
							{
								Ktt = 0;
							}
						}
						if(Setup2_View == 3)
						{
							MB1_adress++;
							if(MB1_adress == 0)
							{
								MB1_adress = 1;
							}
						}
						if(Setup2_View == 4)
						{
							MB1_speed++;
							if(MB1_speed == 5)
							{
								MB1_speed = 0;
							}
						}
						break;
					}
				}
			}
			else
			{
				switch(kbrd_code)
				{
					case 13:
					{
						Setup2_View_cnt = 30000;
						Setup2_View--;
						if(Setup2_View == 255)
						{
							Setup2_View = 4;
						}
						break;
					}
					case 14:
					{
						Setup2_View_cnt = 30000;
						Setup2_View++;
						if(Setup2_View == 5)
						{
							Setup2_View = 0;
						}
						break;
					}
				}
			}
			switch(kbrd_code)
			{
				case 28:
				{
					if(APVvv)
					{
						APVvv = false;
					}
					else
					{
						APVvv = true;
					}
					Save_Settings_f = true;
					break;
				}
				case 29:
				{
					if(ZMNvv)
					{
						ZMNvv = false;
					}
					else
					{
						ZMNvv = true;
					}
					Save_Settings_f = true;
					break;
				}
				case 30:
				{
					VV_Unblock_f = true;
					break;
				}
				case 31:
				{
					LED_Chk_On_f = true;
					break;
				}
				case (31 + 0x80):
				{
					LED_Chk_Off_f = true;
					break;
				}
				case 33:
				{
					if(DUvv)
					{
						DUvv = false;
						APVvv = false;
						ZMNvv = false;
						DUvv_cnt = 43200;
					}
					else
					{
						DUvv = true;
						DUvv_cnt = 0;
					}
					Save_Settings_f = true;
					break;
				}
				case 34:
				{
					if(HAL_GPIO_ReadPin(KEY_35_GPIO_Port, KEY_35_Pin))
					{
						Command_VV_On = true;
						VV_On_Sourse_tmp = 1;
					}
					break;
				}
				case 35:
				{
					Command_VV_Off = true;
					VV_Off_Sourse_tmp = 1;
					break;
				}
			}
			LED_Refresh_f = true;
			kbrd_code = 0;
		}
		kbrd_state++;
		if(kbrd_state == 5)
		{
			kbrd_state = 0;

			if(cb_rep != 0)
			{
				if(cb_rep_cnt != 200)
				{
					cb_rep_cnt++;
					if(cb_rep_cnt == 50)
					{
						cb_rep_spd = 1;
					}
					if(cb_rep_cnt == 100)
					{
						cb_rep_spd = 2;
					}
					if(cb_rep_cnt == 200)
					{
						cb_rep_spd = 3;
					}
				}
			}
 			osDelay(20);
		}
	}
}
