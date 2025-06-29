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

	I2CDriver Batchg::BatchgI2C { hi2c2 };

	void Batchg::TestOnce()
	{
		HAL_Delay(500);

		Bq25792::Bq25792<I2CDriver> charger { BatchgI2C };
		if (!charger.Read())
		{
			printf("[BATCHG] Read failed to start transaction\n");
			return;
		}
		while (charger.IsDirty())
		{

		}
		if (charger.HasError())
		{
			printf("[BATCHG] Read I2C error\n");
			return;
		}
		auto vsysMin = charger.GetMinimalSystemVoltage();
		printf("[BATCHG] REG00_Minimal_System_Voltage\n");
		printf("[BATCHG]\tVSYSMIN: %ld mV\n", vsysMin.Value);
		/*

		 Reg03ChargeCurrentLimit chargeCurrentLimit;
		 ReadRegister(chargeCurrentLimit, batchgRegs);
		 chargeCurrentLimit.SetChargeCurrentLimit(3000_mA);
		 auto chargeCurrentLimitBytes = chargeCurrentLimit.GetOffsetAndData();
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, chargeCurrentLimitBytes.data(), chargeCurrentLimitBytes.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 Bq25792::Reg14ChargerControl5 chargerControl5;
		 ReadRegister(chargerControl5, batchgRegs);
		 chargerControl5.EnIbat.Set(true);
		 auto chargerControl5Bytes = chargerControl5.GetOffsetAndData();
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, chargerControl5Bytes.data(), chargerControl5Bytes.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 Bq25792::Reg18NtcControl1 ntcControl1;
		 ReadRegister(ntcControl1, batchgRegs);
		 ntcControl1.TsIgnore.Set(true);
		 auto ntcControl1Bytes = ntcControl1.GetOffsetAndData();
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, ntcControl1Bytes.data(), ntcControl1Bytes.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 Bq25792::Reg2EAdcControl adcControl;
		 ReadRegister(adcControl, batchgRegs);
		 adcControl.AdcEn.Set(true);
		 adcControl.AdcSampleSpeed.Set(0);
		 auto adcControlBytes = adcControl.GetOffsetAndData();
		 if (HAL_I2C_Master_Transmit(&hi2c2, BatchgAddress << 1, adcControlBytes.data(), adcControlBytes.size(), HAL_MAX_DELAY) != HAL_OK)
		 {
		 printf("[BATCHG] HAL_I2C_Master_Transmit: FAILED\n");
		 return;
		 }

		 regStart = 0x00;
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

		 Reg00MinimalSystemVoltage reg00;
		 ReadRegister(reg00, batchgRegs);
		 printf("[BATCHG] REG00_Minimal_System_Voltage\n");
		 printf("[BATCHG]\tVSYSMIN: %ld mV\n", reg00.GetMinimalSystemVoltage().Value);

		 Reg01ChargeVoltageLimit reg01;
		 ReadRegister(reg01, batchgRegs);
		 printf("[BATCHG] REG01_Charge_Voltage_Limit\n");
		 printf("[BATCHG]\tVREG: %ld mV\n", reg01.GetBatteryVoltageLimit().Value);

		 Reg03ChargeCurrentLimit reg03;
		 ReadRegister(reg03, batchgRegs);
		 printf("[BATCHG] REG03_Charge_Current_Limit\n");
		 printf("[BATCHG]\tICHG: %ld mA\n", reg03.GetChargeCurrentLimit().Value);

		 printf("\n");
		 */
	}

	void Batchg::TestRepeat()
	{
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

		if (hi2c == PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.GetHandlePtr())
		{
			PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.OnErrorCallback(hi2c);
		}
	}

	void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if (hi2c == PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.GetHandlePtr())
		{
			PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.OnMasterTxCplt(hi2c);
		}
	}

	void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if (hi2c == PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.GetHandlePtr())
		{
			PiSubmarine::Chipset::Engineering::Batchg::BatchgI2C.OnMasterRxCplt(hi2c);
		}
	}
}

