#include <tuple>
#include <iostream>
#include "CPU.h"
#include "util.h"
#include "Emulator.h"

// constants
constexpr int RT_C_OR_VALUE{ 0xFF00 };

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

		printf("%04X: %-7s (%02X %02X %02X) A: %02X BC: %02X%02X DE: %02X%02X HL: %02X%02X\n",
			pc, instructionName(m_currInstruction->type), m_curOpcode,
			busRead(pc + 1u), (busRead(pc + 2u)),
			m_registers.a, m_registers.b, m_registers.c,
			m_registers.d, m_registers.e, m_registers.h, m_registers.l
		);

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
	case AM_R_R:
		m_fetchedData = readRegister(m_currInstruction->reg2);
		return;
	case AM_R_D8:
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
		++m_registers.pc;
		return;

	case AM_R_D16:
	case AM_D16: {
		auto low{ busRead(m_registers.pc) };
		emulatorCycles(1);

		auto high{ busRead(m_registers.pc + 1u) };
		emulatorCycles(1);

		m_fetchedData = static_cast<uint16_t>(low | (high << 8));
		m_registers.pc += 2;
		return;
	};

	case AM_MR_R: {
		m_fetchedData = readRegister(m_currInstruction->reg2);
		m_memDest = readRegister(m_currInstruction->reg1);
		m_destIsMem = true;
		
		if (m_currInstruction->reg1 == RegisterType::RT_C) {
			m_memDest |= RT_C_OR_VALUE;
		}
		return;
	}

	case AM_R_MR: {
		auto addr{ readRegister(m_currInstruction->reg2) };
		
		if (m_currInstruction->reg1 == RegisterType::RT_C) {
			addr |= RT_C_OR_VALUE;
		}

		m_fetchedData = busRead(addr);
		emulatorCycles(1);
		return;
	}

	case AM_R_HLI: {
		m_fetchedData = busRead(readRegister(m_currInstruction->reg2));
		emulatorCycles(1);
		setRegister(RegisterType::RT_HL, static_cast<uint16_t>(readRegister(RegisterType::RT_HL) + 1));
		return;
	}

	case AM_R_HLD: {
		m_fetchedData = busRead(readRegister(m_currInstruction->reg2));
		emulatorCycles(1);
		setRegister(RegisterType::RT_HL, static_cast<uint16_t>(readRegister(RegisterType::RT_HL) - 1));
		return;
	}

	case AM_HLI_R: {
		m_fetchedData = readRegister(m_currInstruction->reg2);
		m_memDest = readRegister(m_currInstruction->reg1);
		m_destIsMem = true;
		setRegister(RegisterType::RT_HL, static_cast<uint16_t>(readRegister(RegisterType::RT_HL) + 1));
		return;
	}

	case AM_HLD_R: {
		m_fetchedData = readRegister(m_currInstruction->reg2);
		m_memDest = readRegister(m_currInstruction->reg1);
		m_destIsMem = true;
		setRegister(RegisterType::RT_HL, static_cast<uint16_t>(readRegister(RegisterType::RT_HL) - 1));
		return;
	}

	case AM_R_A8: {
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
		++m_registers.pc;
		return;
	}

	case AM_A8_R: {
		m_fetchedData = static_cast<uint16_t>(busRead(m_registers.pc) | RT_C_OR_VALUE);
		m_destIsMem = true;
		emulatorCycles(1);
		++m_registers.pc;
		return;
	}

	case AM_HL_SPR: {
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
		++m_registers.pc;
		return;
	}

	case AM_D8: {
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
		++m_registers.pc;
		return;
	}

	case AM_A16_R:
	case AM_D16_R: {
		auto low{ static_cast<uint16_t>(busRead(m_registers.pc)) };
		emulatorCycles(1);
		auto high{ static_cast<uint16_t>(busRead(m_registers.pc) + 1) };
		emulatorCycles(1);

		m_memDest = static_cast<uint16_t>(low | (high << 8));
		m_destIsMem = true;

		m_registers.pc += 2;
		m_fetchedData = readRegister(m_currInstruction->reg2);
		return;
	}

	case AM_MR_D8: {
		m_fetchedData = busRead(m_registers.pc);
		emulatorCycles(1);
		++m_registers.pc;
		m_memDest = readRegister(m_currInstruction->reg1);
		m_destIsMem = true;
		return;
	}

	case AM_MR: {
		m_memDest = readRegister(m_currInstruction->reg1);
		m_destIsMem = true;
		m_fetchedData = busRead(readRegister(m_currInstruction->reg1));
		emulatorCycles(1);
		return;
	}

	case AM_R_A16: {
		auto low{ static_cast<uint16_t>(busRead(m_registers.pc)) };
		emulatorCycles(1);
		auto high{ static_cast<uint16_t>(busRead(m_registers.pc) + 1) };
		emulatorCycles(1);

		auto addr{ static_cast<uint16_t>(low | (high << 8)) };

		m_registers.pc += 2;
		m_fetchedData = busRead(addr);
		emulatorCycles(1);
		return;
	}

	default: {
		printf("Unknown Addressing Mode! %d (%02X)\n", m_currInstruction->mode, m_curOpcode);
		exit(-7);
		return;
	}
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

uint8_t CPU::getIeRegister() const noexcept
{
	return m_ieRegister;
}

void CPU::setIeRegister(uint8_t val) noexcept
{
	m_ieRegister = val;
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

void CPU::setRegister(RegisterType type, uint16_t val) noexcept {
	uint8_t AND_VALUE{ 0xFFu };

	switch (type)
	{
	case RegisterType::RT_A:
		m_registers.a = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_B:
		m_registers.b = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_C:
		m_registers.c = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_D:
		m_registers.d = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_E:
		m_registers.e = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_F:
		m_registers.f = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_H:
		m_registers.h = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_L:
		m_registers.l = static_cast<uint8_t>(val & AND_VALUE);
		break;
	case RegisterType::RT_AF:
		*(reinterpret_cast<uint16_t*>(&m_registers.a)) = reverse(val);
		break;
	case RegisterType::RT_BC:
		*(reinterpret_cast<uint16_t*>(&m_registers.b)) = reverse(val);
		break;
	case RegisterType::RT_DE:
		*(reinterpret_cast<uint16_t*>(&m_registers.d)) = reverse(val);
		break;
	case RegisterType::RT_HL:
		*(reinterpret_cast<uint16_t*>(&m_registers.h)) = reverse(val);
		break;
	case RegisterType::RT_PC:
		m_registers.pc = val;
		break;
	case RegisterType::RT_SP:
		m_registers.sp = val;
		break;
	case RegisterType::RT_NONE:
		break;
	}
}

// ------------

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

void CPU::LDH()
{
	if (m_currInstruction->reg1 == RegisterType::RT_A) {
		setRegister(m_currInstruction->reg1, busRead(static_cast<uint16_t>(0xFF00 | m_fetchedData)));
	}
	else {
		busWrite(static_cast<uint16_t>(0xFF00 | m_fetchedData), m_registers.a);
	}

	emulatorCycles(1);
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
	if (m_destIsMem) {
		if (m_currInstruction->reg2 >= RegisterType::RT_AF) {
			// 16 bit register
			emulatorCycles(1);
			busWrite16(m_memDest, m_fetchedData);
		} else {
			busWrite(m_memDest, static_cast<uint8_t>(m_fetchedData));
		}

		return;
	}

	if (m_currInstruction->mode == AddressMode::AM_HL_SPR) {
		uint8_t hflag{ (readRegister(m_currInstruction->reg2) & 0xF) + (m_fetchedData & 0xF) >= 0x10 };
		uint8_t cflag{ (readRegister(m_currInstruction->reg2) & 0xFF) + (m_fetchedData & 0xFF) >= 0x100 };

		setFlags(0, 0, static_cast<char>(hflag), static_cast<char>(cflag));
		setRegister(m_currInstruction->reg1, static_cast<uint16_t>(readRegister(m_currInstruction->reg2) + static_cast<char>(m_fetchedData)));
		return;
	}

	setRegister(m_currInstruction->reg1, m_fetchedData);
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

// helper functions
uint8_t CPU::busRead(uint16_t address)
{
	auto ptr{ m_pBus.lock() };
	if (ptr) {
		return ptr->busRead(address);
	}

	return 0x00;
}

uint16_t CPU::busRead16(uint16_t address)
{
	auto ptr{ m_pBus.lock() };
	if (ptr) {
		return ptr->busRead16(address);
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
void CPU::busWrite16(uint16_t address, uint16_t value)
{
	auto ptr{ m_pBus.lock() };
	if (ptr) {
		ptr->busWrite16(address, value);
		return;
	}
}
