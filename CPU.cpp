#include <tuple>
#include "CPU.h"
#include "util.h"

void CPU::updateFlagRegister(bool zero, bool carry, bool halfCarry, bool subtract) noexcept
{
	m_flagRegister.zero = zero;
	m_flagRegister.subtract = subtract;
	m_flagRegister.carry = carry;
	m_flagRegister.halfCarry = halfCarry;
}

bool CPU::isHalfCarry(uint8_t oldVal, uint8_t newVal) const noexcept
{
	return ((oldVal & 0xF) + (newVal & 0xF)) > 0xF;
}

bool CPU::isHalfCarry16(uint16_t oldVal, uint16_t newVal) const noexcept 
{
	return ((oldVal & 0x0FFF) + (newVal & 0x0FFF)) > 0x0FFF;
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

void CPU::ADD(ArithmeticTarget target) noexcept
{
	auto t{ read(target) };
	auto a{ read(ArithmeticTarget::A) };
	
	auto [result, overflow] { overflowingAdd(a, t) };
	updateFlagRegister(result == 0, overflow, isHalfCarry(a, t), false);

	m_registers.a = result;
}

void CPU::ADDHL(ArithmeticTarget target) noexcept 
{
	auto t{ read(target) };
	auto hl{ getHL() };

	auto [result, overflow] { overflowingAdd(hl, t) };
	updateFlagRegister(result == 0, overflow, isHalfCarry16(hl, t), false);

	setHL(result);
}

void CPU::ADC_A(ArithmeticTarget target) noexcept
{
	auto t{ read(target) };
	auto a{ read(ArithmeticTarget::A) };

	auto [result, overflow] { overflowingAdd(a, t) };
	std::tie(result, overflow) = overflowingAdd(result, m_flagRegister.carry);

	updateFlagRegister(result == 0, overflow, isHalfCarry(a, t), false);

	m_registers.a = result;
}

void CPU::SUB(ArithmeticTarget target) noexcept
{
	auto t{ read(target) };
	auto a{ read(ArithmeticTarget::A) };

	auto [result, underflow] { underflowingSubtract(a, t) };

	updateFlagRegister(result == 0, underflow, isHalfCarry(a, t), true);

	m_registers.a = result;
}

void CPU::SBC(ArithmeticTarget target) noexcept
{
	auto t{ read(target) };
	auto a{ read(ArithmeticTarget::A) };

	t = overflowingAdd(t, m_flagRegister.carry).first;

	auto [result, underflow] { underflowingSubtract(a, t) };
	updateFlagRegister(result == 0, underflow, isHalfCarry(a, t), true);

	m_registers.a = result;
}

void CPU::AND(ArithmeticTarget target) noexcept
{
	auto t{ read(target) };
	auto a{ read(ArithmeticTarget::A) };

	auto result{ static_cast<uint8_t>(t & a) };

	updateFlagRegister(result == 0, false, true, false);

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
