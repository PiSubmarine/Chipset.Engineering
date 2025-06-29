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

namespace PiSubmarine::Chipset::Engineering
{
	class Batchg
	{
	public:
		constexpr static uint8_t BatchgAddress = 0x6B;

		static void TestOnce();
		static void TestRepeat();

		static I2CDriver BatchgI2C;

	};

}
