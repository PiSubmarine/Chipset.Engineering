/*
 * Batmon.cpp
 *
 *  Created on: Jun 27, 2025
 *      Author: DmitriyPC
 */

#include "Batmon.h"
#include <array>
#include <stdio.h>
#include "Utils.h"
#include "main.h"
#include "i2c.h"
#include "I2CDriver.h"

using namespace PiSubmarine::Max1726;

namespace PiSubmarine::Chipset::Engineering
{

	I2CDriver Batmon::I2C { hi2c1 };
	Device<I2CDriver> Batmon::Device { I2C };

	bool Batmon::TransactionWait(Max1726::Device<I2CDriver> &device)
	{
		while (device.IsTransactionInProgress())
		{

		}
		if (device.HasError())
		{
			printf("[Batmon] Read I2C error\n");
			return false;
		}
		return true;
	}

	bool Batmon::ReadBlocking(Max1726::Device<I2CDriver> &device)
	{
		if (!device.Read())
		{
			printf("[Batmon] Read failed to start transaction\n");
			return false;
		}
		return TransactionWait(device);
	}


	bool Batmon::WriteDirtyBlocking(Max1726::Device<I2CDriver> &device)
	{
		if (!device.WriteDirty())
		{
			printf("[Batmon] Write failed to start transaction\n");
			return false;
		}
		return TransactionWait(device);
	}

	bool Batmon::WriteBlocking(Max1726::Device<I2CDriver> &device, PiSubmarine::Max1726::RegOffset reg)
	{
		if (!device.Write(reg))
		{
			printf("[Batmon] Write failed to start transaction\n");
			return false;
		}
		return TransactionWait(device);
	}

	void Batmon::TestOnce()
	{
		HAL_Delay(500);

		if (!ReadBlocking(Device))
		{
			return;
		}

		auto status = Device.GetStatus();
		printf("Status: %u", static_cast<uint16_t>(status));
	}

	void Batmon::TestRepeat()
	{

	}

}



