#include "Instruction.h"

constexpr auto getInstructions()
{
    using enum InstructionType;
    using enum ConditionType;
    using enum RegisterType;
    using enum AddressMode;

    std::array<Instruction, 0x100> instructions{};

    instructions[0x00] = { IN_NOP, AM_IMP };
    instructions[0x01] = { IN_LD, AM_R_D16, RT_BC };
    instructions[0x02] = { IN_LD, AM_MR_R, RT_BC, RT_A };

    instructions[0x05] = { IN_DEC, AM_R, RT_B };
    instructions[0x06] = { IN_LD, AM_R_D8, RT_B };

    instructions[0x08] = { IN_LD, AM_A16_R, RT_NONE, RT_SP };

    instructions[0x0A] = { IN_LD, AM_R_MR, RT_A, RT_BC };
    instructions[0x0E] = { IN_LD, AM_R_D8, RT_C };

    // 0x1X
    instructions[0x11] = { IN_LD, AM_R_D16, RT_DE };
    instructions[0x12] = { IN_LD, AM_MR_R, RT_DE, RT_A };
    instructions[0x15] = { IN_DEC, AM_R, RT_D };
    instructions[0x16] = { IN_LD, AM_R_D8, RT_D };
    instructions[0x18] = { IN_JR, AM_D8 };
    instructions[0x1A] = { IN_LD, AM_R_MR, RT_A, RT_DE };
    instructions[0x1E] = { IN_LD, AM_R_D8, RT_E };

    // 0x2X
    instructions[0x20] = { IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NZ};
    instructions[0x21] = { IN_LD, AM_R_D16, RT_HL };
    instructions[0x22] = { IN_LD, AM_HLI_R, RT_HL, RT_A };
    instructions[0x25] = { IN_DEC, AM_R, RT_H };
    instructions[0x26] = { IN_LD, AM_R_D8, RT_H };
    instructions[0x28] = { IN_JR, AM_D8, RT_NONE, RT_NONE, CT_Z };
    instructions[0x2A] = { IN_LD, AM_R_HLI, RT_A, RT_HL };
    instructions[0x2E] = { IN_LD, AM_R_D8, RT_L };

    // 0x3X
    instructions[0x30] = { IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NC };
    instructions[0x31] = { IN_LD, AM_R_D16, RT_SP };
    instructions[0x32] = { IN_LD, AM_HLD_R, RT_HL, RT_A };
    instructions[0x35] = { IN_DEC, AM_R, RT_HL };
    instructions[0x38] = { IN_JR, AM_D8, RT_NONE, RT_NONE, CT_C };
    instructions[0x36] = { IN_LD, AM_MR_D8, RT_HL };
    instructions[0x3A] = { IN_LD, AM_R_HLD, RT_A, RT_HL };
    instructions[0x3E] = { IN_LD, AM_R_D8, RT_A };

    // 0x4X–0x7X
    instructions[0x76] = { IN_HALT };

    instructions[0xAF] = { IN_XOR, AM_R, RT_A };

    instructions[0xC1] = { IN_POP, AM_IMP, RT_BC };
    instructions[0xC3] = { IN_JP, AM_D16 };
    instructions[0xC4] = { IN_CALL, AM_D16, RT_NONE, RT_NONE, CT_NZ};
    instructions[0xC5] = { IN_PUSH, AM_R, RT_BC };
    instructions[0xCC] = { IN_CALL, AM_D16, RT_NONE, RT_NONE, CT_Z };
    instructions[0xCD] = { IN_CALL, AM_D16 };

    instructions[0xD1] = { IN_POP, AM_IMP, RT_DE };
    instructions[0xD4] = { IN_CALL, AM_D16, RT_NONE, RT_NONE, CT_NC };
    instructions[0xD5] = { IN_PUSH, AM_R, RT_DE };
    instructions[0xDC] = { IN_CALL, AM_D16, RT_NONE, RT_NONE, CT_C };

    // 0xEX
    instructions[0xE0] = { IN_LDH, AM_A8_R, RT_NONE, RT_A };
    instructions[0xE1] = { IN_POP, AM_IMP, RT_HL };
    instructions[0xE2] = { IN_LD, AM_MR_R, RT_C, RT_A };
    instructions[0xE5] = { IN_PUSH, AM_R, RT_HL };
    instructions[0xEA] = { IN_LD, AM_A16_R, RT_NONE, RT_A };

    // 0xFX
    instructions[0xF0] = { IN_LDH, AM_R_A8, RT_A };
    instructions[0xF1] = { IN_POP, AM_IMP, RT_AF };
    instructions[0xF2] = { IN_LD, AM_R_MR, RT_A, RT_C };
    instructions[0xF3] = { IN_DI };
    instructions[0xF5] = { IN_PUSH, AM_R, RT_AF };
    instructions[0xFA] = { IN_LD, AM_R_A16, RT_A };

    return instructions;
}

constexpr auto instructions{ getInstructions() };

std::unique_ptr<Instruction> instructionByOpcode(uint8_t opcode)
{
	return std::make_unique<Instruction>(instructions[opcode]);
}
