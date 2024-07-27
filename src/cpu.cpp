#include "cpu.hpp"

void CPU::Execute(u32 cycles, Memory& memory)
{
	while (cycles > 0)
	{
		Step(cycles, memory);
	}
}

void CPU::Step(u32& cycles, Memory& memory)
{
	Byte ins = FetchByte(cycles, memory);
		switch (ins)
		{
		case INS_TAX: {
			X = A;
			cycles--;
			LoadRegisterSetStatus(X);
		}	break;
		case INS_TXA: {
			A = X;
			cycles--;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_TAY: {
			Y = A;
			cycles--;
			LoadRegisterSetStatus(Y);
		}	break;
		case INS_TYA: {
			A = Y;
			cycles--;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_DEX: {
			X--;
			cycles--;
			LoadRegisterSetStatus(X);
		}	break;
		case INS_INX: {
			X++;
			cycles--;
			LoadRegisterSetStatus(X);
		}	break;
		case INS_DEY: {
			Y--;
			cycles--;
			LoadRegisterSetStatus(Y);
		}	break;
		case INS_INY: {
			Y++;
			cycles--;
			LoadRegisterSetStatus(Y);
		}	break;


		case INS_CPX_IM: {
			Byte value = FetchByte(cycles, memory);
			if (X == value) C = 1;
			LoadRegisterSetStatus(X);
		}	break;
		case INS_CPX_ZP: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte value = ReadByteFromZeroPage(cycles, ZPaddr, memory);
			if (X == value) C = 1;
			LoadRegisterSetStatus(X);
		}	break;
		case INS_CPX_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte value = ReadByte(cycles, addr, memory);
			if (X == value) C = 1;
			LoadRegisterSetStatus(X);
		}	break;

		case INS_CPY_IM: {
			Byte value = FetchByte(cycles, memory);
			if (Y == value) C = 1;
			LoadRegisterSetStatus(Y);
		}	break;
		case INS_CPY_ZP: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte value = ReadByteFromZeroPage(cycles, ZPaddr, memory);
			if (Y == value) C = 1;
			LoadRegisterSetStatus(Y);
		}	break;
		case INS_CPY_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte value = ReadByte(cycles, addr, memory);
			if (Y == value) C = 1;
			LoadRegisterSetStatus(Y);
		}	break;

		case INS_CMP_IM: {
			Byte value = FetchByte(cycles, memory);
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break; 
		case INS_CMP_ZP: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte value = ReadByteFromZeroPage(cycles, ZPaddr, memory);
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_ZPX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte value = ReadByteFromZeroPage(cycles, ZPaddr, memory);
			value += X;
			cycles--;
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte value = ReadByte(cycles, addr, memory);
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_ABSX: {
			Word addr = FetchWord(cycles, memory);
			Byte value = ReadByte(cycles, addr, memory);
			value += X;
			cycles--;
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_ABSY: {
			Word addr = FetchWord(cycles, memory);
			Byte value = ReadByte(cycles, addr, memory);
			value += Y;
			cycles--;
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_INDX: {
			Byte addr = FetchByte(cycles, memory);
			addr += X;
			cycles--;
			Byte valAddr = ReadByteFromZeroPage(cycles, addr, memory);
			Byte value = ReadByte(cycles, valAddr, memory);
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;
		case INS_CMP_INDY: {
			Byte addr = FetchByte(cycles, memory);
			addr += Y;
			cycles--;
			Byte valAddr = ReadByteFromZeroPage(cycles, addr, memory);
			Byte value = ReadByte(cycles, valAddr, memory);
			if (A == value) C = 1;
			LoadRegisterSetStatus(A);
		}	break;

		case INS_ADC_IM: {
			Byte op = FetchByte(cycles, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_ZP: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte op = ReadByte(cycles, ZPaddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_ZPX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Byte op = ReadByte(cycles, ZPaddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break; 
		case INS_ADC_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_INDX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			Byte opAddr = ReadByte(cycles, addr, memory);
			Byte op = ReadByte(cycles, opAddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_ADC_INDY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			Byte opAddr = ReadByte(cycles, addr, memory);
			Byte op = ReadByte(cycles, opAddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum += op;
			sum += C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_IM: {
			Byte op = FetchByte(cycles, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_ZP: {
			Byte ZPaddr = FetchByte(cycles, memory);
			Byte op = ReadByte(cycles, ZPaddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_ZPX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Byte op = ReadByte(cycles, ZPaddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			Byte op = ReadByte(cycles, addr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_INDX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			Byte opAddr = ReadByte(cycles, addr, memory);
			Byte op = ReadByte(cycles, opAddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;
		case INS_SBC_INDY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			Byte opAddr = ReadByte(cycles, addr, memory);
			Byte op = ReadByte(cycles, opAddr, memory);
			const Byte oldA = A;
			Word sum = A;
			sum -= op;
			sum -= C;
			A = (sum & 0xFF);
			C = (sum & 0xFF00) > 0;
			N = (A & 0b10000000) > 0;
			Z = (A == 0);
			V = false;
			if (((oldA & 0b10000000) ^ (op & 0b10000000)) != 0)
			{
				V = ((A & 0b10000000) != (oldA & 0b10000000));
			}
		}	break;

		case INS_DEC_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			memory[zeroPageAddr]--;
			cycles -= 3;
			LoadRegisterSetStatus(memory[zeroPageAddr]);
		}	break; 
		case INS_DEC_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			memory[zeroPageAddr]--;
			cycles -= 3;
			LoadRegisterSetStatus(memory[zeroPageAddr]);
		}	break;
		case INS_DEC_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte value = memory[addr];
			value--;
			memory[addr] = value;
			cycles -= 3;
			LoadRegisterSetStatus(value);
		}	break;
		case INS_DEC_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			Byte value = memory[addr];
			value--;
			memory[addr] = value;
			cycles -= 4;
			LoadRegisterSetStatus(value);
		}	break;

		case INS_INC_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			memory[zeroPageAddr]++;
			cycles -= 3;
			LoadRegisterSetStatus(memory[zeroPageAddr]);
		}	break;
		case INS_INC_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			memory[zeroPageAddr]++;
			cycles -= 3;
			LoadRegisterSetStatus(memory[zeroPageAddr]);
		}	break;
		case INS_INC_ABS: {
			Word addr = FetchWord(cycles, memory);
			Byte value = memory[addr];
			value++;
			memory[addr] = value;
			cycles -= 3;
			LoadRegisterSetStatus(value);
		}	break;
		case INS_INC_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			Byte value = memory[addr];
			value++;
			memory[addr] = value;
			cycles -= 4;
			LoadRegisterSetStatus(value);
		}	break;


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
		case INS_LDA_INDX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A = ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDA_INDY: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += Y;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
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

		case INS_STA_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			memory[zeroPageAddr] = A;
		} break;
		case INS_STA_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			memory[zeroPageAddr] = A;
		} break;
		case INS_STA_ABS: {
			Word addr = FetchWord(cycles, memory);
			memory[addr] = A;
		} break;
		case INS_STA_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			memory[addr] = A;
		} break;
		case INS_STA_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += Y;
			cycles--;
			memory[addr] = A;
		} break;
		case INS_STA_INDX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			memory[addr] = A;
			cycles--;
		} break;
		case INS_STA_INDY: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += Y;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			memory[addr] = A;
			cycles--;
		} break;

		case INS_STX_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			memory[zeroPageAddr] = X;
		} break;
		case INS_STX_ZPY: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += Y;
			cycles--;
			memory[zeroPageAddr] = X;
		} break;
		case INS_STX_ABS: {
			Word addr = FetchWord(cycles, memory);
			memory[addr] = X;
		} break;

		case INS_STY_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			memory[zeroPageAddr] = Y;
		} break;
		case INS_STY_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			memory[zeroPageAddr] = Y;
		} break;
		case INS_STY_ABS: {
			Word addr = FetchWord(cycles, memory);
			memory[addr] = Y;
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
		case INS_AND_INDX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A &= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_AND_INDY: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += Y;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A &= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;

		case INS_ORA_IM: {
			Byte value = FetchByte(cycles, memory);
			A |= value;
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			A |= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			A |= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_ABS: {
			Word addr = FetchWord(cycles, memory);
			A |= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A |= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A |= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_INDX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A |= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_ORA_INDY: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += Y;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A |= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		
		case INS_EOR_IM: {
			Byte value = FetchByte(cycles, memory);
			A ^= value;
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_ZP: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			A ^= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_ZPX: {
			Byte zeroPageAddr = FetchByte(cycles, memory);
			zeroPageAddr += X;
			cycles--;
			A ^= ReadByteFromZeroPage(cycles, zeroPageAddr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_ABS: {
			Word addr = FetchWord(cycles, memory);
			A ^= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_ABSX: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A ^= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_ABSY: {
			Word addr = FetchWord(cycles, memory);
			addr += X;
			cycles--;
			A ^= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_INDX: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += X;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A ^= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_EOR_INDY: {
			Byte ZPaddr = FetchByte(cycles, memory);
			ZPaddr += Y;
			cycles--;
			Word addr = ReadWord(cycles, ZPaddr, memory);
			A ^= ReadByte(cycles, addr, memory);
			LoadRegisterSetStatus(A);
		} break;

		case INS_BEQ: {
			Byte offset = FetchByte(cycles, memory);
			if (Z == 1)
			{
				PC += offset;
				cycles--;
			}
		}	break;
		case INS_BNE: {
			Byte offset = FetchByte(cycles, memory);
			if (Z == 0)
			{
				signed char of = offset;
				PC += of;
				cycles--;
			}
		}	break;
		case INS_BCC: {
			Byte offset = FetchByte(cycles, memory);
			if (C == 0)
			{
				signed char of = offset;
				PC += of;
				cycles--;
			}
		}	break;
		case INS_BCS: {
			Byte offset = FetchByte(cycles, memory);
			if (C == 1)
			{
				signed char of = offset;
				PC += of;
				cycles--;
			}
		}	break;

		case INS_TSX: {
			X = SP;
			LoadRegisterSetStatus(X);
			cycles--;
		} break;
		case INS_TXS: {
			SP = X;
			cycles--;
		} break;
		case INS_PHA: {
			PushByteOntoStack(A, cycles, memory);
			cycles--;
		} break;
		case INS_PLA: {
			PullByteFromStack(A, cycles, memory);
			cycles--;
		} break;
		case INS_JMP_ABS: {
			Word subAddr = FetchWord(cycles, memory);
			PC = subAddr;
		} break;
		case INS_JMP_IND: {
			Word ZPAddr = FetchWord(cycles, memory);
			Word addr = ReadWord(cycles, ZPAddr, memory);
			PC = addr;
		} break;
		case INS_JSR: {
			Word subAddr = FetchWord(cycles, memory);
			memory.WriteWord(PC - 1, SPToAddress(), cycles);
			SP -= 2;
			cycles--;
			PC = subAddr;
		} break;
		case INS_RTS: {
			SP += 2;
			cycles -= 2;
			PC = ReadWord(cycles, SPToAddress(), memory);
			PC++;
			cycles--;
		} break;
		default:
			//printf("Instruction not handled%d", ins);
			break;
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

Byte CPU::ReadByte(u32& cycles, Word addr, Memory& memory)
{
	Byte data = memory[addr];
	cycles--;
	return data;
}

Word CPU::ReadWord(u32& cycles, Word addr, Memory& memory)
{
	return memory.ReadWord(addr, cycles);
}

void CPU::PushByteOntoStack(Byte value, u32& cycles, Memory& memory)
{
	memory[SPToAddress()] = value;
	SP--;
	cycles--;
}

void CPU::PullByteFromStack(Byte& value, u32& cycles, Memory& memory)
{
	SP++;
	value = memory[SPToAddress()];
	cycles--;
}

inline void CPU::LoadRegisterSetStatus(Byte value)
{
	Z = (value == 0);
	N = (value & 0b10000000) > 0;
}