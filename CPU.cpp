#include <tuple>
#include <iostream>
#include "CPU.h"
#include "util.h"
#include "Emulator.h"

CPU::CPU()
{
	m_registers.pc = 0x100;
	m_registers.a = 0x01;
}

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
	if (!m_halted) 
	{
		//auto pc = m_registers.pc;

		fetchInstruction();
		fetchData();

		execute();
	}

	return true;
}

void CPU::fetchInstruction()
{
	m_curOpcode = busRead(m_registers.pc++);
	m_currInstruction = instructionByOpcode(m_curOpcode);
}

void CPU::fetchData()
{
	m_memDest = 0;
	m_destIsMem = false;

	if (m_currInstruction == nullptr)
	{
		std::cout << "Unknown instruction\n";
		return;
	}

	switch (m_currInstruction->mode) {
		using enum AddressMode;

	case AM_IMP: 
		return;
	case AM_R:
		m_fetchedData = readRegister(m_currInstruction->reg1);
		return;
	case AM_R_D8:
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
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
	auto proc = m_processors.find(m_currInstruction->type);

	if (proc == m_processors.end()) {
		return;
	}

	// call function
	(this->*proc->second)();
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

bool CPU::checkCondition() const
{
	bool z = CPU_FLAG_Z;
	bool c = CPU_FLAG_C;

	using enum ConditionType;

	// Based on the condition of the current instruction, return
	// if it is true
	switch (m_currInstruction->cond) {
	case CT_NONE: return true;
	case CT_C: return c;
	case CT_NC: return !c;
	case CT_Z: return z;
	case CT_NZ:	 return !z;
	}

	return false;
}

uint16_t CPU::reverse(uint16_t val) const noexcept
{
	return static_cast<uint16_t>(((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8));
}

uint16_t CPU::readRegister(RegisterType type) noexcept
{
	switch (type)
	{
	case RegisterType::RT_A: return m_registers.a;
	case RegisterType::RT_B: return m_registers.b;
	case RegisterType::RT_C: return m_registers.c;
	case RegisterType::RT_D: return m_registers.d;
	case RegisterType::RT_E: return m_registers.e;
	case RegisterType::RT_F: return m_registers.f;
	case RegisterType::RT_H: return m_registers.h;
	case RegisterType::RT_L: return m_registers.l;

	case RegisterType::RT_AF: return reverse(*(reinterpret_cast<uint16_t*>(&m_registers.a)));
	case RegisterType::RT_BC: return reverse(*(reinterpret_cast<uint16_t*>(&m_registers.b)));
	case RegisterType::RT_DE: return reverse(*(reinterpret_cast<uint16_t*>(&m_registers.d)));
	case RegisterType::RT_HL: return reverse(*(reinterpret_cast<uint16_t*>(&m_registers.h)));

	case RegisterType::RT_PC: return m_registers.pc;
	case RegisterType::RT_SP: return m_registers.sp;
	default: return 0;
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

void CPU::jp()
{

}
