#include "Ram.h"

constexpr int WRAM_OFFSET{ 0xC000 };
constexpr int HRAM_OFFSET{ 0xFF80 };

uint8_t Ram::wramRead(uint16_t address)
{
	address -= WRAM_OFFSET;

    if (address >= 0x2000) {
        printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
        exit(-1);
    }

    return m_wram[address];
}

void Ram::wramWrite(uint16_t address, uint8_t value)
{
    address -= WRAM_OFFSET;
    m_wram[address] = value;
}

uint8_t Ram::hramRead(uint16_t address)
{
    address -= HRAM_OFFSET;
    return m_hram[address];
}

void Ram::hramWrite(uint16_t address, uint8_t value)
{
    address -= HRAM_OFFSET;
    m_hram[address] = value;
}
