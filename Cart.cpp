#include <iostream>
#include <fstream>
#include <memory>
#include "Cart.h"

bool Cart::cartLoad(char* path)
{
	std::ifstream file{path, std::ios_base::in};

	file.seekg(std::ios::end);

	romSize = static_cast<uint32_t>(file.tellg());

	file.clear();
	file.seekg(0);

	std::allocator<uint8_t> alloc;

	romData = alloc.allocate(romSize);
	file.read(reinterpret_cast<char*>(romData), romSize);
	file.close();

	header = reinterpret_cast<RomHeader*>(romData + 0x100);
	header->title[15] = 0;

	return false;
}
