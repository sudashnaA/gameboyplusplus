#include "Bus.h"
#include "CPU.h"
#include "util.h"

// Memory Map
// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

Bus::Bus()
{
}

uint8_t Bus::busRead(uint16_t address)
{
	if (address < 0x8000) {
		// ROM data
		return m_pCart->cartRead(address);
	}
	else if (address < 0xA000) {
		// Character map data
		printf("UNSUPPORTED bus_read(%04X)\n", address);
		NO_IMPL;
	}
	else if (address < 0xC000) {
		// Cartridge ram
		return m_pCart->cartRead(address);
	}
	else if (address < 0xE000) {
		// Working ram (WRAM)
		return m_pRam->wramRead(address);
	}
	else if (address < 0xFE00) {
		// reserved ram
		return 0;
	}
	else if (address < 0xFEA0) {
		// OAM
		printf("UNSUPPORTED bus_read(%04X)\n", address);
		NO_IMPL
	}
	else if (address < 0xFF00) {
		// reserved unusable
		return 0;
	}
	else if (address < 0xFF80) {
		// io registers
		printf("UNSUPPORTED bus_read(%04X)\n", address);
		NO_IMPL
	}
	else if (address == 0xFFFF) {
		// CPU enable register
		return m_pCpu->getIeRegister();
	}

	//0xFF80 - 0xFFFE : Zero Page
	return m_pRam->hramRead(address);
}

uint16_t Bus::busRead16(uint16_t address)
{
	auto low{ static_cast<uint16_t>(busRead(address)) };
	auto high{ static_cast<uint16_t>(busRead(static_cast<uint16_t>(address + 1))) };

	return static_cast<uint16_t>(low | (high << 8));
}

void Bus::busWrite(uint16_t address, uint8_t value)
{
	if (address < 0x8000) {
		m_pCart->cartWrite(address, value);
	} else if (address < 0xA000) {
		//Char/Map Data
		printf("UNSUPPORTED bus_write(%04X)\n", address);
		NO_IMPL
	} else if (address < 0xC000) {
		//EXT-RAM
		m_pCart->cartWrite(address, value);
	} else if (address < 0xE000) {
		//WRAM
		m_pRam->wramWrite(address, value);
	} else if (address < 0xFE00) {
		//reserved echo ram
	} else if (address < 0xFEA0) {
		//OAM
		printf("UNSUPPORTED bus_write(%04X)\n", address);
		NO_IMPL
	} else if (address < 0xFF00) {
		//unusable reserved
	} else if (address < 0xFF80) {
		//IO Registers...
		printf("UNSUPPORTED bus_write(%04X)\n", address);
	} else if (address == 0xFFFF) {
		//CPU SET ENABLE REGISTER
		m_pCpu->setIeRegister(value);
	} else {
		m_pRam->hramWrite(address, value);
	}
}

void Bus::busWrite16(uint16_t address, uint16_t value)
{
	busWrite(static_cast<uint16_t>(address + 1), static_cast<uint8_t>((value >> 8) & 0xFF));
	busWrite(address, static_cast<uint8_t>(value & 0xFF));
}

void Bus::connectCart(std::shared_ptr<Cart> c)
{
	m_pCart = c;
}
void Bus::connectRam(std::shared_ptr<Ram> r)
{
	m_pRam = r;
}

void Bus::connectCPU(std::shared_ptr<CPU> c)
{
	m_pCpu = c;
}

