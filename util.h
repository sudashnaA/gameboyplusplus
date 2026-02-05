#pragma once
#include <type_traits>
#include <utility>
#include <limits>
#include <cstdint>
#include <SDL.h>

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) (on ? (a) |= (1 << n) : (a) &= ~(1 << n))
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))
#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(-5); }

inline void delay(uint32_t ms) {
	SDL_Delay(ms);
}

template<typename E>
constexpr auto inline toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

template <typename T, typename U>
std::pair<T, bool> overflowingAdd(T a, U b)
{
	auto result{ a + b };
	bool overflow{ result > std::numeric_limits<T>::max()};
	return { static_cast<T>(result), overflow };
}

template <typename T, typename U>
std::pair<T, bool> underflowingSubtract(T a, U b)
{
	auto result{ a - b };
	bool underflow{ result < std::numeric_limits<T>::min() };
	return { static_cast<T>(result), underflow };
}