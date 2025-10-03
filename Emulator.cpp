#include "Emulator.h"
#include "SDL.h"
#include <iostream>

Emulator::Emulator()
	: m_cart{ std::make_shared<Cart>() }
	, m_bus{ std::make_shared<Bus>() }
	, m_cpu{ std::make_shared<CPU>() }
{
}

void Emulator::init()
{
	m_bus->connectCart(m_cart);
	m_cpu->connectBus(m_bus);
	m_cpu->connectEmulator(shared_from_this());
}

int Emulator::run(std::string path)
{
	SDL_Init(SDL_INIT_VIDEO);

	m_cart->cartLoad(path);

	while (m_paused)
	{
		if (m_running) {
			delay(10);
			continue;
		}

		if (!m_cpu->cpuStep()) {
			std::cout << "CPU stopped\n";
			return -1;
		}

		++m_ticks;
	}

	return 0;
}

void Emulator::emulatorCycles(int cpuCycles)
{
	(int)(cpuCycles);
}