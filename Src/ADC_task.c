#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "math.h"
#include "main.h"
#include "LED.h"

const float MSZ_gist = 1;
unsigned long VrmsAfiltr = 0, IrmsAfiltr = 0, IrmsCfiltr = 0;
unsigned long VrmsAtmp = 0, IrmsAtmp = 0, IrmsCtmp = 0;
unsigned char MSZ_stage = 0;
unsigned int VrmsA_ist;
unsigned int VrmsAbuf[160], IrmsAbuf[160], IrmsCbuf[160];
unsigned short int ADC_filtr_cnt = 0;

void ADC_Task(void const * argument)
{
	while(1)
	{
		Task_control &= 0xFFFF - 1;
		if(ADC_RST_f == true)
		{
			HAL_GPIO_WritePin(ADC_RESET_GPIO_Port, ADC_RESET_Pin, GPIO_PIN_SET);
			osDelay(1);
			HAL_GPIO_WritePin(ADC_RESET_GPIO_Port, ADC_RESET_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ADC_CONVRS_GPIO_Port, ADC_CONVRS_Pin, GPIO_PIN_RESET);
			ADC_RST_f = false;
		}
		if(RMS_process)
		{
			unsigned long IrmsAtmp = 0, IrmsCtmp = 0, VrmsAtmp = 0;
			
			for(unsigned char cnt = 0; cnt < 39; cnt++)
			{
				VrmsAbuf[cnt] = VrmsAbuf[cnt + 1];
				VrmsAtmp += VrmsAbuf[cnt];
				IrmsAbuf[cnt] = IrmsAbuf[cnt + 1];
				IrmsAtmp += IrmsAbuf[cnt];
				IrmsCbuf[cnt] = IrmsCbuf[cnt + 1];
				IrmsCtmp += IrmsCbuf[cnt];
			}
			VrmsAbuf[39] = VrmsAsend * VrmsAsend;
			IrmsAbuf[39] = IrmsAsend * IrmsAsend;
			IrmsCbuf[39] = IrmsCsend * IrmsCsend;
			VrmsAtmp += VrmsAbuf[39];
			IrmsAtmp += IrmsAbuf[39];
			IrmsCtmp += IrmsCbuf[39];
			VrmsAfiltr += VrmsAtmp / 40;
			IrmsAfiltr += IrmsAtmp / 40;
			IrmsCfiltr += IrmsCtmp / 40;

			ADC_filtr_cnt++;
			if(ADC_filtr_cnt == 10)
			{
				ADC_filtr_cnt = 0;
				VrmsA_ist = VrmsA;
				if(ADC_Start_Timer == 0)
				{
					if(!Debug_mode)
					{
						IrmsA = round(((sqrt((float)(IrmsAfiltr / 10))) * _K_IA) * Ktt);
						IrmsC = round(((sqrt((float)(IrmsCfiltr / 10))) * _K_IC) * Ktt);
						VrmsA = round(((sqrt((float)(VrmsAfiltr / 10))) * _K_UA) * Ktn);
					}
					else
					{
						if(VV_On_State)
						{
							IrmsA = IrmsA_d;
							IrmsC = IrmsC_d;
						}
						else
						{
							IrmsA = 0;
							IrmsC = 0;
						}
						VrmsA = VrmsA_d;
					}
				}
				else
				{
					IrmsA = 0;
					IrmsC = 0;
					VrmsA = 0;
				}
				if(VrmsA < (unsigned short int)(20 * Ktn))
				{
					VrmsA = 0;
				}
				VrmsAfiltr = 0;
				IrmsAfiltr = 0;
				IrmsCfiltr = 0;

//MB_PuskMSZ1, MB_VozvratMSZ1, MB_WorkMSZ1;
//MB_PuskMSZ2, MB_VozvratMSZ2, MB_WorkMSZ2;
//MB_PuskSV, MB_WorkSV, MB_GotovAPV;
//MB_PuskAPV1, MB_WorkAPV1, MB_PuskAPV2, MB_WorkAPV2;
				switch(MSZ_state)
				{
					case 0:
					{
						if(IrmsA > Imsz2)
						{
							if(MSZ2A_timer == 0)
							{
								MSZ2A_timer = Tmsz2 * 100;
								MB_PuskMSZ2 = true;
								MB_VozvratMSZ2 = false;
							}
							if(MSZ_APV_cikl != 0)
							{
								MSZ_state = 1;
								MSZ_Worked_flags |= 2;
							}
						}
						if(IrmsA < (Imsz2 * MSZ_gist))
						{
							if(MSZ2A_timer != 0)
							{
								MSZ2A_timer = 0;
								MB_VozvratMSZ2 = true;
								MB_PuskMSZ2 = false;
							}
						}
						if(IrmsC > Imsz2)
						{
							if(MSZ2B_timer == 0)
							{
								MSZ2B_timer = Tmsz2 * 100;
								MB_PuskMSZ2 = true;
								MB_VozvratMSZ2 = false;
							}
							if(MSZ_APV_cikl != 0)
							{
								MSZ_state = 1;
								MSZ_Worked_flags |= 2;
							}
						}
						if(IrmsC < (Imsz2 * MSZ_gist))
						{
							if(MSZ2B_timer != 0)
							{
								MSZ2B_timer = 0;
								MB_VozvratMSZ2 = true;
								MB_PuskMSZ2 = false;
							}
						}

						if(SVvv)
						{
							if(IrmsA > Isv)
							{
								if(MSZsvA_timer == 0)
								{
									MSZsvA_timer = Tsv * 10;
									MB_PuskSV = true;
								}
								if((MSZ_APV_cikl != 0) || (Tsv == 0))
								{
									MSZ_state = 1;
									MSZ_Worked_flags |= 4;
								}
							}
							if(IrmsA < (Isv * MSZ_gist))
							{
								MSZsvA_timer = 0;
							}
							if(IrmsC > Isv)
							{
								if(MSZsvB_timer == 0)
								{
									MSZsvB_timer = Tsv * 10;
									MB_PuskSV = true;
								}
								if((MSZ_APV_cikl != 0)  || (Tsv == 0))
								{
									MSZ_state = 1;
									MSZ_Worked_flags |= 4;
								}
							}
							if(IrmsC < (Isv * MSZ_gist))
							{
								MSZsvB_timer = 0;
							}
						}
						if(MSZ1vv)
						{
							if(IrmsA > Imsz1)
							{
								if(Imsz1A == 0)
								{
									MSZ1A_timer = 0;
									MB_PuskMSZ1 = true;
									MB_VozvratMSZ1 = false;
								}
								if((IrmsA > Imsz1A) && (MSZ2A_timer == 0))
								{
									Imsz1A = IrmsA;
									Tmsz1A = (Tmsz1 * 1000) + (((double)(Tmsz2 * 100) - (double)(Tmsz1 * 1000)) / ((double)Imsz2 - (double)Imsz1)) * (IrmsA - Imsz1);
								}
								if(MSZ_APV_cikl != 0)
								{
									MSZ_state = 1;
									MSZ_Worked_flags |= 1;
								}
							}
							if(IrmsA < (Imsz1 * MSZ_gist))
							{
								if(Imsz1A != 0)
								{
									Imsz1A = 0;
									MB_VozvratMSZ1 = true;
									MB_PuskMSZ1 = false;
								}
							}
							if((Imsz1A != 0) && (MSZ1A_timer > Tmsz1A))
							{
								MSZ_state = 1;
								MSZ_Worked_flags |= 1;
							}

							if(IrmsC > Imsz1)
							{
								if(Imsz1B == 0)
								{
									MSZ1B_timer = 0;
									MB_PuskMSZ1 = true;
									MB_VozvratMSZ1 = false;
								}
								if((IrmsC > Imsz1B) && (MSZ2B_timer == 0))
								{
									Imsz1B = IrmsC;
									Tmsz1B = (Tmsz1 * 1000) + (((double)(Tmsz2 * 100) - (double)(Tmsz1 * 1000)) / ((double)Imsz2 - (double)Imsz1)) * (IrmsC - Imsz1);
								}
								if(MSZ_APV_cikl != 0)
								{
									MSZ_state = 1;
									MSZ_Worked_flags |= 1;
								}
							}
							if(IrmsC < (Imsz1 * MSZ_gist))
							{
								if(Imsz1B != 0)
								{
									Imsz1B = 0;
									MB_VozvratMSZ1 = true;
									MB_PuskMSZ1 = false;
								}
							}
							if((Imsz1B != 0) && (MSZ1B_timer > Tmsz1B))
							{
								MSZ_state = 1;
								MSZ_Worked_flags |= 1;
							}
						}
						break;
					}
					case 1:
					{
						if(MSZ_Worked_flags & 1)
						{
							MB_WorkMSZ1 = true;
							MB_PuskMSZ1 = false;
							MB_PuskMSZ2 = false;
							MB_PuskSV = false;
						}
						if(MSZ_Worked_flags & 2)
						{
							MB_WorkMSZ2 = true;
							MB_PuskMSZ1 = false;
							MB_PuskMSZ2 = false;
							MB_PuskSV = false;
						}
						if(MSZ_Worked_flags & 4)
						{
							MB_WorkSV = true;
							MB_PuskMSZ1 = false;
							MB_PuskMSZ2 = false;
							MB_PuskSV = false;
						}
						Imsz1A = 0;
						Imsz1B = 0;
						MSZ2A_timer = 0;
						MSZsvA_timer = 0;
						MSZ2B_timer = 0;
						MSZsvB_timer = 0;
						Command_VV_Off = true;
						VV_Off_Sourse_tmp = 3;
						IrmsA_poshk = IrmsA;
						IrmsC_poshk = IrmsC;
						Save_State_f = true;
//						if(((MSZ_Worked_flags & 1) && (Tmsz1A > (APV_Tgot * 1000))) || ((MSZ_Worked_flags & 1) && (Tmsz1B > (APV_Tgot * 1000))) || ((MSZ_Worked_flags & 2) && ((APV_Tgot * 1000) < (Tmsz2 * 100))) || ((MSZ_Worked_flags & 4) && ((APV_Tgot * 1000) < (Tsv * 10))))
/*						if(((MSZ_Worked_flags & 1) && (Tmsz1A > (APV_Tgot * 1000))) || ((MSZ_Worked_flags & 1) && (Tmsz1B > (APV_Tgot * 1000))))
						{
							ChasGotovn_timer = 0;
							VV_Block = true;
							MSZ_state = 10;
							break;
						}
*/
						if(((ChasGotovn_timer == 0) || (MSZ_APV_cikl == 1) || (MSZ_APV_cikl == 2)) && APVvv)
						{
							MSZ_APV_cikl++;
							if(MSZ_APV_cikl == 1)
							{
								MSZ_Worked_flags |= 64;
								MSZ_APV_timer = APV1_T * 100;
								MSZ_state = 2;
								MB_PuskAPV1 = true;
								break;
							}
							if((MSZ_APV_cikl == 2) && APV2vv)
							{
								MSZ_Worked_flags |= 128;
								MSZ_APV_timer = APV2_T * 1000;
								MSZ_state = 2;
								MB_PuskAPV2 = true;
								break;
							}
						}
						ChasGotovn_timer = 0;
						VV_Block = true;
						Save_State_f = true;
						MSZ_state = 10;
						break;
					}
					case 2:
					{
						if(MSZ_APV_timer == 0)
						{
							if(MB_PuskAPV1)
							{
								MB_WorkAPV1 = true;
								MB_PuskAPV1 = false;
							}
							if(MB_PuskAPV2)
							{
								MB_WorkAPV2 = true;
								MB_PuskAPV2 = false;
							}
							Command_VV_On = true;
							VV_On_Sourse_tmp = 3;
							MSZ_state = 0;
						}
						break;
					}
					case 10:
					{
						
						break;
					}
				}
				if(ZMNvv && !VV_Block)
				{
					switch(OZZ_state)
					{
						case 0:
						{
							if((VrmsA < (((unsigned short int)(Ubazovoe * Ktn) * ZMN_level) / 100)) && (VV_On_State) && (!VV_Off_State))
							{
								if(Timer_OZZ == 0)
								{
									Timer_OZZ = ZMN_Tvidkl * 1000;
									MB_PuskZMN = true;
									MB_VozvratZMN = false;
									LED12b = true;
								}
							}
							else
							{
								if(Timer_OZZ != 0)
								{
									Timer_OZZ = 0;
									MB_VozvratZMN = true;
									MB_PuskZMN = false;
									LED12b = false;
								}
							}
							break;
						}
						case 1:
						{
							LED11b = true;
							Command_VV_Off = true;
							VV_Off_Sourse_tmp = 3;
							MB_WorkZMN = true;
							MB_PuskZMN = false;
							MB_VozvratAPV_ZMN = false;
							LED12b = false;
							VrmsA_poshk = VrmsA;
							Save_State_f = true;
							if(!ZMN_APVvv)
							{
								OZZ_state = 0;
								VV_Block = true;
								Save_State_f = true;
							}
							else
							{
								OZZ_state = 2;
								Save_State_f = true;
							}
							break;
						}
						case 2:
						{
							LED11b = true;
							if(VrmsA > (((unsigned short int)(Ubazovoe * Ktn) * ZMN_level) / 100))
							{
								if(ZMN_APVvv)
								{
									if(Timer_OZZ == 0)
									{
										Timer_OZZ = ZMN_Tvkl * 1000;
										MB_PuskAPV_ZMN = true;
										MB_VozvratAPV_ZMN = false;
										LED13b = true;
									}
								}
								else
								{
									OZZ_state = 3;
								}
							}
							else
							{
								if(Timer_OZZ != 0)
								{
									Timer_OZZ = 0;
									MB_PuskAPV_ZMN = false;
									MB_VozvratAPV_ZMN = true;
									LED13b = false;
								}
							}
							break;
						}
						case 3:
						{
							Command_VV_On = true;
							MB_WorkAPV_ZMN = true;
							MB_PuskAPV_ZMN = false;
							LED13b = false;
							VV_On_Sourse_tmp = 4;
							OZZ_state = 4;
							MB_WorkZMN = false;
							Timer_OZZ = ZMN_Tvidkl * 1000;
							break;
						}
						case 4:
						{
							if(VrmsA < (((unsigned short int)(Ubazovoe * Ktn) * ZMN_level) / 100))
							{
								Command_VV_Off = true;
								VV_Off_Sourse_tmp = 3;
								MB_WorkZMN = true;
								MB_VozvratAPV_ZMN = false;
								VrmsA_poshk = VrmsA;
								Save_State_f = true;
								VV_Block = true;
								Save_State_f = true;
							}
							break;
						}
						case 5:
						{
							OZZ_state = 0;
							Save_State_f = true;
							break;
						}
					}
				}
				else
				{
					Timer_OZZ = 0;
					if(OZZ_state != 0)
					{
						OZZ_state = 0;
						Save_State_f = true;
					}
				}
			}
			RMS_process = false;
		}
		osDelay(300);
	}
}
