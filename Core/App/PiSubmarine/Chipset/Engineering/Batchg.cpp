/*
 * Batchg.cpp
 *
 *  Created on: Jun 27, 2025
 *      Author: DmitriyPC
 */

#include "Batchg.h"
#include <array>
#include <stdio.h>
#include "Utils.h"
#include "main.h"
#include "i2c.h"
#include "PiSubmarine/Bq25792/Bq25792.h"
#include "I2CDriver.h"

using namespace PiSubmarine::Bq25792;

namespace PiSubmarine::Chipset::Engineering
{

	I2CDriver Batchg::I2C { hi2c2 };
	Device<I2CDriver> Batchg::Device { I2C };

	bool Batchg::TransactionWait(Bq25792::Device<I2CDriver> &charger)
	{
		while (charger.IsTransactionInProgress())
		{
			HAL_Delay(0);
		}
		if (charger.HasError())
		{
			printf("[BATCHG] Read I2C error\n");
			return false;
		}
		return true;
	}

	bool Batchg::ReadBlocking(Bq25792::Device<I2CDriver> &charger)
	{
		if (!charger.Read())
		{
			printf("[BATCHG] Read failed to start transaction\n");
			return false;
		}
		return TransactionWait(charger);
	}

	bool Batchg::WriteBlocking(Bq25792::Device<I2CDriver> &charger)
	{
		if (!charger.Write())
		{
			printf("[BATCHG] Write failed to start transaction\n");
			return false;
		}
		return TransactionWait(charger);
	}

	bool Batchg::WriteDirtyBlocking(Bq25792::Device<I2CDriver> &charger)
	{
		if (!charger.WriteDirty())
		{
			printf("[BATCHG] Write failed to start transaction\n");
			return false;
		}
		return TransactionWait(charger);
	}

	bool Batchg::WriteBlocking(Bq25792::Device<I2CDriver> &charger, PiSubmarine::Bq25792::RegOffset reg)
	{
		if (!charger.Write(reg))
		{
			printf("[BATCHG] Write failed to start transaction\n");
			return false;
		}
		return TransactionWait(charger);
	}

	void Batchg::TestOnce()
	{
		HAL_Delay(500);

		if (!ReadBlocking(Device))
		{
			return;
		}

		Device.SetChargeCurrentLimit(3000_mA);
		Device.SetTsIgnore(true);
		Device.SetAdcEnabled(true);
		Device.SetDischargeOcpEnabled(true);
		Device.SetDischargeCurrentSensingEnabled(true);
		Device.SetIlimHizCurrentLimitEnabled(false);
		if (!WriteDirtyBlocking(Device))
		{
			return;
		}

		if (!ReadBlocking(Device))
		{
			return;
		}

		printf("\n");
	}

	void Batchg::TestRepeat()
	{
		Device.SetWdRst(true);

		if(!WriteDirtyBlocking(Device))
		{
			return;
		}

	}

}

