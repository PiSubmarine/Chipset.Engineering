/*
 * AppMain.cpp
 *
 *  Created on: Jan 13, 2025
 *      Author: DmitriyPC
 */

#include "PiSubmarine/Chipset/Engineering/AppMain.h"
#include "PiSubmarine/Chipset/Engineering/Batchg.h"
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

		Batchg::TestOnce();
		while (true)
		{
			HAL_Delay(1000);
			Batchg::TestRepeat();
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


}
