/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7rsxx_hal.h"

#include "stm32h7rsxx_ll_i2c.h"
#include "stm32h7rsxx_ll_system.h"
#include "stm32h7rsxx_ll_gpio.h"
#include "stm32h7rsxx_ll_exti.h"
#include "stm32h7rsxx_ll_bus.h"
#include "stm32h7rsxx_ll_cortex.h"
#include "stm32h7rsxx_ll_rcc.h"
#include "stm32h7rsxx_ll_utils.h"
#include "stm32h7rsxx_ll_pwr.h"
#include "stm32h7rsxx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED2_Pin LL_GPIO_PIN_3
#define LED2_GPIO_Port GPIOM
#define LED1_Pin LL_GPIO_PIN_2
#define LED1_GPIO_Port GPIOM
#define LED0_Pin LL_GPIO_PIN_1
#define LED0_GPIO_Port GPIOM
#define DISP_Pin LL_GPIO_PIN_4
#define DISP_GPIO_Port GPIOD
#define Backlight_Pin LL_GPIO_PIN_11
#define Backlight_GPIO_Port GPIOE
#define SD_PRE_Pin LL_GPIO_PIN_6
#define SD_PRE_GPIO_Port GPIOF
#define PWR_ON_Pin LL_GPIO_PIN_14
#define PWR_ON_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
