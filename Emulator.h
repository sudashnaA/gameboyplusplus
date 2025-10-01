#pragma once
#include "CPU.h"
#include "Cart.h"
#include "Bus.h"
#include <cstdint>
#include <memory>

class Emulator : public std::enable_shared_from_this<Emulator> {
public:
	Emulator();
	int run(std::string path);
	void emulatorCycles(int cpuCycles);
	void init();

private:
	bool m_running{ false };
	bool m_paused{ true };
	uint64_t m_ticks{ 0 };

	std::shared_ptr<Cart> m_cart;
	std::shared_ptr<CPU> m_cpu;
	std::shared_ptr<Bus> m_bus;
};