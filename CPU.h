#pragma once
#include <cstdint>
#include "util.h"

class CPU
{
public:


private:
	enum class FlagRegisterPositions {
		carry = 4,
		halfCarry = 5,
		subtract = 6,
		zero = 7,
	};

	enum class ArithmeticTarget {
		A,
		B,
		C,
		D,
		E,
		H,
		L,
	};

	void updateFlagRegister(uint8_t val) noexcept;

	uint8_t read(ArithmeticTarget target) const noexcept;
	void add(ArithmeticTarget target) noexcept;

	struct FlagRegister {
		FlagRegister() = default;

		FlagRegister(uint8_t val)
			: zero{ static_cast<bool>(val & (1 << toUType(FlagRegisterPositions::zero))) }
			, subtract{ static_cast<bool>(val & (1 << toUType(FlagRegisterPositions::subtract))) }
			, halfCarry{ static_cast<bool>(val & (1 << toUType(FlagRegisterPositions::halfCarry))) }
			, carry{ static_cast<bool>(val & (1 << toUType(FlagRegisterPositions::carry))) }
		{}

		bool zero{};
		bool subtract{};
		bool halfCarry{};
		bool carry{};

		operator uint8_t() const {
			uint8_t val{};

			if (zero) { val |= 1 << toUType(FlagRegisterPositions::zero); }
			if (subtract) { val |= 1 << toUType(FlagRegisterPositions::subtract); };
			if (halfCarry) { val |= 1 << toUType(FlagRegisterPositions::halfCarry); };
			if (carry) { val |= 1 << toUType(FlagRegisterPositions::carry); };

			return val;
		}
	};

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
	FlagRegister m_flagRegister{};

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


