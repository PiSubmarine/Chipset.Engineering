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
#include "stm32u0xx_hal.h"

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
#define REG5_SENSE_Pin GPIO_PIN_1
#define REG5_SENSE_GPIO_Port GPIOA
#define REGPI_SENSE_Pin GPIO_PIN_2
#define REGPI_SENSE_GPIO_Port GPIOA
#define BATCHG_SDA_Pin GPIO_PIN_6
#define BATCHG_SDA_GPIO_Port GPIOA
#define BATCHG_SCL_Pin GPIO_PIN_7
#define BATCHG_SCL_GPIO_Port GPIOA
#define BATCHG_INT_Pin GPIO_PIN_0
#define BATCHG_INT_GPIO_Port GPIOB
#define REG5_EN_Pin GPIO_PIN_12
#define REG5_EN_GPIO_Port GPIOB
#define REG12_EN_Pin GPIO_PIN_13
#define REG12_EN_GPIO_Port GPIOB
#define REG12_PG_Pin GPIO_PIN_8
#define REG12_PG_GPIO_Port GPIOA
#define RPI_SCL_Pin GPIO_PIN_6
#define RPI_SCL_GPIO_Port GPIOB
#define RPI_SDA_Pin GPIO_PIN_7
#define RPI_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
