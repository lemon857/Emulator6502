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
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			A = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			A = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_ABS: {
			Word addr = FetchWord(cycles, memory);
			A = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			A = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;

		case INS_LDX_IM: {
			Byte value = FetchByte(cycles, memory);
			X = value;
			LoadRegisterSetStatus(X);
		} break;
		case INS_LDX_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			X = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(X);
		} break;
		case INS_LDX_ZPY: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += Y;
			cycles--;
			X = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(X);
		} break;
		case INS_LDX_ABS: {
			Word addr = FetchWord(cycles, memory);
			X = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(X);
		} break;
		case INS_LDX_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			X = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(X);
		} break;

		case INS_LDY_IM: {
			Byte value = FetchByte(cycles, memory);
			Y = value;
			LoadRegisterSetStatus(Y);
		} break;
		case INS_LDY_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			Y = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(Y);
		} break;
		case INS_LDY_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			Y = ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(Y);
		} break;
		case INS_LDY_ABS: {
			Word addr = FetchWord(cycles, memory);
			Y = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(Y);
		} break;
		case INS_LDY_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			Y = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(Y);
		} break;

		case INS_AND_IM: {
			Byte value = FetchByte(cycles, memory);
			A &= value;
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			A &= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			A &= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_ABS: {
			Word addr = FetchWord(cycles, memory);
			A &= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A &= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A &= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
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
	PC = START_PROGRAM; // program start address
	SP = STACK_POINTER; // stack address
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
	Word data = (memory[PC] << 8);
	PC++;
	data |= memory[PC];
	PC++;
	cycles -= 2;
	return data;
}

Byte CPU::ReadByteFromZeroPage(u32& cycles, Byte& addr, Memory& memory)
{
	Byte data = memory[addr];
	cycles--;
	return data;
}

Byte CPU::ReadByte(u32& cycles, Word& addr, Memory& memory)
{
	Byte data = memory[addr];
	cycles--;
	return data;
}

inline void CPU::LoadRegisterSetStatus(Byte value)
{
	Z = (value == 0);
	N = (value & 0b10000000) > 0;
}