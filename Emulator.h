#pragma once
#include "CPU.h"
#include "Cart.h"
#include "Bus.h"
#include <cstdint>
#include <memory>

class Emulator {
public:
	Emulator();
	int run(std::string path);

private:
	bool paused{ false };
	bool running{ true };
	uint64_t ticks{ 0 };

	std::shared_ptr<Cart> cart;
	std::shared_ptr<CPU> cpu;
	Bus bus;
};