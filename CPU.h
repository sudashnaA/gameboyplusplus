#pragma once
#include <cstdint>
#include <memory>
#include "util.h"
#include "Bus.h"
#include "Instruction.h"

class Emulator;

class CPU
{
public:
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
	std::unique_ptr<Instruction> m_currInstruction{};
	std::weak_ptr<Bus> m_pBus;
	std::weak_ptr<Emulator> m_pEmu;

	uint16_t reverse(uint16_t val) const noexcept;
	uint16_t readRegister(RegisterType type) noexcept;

	void emulatorCycles(int cpuCycles);
	uint8_t busRead(uint16_t address);
	void busWrite(uint16_t address, uint8_t value);
};


