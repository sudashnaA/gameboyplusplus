#include "CPU.h"

uint16_t CPU::getBC() const noexcept
{
	return static_cast<uint16_t>(m_registers.b << 8 | m_registers.c);
}

void CPU::setBC(uint16_t val) noexcept
{
	auto b = static_cast<uint8_t>((val && 0xFF00) >> 8);
	auto a = static_cast<uint8_t>(val && 0x00FF);

	m_registers.a = a;
	m_registers.b = b;
}
