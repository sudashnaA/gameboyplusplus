#pragma once
#include <cstdint>
#include <memory>
#include "Cart.h"

class Bus {
public:
	Bus(std::shared_ptr<Cart> c);

	uint8_t busRead(uint16_t address);
	uint8_t busWrite(uint16_t address, uint8_t value);

private:
	std::weak_ptr<Cart> m_pCart;
};