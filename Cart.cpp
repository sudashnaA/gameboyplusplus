#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>
#include "Cart.h"

uint8_t Cart::cartRead(uint16_t address)
{
	return m_romData[address];
}

void Cart::cartWrite(uint16_t address, uint8_t value)
{
	return;
}

bool Cart::cartLoad(std::string path)
{
	std::ifstream file{path, std::ios::binary | std::ios::ate};

	if (!file) {
		std::cout << "Unable to open file\n";
		return false;
	}

	file.seekg(0, std::ios::end);
	m_romSize = static_cast<uint32_t>(file.tellg());

	std::cout << m_romSize << std::endl;

	// rewind
	file.clear();
	file.seekg(0, std::ios::beg);

	m_romData = new uint8_t[m_romSize];
	file.read(reinterpret_cast<char*>(m_romData), m_romSize);
	file.close();

	m_header = reinterpret_cast<RomHeader*>(m_romData + 0x100);
	m_header->title[15] = 0;

	printCartInfo();
	return true;
}

void Cart::printCartInfo() const
{
	std::cout << "Title: " << m_header->title.data() << std::endl;
	std::cout << "Type: " << (m_header->type | 0) << std::endl;
	std::cout << "ROM Size: " << (32 << (m_header->romSize | 0)) << " KB" << std::endl;
	std::cout << "RAM Size: " << (m_header->ramSize | 0) << std::endl;
	std::cout << "LIC Code: " << (m_header->licCode | 0) << std::endl;
	std::cout << "ROM Version: " << (m_header->version | 0) << std::endl;

	uint16_t x{ 0 };
	for (uint16_t i{ 0x0134 }; i <= 0x014C; i++) {
		x = static_cast<uint16_t>(x - m_romData[i] - 1);
	}

	std::cout << "Checksum: " << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << m_header->checkSum
		<< " " << ((x & 0xFF) ? "PASSED" : "FAILED") << std::endl;
}
