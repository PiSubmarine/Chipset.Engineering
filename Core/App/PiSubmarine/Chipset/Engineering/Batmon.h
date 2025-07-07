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
#include "PiSubmarine/Max1726/MicroAmpereHours.h"
#include "PiSubmarine/Max1726/Max1726.h"

namespace PiSubmarine::Chipset::Engineering
{
	class Batmon
	{
	public:
		static constexpr Max1726::MicroAmpereHours capacity{3500000};
		static constexpr Max1726::MicroAmperes terminationCurrent{200000};
		static constexpr Max1726::MicroVolts emptyVoltage {3500000};

		static void TestOnce();
		static void TestRepeat();

		static I2CDriver I2C;
		static Max1726::Device<I2CDriver> Device;

		static bool TransactionWait(Max1726::Device<I2CDriver>& charger);
		static bool ReadBlocking(Max1726::Device<I2CDriver>& charger);
		static bool WriteDirtyBlocking(Max1726::Device<I2CDriver>& charger);
		static bool WriteBlocking(Max1726::Device<I2CDriver>& charger, PiSubmarine::Max1726::RegOffset reg);
	};

}
