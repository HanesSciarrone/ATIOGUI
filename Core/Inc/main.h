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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
	FALSE = 0,
	TRUE
}bool_t;
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_USER1_Pin GPIO_PIN_13
#define LED_USER1_GPIO_Port GPIOJ
#define Audio_INT_Pin GPIO_PIN_12
#define Audio_INT_GPIO_Port GPIOJ
#define NC4_Pin GPIO_PIN_8
#define NC4_GPIO_Port GPIOI
#define NC3_Pin GPIO_PIN_7
#define NC3_GPIO_Port GPIOK
#define NC2_Pin GPIO_PIN_6
#define NC2_GPIO_Port GPIOK
#define NC1_Pin GPIO_PIN_5
#define NC1_GPIO_Port GPIOK
#define WIFI_RST_Pin GPIO_PIN_14
#define WIFI_RST_GPIO_Port GPIOJ
#define RMII_RXER_Pin GPIO_PIN_5
#define RMII_RXER_GPIO_Port GPIOD
#define SPI2_CS_Pin GPIO_PIN_11
#define SPI2_CS_GPIO_Port GPIOA
#define NC8_Pin GPIO_PIN_4
#define NC8_GPIO_Port GPIOK
#define NC7_Pin GPIO_PIN_3
#define NC7_GPIO_Port GPIOK
#define DSI_RESET_Pin GPIO_PIN_15
#define DSI_RESET_GPIO_Port GPIOJ
#define OTG_FS_OverCurrent_Pin GPIO_PIN_4
#define OTG_FS_OverCurrent_GPIO_Port GPIOD
#define NC5_Pin GPIO_PIN_12
#define NC5_GPIO_Port GPIOI
#define OSC_25M_Pin GPIO_PIN_0
#define OSC_25M_GPIO_Port GPIOH
#define LCD_INT_Pin GPIO_PIN_13
#define LCD_INT_GPIO_Port GPIOI
#define uSD_Detect_Pin GPIO_PIN_15
#define uSD_Detect_GPIO_Port GPIOI
#define CONTROLLER_PUMP_RX_Pin GPIO_PIN_7
#define CONTROLLER_PUMP_RX_GPIO_Port GPIOC
#define LCD_BL_CTRL_Pin GPIO_PIN_14
#define LCD_BL_CTRL_GPIO_Port GPIOI
#define CONTROLLER_PUMP_TX_Pin GPIO_PIN_6
#define CONTROLLER_PUMP_TX_GPIO_Port GPIOC
#define EXT_SDA_Pin GPIO_PIN_6
#define EXT_SDA_GPIO_Port GPIOG
#define ARD_D8_Pin GPIO_PIN_4
#define ARD_D8_GPIO_Port GPIOJ
#define EXT_SCL_Pin GPIO_PIN_3
#define EXT_SCL_GPIO_Port GPIOG
#define LD_USER2_Pin GPIO_PIN_5
#define LD_USER2_GPIO_Port GPIOJ
#define B_USER_Pin GPIO_PIN_0
#define B_USER_GPIO_Port GPIOA
#define ARD_D7_Pin GPIO_PIN_3
#define ARD_D7_GPIO_Port GPIOJ
#define NFC_IRQ_Pin GPIO_PIN_6
#define NFC_IRQ_GPIO_Port GPIOH
#define ARD_D4_Pin GPIO_PIN_0
#define ARD_D4_GPIO_Port GPIOJ
#define ARD_D2_Pin GPIO_PIN_1
#define ARD_D2_GPIO_Port GPIOJ
/* USER CODE BEGIN Private defines */
void MX_I2C4_Init(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
