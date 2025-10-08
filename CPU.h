#pragma once
#include <cstdint>
#include <memory>
#include <map>
#include "util.h"
#include "Bus.h"
#include "Instruction.h"
#include "functional"

#define CPU_FLAG_Z BIT(m_registers.f, 7)
#define CPU_FLAG_C BIT(m_registers.f, 4)

class Emulator;

class CPU
{
public:
	CPU();

	void connectEmulator(std::shared_ptr<Emulator> e);
	void connectBus(std::shared_ptr<Bus> b);

	bool cpuStep();
	void fetchInstruction();
	void fetchData();
	void execute();

private:
	struct Registers {
		uint8_t a{};
		uint8_t f{};
		uint8_t b{};
		uint8_t c{};
		uint8_t d{};
		uint8_t e{};
		uint8_t h{};
		uint8_t l{};
		uint16_t pc{};
		uint16_t sp{};
	};

	Registers m_registers{};
	uint16_t m_fetchedData{};
	uint16_t m_memDest{};
	uint8_t m_curOpcode{};
	bool m_destIsMem{};
	bool m_halted{};
	bool m_stepping{};
	bool m_intMasterEnabled{ false };
	std::unique_ptr<Instruction> m_currInstruction{};
	std::weak_ptr<Bus> m_pBus;
	std::weak_ptr<Emulator> m_pEmu;

	uint16_t reverse(uint16_t val) const noexcept;
	uint16_t readRegister(RegisterType type) noexcept;

	void emulatorCycles(int cpuCycles);
	uint8_t busRead(uint16_t address);
	void busWrite(uint16_t address, uint8_t value);

	bool checkCondition() const;
	void setFlags(char z, char n, char h, char c);

	// instructions
	void JP();
	void XOR();
	void NOP();
	void NONE();
	void DI();
	void LD();

	using FuncPtr = void (CPU::*)();

	// Map InstructionType to the function for that instruction
	std::map<InstructionType, FuncPtr> m_processors
	{
		{IN_JP, &CPU::JP},
		{IN_XOR, &CPU::XOR},
		{IN_NOP, &CPU::NOP},
		{IN_NONE, &CPU::NONE},
		{IN_DI, &CPU::DI},
		{IN_LD, &CPU::LD},
	};

	// 
};


