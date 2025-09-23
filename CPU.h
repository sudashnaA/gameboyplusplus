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

	uint16_t getBC() const noexcept;
	void setBC(uint16_t val) noexcept;
};

