/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define Imsz1_MIN 5
#define Imsz1_MAX 400
#define Tmsz1_MIN 0
#define Tmsz1_MAX 7200
#define Imsz2_MIN 50
#define Imsz2_MAX 800
#define Tmsz2_MIN 0
#define Tmsz2_MAX 300
#define Isv_MIN 50
#define Isv_MAX 1600
#define Tsv_MIN 0
#define Tsv_MAX 500
#define APV_Tgot_MIN 1
#define APV_Tgot_MAX 90
#define APV1_T_MIN 1
#define APV1_T_MAX 50
#define APV2_T_MIN 10
#define APV2_T_MAX 60
#define ZMN_level_MIN 20
#define ZMN_level_MAX 80
#define ZMN_Tvidkl_MIN 1
#define ZMN_Tvidkl_MAX 99
#define ZMN_Tvkl_MIN 10
#define ZMN_Tvkl_MAX 99

#define INPUT_POWER_FAULT 8
#define INPUT_RES_POWER 16
#define INPUT_DOOR 32
#define INPUT_COIL_FAULT 64
#define INPUT_DV8 128

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern const unsigned short int _PO_VERSION;
extern unsigned short int _SERIAL_NBR;
extern const unsigned char _SETTINGS_VER_H, _SETTINGS_VER_L;
extern short int _B_IA, _B_IC, _B_UA;
extern float _K_IA, _K_IC,_K_UA;
extern short int Imsz1, Tmsz1, Imsz2, Tmsz2, Isv, Tsv, APV_Tgot, APV1_T, APV2_T, ZMN_level, ZMN_Tvidkl, ZMN_Tvkl;
extern bool MSZ1vv, MSZ1vv_tmp, SVvv, SVvv_tmp, APV2vv, APV2vv_tmp, ZMN_APVvv, ZMN_APVvv_tmp;
extern short int Imsz1_tmp, Tmsz1_tmp, Imsz2_tmp, Tmsz2_tmp, Isv_tmp, Tsv_tmp, APV_Tgot_tmp, APV1_T_tmp, APV2_T_tmp;
extern short int ZMN_level_tmp, ZMN_Tvidkl_tmp, ZMN_Tvkl_tmp;
extern unsigned short int Ubazovoe;
extern unsigned char LED_Brightness, Ktt;
extern float Ktn;
extern unsigned char Year, Month, Date, Hour, Minute, Second, Temp;
extern bool Summer_Time;
extern short int VV_On_cnt;
extern unsigned char Time_Save_Part_adress, Setup2_View, Inputs_State;
extern bool APVvv, ZMNvv, DUvv;
extern unsigned char MB1_adress, MB1_speed, MB2_adress, MB2_speed;
extern bool SW_auto;
extern bool LED_1, LED_NesprVimik, LED_3, LED_4, LED_5;
extern bool LED_recl_on, LED_recl_off;
extern unsigned char MSZ_state, APV_cikl;
extern unsigned int Imsz1A, Imsz1B, Tmsz1A, Tmsz1B;
extern bool LED_Clear_f, LED_Init_f, LED_Chk_On_f, LED_Chk_Off_f, LED_Refresh_f, LED_Off_f, LED_On_f, VV_Unblock_f, ADC_RST_f;
extern bool Save_Settings_f, Load_Settings_f, Setup_En, Setup2_En;
extern bool Save_Settings_Del_f, Time_Load_f, Time_Save_f, Time_Save_Part_f;
extern bool LED1b, LED2b, LED3b, LED4b, LED5b, LED6b, LED7b, LED8b, LED9b, LED10b, LED11b, LED12b, LED13b;
extern unsigned char Setup1Error_flags;
extern bool USB_Read_f, LED_check;
extern unsigned short int Setup2_En_cnt, Setup2_View_cnt, Save_Settings_Del_cnt, Blink_cnt, DUvv_cnt;
extern unsigned int Setup_En_cnt;
extern unsigned int VrmsA, IrmsA, IrmsC, VrmsA_d, IrmsA_d, IrmsC_d;
extern unsigned int VrmsA_poshk, IrmsA_poshk, IrmsC_poshk;
extern char USB_Send_buf[200], USB_Read_buf[200];
extern unsigned int USB_Menu;
extern bool RMS_process, Debug_mode;
extern bool VV_Block, Command_VV_On, Command_VV_Off;
extern unsigned int ChasGotovn_timer, VV_Ready_Timer, VV_Error_Timer;
extern unsigned int Timer_OZZ, MSZ_APV_timer;
extern unsigned int MSZ1A_timer, MSZ2A_timer, MSZsvA_timer, MSZ1B_timer, MSZ2B_timer, MSZsvB_timer;
extern unsigned char MSZ_APV_cikl, OZZ_state, MSZ_Worked_flags, VV_Faults;
extern bool VV_Off_State, VV_On_State, VV_Ready_State;
extern unsigned char VV_On_Sourse, VV_Off_Sourse, VV_On_Sourse_tmp, VV_Off_Sourse_tmp, VV_State;
extern short int VrmsAsend, IrmsAsend, IrmsCsend;
extern unsigned int Control_VV_Off_timer, Control_VV_On_timer, ADC_Start_Timer, MB_CoilFault_timer;
extern bool LED_state, Save_State_f;
extern unsigned short int Journal_write_address, Journal_record_number;
extern unsigned short int Internal_Faults, Task_control;

extern bool MB_PuskMSZ1, MB_VozvratMSZ1, MB_WorkMSZ1;
extern bool MB_PuskMSZ2, MB_VozvratMSZ2, MB_WorkMSZ2;
extern bool MB_PuskSV, MB_WorkSV, MB_GotovAPV;
extern bool MB_PuskAPV1, MB_WorkAPV1, MB_PuskAPV2, MB_WorkAPV2;
extern bool MB_PuskZMN, MB_VozvratZMN, MB_WorkZMN, MB_PuskAPV_ZMN;
extern bool MB_VozvratAPV_ZMN, MB_WorkAPV_ZMN;

extern const unsigned long MB_BR[5];
void Setup1End(void);
void Setup2End(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RELE_3_Pin GPIO_PIN_2
#define RELE_3_GPIO_Port GPIOE
#define RELE_4_Pin GPIO_PIN_3
#define RELE_4_GPIO_Port GPIOE
#define RELE_5_Pin GPIO_PIN_4
#define RELE_5_GPIO_Port GPIOE
#define RELE_6_Pin GPIO_PIN_5
#define RELE_6_GPIO_Port GPIOE
#define RELE_7_Pin GPIO_PIN_6
#define RELE_7_GPIO_Port GPIOE
#define LED_TEST_Pin GPIO_PIN_13
#define LED_TEST_GPIO_Port GPIOC
#define SIM800_ON_Pin GPIO_PIN_0
#define SIM800_ON_GPIO_Port GPIOC
#define IRLZ_ON_Pin GPIO_PIN_1
#define IRLZ_ON_GPIO_Port GPIOC
#define IRLZ_OFF_Pin GPIO_PIN_2
#define IRLZ_OFF_GPIO_Port GPIOC
#define ADC_CS_Pin GPIO_PIN_4
#define ADC_CS_GPIO_Port GPIOA
#define ADC_BUSY_Pin GPIO_PIN_4
#define ADC_BUSY_GPIO_Port GPIOC
#define ADC_BUSY_EXTI_IRQn EXTI4_IRQn
#define ADC_RESET_Pin GPIO_PIN_5
#define ADC_RESET_GPIO_Port GPIOC
#define KBRD_O1_Pin GPIO_PIN_0
#define KBRD_O1_GPIO_Port GPIOB
#define KBRD_O2_Pin GPIO_PIN_1
#define KBRD_O2_GPIO_Port GPIOB
#define KBRD_O3_Pin GPIO_PIN_2
#define KBRD_O3_GPIO_Port GPIOB
#define KEY_34_Pin GPIO_PIN_7
#define KEY_34_GPIO_Port GPIOE
#define KEY_35_Pin GPIO_PIN_8
#define KEY_35_GPIO_Port GPIOE
#define D_14_Pin GPIO_PIN_9
#define D_14_GPIO_Port GPIOE
#define D_15_Pin GPIO_PIN_10
#define D_15_GPIO_Port GPIOE
#define NRF_CE_Pin GPIO_PIN_11
#define NRF_CE_GPIO_Port GPIOE
#define NRF_CS_Pin GPIO_PIN_12
#define NRF_CS_GPIO_Port GPIOE
#define NRF_SCK_Pin GPIO_PIN_13
#define NRF_SCK_GPIO_Port GPIOE
#define NRF_MOSI_Pin GPIO_PIN_14
#define NRF_MOSI_GPIO_Port GPIOE
#define NRF_MISO_Pin GPIO_PIN_15
#define NRF_MISO_GPIO_Port GPIOE
#define LED_CS_Pin GPIO_PIN_12
#define LED_CS_GPIO_Port GPIOB
#define DEBUG_OUT_Pin GPIO_PIN_14
#define DEBUG_OUT_GPIO_Port GPIOB
#define KBRD_I1_Pin GPIO_PIN_8
#define KBRD_I1_GPIO_Port GPIOD
#define KBRD_I2_Pin GPIO_PIN_9
#define KBRD_I2_GPIO_Port GPIOD
#define KBRD_I3_Pin GPIO_PIN_10
#define KBRD_I3_GPIO_Port GPIOD
#define KBRD_I4_Pin GPIO_PIN_11
#define KBRD_I4_GPIO_Port GPIOD
#define KBRD_I5_Pin GPIO_PIN_12
#define KBRD_I5_GPIO_Port GPIOD
#define KBRD_I6_Pin GPIO_PIN_13
#define KBRD_I6_GPIO_Port GPIOD
#define KBRD_I7_Pin GPIO_PIN_14
#define KBRD_I7_GPIO_Port GPIOD
#define ADC_CONVRS_Pin GPIO_PIN_6
#define ADC_CONVRS_GPIO_Port GPIOC
#define ADC_OS0_Pin GPIO_PIN_7
#define ADC_OS0_GPIO_Port GPIOC
#define ADC_OS1_Pin GPIO_PIN_8
#define ADC_OS1_GPIO_Port GPIOC
#define ADC_OS2_Pin GPIO_PIN_9
#define ADC_OS2_GPIO_Port GPIOC
#define MODBUS1_DIR_Pin GPIO_PIN_8
#define MODBUS1_DIR_GPIO_Port GPIOA
#define INPUT_1_Pin GPIO_PIN_15
#define INPUT_1_GPIO_Port GPIOA
#define INPUT_2_Pin GPIO_PIN_0
#define INPUT_2_GPIO_Port GPIOD
#define INPUT_3_Pin GPIO_PIN_1
#define INPUT_3_GPIO_Port GPIOD
#define INPUT_4_Pin GPIO_PIN_3
#define INPUT_4_GPIO_Port GPIOD
#define INPUT_5_Pin GPIO_PIN_4
#define INPUT_5_GPIO_Port GPIOD
#define INPUT_6_Pin GPIO_PIN_5
#define INPUT_6_GPIO_Port GPIOD
#define INPUT_7_Pin GPIO_PIN_6
#define INPUT_7_GPIO_Port GPIOD
#define INPUT_8_Pin GPIO_PIN_7
#define INPUT_8_GPIO_Port GPIOD
#define KBRD_O4_Pin GPIO_PIN_3
#define KBRD_O4_GPIO_Port GPIOB
#define KBRD_O5_Pin GPIO_PIN_4
#define KBRD_O5_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
#define MODBUS2_DIR_Pin GPIO_PIN_9
#define MODBUS2_DIR_GPIO_Port GPIOB
#define RELE_1_Pin GPIO_PIN_0
#define RELE_1_GPIO_Port GPIOE
#define RELE_2_Pin GPIO_PIN_1
#define RELE_2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
