#pragma once
#include <cstdint>
#include <memory>
#include "Cart.h"

class Bus {
public:
	Bus();
	void connectCart(std::shared_ptr<Cart> c);

	uint8_t busRead(uint16_t address);
	uint16_t busRead16(uint16_t address);
	void busWrite(uint16_t address, uint8_t value);
	void busWrite16(uint16_t address, uint16_t value);

private:
	std::shared_ptr<Cart> m_pCart;
};