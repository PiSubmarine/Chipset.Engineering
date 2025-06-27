/*
 * AppMain.cpp
 *
 *  Created on: Jan 13, 2025
 *      Author: DmitriyPC
 */

#include "PiSubmarine/Chipset/Engineering/AppMain.h"
#include "usart.h"
#include "i2c.h"
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

	void AppMain::TestBatchg()
	{
		std::array<uint8_t, 0x49> batchgRegs { 0 };

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

		uint16_t vsysMin = 2500 + batchgRegs[0] * 250;
		printf("[BATCHG] REG00_Minimal_System_Voltage\n");
		printf("[BATCHG]\tVSYSMIN: %d mV\n", vsysMin);

		uint16_t vreg = GetHalfWord(batchgRegs, 0x01) * 10;
		printf("[BATCHG] REG01_Charge_Voltage_Limit\n");
		printf("[BATCHG]\tVREG: %d mV\n", vreg);

		uint16_t ichg = GetHalfWord(batchgRegs, 0x03) * 10;
		printf("[BATCHG] REG03_Charge_Current_Limit\n");
		printf("[BATCHG]\tICHG: %d mA\n", ichg);

		uint16_t vindpm = batchgRegs[0x05] * 100;
		printf("[BATCHG] REG05_Input_Voltage_Limit\n");
		printf("[BATCHG]\tVINDPM: %d mV\n", vindpm);

		uint16_t iindpm = GetHalfWord(batchgRegs, 0x06) * 10;
		printf("[BATCHG] REG06_Input_Current_Limit\n");
		printf("[BATCHG]\tIINDPM: %d mA\n", iindpm);

		uint8_t prechargeControl = batchgRegs[0x08];
		printf("[BATCHG] REG08_Precharge_Control.\n");
		printf("[BATCHG]\tIPRECHG: %d mA\n", (prechargeControl & 0b00111111) * 40);
		uint8_t vbatLowV = prechargeControl >> 6;
		printf("[BATCHG]\tVBAT_LOWV: ");
		switch (vbatLowV)
		{
		case 0:
			printf("15%% * VREG (%d)\n", (int) (vreg * 0.15));
			break;
		case 1:
			printf("62.2%% * VREG (%d)\n", (int) (vreg * 0.622));
			break;
		case 2:
			printf("66.7%% * VREG (%d)\n", (int) (vreg * 0.667));
			break;
		case 3:
			printf("71.4%% * VREG (%d)\n", (int) (vreg * 0.714));
			break;
		}

		uint8_t terminationControl = batchgRegs[0x09];
		uint8_t iterm = terminationControl & 0b00011111;
		uint8_t regRst = terminationControl & 0b01000000;
		printf("[BATCHG] REG09_Termination_Control.\n");
		printf("[BATCHG]\tITERM: %d mA\n", iterm * 40);
		printf("[BATCHG]\tREG_RST: %d mA\n", regRst);

		uint8_t rechargeControl = batchgRegs[0x0A];
		uint8_t cell = (rechargeControl & 0b11000000) >> 6;
		uint8_t trechg = (rechargeControl & 0b00110000) >> 4;
		uint8_t vrechg = (rechargeControl & 0b00000111);
		printf("[BATCHG] REG0A_Recharge_Control.\n");
		printf("[BATCHG]\tCELL: %d\n", cell + 1);
		printf("[BATCHG]\tTRECHG: ");
		switch (trechg)
		{
		case 0:
			printf("64ms\n");
			break;
		case 1:
			printf("256ms\n");
			break;
		case 2:
			printf("1024ms\n");
			break;
		case 3:
			printf("2048ms\n");
			break;
		}

		printf("[BATCHG]\tVRECHG: %d mV\n", vrechg * 50 + 50);

		printf("[BATCHG] REG0B_VOTG_regulation.\n");
		printf("[BATCHG]\tVOTG: %d mV\n", GetHalfWord(batchgRegs, 0x0B) * 10 + 2800);

		printf("[BATCHG] REG0D_IOTG_regulation.\n");
		printf("[BATCHG]\tPRECHG_TMR: ");
		if (batchgRegs[0x0D] & (1 << 7))
		{
			printf("0.5 hrs\n");
		}
		else
		{
			printf("2 hrs\n");
		}
		printf("[BATCHG]\tIOTG: %d mA\n", (batchgRegs[0x0D] & 0b01111111) * 40);

		uint8_t timerControl = batchgRegs[0x0E];
		printf("[BATCHG] REG0E_Timer_Control.\n");
		printf("[BATCHG]\tTOPOFF_TMP: ");
		switch (timerControl >> 6)
		{
		case 0:
			printf("Disabled\n");
			break;
		case 1:
			printf("15 mins\n");
			break;
		case 2:
			printf("30 mins\n");
			break;
		case 3:
			printf("45 mins\n");
			break;
		}
		printf("[BATCHG]\tEN_TRICHG_TMR: %s\n", timerControl & (1 << 5) ? "Enabled (1h)" : "Disabled");
		printf("[BATCHG]\tEN_PRECHG_TMR: %s\n", timerControl & (1 << 4) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tEN_CHG_TMR: %s\n", timerControl & (1 << 3) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tCHG_TMR: ");
		switch ((timerControl & 0b00000110) >> 1)
		{
		case 0:
			printf("5 hrs\n");
			break;
		case 1:
			printf("8 hrs\n");
			break;
		case 2:
			printf("12 hrs\n");
			break;
		case 3:
			printf("24 hrs\n");
			break;
		}
		printf("[BATCHG]\tTMR2X_EN: %s\n", timerControl & (1 << 0) ? "Enabled" : "Disabled");

		uint8_t chargerControl = batchgRegs[0x0F];
		printf("[BATCHG] REG0F_Charger_Control_0.\n");
		printf("[BATCHG]\tEN_AUTO_IBATDIS: %s\n", chargerControl & (1 << 7) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tFORCE_IBATDIS: %s\n", chargerControl & (1 << 6) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tEN_CHG: %s\n", chargerControl & (1 << 5) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tEN_ICO: %s\n", chargerControl & (1 << 4) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tFORCE_ICO: %s\n", chargerControl & (1 << 3) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tEN_HIZ: %s\n", chargerControl & (1 << 2) ? "Enabled" : "Disabled");
		printf("[BATCHG]\tEN_TERM: %s\n", chargerControl & (1 << 1) ? "Enabled" : "Disabled");

		uint16_t vbatAdc = GetHalfWord(batchgRegs, 0x3B);
		printf("[BATCHG] REG3B_VBAT_ADC: %d\n", vbatAdc);

		uint16_t vsysAdc = GetHalfWord(batchgRegs, 0x3D);
		printf("[BATCHG] REG3D_VSYS_ADC: %d\n", vsysAdc);

		printf("\n");
	}

	void AppMain::Run()
	{
		printf("AppMain::Run()\n");

		while (true)
		{
			HAL_Delay(1000);
			TestBatchg();
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
