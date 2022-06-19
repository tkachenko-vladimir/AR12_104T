#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "LED.h"

void LED_Task(void const * argument)
{
	while(1)
	{
		Task_control &= 0xFFFF - 4;
		if(LED_Init_f)
		{
			LED_Init_f = false;
			LED_Init();
			osDelay(100);
		}
		if(LED_Clear_f)
		{
			LED_Clear_f = false;
			LED_Clear();
			osDelay(100);
		}
		if(LED_Chk_On_f)
		{
			LED_Chk_On_f = false;
			LED_Chk_On();
			LED_check = true;
			osDelay(100);
		}
		if(LED_Chk_Off_f)
		{
			LED_Chk_Off_f = false;
			LED_Chk_Off();
			LED_check = false;
			osDelay(100);
		}
		if(LED_Off_f)
		{
			LED_Off_f = false;
			LED_Off();
			osDelay(100);
		}
		if(LED_On_f)
		{
			LED_On_f = false;
			LED_On();
			osDelay(100);
		}
		if(LED_Refresh_f)
		{
			LED_Refresh_f = false;
			LED_Refresh();
			osDelay(100);
		}
		osDelay(300);
	}
}
