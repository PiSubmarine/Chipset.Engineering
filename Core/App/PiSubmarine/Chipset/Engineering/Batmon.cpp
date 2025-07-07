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
using namespace PiSubmarine::RegUtils;

namespace PiSubmarine::Chipset::Engineering
{

	I2CDriver Batmon::I2C { hi2c1 };
	Device<I2CDriver> Batmon::Device { I2C };

	void Batmon::TestOnce()
	{
		HAL_Delay(500);
		WaitFunc delayFunc = [](std::chrono::milliseconds delay)
		{	HAL_Delay(delay.count());};

		if (!Device.InitBlocking(delayFunc, capacity, terminationCurrent, emptyVoltage))
		{
			// printf("InitBlocking failed\n");
			return;
		}

		Device.ReadAndWait(RegOffset::Config, delayFunc);
		auto config = Device.GetConfig();
		config = config & ~(ConfigFlags::EnableTemperatureChannel | ConfigFlags::EnableThermistor | ConfigFlags::TemperatureAlertSticky);
		Device.SetConfig(config);
		Device.WriteAndWait(RegOffset::Config, delayFunc);
		// printf("\n");
	}

	void Batmon::TestRepeat()
	{
		WaitFunc delayFunc = [](std::chrono::milliseconds delay)
		{	HAL_Delay(delay.count());};

		Device.ReadAndWait(RegOffset::Status, delayFunc);
		auto status = Device.GetStatus();
		if (HasAnyFlag(status, Status::PowerOnReset) && !Device.InitBlocking(delayFunc, capacity, terminationCurrent, emptyVoltage, true))
		{
			// printf("InitBlocking failed\n");
			return;
		}

		Device.ReadAndWait(RegOffset::DesignCap, delayFunc);
		auto designCapacity = Device.GetDesignCapacity();
		uint32_t designCapacitymAh = designCapacity.GetMicroAmpereHours() / 1000;
		Device.ReadAndWait(RegOffset::RepCap, delayFunc);
		auto repCap = Device.GetRemainingCapacity();

		Device.ReadAndWait(RegOffset::RepSOC, delayFunc);
		uint16_t repSoc = Device.GetRemainingSoc();
		Device.ReadAndWait(RegOffset::TTE, delayFunc);
		uint16_t tte = Device.GetTimeToEmpty();
		Device.ReadAndWait(RegOffset::TTF, delayFunc);
		uint16_t ttf = Device.GetTimeToFull();
		Device.ReadAndWait(RegOffset::Current, delayFunc);
		auto current = Device.GetCurrent();
		Device.ReadAndWait(RegOffset::VCell, delayFunc);
		auto vcell = Device.GetVCell();
		uint32_t voltage = vcell.GetMicroVolts() * 4 / 1000;
		int32_t uAValue = current.GetMicroAmperes();
		uint32_t repCapmAhValue = repCap.GetMicroAmpereHours() / 1000;

		printf("M5 Outputs:\n");
		printf("\tTTE: %d s\n", tte * 1000 / 5625);
		printf("\tTTF: %d s\n", ttf * 1000 / 5625);
		printf("\tCurrent: %d uA\n", uAValue);
		printf("\tVoltage: %lu mV\n", voltage);
		printf("\tRepSoc: %lu%%\n", repSoc / 256UL);
		printf("\tRepCap: %lu mAh\n", repCapmAhValue);

		Device.ReadAndWait(RegOffset::FullCapRep, delayFunc);
		auto fullCapRep = Device.GetEstimatedFullCapacity();
		uint32_t fullCapRepuAhValue = fullCapRep.GetMicroAmpereHours();
		Device.ReadAndWait(RegOffset::RComp0, delayFunc);
		uint16_t rcomp0 = Device.GetRcomp0();
		Device.ReadAndWait(RegOffset::TempCo, delayFunc);
		uint16_t tempCo = Device.GetTempCo();
		Device.ReadAndWait(RegOffset::Cycles, delayFunc);
		uint16_t cycles = Device.GetCycles();
		Device.ReadAndWait(RegOffset::FullCapNom, delayFunc);
		auto fullCapNom = Device.GetNominalFullCapacity();
		uint32_t fullCapNomuAhValue = fullCapNom.GetMicroAmpereHours();

		printf("M5 Learning:\n");
		printf("\tRComp0: %lu\n", rcomp0);
		printf("\tTempCo: %lu\n", tempCo);
		printf("\tFullCapRep: %lu mAh (%lu uAh)\n", fullCapRepuAhValue / 1000, fullCapRepuAhValue);
		printf("\tCycles: %lu\n", cycles);
		printf("\tFullCapNom: %lu mAh (%lu uAh)\n", fullCapNomuAhValue / 1000, fullCapNomuAhValue);
		printf("\n");
	}

}

