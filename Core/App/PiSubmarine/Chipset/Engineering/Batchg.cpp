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

		auto vsysMin = Device.GetMinimalSystemVoltage();
		printf("[BATCHG] REG00_Minimal_System_Voltage\n");
		printf("[BATCHG]\tVSYSMIN: %ld mV\n", vsysMin.Value);

		auto ichg = Device.GetChargeCurrentLimit();
		printf("[BATCHG] REG03_Charge_Current_Limit\n");
		printf("[BATCHG]\tICHG: %ld mA\n", ichg.Value);

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
		auto ichgNew = Device.GetChargeCurrentLimit();
		printf("[BATCHG] REG03_Charge_Current_Limit\n");
		printf("[BATCHG]\tICHG: %ld mA\n", ichgNew.Value);

		printf("\n");
	}

	void Batchg::TestRepeat()
	{
		Device.SetWdRst(true);

		if(!WriteDirtyBlocking(Device))
		{
			return;
		}

		if (!ReadBlocking(Device))
		{
			return;
		}

		ChargerStatus0Flags status0 = Device.GetChargerStatus0();
		printf("[BATCHG] REG1B_Charger_Status_0:\n");
		if (RegUtils::HasAnyFlag(status0, ChargerStatus0Flags::VbusPresentStat))
		{
			printf("[BATCHG]\tVBUS Present\n");
		}
		if (RegUtils::HasAnyFlag(status0, ChargerStatus0Flags::PoorSrcStat))
		{
			printf("[BATCHG]\tPoor Source\n");
		}
		if (RegUtils::HasAnyFlag(status0, ChargerStatus0Flags::PgStat))
		{
			printf("[BATCHG]\tPowerGood\n");
		}

		printf("[BATCHG] REG1C_Charger_Status_1.\n");
		auto chargeStatus = Device.GetChargeStatus();
		printf("[BATCHG]\tCHG_STAT: ");
		switch (chargeStatus)
		{
		case ChargeStatus::NotCharging:
			printf("Not Charging\n");
			break;
		case ChargeStatus::TrickleCharge:
			printf("Trickle Charge\n");
			break;
		case ChargeStatus::PreCharge:
			printf("Pre-charge\n");
			break;
		case ChargeStatus::FastCharge:
			printf("Fast charge (CC mode)\n");
			break;
		case ChargeStatus::TaperCharge:
			printf("Taper charge (CV mode)\n");
			break;
		case ChargeStatus::TopOffTimerActiveCharging:
			printf("Top-off timer active charging\n");
			break;
		case ChargeStatus::ChargindTerminationDone:
			printf("Charging termination done\n");
			break;
		}
		printf("[BATCHG]\tVBUS_STAT: ");
		auto vbusStatus = Device.GetVbusStatus();
		switch (vbusStatus)
		{
		case VbusStatus::NoInputOrBhotOrBcold:
			printf("No Input or BHOT or BCOLD in OTG mode\n");
			break;
		case VbusStatus::UsbSdp:
			printf("USB SDP (500mA)\n");
			break;
		case VbusStatus::UsbCdp:
			printf("USB CDP (1.5A)\n");
			break;
		case VbusStatus::UsbDcp:
			printf("USB DCP (3.25A)\n");
			break;
		case VbusStatus::HvDcp:
			printf("Adjustable High Voltage DCP (HVDCP) (1.5A)\n");
			break;
		case VbusStatus::UnknownAdapter:
			printf("Unknown adaptor (3A)\n");
			break;
		case VbusStatus::NonStandardAdapter:
			printf("Non-Standard Adapter (1A/2A/2.1A/2.4A)\n");
			break;
		case VbusStatus::Otg:
			printf("In OTG mode\n");
			break;
		case VbusStatus::NotQualifiedAdaptor:
			printf("Not qualified adaptor\n");
			break;
		case VbusStatus::PoweredFromVbus:
			printf("Device directly powered from VBUS\n");
			break;
		default:
			printf("RESERVED\n");
		}

		printf("[BATCHG] REG1D_Charger_Status_2.\n");
		auto icoStat = Device.GetIcoStatus();
		printf("[BATCHG]\tICO_STAT: ");
		switch (icoStat)
		{
		case IcoStatus::IcoDisabled:
			printf("ICO disabled\n");
			break;
		case IcoStatus::IcoOptimizationInProgress:
			printf("ICO optimization in progress\n");
			break;
		case IcoStatus::MaximumInputCurrent:
			printf("Maximum input current detected\n");
			break;
		default:
			printf("RESERVED\n");
			break;
		}

		if (Device.IsInThermalRegulation())
		{
			printf("[BATCHG]\tDevice in thermal regulation\n");
		}
		if (Device.IsBatteryPresent())
		{
			printf("[BATCHG]\tBattery connected\n");
		}
		else
		{
			printf("[BATCHG]\tBattery disconnected\n");
		}

		auto ibusAdc = Device.GetIbusCurrent();
		printf("[BATCHG] REG31_IBUS_ADC: %ld mA\n", ibusAdc.Value);

		auto ibatAdc = Device.GetIbatCurrent();
		printf("[BATCHG] REG33_IBAT_ADC: %ld mA\n", ibatAdc.Value);

		auto vbusAdc = Device.GetVbusVoltage();
		printf("[BATCHG] REG35_VBUS_ADC: %ld mV\n", vbusAdc.Value);

		auto vbatAdc = Device.GetVbatVoltage();
		printf("[BATCHG] REG3B_VBAT_ADC: %ld mV\n", vbatAdc.Value);

		auto vsysAdc = Device.GetVsysVoltage();
		printf("[BATCHG] REG3D_VSYS_ADC: %ld mV\n", vsysAdc.Value);

		auto tsAdc = Device.GetTsPercentage();
		printf("[BATCHG] REG3F_TS_ADC: %d%%\n", static_cast<int>(tsAdc * 0.0976563));

		auto tdieAdc = Device.GetDieTemperature();
		printf("[BATCHG] REG41_TDIE_ADC: %ld C\n", tdieAdc.Halves / 2);

		printf("\n");

		/*
		 std::array<uint8_t, 0x49> batchgRegs { 0 };
		 std::array<uint8_t, 3> regData { 0 };

		 uint8_t regStart = 0x00;
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, &regStart, 1, HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 if (HAL_I2C_Master_Receive(&hi2c2, BatchgAddress << 1, batchgRegs.data(), batchgRegs.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Receive: FAILED\n");
		 return;
		 }

		 Reg10ChargerControl1 reg10;
		 ReadRegister(reg10, batchgRegs);
		 reg10.WdRst = static_cast<uint8_t>(1);
		 auto reg10Bytes = reg10.GetOffsetAndData();
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, reg10Bytes.data(), reg10Bytes.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 printf("[BATCHG] REG1B_Charger_Status_0.\n");
		 uint8_t chargerStatus0 = batchgRegs[0x1B];
		 printf("[BATCHG]\tIINDPM_STAT: %s\n", chargerStatus0 & (1 << 7) ? "In IINDPM regulation or IOTG regulation" : "Normal");
		 printf("[BATCHG]\tVINDPM_STAT: %s\n", chargerStatus0 & (1 << 6) ? "In VINDPM regulation or VOTG regulation" : "Normal");
		 printf("[BATCHG]\tWD_STAT: %s\n", chargerStatus0 & (1 << 5) ? "WD timer expired" : "Normal");
		 printf("[BATCHG]\tPOORSRC_STAT: %s\n", chargerStatus0 & (1 << 4) ? "Weak adaptor detected" : "Normal");
		 printf("[BATCHG]\tPG_STAT: %s\n", chargerStatus0 & (1 << 3) ? "Power good" : "Not power good");
		 printf("[BATCHG]\tAC2_PRESENT_STAT: %s\n", chargerStatus0 & (1 << 2) ? "VAC2 Present" : "VAC2 Not Present");
		 printf("[BATCHG]\tAC1_PRESENT_STAT: %s\n", chargerStatus0 & (1 << 1) ? "VAC1 Present" : "VAC1 Not Present");
		 printf("[BATCHG]\tVBUS_PRESENT_STAT: %s\n", chargerStatus0 & (1 << 0) ? "VBUS Present" : "VBUS Not Present");

		 printf("[BATCHG] REG1C_Charger_Status_1.\n");
		 uint8_t chargerStatus1 = batchgRegs[0x1C];
		 uint8_t chgStat = chargerStatus1 >> 5;
		 printf("[BATCHG]\tCHG_STAT: ");
		 switch (chgStat)
		 {
		 case 0:
		 printf("Not Charging\n");
		 break;
		 case 1:
		 printf("Trickle Charge\n");
		 break;
		 case 2:
		 printf("Pre-charge\n");
		 break;
		 case 3:
		 printf("Fast charge (CC mode)\n");
		 break;
		 case 4:
		 printf("Taper charge (CV mode)\n");
		 break;
		 case 5:
		 printf("RESERVED\n");
		 break;
		 case 6:
		 printf("Top-off timer active charging\n");
		 break;
		 case 7:
		 printf("Charging termination done\n");
		 break;
		 }

		 uint8_t vbusStat = (chargerStatus1 & 0b00011110) >> 1;
		 printf("[BATCHG]\tVBUS_STAT: ");
		 switch (vbusStat)
		 {
		 case 0:
		 printf("No Input or BHOT or BCOLD in OTG mode\n");
		 break;
		 case 1:
		 printf("USB SDP (500mA)\n");
		 break;
		 case 2:
		 printf("USB CDP (1.5A)\n");
		 break;
		 case 3:
		 printf("USB DCP (3.25A)\n");
		 break;
		 case 4:
		 printf("Adjustable High Voltage DCP (HVDCP) (1.5A)\n");
		 break;
		 case 5:
		 printf("Unknown adaptor (3A)\n");
		 break;
		 case 6:
		 printf("Non-Standard Adapter (1A/2A/2.1A/2.4A)\n");
		 break;
		 case 7:
		 printf("In OTG mode\n");
		 break;
		 case 8:
		 printf("Not qualified adaptor\n");
		 break;
		 case 0xB:
		 printf("Device directly powered from VBUS\n");
		 break;
		 default:
		 printf("RESERVED\n");
		 }
		 printf("[BATCHG]\tBC1.2_DONE_STAT: %s\n",
		 chargerStatus1 & (1 << 0) ? "BC1.2 or non-standard detection complete" : "BC1.2 or non-standard detection NOT complete");

		 printf("[BATCHG] REG1D_Charger_Status_2.\n");
		 uint8_t chargerStatus2 = batchgRegs[0x1D];
		 uint8_t icoStat = chargerStatus2 >> 6;
		 printf("[BATCHG]\tICO_STAT: ");
		 switch (icoStat)
		 {
		 case 0:
		 printf("ICO disabled\n");
		 break;
		 case 1:
		 printf("ICO optimization in progress\n");
		 break;
		 case 2:
		 printf("Maximum input current detected\n");
		 break;
		 case 3:
		 printf("RESERVED\n");
		 break;
		 }
		 printf("[BATCHG]\tTREG_STAT: %s\n", chargerStatus2 & (1 << 2) ? "Device in thermal regulation" : "Device NOT in thermal regulation");
		 printf("[BATCHG]\tDPDM_STAT: %s\n", chargerStatus2 & (1 << 1) ? "D+/D- detection ongoing" : "D+/D- detection not started or done");
		 printf("[BATCHG]\tVBAT_PRESENT_STAT: %s\n", chargerStatus2 & (1 << 0) ? "Battery connected" : "Battery NOT CONNECTED");

		 // ----
		 // Skipped registers
		 // ----

		 uint8_t faultStatus0 = batchgRegs[0x20];
		 printf("[BATCHG] REG20_FAULT_Status_0.\n");
		 printf("[BATCHG]\tIBAT_REG_STAT: %s\n", faultStatus0 & (1 << 7) ? "Device in battery discharging current regulation" : "Normal");
		 printf("[BATCHG]\tVBUS_OVP_STAT: %s\n", faultStatus0 & (1 << 6) ? "VBUS OVP" : "Normal");
		 printf("[BATCHG]\tVBAT_OVP_STAT: %s\n", faultStatus0 & (1 << 5) ? "VBAT OVP" : "Normal");
		 printf("[BATCHG]\tIBUS_OCP_STAT: %s\n", faultStatus0 & (1 << 4) ? "VBUS OCP" : "Normal");
		 printf("[BATCHG]\tIBAT_OCP_STAT: %s\n", faultStatus0 & (1 << 3) ? "IBAT OCP" : "Normal");
		 printf("[BATCHG]\tCONV_OCP_STAT: %s\n", faultStatus0 & (1 << 2) ? "CONV OCP" : "Normal");
		 printf("[BATCHG]\tVAC2_OVP_STAT: %s\n", faultStatus0 & (1 << 1) ? "VAC2 OCP" : "Normal");
		 printf("[BATCHG]\tVAC1_OVP_STAT: %s\n", faultStatus0 & (1 << 0) ? "VAC1 OCP" : "Normal");

		 uint8_t faultStatus1 = batchgRegs[0x21];
		 printf("[BATCHG] REG21_FAULT_Status_1.\n");
		 printf("[BATCHG]\tVSYS_SHORT_STAT: %s\n", faultStatus1 & (1 << 7) ? "VSYS SHORT" : "Normal");
		 printf("[BATCHG]\tVSYS_OVP_STAT: %s\n", faultStatus1 & (1 << 6) ? "VSYS OVP" : "Normal");
		 printf("[BATCHG]\tOTG_OVP_STAT: %s\n", faultStatus1 & (1 << 5) ? "OTG OVP" : "Normal");
		 printf("[BATCHG]\tOTG_UVP_STAT: %s\n", faultStatus1 & (1 << 4) ? "OTG UVP" : "Normal");
		 printf("[BATCHG]\tTSHUT_STAT: %s\n", faultStatus1 & (1 << 3) ? "Device overheat" : "Normal");

		 int16_t ibusAdc = GetHalfWordTwoComplement(batchgRegs, 0x31);
		 printf("[BATCHG] REG31_IBUS_ADC: %d mA\n", ibusAdc);

		 int16_t ibatAdc = GetHalfWordTwoComplement(batchgRegs, 0x33);
		 printf("[BATCHG] REG33_IBAT_ADC: %d mA\n", ibatAdc);

		 uint16_t vbusAdc = GetHalfWord(batchgRegs, 0x35);
		 printf("[BATCHG] REG35_VBUS_ADC: %d mV\n", vbusAdc);

		 uint16_t vbatAdc = GetHalfWord(batchgRegs, 0x3B);
		 printf("[BATCHG] REG3B_VBAT_ADC: %d mV\n", vbatAdc);

		 uint16_t vsysAdc = GetHalfWord(batchgRegs, 0x3D);
		 printf("[BATCHG] REG3D_VSYS_ADC: %d mV\n", vsysAdc);

		 uint16_t tsAdc = GetHalfWord(batchgRegs, 0x3F);
		 printf("[BATCHG] REG3F_TS_ADC: %d%%\n", static_cast<int>(tsAdc * 0.0976563));

		 int16_t tdieAdc = GetHalfWordTwoComplement(batchgRegs, 0x41);
		 printf("[BATCHG] REG41_TDIE_ADC: %d C\n", static_cast<int>(tdieAdc * 0.5));

		 uint16_t dpAdc = GetHalfWord(batchgRegs, 0x43);
		 printf("[BATCHG] REG43_D+_ADC: %d mV\n", dpAdc);

		 uint16_t dmAdc = GetHalfWord(batchgRegs, 0x45);
		 printf("[BATCHG] REG44_D-_ADC: %d mV\n", dmAdc);

		 printf("\n");
		 */
	}

}

