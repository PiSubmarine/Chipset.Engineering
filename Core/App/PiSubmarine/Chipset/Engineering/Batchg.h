/*
 * Batchg.h
 *
 *  Created on: Jun 27, 2025
 *      Author: DmitriyPC
 */

#pragma once

#include <cstdint>
#include "i2c.h"
#include "I2CDriver.h"
#include "PiSubmarine/Bq25792/Bq25792.h"

namespace PiSubmarine::Chipset::Engineering
{
	class Batchg
	{
	public:
		constexpr static uint8_t BatchgAddress = 0x6B;

		static void TestOnce();
		static void TestRepeat();

		static I2CDriver I2C;
		static Bq25792::Device<I2CDriver> Device;

		static bool TransactionWait(Bq25792::Device<I2CDriver>& charger);
		static bool ReadBlocking(Bq25792::Device<I2CDriver>& charger);
		static bool WriteBlocking(Bq25792::Device<I2CDriver>& charger);
		static bool WriteDirtyBlocking(Bq25792::Device<I2CDriver>& charger);
		static bool WriteBlocking(Bq25792::Device<I2CDriver>& charger, PiSubmarine::Bq25792::RegOffset reg);
	};

}
