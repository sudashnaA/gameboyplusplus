#pragma once
#include <type_traits>
#include <tuple>
#include <cstdint>

template<typename E>
constexpr auto inline toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

std::tuple<uint8_t, bool> overflowingAdd(uint8_t a, uint8_t b)
{
	auto result = static_cast<uint16_t>(a + b);
	bool overflow = result > 0xFF;
	return { static_cast<uint8_t>(result), overflow };
}