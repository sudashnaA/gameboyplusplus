#include "Cart.h"
#include <iostream>
#include <fstream>

bool Cart::cartLoad(char* path)
{
	std::ifstream file{path, std::ios_base::in};

	file.seekg(std::ios::end);

	rom_size = static_cast<uint32_t>(file.tellg());

	file.clear();
	file.seekg(0);

	file.read(reinterpret_cast<char*>(rom_data), rom_size);
	file.close();

	return false;
}
