#include "Emulator.h"
#include "SDL.h"
#include <iostream>

int Emulator::run(std::string path)
{
	SDL_Init(SDL_INIT_VIDEO);

	cart.cartLoad(path);

	while (running)
	{
		if (paused) {
			delay(10);
			continue;
		}

		if (!cpu.cpuStep()) {
			std::cout << "CPU stopped\n";
			return -1;
		}

		++ticks;
	}

	return 0;
}
