#pragma once

#include <cstdint>
#include <array>
#include <stddef.h>

namespace PiSubmarine::Chipset::Engineering
{
	template<size_t Size>
	uint16_t GetHalfWord(const std::array<uint8_t, Size>& arr, size_t offset)
	{
		return (arr[offset] << 8) + arr[offset + 1];
	}

	template<size_t Size>
	void SetHalfWord(std::array<uint8_t, Size>& arr, size_t offset, uint16_t value)
	{
		arr[offset] = value >> 8;
		arr[offset + 1] = value;
	}

	template<size_t Size>
	int16_t GetHalfWordTwoComplement(const std::array<uint8_t, Size>& arr, size_t offset)
	{
		uint16_t halfWord = GetHalfWord(arr, offset);

		if (halfWord & (1 << (sizeof(uint16_t) * 8 - 1)))
		{
			halfWord |= (~0u << sizeof(uint16_t) * 8);
		}
		return static_cast<int16_t>(halfWord);

	}
}
