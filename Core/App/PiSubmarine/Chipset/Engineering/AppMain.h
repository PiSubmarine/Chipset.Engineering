#pragma once

#ifdef __cplusplus

#include <cstdint>
#include <array>
#include <memory>
#include <array>
#include "i2c.h"
#include <cstring>
#include <vector>

namespace PiSubmarine::Chipset::Engineering
{
	class AppMain
	{
	public:

		static AppMain* GetInstance(){return Instance;}
		AppMain();

		virtual ~AppMain();
		void Run();

	private:
		static AppMain* Instance;

		constexpr static uint8_t BatchgAddress = 0x6B;

		template<size_t Size>
		uint16_t GetHalfWord(const std::array<uint8_t, Size>& arr, size_t offset)
		{
			return (arr[offset] << 8) + arr[offset + 1];
		}

		void TestBatchg();
	};
}

extern "C"
{
#endif
void AppMainRun(void *argument);

#ifdef __cplusplus
}
#endif
