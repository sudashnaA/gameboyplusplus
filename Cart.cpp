#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>
#include "Cart.h"

bool Cart::cartLoad(std::string path)
{
	std::ifstream file{path, std::ios::binary | std::ios::ate};

	if (!file) {
		std::cout << "Unable to open file\n";
		return false;
	}

	file.seekg(0, std::ios::end);
	romSize = static_cast<uint32_t>(file.tellg());

	std::cout << romSize << std::endl;

	// rewind
	file.clear();
	file.seekg(0, std::ios::beg);

	romData = new uint8_t[romSize];
	file.read(reinterpret_cast<char*>(romData), romSize);
	file.close();

	header = reinterpret_cast<RomHeader*>(romData + 0x100);
	header->title[15] = 0;

	printCartInfo();
	return true;
}

void Cart::printCartInfo() const
{
	std::cout << "Title: " << header->title.data() << std::endl;
	std::cout << "Type: " << (header->type | 0) << std::endl;
	std::cout << "ROM Size: " << (32 << (header->romSize | 0)) << " KB" << std::endl;
	std::cout << "RAM Size: " << (header->ramSize | 0) << std::endl;
	std::cout << "LIC Code: " << (header->licCode | 0) << std::endl;
	std::cout << "ROM Version: " << (header->version | 0) << std::endl;

	uint16_t x{ 0 };
	for (uint16_t i{ 0x0134 }; i <= 0x014C; i++) {
		x = static_cast<uint16_t>(x - romData[i] - 1);
	}

	std::cout << "Checksum: " << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << header->checkSum
		<< " " << ((x & 0xFF) ? "PASSED" : "FAILED") << std::endl;
}
