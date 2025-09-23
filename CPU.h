#pragma once
#include <cstdint>

class CPU
{
public:


private:
	struct Registers {
		uint8_t a{};
		uint8_t b{};
		uint8_t c{};
		uint8_t d{};
		uint8_t e{};
		uint8_t f{};
		uint8_t g{};
		uint8_t h{};
		uint8_t l{};
	};

	Registers m_registers{};

	uint16_t getBC() const noexcept { return getVirtual(m_registers.b, m_registers.c); };
	void setBC(uint16_t val) noexcept { setVirtual(val, m_registers.b, m_registers.c); };

	uint16_t getAF() const noexcept { return getVirtual(m_registers.a, m_registers.f); };
	void setAF(uint16_t val) noexcept { setVirtual(val, m_registers.a, m_registers.f); };

	uint16_t getDE() const noexcept { return getVirtual(m_registers.d, m_registers.e); };
	void setDE(uint16_t val) noexcept { setVirtual(val, m_registers.d, m_registers.e); };

	uint16_t getHL() const noexcept { return getVirtual(m_registers.h, m_registers.l); };
	void setHL(uint16_t val) noexcept { setVirtual(val, m_registers.h, m_registers.l); };

	uint16_t getVirtual(const uint8_t& high, const uint8_t& low) const noexcept;
	void setVirtual(uint16_t val, uint8_t& high, uint8_t& low) noexcept;
};


