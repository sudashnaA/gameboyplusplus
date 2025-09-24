#pragma once
#include <type_traits>
#include <tuple>
#include <limits>
#include <cstdint>

template<typename E>
constexpr auto inline toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

template <typename T, typename U>
std::tuple<T, bool> overflowingAdd(T a, U b)
{
	auto result{ a + b };
	bool overflow{ result > std::numeric_limits<T>::max()};
	return { static_cast<T>(result), overflow };
}