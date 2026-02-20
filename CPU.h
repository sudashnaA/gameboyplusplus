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
struct Registers;

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

	uint8_t getIeRegister() const noexcept;
	void setIeRegister(uint8_t val) noexcept;

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
	uint8_t m_ieRegister{};
	std::unique_ptr<Instruction> m_currInstruction{};
	std::weak_ptr<Bus> m_pBus;
	std::shared_ptr<Emulator> m_pEmu;

	uint16_t reverse(uint16_t val) const noexcept;
	uint16_t readRegister(RegisterType type) noexcept;
	void setRegister(RegisterType type, uint16_t val) noexcept;

	void emulatorCycles(int cpuCycles);

	// bus helper functions
	uint8_t busRead(uint16_t address);
	uint16_t busRead16(uint16_t address);
	void busWrite(uint16_t address, uint8_t value);
	void busWrite16(uint16_t address, uint16_t value);

	bool checkCondition() const;
	void setFlags(char z, char n, char h, char c);

	// instructions
	void JP();
	void CALL();
	void RST();
	void RET();
	void RETI();
	void JR();
	void XOR();
	void NOP();
	void NONE();
	void DI();
	void LD();
	void LDH();
	void POP();
	void PUSH();
	void INC();
	void DEC();
	void ADD();
	void ADC();
	void SUB();
	void SBC();

	// helper function that is used in instructions
	void gotoAddr(uint16_t addr, bool pushpc);
	constexpr bool is16Bit(RegisterType rt) const noexcept;

	using FuncPtr = void (CPU::*)();

	// Map InstructionType to the function for that instruction
	std::map<InstructionType, FuncPtr> m_processors
	{
		{IN_NOP, &CPU::NOP},
		{IN_NONE, &CPU::NONE},
		{IN_LD, &CPU::LD},
		{IN_LDH, &CPU::LDH},
		{IN_JP, &CPU::JP},
		{IN_POP, &CPU::POP},
		{IN_PUSH, &CPU::PUSH},
		{IN_DI, &CPU::DI},
		{IN_JR, &CPU::JR},
		{IN_CALL, &CPU::CALL},
		{IN_RST, &CPU::RST},
		{IN_RET, &CPU::RET},
		{IN_RETI, &CPU::RETI},
		{IN_INC, &CPU::INC},
		{IN_ADD, &CPU::ADD},
		{IN_ADC, &CPU::ADC},
		{IN_SUB, &CPU::SUB},
		{IN_SBC, &CPU::SBC},
		{IN_DEC, &CPU::DEC},
		{IN_XOR, &CPU::XOR},
	};

	// 

	FuncPtr getProcessor(InstructionType type);

	// stack
	void stackPush(uint8_t data);
	void stackPush16(uint16_t data);
	uint8_t stackPop();
	uint16_t stackPop16();

	// debug
	int curr{};
};


