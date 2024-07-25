#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "memory.hpp"

const Word STACK_POINTER = 0x01FF; // stack 0x0100 - 0x01FF and go from up to down

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

		INS_ADC_IM = 0x69,		// add with carry A and value
		INS_ADC_ZP = 0x65,		// add with carry A and (from ZeroPage (8-bit) address)
		INS_ADC_ZPX = 0x75,		// add with carry A	and (from ZeroPage (8-bit) address + value X )
		INS_ADC_ABS = 0x6D,		// add with carry A	and (from absolute address)
		INS_ADC_ABSX = 0x7D,	// add with carry A	and (from absolute address + value X)
		INS_ADC_ABSY = 0x79,	// add with carry A	and (from absolute address + value Y)
		INS_ADC_INDX = 0x61,	// add with carry A	and from address -> (from absolute address + value X)
		INS_ADC_INDY = 0x71,	// add with carry A	and from address -> (from absolute address + value Y)

		INS_SBC_IM = 0xE9,		// substract with carry A and value
		INS_SBC_ZP = 0xE5,		// substract with carry A and (from ZeroPage (8-bit) address)
		INS_SBC_ZPX = 0xF5,		// substract with carry A and (from ZeroPage (8-bit) address + value X )
		INS_SBC_ABS = 0xED,		// substract with carry A and (from absolute address)
		INS_SBC_ABSX = 0xFD,	// substract with carry A and (from absolute address + value X)
		INS_SBC_ABSY = 0xF9,	// substract with carry A and (from absolute address + value Y)
		INS_SBC_INDX = 0xE1,	// substract with carry A and from address -> (from absolute address + value X)
		INS_SBC_INDY = 0xF1,	// substract with carry A and from address -> (from absolute address + value Y)

		INS_AND_IM = 0x29,		// logical A and value
		INS_AND_ZP = 0x25,		// logical A and from (ZeroPage (8-bit) address)
		INS_AND_ZPX = 0x35,		// logical A and from (ZeroPage (8-bit) address + value X)
		INS_AND_ABS = 0x2D,		// logical A and from (absolute address)
		INS_AND_ABSX = 0x3D,	// logical A and from (absolute address + value X)
		INS_AND_ABSY = 0x39,	// logical A and from (absolute address + value Y)
		INS_AND_INDX = 0x21,	// logical A and from from address -> (absolute address + value X)
		INS_AND_INDY = 0x31,	// logical A and from from address -> (absolute address + value Y)

		INS_ORA_IM = 0x09,		// logical A or value
		INS_ORA_ZP = 0x05,		// logical A or from (ZeroPage (8-bit) address)
		INS_ORA_ZPX = 0x15,		// logical A or from (ZeroPage (8-bit) address + value X)
		INS_ORA_ABS = 0x0D,		// logical A or from (absolute address)
		INS_ORA_ABSX = 0x1D,	// logical A or from (absolute address + value X)
		INS_ORA_ABSY = 0x19,	// logical A or from (absolute address + value Y)
		INS_ORA_INDX = 0x01,	// logical A or from from address -> (absolute address + value X)
		INS_ORA_INDY = 0x11,	// logical A or from from address -> (absolute address + value Y)

		INS_EOR_IM = 0x49,		// logical A exclusive or value
		INS_EOR_ZP = 0x45,		// logical A exclusive or from (ZeroPage (8-bit) address)
		INS_EOR_ZPX = 0x55,		// logical A exclusive or from (ZeroPage (8-bit) address + value X)
		INS_EOR_ABS = 0x4D,		// logical A exclusive or from (absolute address)
		INS_EOR_ABSX = 0x5D,	// logical A exclusive or from (absolute address + value X)
		INS_EOR_ABSY = 0x59,	// logical A exclusive or from (absolute address + value Y)
		INS_EOR_INDX = 0x41,	// logical A exclusive or from from address -> (absolute address + value X)
		INS_EOR_INDY = 0x51,	// logical A exclusive or from from address -> (absolute address + value Y)

		INS_STA_ZP = 0x85,		// store value from A to (ZeroPage (8-bit) address)
		INS_STA_ZPX = 0x95,		// store value from A to (ZeroPage (8-bit) address + value X)
		INS_STA_ABS = 0x8D,		// store value from A to (absolute address)
		INS_STA_ABSX = 0x9D,	// store value from A to (absolute address + value X)
		INS_STA_ABSY = 0x99,	// store value from A to (absolute address + value Y)
		INS_STA_INDX = 0x81,	// store value from A to address from -> (absolute address + value X)
		INS_STA_INDY = 0x91,	// store value from A to address from -> (absolute address + value Y)

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

		INS_DEC_ZP = 0xC6,		// decrement value from (ZeroPage (8-bit) address)
		INS_DEC_ZPX = 0xD6,		// decrement value from (ZeroPage (8-bit) address + value X)
		INS_DEC_ABS = 0xCE,		// decrement value from (absolute address)
		INS_DEC_ABSX = 0xDE,	// decrement value from (absolute address + value X)

		INS_INC_ZP = 0xE6,		// increment value from (ZeroPage (8-bit) address)
		INS_INC_ZPX = 0xF6,		// increment value from (ZeroPage (8-bit) address + value X)
		INS_INC_ABS = 0xEE,		// increment value from (absolute address)
		INS_INC_ABSX = 0xFE,	// increment value from (absolute address + value X)

		INS_BEQ = 0xF0,			// branch if Zero flag true
		INS_BNE = 0xD0,			// branch if Zero flag false
		INS_BCS = 0xB0,			// branch if Carry flag true
		INS_BCC = 0x90,			// branch if Carry flag false

		INS_CLC = 0x18,			// clear Carry flag
		INS_SEC = 0x38,			// set Carry flag
		INS_CLV = 0xB8,			// clear Overflow flag

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

	Byte C : 1;		// carry flag		
	Byte Z : 1;		// zero flag
	Byte I : 1;		// interrupt flag
	Byte D : 1;		// decimal flag
	Byte B : 1;		// unused
	Byte V : 1;		// overflow flag
	Byte N : 1;		// negative flag

	Word SPToAddress() const // because stack works top down
	{
		return 0x100 | SP;
	}

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

	void Step(u32& cycles, Memory& memory);
};

#endif // _CPU_HPP_
