#include "CPU.h"

uint16_t CPU::getVirtual(const uint8_t& high, const uint8_t& low) const noexcept
{
	return static_cast<uint16_t>(high << 8 | low);
}

void CPU::setVirtual(uint16_t val, uint8_t& high, uint8_t& low) noexcept
{
	auto h = static_cast<uint8_t>((val & 0xFF00) >> 8);
	auto l = static_cast<uint8_t>(val & 0x00FF);

	high = h;
	low = l;
}
