#pragma once
#include <cstdint>

class Bus {
public:
	Bus();

	uint8_t busRead(uint16_t address);
	uint8_t busWrite(uint16_t address, uint8_t value);

private:
};