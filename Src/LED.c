#include "LED.h"
#include "main.h"
#include "cmsis_os.h"

unsigned short int LED_Send_Buf[7];

//unsigned short int LTMP = 0, LTMP1 = 0;

void LED_Off(void)
{
	unsigned char i;
	
	LED_state = false;
  for(i = 0; i < 7; i++)
  {
		LED_Send_Buf[i] = 0x0C00;
	}
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);;
}

void LED_On(void)
{
	unsigned char i;
	
	LED_state = true;
  for(i = 0; i < 7; i++)
  {
		LED_Send_Buf[i] = 0x0C01;
	}
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
}

void LED_Clear(void)
{
	unsigned char i;
	
  for(i = 1; i < 9; i++)
  {
		LED_Send_Buf[0] = ((unsigned short int)i << 8) + 0x0F;
		LED_Send_Buf[1] = ((unsigned short int)i << 8) + 0x0F;
		LED_Send_Buf[2] = ((unsigned short int)i << 8) + 0x0F;
		LED_Send_Buf[3] = ((unsigned short int)i << 8) + 0x0F;
		LED_Send_Buf[4] = ((unsigned short int)i << 8) + 0x0F;
		LED_Send_Buf[5] = ((unsigned short int)i << 8) + 0x0F;
		if(i > 4)
		{
			LED_Send_Buf[6] = ((unsigned short int)i << 8);
		}
		else
		{
			LED_Send_Buf[6] = ((unsigned short int)i << 8) + 0x0F;
		}
		HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
		while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
  }
}

void LED_Init(void)
{
	LED_Send_Buf[0] = 0x09FF;
	LED_Send_Buf[1] = 0x09FF;
	LED_Send_Buf[2] = 0x09FF;
	LED_Send_Buf[3] = 0x09FF;
	LED_Send_Buf[4] = 0x09FF;
	LED_Send_Buf[5] = 0x09FF;
	LED_Send_Buf[6] = 0x090F;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	LED_Send_Buf[0] = 0x0B07;
	LED_Send_Buf[1] = 0x0B07;
	LED_Send_Buf[2] = 0x0B07;
	LED_Send_Buf[3] = 0x0B07;
	LED_Send_Buf[4] = 0x0B07;
	LED_Send_Buf[5] = 0x0B07;
	LED_Send_Buf[6] = 0x0B07;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	LED_Send_Buf[0] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[1] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[2] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[3] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[4] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[5] = 0x0A00 + LED_Brightness;
	LED_Send_Buf[6] = 0x0A00 + LED_Brightness;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	LED_Send_Buf[0] = 0x0C01;
	LED_Send_Buf[1] = 0x0C01;
	LED_Send_Buf[2] = 0x0C01;
	LED_Send_Buf[3] = 0x0C01;
	LED_Send_Buf[4] = 0x0C01;
	LED_Send_Buf[5] = 0x0C01;
	LED_Send_Buf[6] = 0x0C01;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	LED_Clear();
}

void LED_Chk_On(void)
{
	LED_Send_Buf[0] = 0x0F01;
	LED_Send_Buf[1] = 0x0F01;
	LED_Send_Buf[2] = 0x0F01;
	LED_Send_Buf[3] = 0x0F01;
	LED_Send_Buf[4] = 0x0F01;
	LED_Send_Buf[5] = 0x0F01;
	LED_Send_Buf[6] = 0x0F01;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	HAL_GPIO_WritePin(D_14_GPIO_Port, D_14_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D_15_GPIO_Port, D_15_Pin, GPIO_PIN_SET);
}

void LED_Chk_Off(void)
{
	LED_Send_Buf[0] = 0x0F00;
	LED_Send_Buf[1] = 0x0F00;
	LED_Send_Buf[2] = 0x0F00;
	LED_Send_Buf[3] = 0x0F00;
	LED_Send_Buf[4] = 0x0F00;
	LED_Send_Buf[5] = 0x0F00;
	LED_Send_Buf[6] = 0x0F00;
	HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
	while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	HAL_GPIO_WritePin(D_14_GPIO_Port, D_14_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D_15_GPIO_Port, D_15_Pin, GPIO_PIN_RESET);
}

void LED_Refresh(void)
{
	unsigned short int tmp, MAX_IC1[8], MAX_IC2[8], MAX_IC3[8], MAX_IC4[8], MAX_IC5[8], MAX_IC6[8], MAX_IC7[8];
	unsigned char i;

	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags == 1) || (Setup1Error_flags & 1)  || (Imsz1A != 0) || (Imsz1B != 0) || (MSZ_Worked_flags & 1) || LED1b))
	{
		MAX_IC7[0] = 0x0F;
		MAX_IC7[1] = 0x0F;
		MAX_IC7[2] = 0x0F;
		MAX_IC7[3] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			if(MSZ1vv_tmp)
			{
				tmp = Imsz1_tmp;
				MAX_IC7[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC7[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC7[2] = tmp / 10;
				MAX_IC7[3] = tmp % 10;
			}
			else
			{
				MAX_IC7[0] = 0x0A;
				MAX_IC7[1] = 0x0A;
				MAX_IC7[2] = 0x0A;
				MAX_IC7[3] = 0x0A;
			}
		}
		else
		{
			if(MSZ1vv)
			{
				tmp = Imsz1;
				MAX_IC7[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC7[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC7[2] = tmp / 10;
				MAX_IC7[3] = tmp % 10;
			}
			else
			{
				MAX_IC7[0] = 0x0A;
				MAX_IC7[1] = 0x0A;
				MAX_IC7[2] = 0x0A;
				MAX_IC7[3] = 0x0A;
			}
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags & 4) || LED2b))
	{
		MAX_IC6[0] = 0x0F;
		MAX_IC6[1] = 0x0F;
		MAX_IC6[2] = 0x0F;
		MAX_IC6[3] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			if(MSZ1vv_tmp)
			{
				tmp = Tmsz1_tmp;
				MAX_IC6[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC6[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC6[2] = tmp / 10;
				MAX_IC6[3] = tmp % 10;
			}
			else
			{
				MAX_IC6[0] = 0x0A;
				MAX_IC6[1] = 0x0A;
				MAX_IC6[2] = 0x0A;
				MAX_IC6[3] = 0x0A;
			}
		}
		else
		{
			if(MSZ1vv)
			{
				tmp = Tmsz1;
				MAX_IC6[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC6[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC6[2] = tmp / 10;
				MAX_IC6[3] = tmp % 10;
			}
			else
			{
				MAX_IC6[0] = 0x0A;
				MAX_IC6[1] = 0x0A;
				MAX_IC6[2] = 0x0A;
				MAX_IC6[3] = 0x0A;
			}
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags & 3) || (MSZ2A_timer != 0) || (MSZ2B_timer != 0) || (MSZ_Worked_flags & 2) || LED3b))
	{
		MAX_IC6[4] = 0x0F;
		MAX_IC6[5] = 0x0F;
		MAX_IC6[6] = 0x0F;
		MAX_IC6[7] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = Imsz2_tmp;
		}
		else
		{
			tmp = Imsz2;
		}
		MAX_IC6[4] = tmp / 1000;
		tmp = tmp % 1000;
		MAX_IC6[5] = tmp / 100;
		tmp = tmp % 100;
		MAX_IC6[6] = tmp / 10;
		MAX_IC6[7] = tmp % 10;
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags & 12) || LED4b))
	{
		MAX_IC5[4] = 0x0F;
		MAX_IC5[5] = 0x0F;
		MAX_IC5[6] = 0x0F;
		MAX_IC5[7] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = Tmsz2_tmp;
		}
		else
		{
			tmp = Tmsz2;
		}
		MAX_IC5[4] = tmp / 1000;
		tmp = tmp % 1000;
		MAX_IC5[5] = tmp / 100;
		tmp = tmp % 100;
		MAX_IC5[6] = (tmp / 10) + 0x80;
		MAX_IC5[7] = tmp % 10;
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags & 2)|| (MSZsvA_timer != 0) || (MSZsvB_timer != 0) || (MSZ_Worked_flags & 4) || LED5b))
	{
		MAX_IC5[0] = 0x0F;
		MAX_IC5[1] = 0x0F;
		MAX_IC5[2] = 0x0F;
		MAX_IC5[3] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			if(SVvv_tmp)
			{
				tmp = Isv_tmp;
				MAX_IC5[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC5[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC5[2] = tmp / 10;
				MAX_IC5[3] = tmp % 10;
			}
			else
			{
				MAX_IC5[0] = 0x0A;
				MAX_IC5[1] = 0x0A;
				MAX_IC5[2] = 0x0A;
				MAX_IC5[3] = 0x0A;
			}
		}
		else
		{
			if(SVvv)
			{
				tmp = Isv;
				MAX_IC5[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC5[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC5[2] = tmp / 10;
				MAX_IC5[3] = tmp % 10;
			}
			else
			{
				MAX_IC5[0] = 0x0A;
				MAX_IC5[1] = 0x0A;
				MAX_IC5[2] = 0x0A;
				MAX_IC5[3] = 0x0A;
			}
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (Setup1Error_flags & 8) || LED6b))
	{
		MAX_IC4[4] = 0x0F;
		MAX_IC4[5] = 0x0F;
		MAX_IC4[6] = 0x0F;
		MAX_IC4[7] = 0x0F;
	}
	else
	{


		if(Setup_En)
		{
			if(SVvv_tmp)
			{
				tmp = Tsv_tmp;
				MAX_IC4[4] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC4[5] = (tmp / 100) + 0x80;
				tmp = tmp % 100;
				MAX_IC4[6] = tmp / 10;
				MAX_IC4[7] = tmp % 10;
			}
			else
			{
				MAX_IC4[4] = 0x0A;
				MAX_IC4[5] = 0x0A;
				MAX_IC4[6] = 0x0A;
				MAX_IC4[7] = 0x0A;
			}
		}
		else
		{
			if(SVvv)
			{
				tmp = Tsv;
				MAX_IC4[4] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC4[5] = (tmp / 100) + 0x80;
				tmp = tmp % 100;
				MAX_IC4[6] = tmp / 10;
				MAX_IC4[7] = tmp % 10;
			}
			else
			{
				MAX_IC4[4] = 0x0A;
				MAX_IC4[5] = 0x0A;
				MAX_IC4[6] = 0x0A;
				MAX_IC4[7] = 0x0A;
			}
		}
	}
	if((Blink_cnt < 200) && (Setup2_En || LED7b))
	{
		MAX_IC4[0] = 0x0F;
		MAX_IC4[1] = 0x0F;
		MAX_IC4[2] = 0x0F;
		MAX_IC4[3] = 0x0F;
	}
	else
	{
		switch(Setup2_View)
		{
			case 0:
			{
				MAX_IC4[0] = Hour / 10;
				MAX_IC4[1] = Hour % 10;
				if((Blink_cnt > 200) && !(Internal_Faults & 4))
				{
					MAX_IC4[1] += 0x80;
				}
				MAX_IC4[2] = Minute / 10;
				MAX_IC4[3] = Minute % 10;
				break;
			}
			case 1:
			{
				tmp = (unsigned short int)(VrmsA / 1000);
				MAX_IC4[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC4[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC4[2] = tmp / 10;
				MAX_IC4[3] = tmp % 10;
				break;
			}
			case 2:
			{
				if(Blink_cnt < 500)
				{
					if(Ktt == 0)
					{
						tmp = 50;
					}
					if(Ktt == 1)
					{
						tmp = 100;
					}
					if(Ktt == 2)
					{
						tmp = 150;
					}
					if(Ktt == 3)
					{
						tmp = 200;
					}
					if(Ktt == 4)
					{
						tmp = 250;
					}
					if(Ktt == 5)
					{
						tmp = 400;
					}
					MAX_IC4[0] = tmp / 1000;
					tmp = tmp % 1000;
					MAX_IC4[1] = tmp / 100;
					tmp = tmp % 100;
					MAX_IC4[2] = tmp / 10;
					MAX_IC4[3] = tmp % 10;
				}
				else
				{
					MAX_IC4[0] = 0x0F;
					MAX_IC4[1] = 0x0F;
					MAX_IC4[2] = 0x0F;
					MAX_IC4[3] = 5;
				}
				break;
			}
			case 3:
			{
				tmp = MB1_adress;
				MAX_IC4[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC4[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC4[2] = tmp / 10;
				MAX_IC4[3] = tmp % 10;
				break;
			}
			case 4:
			{
				tmp = (unsigned int)(MB_BR[MB1_speed] / 100);
				MAX_IC4[0] = tmp / 1000;
				tmp = tmp % 1000;
				MAX_IC4[1] = tmp / 100;
				tmp = tmp % 100;
				MAX_IC4[2] = tmp / 10;
				MAX_IC4[3] = tmp % 10;
				break;
			}
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (ChasGotovn_timer != 0) || LED8b))
	{
		MAX_IC3[0] = 0x0F;
		MAX_IC3[1] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = APV_Tgot_tmp;
			MAX_IC3[0] = tmp / 10;
			MAX_IC3[1] = tmp % 10;
		}
		else
		{
			tmp = APV_Tgot;
			MAX_IC3[0] = tmp / 10;
			MAX_IC3[1] = tmp % 10;
		}
	}
	if(Internal_Faults & 128)
	{
		MAX_IC3[1] += 0x80;
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (MSZ_Worked_flags & 64) || LED9b))
	{
		MAX_IC3[2] = 0x0F;
		MAX_IC3[3] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = APV1_T_tmp;
			MAX_IC3[2] = (tmp / 10) + 0x80;
			MAX_IC3[3] = tmp % 10;
		}
		else
		{
			tmp = APV1_T;
			MAX_IC3[2] = (tmp / 10) + 0x80;
			MAX_IC3[3] = tmp % 10;
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || (MSZ_Worked_flags & 128) || LED10b))
	{
		MAX_IC3[4] = 0x0F;
		MAX_IC3[5] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			if(APV2vv_tmp)
			{
				tmp = APV2_T_tmp;
				MAX_IC3[4] = tmp / 10;
				MAX_IC3[5] = tmp % 10;
			}
			else
			{
				MAX_IC3[4] = 0x0A;
				MAX_IC3[5] = 0x0A;
			}
		}
		else
		{
			if(APV2vv)
			{
				tmp = APV2_T;
				MAX_IC3[4] = tmp / 10;
				MAX_IC3[5] = tmp % 10;
			}
			else
			{
				MAX_IC3[4] = 0x0A;
				MAX_IC3[5] = 0x0A;
			}
		}
	}
	MAX_IC3[6] = 0x0F;
	MAX_IC3[7] = 0x0F;

	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || LED11b))
	{
		MAX_IC2[4] = 0x0F;
		MAX_IC2[5] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = ZMN_level_tmp;
			MAX_IC2[4] = tmp / 10;
			MAX_IC2[5] = tmp % 10;
		}
		else
		{
			tmp = ZMN_level;
			MAX_IC2[4] = tmp / 10;
			MAX_IC2[5] = tmp % 10;
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || LED12b))
	{
			MAX_IC2[2] = 0x0F;
			MAX_IC2[3] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			tmp = ZMN_Tvidkl_tmp;
			MAX_IC2[2] = tmp / 10;
			MAX_IC2[3] = tmp % 10;
		}
		else
		{
			tmp = ZMN_Tvidkl;
			MAX_IC2[2] = tmp / 10;
			MAX_IC2[3] = tmp % 10;
		}
	}
	if((Blink_cnt < 200) && ((Setup_En && (Setup1Error_flags == 0)) || LED13b))
	{
		MAX_IC2[0] = 0x0F;
		MAX_IC2[1] = 0x0F;
	}
	else
	{
		if(Setup_En)
		{
			if(ZMN_APVvv_tmp)
			{
				tmp = ZMN_Tvkl_tmp;
				MAX_IC2[0] = tmp / 10;
				MAX_IC2[1] = tmp % 10;
			}
			else
			{
				MAX_IC2[0] = 0x0A;
				MAX_IC2[1] = 0x0A;
			}
		}
		else
		{
			if(ZMN_APVvv)
			{
				tmp = ZMN_Tvkl;
				MAX_IC2[0] = tmp / 10;
				MAX_IC2[1] = tmp % 10;
			}
			else
			{
				MAX_IC2[0] = 0x0A;
				MAX_IC2[1] = 0x0A;
			}
		}
	}
	MAX_IC2[6] = 0x0F;
	MAX_IC2[7] = 0x0F;

	tmp = (unsigned short int)IrmsA;
	MAX_IC1[0] = tmp / 1000;
	tmp = tmp % 1000;
	MAX_IC1[1] = tmp / 100;
	tmp = tmp % 100;
	MAX_IC1[2] = tmp / 10;
	MAX_IC1[3] = tmp % 10;
	if(!(Internal_Faults & 2))
	{
		MAX_IC1[3] += 0x80;
	}
	tmp = (unsigned short int)IrmsC;
	MAX_IC1[4] = tmp / 1000;
	tmp = tmp % 1000;
	MAX_IC1[5] = tmp / 100;
	tmp = tmp % 100;
	MAX_IC1[6] = tmp / 10;
	MAX_IC1[7] = tmp % 10;
	if(!(Internal_Faults & 1))
	{
		MAX_IC1[7] += 0x80;
	}
	
	MAX_IC7[4] = 0;
	MAX_IC7[5] = 0;
	MAX_IC7[6] = 0;
	MAX_IC7[7] = 0;

	if(APVvv)
	{
		MAX_IC7[4] += 16;
	}
	else
	{
		MAX_IC7[4] += 8;
	}
	if(ZMNvv)
	{
		MAX_IC7[4] += 4;
	}
	else
	{
		MAX_IC7[4] += 2;
	}
	if(DUvv)
	{
		MAX_IC7[5] += 8;
	}
	else
	{
		MAX_IC7[5] += 4;
	}
	if(LED_1)
	{
		MAX_IC7[4] += 1;
	}
	if(LED_NesprVimik)
	{
		MAX_IC7[4] += 128;
	}
	if(LED_3)
	{
		MAX_IC7[5] += 64;
	}
	if(LED_4)
	{
		MAX_IC7[5] += 32;
	}
	if(LED_5)
	{
		MAX_IC7[5] += 16;
	}

  for(i = 1; i < 9; i++)
  {
		LED_Send_Buf[0] = ((unsigned short int)i << 8) + MAX_IC1[i-1];
		LED_Send_Buf[1] = ((unsigned short int)i << 8) + MAX_IC2[i-1];
		LED_Send_Buf[2] = ((unsigned short int)i << 8) + MAX_IC3[i-1];
		LED_Send_Buf[3] = ((unsigned short int)i << 8) + MAX_IC4[i-1];
		LED_Send_Buf[4] = ((unsigned short int)i << 8) + MAX_IC5[i-1];
		LED_Send_Buf[5] = ((unsigned short int)i << 8) + MAX_IC6[i-1];
		LED_Send_Buf[6] = ((unsigned short int)i << 8) + MAX_IC7[i-1];
		HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LED_Send_Buf, 7);
		while(!HAL_GPIO_ReadPin(LED_CS_GPIO_Port, LED_CS_Pin)) osDelay(2);
	}
}
