#pragma once
#include <array>

constexpr int WRAM_SIZE{ 0x2000 };
constexpr int HRAM_SIZE{ 0x80 };

class Ram
{
public:
	uint8_t wramRead(uint16_t address);
	void wramWrite(uint16_t address, uint8_t value);
	uint8_t hramRead(uint16_t address);
	void hramWrite(uint16_t address, uint8_t value);
private:
	std::array<uint8_t, WRAM_SIZE> m_wram{};
	std::array<uint8_t, HRAM_SIZE> m_hram{};
};

