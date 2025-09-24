#include "CPU.h"

void CPU::updateFlagRegister(uint8_t val) noexcept
{
	m_flagRegister.zero = val == 0;
	m_flagRegister.subtract = false;
}

uint8_t CPU::read(ArithmeticTarget target) const noexcept
{
	switch (target) {
	case ArithmeticTarget::A: return m_registers.a;
	case ArithmeticTarget::B: return m_registers.b;
	case ArithmeticTarget::C: return m_registers.c;
	case ArithmeticTarget::D: return m_registers.d;
	case ArithmeticTarget::E: return m_registers.e;
	case ArithmeticTarget::H: return m_registers.h;
	case ArithmeticTarget::L: return m_registers.l;
	}

	return 0xFF;
}

void CPU::add(ArithmeticTarget target) noexcept
{
	auto reg{ read(target) };
	auto a{ read(ArithmeticTarget::A) };
	
	auto result{ a += reg };
	m_registers.a = result;
}

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
