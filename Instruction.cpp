#include "Instruction.h"
#include <array>


constexpr auto getInstructions()
{
	using enum InstructionType;
	using enum ConditionType;
	using enum RegisterType;
	using enum AddressMode;

	std::array<Instruction, 0x100> instructions{};
	instructions[0x05] = { IN_NOP, AM_IMP };
	instructions[0x05] = { IN_DEC, AM_R, RT_B };
	instructions[0x0E] = { IN_LD, AM_R_D8, RT_C };
	instructions[0xAF] = { IN_XOR, AM_R, RT_A };
	instructions[0xC3] = { IN_JP, AM_D16 };
	instructions[0xF3] = { IN_DI };

	return instructions;
}

constexpr auto instructions = getInstructions();

std::unique_ptr<Instruction> instructionByOpcode(uint8_t opcode)
{
	if (instructions[opcode].type == InstructionType::IN_NONE) {
		return nullptr;
	}

	return std::make_unique<Instruction>(instructions[opcode]);
}
