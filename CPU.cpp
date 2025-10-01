#include <tuple>
#include "CPU.h"
#include "util.h"

CPU::CPU(std::shared_ptr<Bus> b)
	: m_pBus{b}
{}

void CPU::connectEmulator(std::shared_ptr<Emulator> e)
{
	m_pEmu = e;
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
	m_curOpcode = m_pBus->busRead(m_registers.pc++);
	m_currInstruction = instructionByOpcode(m_curOpcode);
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
		m_fetchedData = m_pBus->busRead(m_registers.pc);
		++m_registers.pc;
		return;
	case AM_D16: {
		auto low = m_pBus->busRead(m_registers.pc);
		auto high = m_pBus->busRead(m_registers.pc + 1u);
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