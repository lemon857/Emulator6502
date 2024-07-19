#include "cpu.h"

void CPU::Execute(u32 cycles, Memory& memory)
{
	while (cycles > 0)
	{
		Byte ins = FetchByte(cycles, memory);
		switch (ins)
		{
		case INS_LDA_IM: {
			Byte value = FetchByte(cycles, memory);
			A = value;
			LDASetStatus();
		} break;
		case INS_LDA_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			A = ReadByte(cycles, zeroPageAddr, memory);
			LDASetStatus();
		} break;
		case INS_LDA_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			A = ReadByte(cycles, zeroPageAddr, memory);
			LDASetStatus();
		} break;
		case INS_JMP: {
			Word subAddr = FetchWord(cycles, memory);
			PC = subAddr;
		} break;
		case INS_JSR: {
			Word subAddr = FetchWord(cycles, memory);
			memory.WriteWord(PC - 1, SP, cycles);
			SP += 2;
			cycles--;
			PC = subAddr;
		} break;
		case INS_RTS: {
			SP -= 2;
			cycles -= 2;
			PC = memory.ReadWord(SP, cycles);
			PC++;
			cycles--;
		} break;
		default:
			//printf("Instruction not handled%d", ins);
			break;
		}
	}
}

void CPU::Reset(Memory& memory)
{
	PC = 0xFFFC; // program start address
	SP = 0x0100; // stack address
	C = Z = I = D = B = V = N = 0;
	A = X = Y = 0;
	memory.Init();
}

Byte CPU::FetchByte(u32& cycles, Memory& memory)
{
	Byte data = memory[PC];
	PC++;
	cycles--;
	return data;
}

Word CPU::FetchWord(u32& cycles, Memory& memory)
{
	// 6502 is little endian
	Word data = memory[PC];
	PC++;
	data |= (memory[PC] << 8);
	PC++;
	cycles -= 2;
	return data;
}

Byte CPU::ReadByte(u32& cycles, Byte& addr, Memory& memory)
{
	Byte data = memory[addr];
	cycles--;
	return data;
}

inline void CPU::LDASetStatus()
{
	Z = (A == 0);
	N = (A & 0b10000000) > 0;
}