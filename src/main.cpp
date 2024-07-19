#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];
	void Init()
	{
		for (u32 i = 0; i < MAX_MEM; i++)
		{
			Data[i] = 0;
		}
	}

	/* read one byte */
	Byte operator[](u32 address) const
	{
		// assert here if address > MAX_MEMORY
		return Data[address];
	}

	/* write one byte */
	Byte& operator[](u32 address)
	{
		// assert here if address > MAX_MEMORY
		return Data[address];
	}

	/* write two bytes*/
	void WriteWord(Word value, u32 address, u32& cycles)
	{
		Data[address] = value & 0xFF;
		Data[address + 1] = (value >> 8);
		cycles -= 2;
	}

	/* read two bytes*/
	Word ReadWord(u32 address, u32& cycles)
	{
		Word value = Data[address];
		value |= (Data[address + 1] << 8);
		cycles -= 2;
		return value;
	}
};


struct CPU
{
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

	void Reset(Mem& memory)
	{
		PC = 0xFFFC; // program start address
		SP = 0x0100; // stack address
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;	
		memory.Init();		
	}

	Byte FetchByte(u32& cycles, Mem& memory)
	{
		Byte data = memory[PC];
		PC++;
		cycles--;
		return data;
	}
	
	Word FetchWord(u32& cycles, Mem& memory)
	{
		// 6502 is little endian
		Word data = memory[PC];
		PC++;
		data |= (memory[PC] << 8);
		PC++;
		cycles-=2;
		return data;
	}

	Byte ReadByte(u32& cycles, Byte& addr, Mem& memory)
	{
		Byte data = memory[addr];
		cycles--;
		return data;
	}
	// opcodes
	static constexpr Byte 
		INS_LDA_IM = 0xA9,		// set value A
		INS_LDA_ZP = 0xA5,		// set value A from ZeroPage (8-bit) address
		INS_LDA_ZPX = 0xB5,		// set value A from ZeroPage (8-bit) address + value X 
		INS_JMP = 0x4C,			// jump to address
		INS_JSR = 0x20,			// jump to subroutine 
		INS_RTS = 0x60;			// return from subroutine

	void LDASetStatus()
	{
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	void Execute(u32 cycles, Mem& memory)
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
				SP+=2;
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
				printf("Instruction not handled%d", ins);
				break;
			}
		}
	}


};



int main(void)
{
	Mem mem;
	CPU my_cpu;
	my_cpu.Reset(mem);
	// start program
	mem[0xFFFC] = CPU::INS_JMP;
	mem[0xFFFD] = 0xE0;
	mem[0xFFFE] = 0xFF;
	// main
	mem[0xFFE0] = CPU::INS_JSR;
	mem[0xFFE1] = 0xD0;
	mem[0xFFE2] = 0xFF;
	mem[0xFFE3] = CPU::INS_LDA_IM;
	mem[0xFFE4] = 0xF1;
	// routine
	mem[0xFFD0] = CPU::INS_LDA_IM;
	mem[0xFFD1] = 0xA2;
	mem[0xFFD2] = CPU::INS_RTS;
	// end program
	my_cpu.Execute(25, mem);
	return 0;
}
