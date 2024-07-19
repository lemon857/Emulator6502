#ifndef _CPU_H_
#define _CPU_H_

#include "memory.h"

struct CPU
{
	// opcodes
	static constexpr Byte
		INS_LDA_IM = 0xA9,		// set value A
		INS_LDA_ZP = 0xA5,		// set value A from ZeroPage (8-bit) address
		INS_LDA_ZPX = 0xB5,		// set value A from ZeroPage (8-bit) address + value X 
		INS_JMP = 0x4C,			// jump to address
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

	Byte ReadByte(u32& cycles, Byte& addr, Memory& memory);

	inline void LDASetStatus();

	void Execute(u32 cycles, Memory& memory);
};

#endif // !_CPU_H_
