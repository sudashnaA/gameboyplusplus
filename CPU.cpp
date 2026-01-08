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
		auto pc = m_registers.pc;

		fetchInstruction();
		fetchData();

		//printf("Executing Instruction: %02X   PC: %04X\n", m_curOpcode, pc);

		printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
			pc, instructionName(m_currInstruction->type), m_curOpcode,
			busRead(pc + 1u), (busRead(pc + 2u)),
			m_registers.a, m_registers.b, m_registers.c);

		if (m_currInstruction == nullptr) {
			printf("Unknown Instruction! %02X\n", m_curOpcode);
			exit(-7);
		}

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
		printf("Unknown Addressing Mode! %d (%02X)\n", m_currInstruction->mode, m_curOpcode);
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

// helper functions
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

// util
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

void CPU::JP()
{
	if (checkCondition()) {
		m_registers.pc = m_fetchedData;
		emulatorCycles(1);
	}
}

void CPU::XOR() 
{
	// Set A to the bitwise XOR between the value in fetchedData and A. One byte
	m_registers.a ^= m_fetchedData & 0xFF;
	// Z flag is set if result is 0
	setFlags(m_registers.a == 0, 0, 0, 0);
}

void CPU::LD()
{
}

using FuncPtr = void (CPU::*)();

FuncPtr CPU::getProcessor(InstructionType type)
{
	return m_processors[type];
}

void CPU::NOP()
{
}

void CPU::NONE()
{
	std::cout << "INVALID INSTRUCTION!\n";
	exit(-7);
}

void CPU::DI()
{
	m_intMasterEnabled = false;
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

void CPU::setFlags(char z, char n, char h, char c)
{
	if (z != -1) {
		BIT_SET(m_registers.f, 7, z);
	}
	if (n != -1) {
		BIT_SET(m_registers.f, 6, n);
	}
	if (h != -1) {
		BIT_SET(m_registers.f, 5, h);
	}
	if (c != -1) {
		BIT_SET(m_registers.f, 4, c);
	}
}
