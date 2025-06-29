#pragma once

#include "main.h"
#include <functional>

namespace PiSubmarine::Chipset::Engineering
{
	class I2CDriver
	{
		using I2CCallback = std::function<void(uint8_t deviceAddress, bool)>;

	public:
		I2CDriver(I2C_HandleTypeDef& i2cHandle) : m_I2CHandle(i2cHandle)
		{

		}

		bool Read(uint8_t deviceAddress, uint8_t* rxData, size_t len)
		{
			return HAL_I2C_Master_Receive(&m_I2CHandle, deviceAddress << 1, rxData, len, HAL_MAX_DELAY) == HAL_OK;
		}

		bool Write(uint8_t deviceAddress, uint8_t* txData, size_t len)
		{
			return HAL_I2C_Master_Transmit(&m_I2CHandle, deviceAddress << 1, txData, len, HAL_MAX_DELAY) == HAL_OK;
		}

		bool ReadAsync(uint8_t deviceAddress, uint8_t* rxData, size_t len, I2CCallback callback)
		{
			if(m_Callback)
			{
				return false;
			}
			m_LastAddress = deviceAddress;
			m_Callback = callback;
			return HAL_I2C_Master_Receive_DMA(&m_I2CHandle, deviceAddress << 1, rxData, len) == HAL_OK;
		}

		bool WriteAsync(uint8_t deviceAddress, uint8_t* txData, size_t len, I2CCallback callback)
		{
			if(m_Callback)
			{
				return false;
			}

			m_LastAddress = deviceAddress;
			m_Callback = callback;
			return HAL_I2C_Master_Transmit_DMA(&m_I2CHandle, deviceAddress << 1, txData, len) == HAL_OK;
		}

		void OnMasterTxCplt(I2C_HandleTypeDef *hi2c)
		{
			if(&m_I2CHandle != hi2c)
			{
				return;
			}

			if(m_Callback)
			{
				m_Callback(m_LastAddress, true);
			}

			m_Callback = nullptr;
		}

		void OnMasterRxCplt(I2C_HandleTypeDef *hi2c)
		{
			if(&m_I2CHandle != hi2c)
			{
				return;
			}

			if(m_Callback)
			{
				m_Callback(m_LastAddress, true);
			}

			m_Callback = nullptr;
		}

		void OnErrorCallback(I2C_HandleTypeDef *hi2c)
		{
			if(&m_I2CHandle != hi2c)
			{
				return;
			}

			if(m_Callback)
			{
				m_Callback(m_LastAddress, false);
			}

			m_Callback = nullptr;
		}

		I2C_HandleTypeDef* GetHandlePtr() const
		{
			return &m_I2CHandle;
		}

	private:
		I2C_HandleTypeDef& m_I2CHandle;
		uint8_t m_LastAddress;
		I2CCallback m_Callback;
	};
}
