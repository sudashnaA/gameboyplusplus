#include <tuple>
#include <iostream>
#include "CPU.h"
#include "util.h"
#include "Emulator.h"

void CPU::connectEmulator(std::shared_ptr<Emulator> e)
{
	m_pEmu = e;
}

void CPU::connectBus(std::shared_ptr<Bus> b)
{
	m_pBus = b;
}

bool CPU::cpuStep()
{
	if (!m_halted) {
		fetchInstruction();
		fetchData();
		execute();
	}

	return false;
}

void CPU::fetchInstruction()
{
	m_curOpcode = busRead(m_registers.pc++);
	m_currInstruction = instructionByOpcode(m_curOpcode);

	if (m_currInstruction == nullptr)
	{
		std::cout << "Unknown instruction\n";
		exit(-7);
	}
}

void CPU::fetchData()
{
	m_memDest = 0;
	m_destIsMem = false;

	switch (m_currInstruction->mode) {
		using enum AddressMode;

	case AM_IMP: 
		return;
	case AM_R:
		//m_fetchedData = m_cpuReadRegister(m_currInstruction->reg1);
		return;
	case AM_R_D8:
		m_fetchedData = busRead(m_registers.pc);
		++m_registers.pc;
		return;
	case AM_D16: {
		auto low = busRead(m_registers.pc);
		emulatorCycles(1);

		auto high = busRead(m_registers.pc + 1u);
		emulatorCycles(1);

		m_fetchedData = static_cast<uint16_t>(low | (high << 8));
		m_registers.pc += 2;
		return;
	};
	default:
		exit(-7);
		return;
	}
}

void CPU::execute()
{
}

uint8_t CPU::busRead(uint16_t address)
{
	auto ptr{ m_pBus.lock() };
	if (ptr) {
		return ptr->busRead(address);
	}

	return 0x00;
}

void CPU::busWrite(uint16_t address, uint8_t value)
{
	auto ptr{ m_pBus.lock() };
	if (ptr) {
		ptr->busWrite(address, value);
		return;
	}
}

void CPU::emulatorCycles(int cpuCycles)
{
	auto ptr{ m_pEmu.lock() };
	if (ptr) {
		ptr->emulatorCycles(cpuCycles);
		return;
	}
}
