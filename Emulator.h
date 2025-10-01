#pragma once
#include "CPU.h"
#include "Cart.h"
#include <cstdint>

class Emulator {
public:
	int run(std::string path);

private:
	bool paused{ false };
	bool running{ true };
	uint64_t ticks{ 0 };
	Cart cart{};
	CPU cpu{};
};