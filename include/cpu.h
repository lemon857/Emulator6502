#ifndef _CPU_H_
#define _CPU_H_

#include "memory.h"

struct CPU
{
	// opcodes
	static constexpr Byte
		INS_TAX = 0xAA,			// transfer A to X
		INS_TXA = 0x8A,			// transfer X to A
		INS_DEX = 0xCA,			// decrement X
		INS_INX = 0xE8,			// increment X
		INS_TAY = 0xA8,			// transfer Y to A
		INS_TYA = 0x98,			// transfer A to Y
		INS_DEY = 0x88,			// decrement Y
		INS_INY = 0xC8,			// increment Y

		INS_ADC_IM = 0x69,		// add 

		INS_AND_IM = 0x29,		// logical A and value
		INS_AND_ZP = 0x25,		// logical A and from (ZeroPage (8-bit) address)
		INS_AND_ZPX = 0x35,		// logical A and from (ZeroPage (8-bit) address + value X)
		INS_AND_ABS = 0x2D,		// logical A and from (absolute address)
		INS_AND_ABSX = 0x3D,	// logical A and from (absolute address + value X)
		INS_AND_ABSY = 0x39,	// logical A and from (absolute address + value Y)

		INS_STA_ZP = 0x85,		// store value from A to (ZeroPage (8-bit) address)
		INS_STA_ZPX = 0x95,		// store value from A to (ZeroPage (8-bit) address + value X)
		INS_STA_ABS = 0x8D,		// store value from A to (absolute address)
		INS_STA_ABSX = 0x9D,	// store value from A to (absolute address + value X)
		INS_STA_ABSY = 0x99,	// store value from A to (absolute address + value Y)

		INS_STX_ZP = 0x86,		// store value from X to (ZeroPage (8-bit) address)
		INS_STX_ZPY = 0x96,		// store value from X to (ZeroPage (8-bit) address + value Y)
		INS_STX_ABS = 0x8E,		// store value from X to (absolute address)

		INS_STY_ZP = 0x84,		// store value from Y to (ZeroPage (8-bit) address)
		INS_STY_ZPX = 0x94,		// store value from Y to (ZeroPage (8-bit) address + value X)
		INS_STY_ABS = 0x8C,		// store value from Y to (absolute address)

		INS_LDA_IM = 0xA9,		// set value A
		INS_LDA_ZP = 0xA5,		// set value A (from ZeroPage (8-bit) address)
		INS_LDA_ZPX = 0xB5,		// set value A (from ZeroPage (8-bit) address + value X )
		INS_LDA_ABS = 0xAD,		// set value A (from absolute address)
		INS_LDA_ABSX = 0xBD,	// set value A (from absolute address + value X)
		INS_LDA_ABSY = 0xB9,	// set value A (from absolute address + value Y)
		INS_LDA_INDX = 0xA1,	// set value A from address -> (from absolute address + value X)
		INS_LDA_INDY = 0xB1,	// set value A from address -> (from absolute address + value Y)

		INS_LDX_IM = 0xA2,		// set value X
		INS_LDX_ZP = 0xA6,		// set value X from (ZeroPage (8-bit) address)
		INS_LDX_ZPY = 0xB6,		// set value X from (ZeroPage (8-bit) address + value Y)
		INS_LDX_ABS = 0xAE,		// set value X from (absolute address)
		INS_LDX_ABSY = 0xBE,	// set value X from (absolute address + value Y)

		INS_LDY_IM = 0xA0,		// set value Y
		INS_LDY_ZP = 0xA4,		// set value Y from (ZeroPage (8-bit) address)
		INS_LDY_ZPX = 0xB4,		// set value Y from (ZeroPage (8-bit) address + value X)
		INS_LDY_ABS = 0xAC,		// set value Y from (absolute address)
		INS_LDY_ABSX = 0xBC,	// set value Y from (absolute address + value X)

		INS_BEQ = 0xF0,			// branch if Zero flag true
		INS_BNE = 0xD0,			// branch if Zero flag false

		INS_TXS = 0x9A,			// transfer X to stack pointer
		INS_TSX = 0xBA,			// transfer stack pointer to X
		INS_PHA = 0x48,			// push accumulator
		INS_PLA = 0x68,			// pull accumulator from stack


		INS_JMP = 0x4C,			// jump to address
		INS_JMP_IND = 0x6C,		// jump to address
		INS_JSR = 0x20,			// jump to subroutine 
		INS_RTS = 0x60;			// return from subroutine


	Word PC;		// program counter
	Word SP;		// stack pointer

	Byte A, X, Y;	// registers

	Byte C : 1;		// status flags		
	Byte Z : 1;		// status flags
	Byte I : 1;		// status flags
	Byte D : 1;		// status flags
	Byte B : 1;		// status flags
	Byte V : 1;		// status flags
	Byte N : 1;		// status flags

	void Reset(Memory& memory);

	Byte FetchByte(u32& cycles, Memory& memory);

	Word FetchWord(u32& cycles, Memory& memory);

	Byte ReadByteFromZeroPage(u32& cycles, Byte& addr, Memory& memory);

	Byte ReadByte(u32& cycles, Word addr, Memory& memory);

	Word ReadWord(u32& cycles, Word addr, Memory& memory);

	void PushByteOntoStack(Byte value, u32& cycles, Memory& memory);
	void PullByteFromStack(Byte& value, u32& cycles, Memory& memory);

	inline void LoadRegisterSetStatus(Byte value);

	void Execute(u32 cycles, Memory& memory);
};

#endif // !_CPU_H_
