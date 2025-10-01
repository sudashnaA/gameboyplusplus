#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <memory>

class Cart {
private:
	struct RomHeader {
		std::array<uint8_t, 4> entry;
		std::array<uint8_t, 48> logo;
		std::array<char, 16> title;
		uint16_t newLicCode;
		uint8_t sgbFlag;
		uint8_t type;
		uint8_t romSize;
		uint8_t ramSize;
		uint8_t destCode;
		uint8_t licCode;
		uint8_t version;
		uint8_t checkSum;
		uint16_t globalChecksum;
	};

	std::array<char, 1024> m_filename;
	uint32_t m_romSize;
	uint8_t* m_romData;
	RomHeader* m_header;

public:
	uint8_t cartRead(uint16_t address);
	void cartWrite(uint16_t address, uint8_t value);

	bool cartLoad(std::string path);
	void printCartInfo() const;
};
