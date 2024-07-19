#ifndef _CPU_H_
#define _CPU_H_

#include "memory.h"

struct CPU
{
	/*Immediate     AND #$44      $29  2   2
	Zero Page     AND $44       $25  2   3
	Zero Page,X   AND $44,X     $35  2   4
	Absolute      AND $4400     $2D  3   4
	Absolute,X    AND $4400,X   $3D  3   4+
	Absolute,Y    AND $4400,Y   $39  3   4+
*/
	// opcodes
	static constexpr Byte
		INS_AND_IM = 0x29,		// logical A and value
		INS_AND_ZP = 0x25,		// logical A and from (ZeroPage (8-bit) address)
		INS_AND_ZPX = 0x35,		// logical A and from (ZeroPage (8-bit) address + value X)
		INS_AND_ABS = 0x2D,		// logical A and from (absolute address)
		INS_AND_ABSX = 0x3D,	// logical A and from (absolute address + value X)
		INS_AND_ABSY = 0x39,	// logical A and from (absolute address + value Y)

		INS_LDA_IM = 0xA9,		// set value A
		INS_LDA_ZP = 0xA5,		// set value A (from ZeroPage (8-bit) address)
		INS_LDA_ZPX = 0xB5,		// set value A (from ZeroPage (8-bit) address + value X )
		INS_LDA_ABS = 0xAD,		// set value A (from absolute address)
		INS_LDA_ABSX = 0xBD,	// set value A (from absolute address + value X)
		INS_LDA_ABSY = 0xB9,	// set value A (from absolute address + value Y)

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

	Byte ReadByteFromZeroPage(u32& cycles, Byte& addr, Memory& memory);

	Byte ReadByte(u32& cycles, Word& addr, Memory& memory);

	inline void LoadRegisterSetStatus(Byte value);

	void Execute(u32 cycles, Memory& memory);
};

#endif // !_CPU_H_
