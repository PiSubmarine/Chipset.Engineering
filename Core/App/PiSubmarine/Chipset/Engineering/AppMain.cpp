/*
 * AppMain.cpp
 *
 *  Created on: Jan 13, 2025
 *      Author: DmitriyPC
 */

#include "PiSubmarine/Chipset/Engineering/AppMain.h"
#include "PiSubmarine/Chipset/Engineering/Batchg.h"
#include "PiSubmarine/Chipset/Engineering/Batmon.h"
#include "usart.h"
#include "i2c.h"
#include "lptim.h"
#include <vector>
#include <stdio.h>

namespace PiSubmarine::Chipset::Engineering
{
	AppMain *AppMain::Instance = nullptr;

	AppMain::AppMain()
	{
		Instance = this;
	}

	AppMain::~AppMain()
	{
		Instance = nullptr;
	}

	void AppMain::Run()
	{
		printf("AppMain::Run()\n");
		HAL_LPTIM_PWM_Start(&hlptim2, LPTIM_CHANNEL_1);

		HAL_GPIO_WritePin(REG12_EN_GPIO_Port, REG12_EN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(REG5_EN_GPIO_Port, REG5_EN_Pin, GPIO_PIN_SET);

		// Batchg::TestOnce();
		Batmon::TestOnce();
		while (true)
		{
			HAL_Delay(1000);
			// Batchg::TestRepeat();
			Batmon::TestRepeat();
		}
	}

}

extern "C"
{
	void AppMainRun(void *argument)
	{
		(void) argument;
		PiSubmarine::Chipset::Engineering::AppMain app;
		app.Run();
	}

	int __io_putchar(int ch)
	{
		HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 0xFFFF);
		return ch;
	}

	extern "C"
	{
		void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
		{
			uint32_t error = HAL_I2C_GetError(hi2c);

			if (error & HAL_I2C_ERROR_BERR)
			{
				// Handle Bus Error
			}
			if (error & HAL_I2C_ERROR_ARLO)
			{
				// Handle Arbitration Lost
			}
			if (error & HAL_I2C_ERROR_AF)
			{
				// Handle Acknowledge Failure
			}
			if (error & HAL_I2C_ERROR_OVR)
			{
				// Handle Overrun/Underrun
			}
			if (error & HAL_I2C_ERROR_TIMEOUT)
			{
				// Handle Timeout
			}

			if (hi2c == PiSubmarine::Chipset::Engineering::Batchg::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnErrorCallback(hi2c);
			}
			else if (hi2c == PiSubmarine::Chipset::Engineering::Batmon::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnErrorCallback(hi2c);
			}
		}

		void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
		{
			if (hi2c == PiSubmarine::Chipset::Engineering::Batmon::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnMasterTxCplt(hi2c);
			}
			else if (hi2c == PiSubmarine::Chipset::Engineering::Batmon::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnMasterTxCplt(hi2c);
			}
		}

		void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
		{
			if (hi2c == PiSubmarine::Chipset::Engineering::Batmon::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnMasterRxCplt(hi2c);
			}
			else if (hi2c == PiSubmarine::Chipset::Engineering::Batmon::I2C.GetHandlePtr())
			{
				PiSubmarine::Chipset::Engineering::Batmon::I2C.OnMasterRxCplt(hi2c);
			}
		}
	}
}
