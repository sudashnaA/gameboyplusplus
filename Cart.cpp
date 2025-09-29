#include "Cart.h"
#include <iostream>
#include <fstream>

bool Cart::cartLoad(char* path)
{
	std::ifstream file{path, std::ios_base::in};

	file.seekg(std::ios::end);

	context.rom_size = static_cast<uint32_t>(file.tellg());

	file.clear();
	file.seekg(0);

	file.read(reinterpret_cast<char*>(context.rom_data), context.rom_size);
	file.close();



	return false;
}
