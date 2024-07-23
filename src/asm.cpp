#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <math.h>

#include "asm.hpp"
#include "cpu.hpp"

struct RequirePointName
{
	std::string name;
	Word posReq;
	bool is_branch = false;
};

static std::map<std::string, Word> pointPseudonyms;	// if line start '.' it's pseudonym
static std::queue<RequirePointName> pseudonymsReq;		// if args start '.' it's pseudonym

void Assembler::Compile(std::string sourceCodePath, Memory& memory)
{
	std::ifstream f;
	f.open(sourceCodePath);
	if (!f.is_open())
	{
		ErrorHandler("error open file for read: " + sourceCodePath);
		return;
	}

	std::string line;
	Word prgpos = START_PROGRAM;
	while (std::getline(f, line))
	{
		int p = 0;
		// delete comment
		if ((p = line.find(';')) != -1) line = line.substr(0, (p-1) < 0 ? 0 : (p-1));
		// clear end spaces
		while (!line.empty() && (line[line.length() - 1] == ' ' || line[line.length() - 1] == '\t')) line = line.substr(0, line.length() - 1);
		if (line.empty()) continue;
		// save pos next command after pointer 
		if (line[0] == '.') 
		{
			pointPseudonyms.emplace(line, ++prgpos);
			continue;
		}	
		// for comfort write code set pos with ':' ex. :$4444 next code start from this address
		// if have args hexadecemal, stand for set value BYTE in memory ex. :$4444 $44
		else if (line[0] == ':')
		{
			int posSp = line.find(' ');
			if (line.find('$') == -1)
			{
				ErrorHandler("invalid value memory pointer");
				continue;
			}
			if (posSp != -1)
			{
				Word addr = StrToWord(line.substr(line.find('$') + 1, posSp - line.find('$') - 1));
				Byte val = StrToWord(line.substr(posSp+2));
				memory[addr] = val;
			}
			else
			{
				prgpos = StrToWord(line.substr(line.find('$'), posSp));
			}
			continue;
		}
		int posSpace = line.find(' ');
		std::string ins = line.substr(0, posSpace);
		int posPoint = line.find(',');
		std::string arg1, arg2;
		if (posPoint == -1)
		{
			arg1 = line.substr(posSpace + 1);
			HandleIns(ins, arg1, "", memory, prgpos);
		}
		else
		{
			arg1 = line.substr(posSpace + 1, posPoint - posSpace - 1);
			arg2 = line.substr(posPoint + 1);
			while (arg2[0] == ' ') { arg2 = arg2.substr(1); }
			if (arg2.length() != 1)
			{
				ErrorHandler("invalid syntaxis arg2 have too long lenght: " + arg2);
			}
			HandleIns(ins, arg1, arg2, memory, prgpos);
		}
	}
	
	while (!pseudonymsReq.empty())
	{
		RequirePointName ptr = pseudonymsReq.front();

		auto it = pointPseudonyms.find(ptr.name);
		if (it != pointPseudonyms.end())
		{
			if (ptr.is_branch)
			{
				Word A = it->second;
				memory[ptr.posReq] = A - ptr.posReq - 1;
			}			
			else
			{
				Word A = it->second;
				memory[ptr.posReq] = (A & 0xFF00) >> 8;
				memory[ptr.posReq + 1] = (A & 0x00FF);
			}
		}
		else
			ErrorHandler("Undefined name: " + ptr.name);
		pseudonymsReq.pop();
	}
	std::cout << "Successful compile!\n";
}
/*else if (arg[0] == '.')
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_LDA_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_LDA_ABS;
					rpn.posReq = pos;
					pos += 2;
					pseudonymsReq.push(rpn);
				}
			}*/

void Assembler::HandleIns(std::string ins, std::string arg1, std::string arg2, Memory& memory, Word& pos)
{
	if (arg2 == "")
	{
		if (ins == "lda")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_LDA_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value arg: " + arg);
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_LDA_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_LDA_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address arg: " + arg);
				}
			}
		}
		else if (ins == "ldx")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_LDX_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_LDX_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_LDX_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "ldy")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_LDY_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_LDY_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_LDY_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "beq")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			if (arg.find('.') != -1)
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_BEQ;
					memory[pos++] = A - pos;
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_BEQ;
					rpn.posReq = pos;
					rpn.is_branch = true;
					pos++;
					pseudonymsReq.push(rpn);
				}
			}
			else if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				memory[pos++] = CPU::INS_BEQ;
				memory[pos++] = A - pos;
			}
			else ErrorHandler("invalid argument: " + arg);
		}
		else if (ins == "bne")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			if (arg.find('.') != -1)
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_BNE;
					memory[pos++] = A - pos;
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_BNE;
					rpn.posReq = pos;
					rpn.is_branch = true;
					pos++;
					pseudonymsReq.push(rpn);
				}
			}
			else if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				memory[pos++] = CPU::INS_BNE;
				memory[pos++] = A - pos;
			}
			else ErrorHandler("invalid argument: " + arg);
		}
		else if (ins == "bcs")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			if (arg.find('.') != -1)
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_BCS;
					memory[pos++] = A - pos;
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_BCS;
					rpn.posReq = pos;
					rpn.is_branch = true;
					pos++;
					pseudonymsReq.push(rpn);
				}
			}
			else if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				memory[pos++] = CPU::INS_BCS;
				memory[pos++] = A - pos;
			}
			else ErrorHandler("invalid argument: " + arg);
		}
		else if (ins == "bcc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			if (arg.find('.') != -1)
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_BCC;
					memory[pos++] = A - pos;
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_BCC;
					rpn.posReq = pos;
					rpn.is_branch = true;
					pos++;
					pseudonymsReq.push(rpn);
				}
			}
			else if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				memory[pos++] = CPU::INS_BCC;
				memory[pos++] = A - pos;
			}
			else ErrorHandler("invalid argument: " + arg);
		}
		else if (ins == "clc")
		{
			memory[pos++] = CPU::INS_CLC;
		}
		else if (ins == "sec")
		{
			memory[pos++] = CPU::INS_SEC;
		}
		else if (ins == "clv")
		{
			memory[pos++] = CPU::INS_CLV;
		}
		else if (ins == "jmp")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find(')')) != -1)
			{
				A = StrToWord(arg.substr(arg.find('$') + 1, p - arg.find('$') - 1));
				memory[pos++] = CPU::INS_JMP_IND;
				memory[pos++] = (A & 0xFF00) >> 8;
				memory[pos++] = (A & 0x00FF);
			}
			else if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				memory[pos++] = CPU::INS_JMP;
				memory[pos++] = (A & 0xFF00) >> 8;
				memory[pos++] = (A & 0x00FF);
			}
			else if (arg[0] == '.')
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_JMP;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_JMP;
					rpn.posReq = pos;
					pos += 2;
					pseudonymsReq.push(rpn);
				}
			}
		}
		else if (ins == "jsr")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_JSR;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			else if (arg[0] == '.')
			{
				auto it = pointPseudonyms.find(arg);
				if (it != pointPseudonyms.end())
				{
					A = it->second;
					memory[pos++] = CPU::INS_JSR;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					RequirePointName rpn;
					rpn.name = arg;
					memory[pos++] = CPU::INS_JSR;
					rpn.posReq = pos;
					pos += 2;
					pseudonymsReq.push(rpn);
				}
			}
		}
		else if (ins == "rts")
		{
			memory[pos] = CPU::INS_RTS;
			pos++;
		}
		else if (ins == "and")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_AND_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_AND_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_AND_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "ora")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_ORA_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_ORA_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_ORA_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
		else if (ins == "eor")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4) // hexadecemal value
				{
					memory[pos++] = CPU::INS_EOR_IM;
					memory[pos++] = A;
				}
				else if (arg[0] == '#' && len > 4)
				{
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
				}
				else if (len == 3)				// ZeroPointer
				{
					memory[pos++] = CPU::INS_EOR_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_EOR_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
		else if (ins == "sta")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					memory[pos++] = CPU::INS_STA_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_STA_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "stx")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					memory[pos++] = CPU::INS_STX_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_STX_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "sty")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					memory[pos++] = CPU::INS_STY_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_STY_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "tax")
		{
			memory[pos] = CPU::INS_TAX;
			pos++;
		}
		else if (ins == "txa")
		{
			memory[pos] = CPU::INS_TXA;
			pos++;
		}
		else if (ins == "tay")
		{
			memory[pos] = CPU::INS_TAY;
			pos++;
		}
		else if (ins == "tya")
		{
			memory[pos] = CPU::INS_TYA;
			pos++;
		}
		else if (ins == "dex")
		{
			memory[pos] = CPU::INS_DEX;
			pos++;
		}
		else if (ins == "inx")
		{
			memory[pos] = CPU::INS_INX;
			pos++;
		}
		else if (ins == "dey")
		{
			memory[pos] = CPU::INS_DEY;
			pos++;
		}
		else if (ins == "iny")
		{
			memory[pos] = CPU::INS_INY;
			pos++;
		}
		else if (ins == "txs")
		{
			memory[pos++] = CPU::INS_TXS;
		}
		else if (ins == "tsx")
		{
			memory[pos++] = CPU::INS_TSX;
		}
		else if (ins == "pha")
		{
			memory[pos++] = CPU::INS_PHA;
		}
		else if (ins == "pla")
		{
			memory[pos++] = CPU::INS_PLA;
		}
		else if (ins == "dec")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					memory[pos++] = CPU::INS_DEC_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_DEC_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "inc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					memory[pos++] = CPU::INS_INC_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					memory[pos++] = CPU::INS_INC_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "adc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (arg[0] == '#' && len > 2 && len <= 4)	// hexadecemal value
				{
					memory[pos++] = CPU::INS_ADC_IM;
					memory[pos++] = A;
				}
				else if (len == 3)							// ZeroPointer
				{
					memory[pos++] = CPU::INS_ADC_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)				// absolute address
				{
					memory[pos++] = CPU::INS_ADC_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
		else if (ins == "sbc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1)); 
				if (arg[0] == '#' && len > 2 && len <= 4)	// hexadecemal value
				{
					memory[pos++] = CPU::INS_SBC_IM;
					memory[pos++] = A;
				}
				else if (len == 3)							// ZeroPointer
				{
					memory[pos++] = CPU::INS_SBC_ZP;
					memory[pos++] = A;
				}
				else if (len > 3 && len <= 5)				// absolute address
				{
					memory[pos++] = CPU::INS_SBC_ABS;
					memory[pos++] = (A & 0xFF00) >> 8;
					memory[pos++] = (A & 0x00FF);
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
		else
		{
			ErrorHandler("invalid instrucion: " + ins);
		}
	}
	else
	{
		if (ins == "lda")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_LDA_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_LDA_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
						else if (arg2 == "Y")
						{
							memory[pos++] = CPU::INS_LDA_ABSY;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_LDA_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_LDA_INDY;
					memory[pos++] = A;
				}
			}
		}
		else if (ins == "ldx")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "Y")
					{
						memory[pos++] = CPU::INS_LDX_ZPY;
						memory[pos++] = A;
					}
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					if (arg2 == "Y")
					{
						memory[pos++] = CPU::INS_LDX_ABSY;
						memory[pos++] = (A & 0xFF00) >> 8;
						memory[pos++] = (A & 0x00FF);
					}
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "ldy")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_LDY_ZPX;
						memory[pos++] = A;
					}
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_LDY_ABSX;
						memory[pos++] = (A & 0xFF00) >> 8;
						memory[pos++] = (A & 0x00FF);
					}
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "and")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_AND_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_AND_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
						else if (arg2 == "Y")
						{
							memory[pos++] = CPU::INS_AND_ABSY;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_AND_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_AND_INDY;
					memory[pos++] = A;
				}
			}
		}
		else if (ins == "ora")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_ORA_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_ORA_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
						else if (arg2 == "Y")
						{
							memory[pos++] = CPU::INS_ORA_ABSY;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_ORA_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_ORA_INDY;
					memory[pos++] = A;
				}
			}
			}
		else if (ins == "eor")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_EOR_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_EOR_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
						else if (arg2 == "Y")
						{
							memory[pos++] = CPU::INS_EOR_ABSY;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_EOR_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_EOR_INDY;
					memory[pos++] = A;
				}
			}
			}
		else if (ins == "sta")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_STA_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_STA_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
						else if (arg2 == "Y")
						{
							memory[pos++] = CPU::INS_STA_ABSY;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_STA_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_STA_INDY;
					memory[pos++] = A;
				}
			}
		}
		else if (ins == "stx")
		{
		Word A;
		int p, len;
		std::string arg = arg1;
		len = arg.length();
		if ((p = arg.find('$')) != -1)
		{
			A = StrToWord(arg.substr(p + 1));
			if (len == 3)					// ZeroPointer
			{
				if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_STX_ZPY;
					memory[pos++] = A;
				}
			}
			else
			{
				ErrorHandler("error invalid second argument at: " + std::to_string(pos));
			}
		}
		}
		else if (ins == "sty")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_STY_ZPX;
						memory[pos++] = A;
					}
				}
				else
				{
					ErrorHandler("error invalid second argument at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "dec")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_DEC_ZPX;
						memory[pos++] = A;
					}
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_DEC_ABSX;
						memory[pos++] = (A & 0xFF00) >> 8;
						memory[pos++] = (A & 0x00FF);
					}
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
		}
		else if (ins == "inc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_INC_ZPX;
						memory[pos++] = A;
					}
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_INC_ABSX;
						memory[pos++] = (A & 0xFF00) >> 8;
						memory[pos++] = (A & 0x00FF);
					}
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
		else if (ins == "adc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
			if ((p = arg.find('$')) != -1)
			{
				A = StrToWord(arg.substr(p + 1));
				if (len == 3)					// ZeroPointer
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_ADC_ZPX;
						memory[pos++] = A;
					}
				}
				else if (len > 3 && len <= 5)	// absolute address
				{
					if (arg2 == "X")
					{
						memory[pos++] = CPU::INS_ADC_ABSX;
						memory[pos++] = (A & 0xFF00) >> 8;
						memory[pos++] = (A & 0x00FF);
					}
				}
				else
				{
					ErrorHandler("error too much address at: " + std::to_string(pos));
				}
			}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_ADC_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_ADC_INDY;
					memory[pos++] = A;
				}
			}
		}
		else if (ins == "sbc")
		{
			Word A;
			int p, len;
			std::string arg = arg1;
			len = arg.length();
			if ((p = arg.find('(')) == -1)
			{
				if ((p = arg.find('$')) != -1)
				{
					A = StrToWord(arg.substr(p + 1));
					if (len == 3)					// ZeroPointer
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_SBC_ZPX;
							memory[pos++] = A;
						}
					}
					else if (len > 3 && len <= 5)	// absolute address
					{
						if (arg2 == "X")
						{
							memory[pos++] = CPU::INS_SBC_ABSX;
							memory[pos++] = (A & 0xFF00) >> 8;
							memory[pos++] = (A & 0x00FF);
						}
					}
					else
					{
						ErrorHandler("error too much address at: " + std::to_string(pos));
					}
				}
			}
			else // indirect
			{
				A = StrToWord(arg.substr(p + 2, arg.find(')') - p - 2));
				if (arg2 == "X")
				{
					memory[pos++] = CPU::INS_SBC_INDX;
					memory[pos++] = A;
				}
				else if (arg2 == "Y")
				{
					memory[pos++] = CPU::INS_SBC_INDY;
					memory[pos++] = A;
				}
			}
			}
		else
		{
			ErrorHandler("invalid instrucion: " + ins);

		}			
	}
}

Word Assembler::StrToWord(std::string str)
{
	Word value = 0;
	int len = str.length() - 1;
	int i = 0;
	while (str != "")
	{
		unsigned char a = str[0];
		if (a == ' ') continue;
		if (std::isalpha(a))
		{
			value += (std::tolower(a) - 0x57) * (unsigned short)pow(16, len - i);
		}
		else
		{
			value += (a - '0') * (unsigned short)pow(16, len - i);
		}
		str = str.substr(1);
		i++;
	}
	return value;
}

Byte Assembler::StrToSignedByte(std::string str)
{
	Byte value;
	bool sign = true;

	if (str[0] == '-') sign = false;
	str = str.substr(1);

	value = StrToWord(str);

	if (!sign) value = -value;

	return value;
}

void Assembler::ErrorHandler(std::string text)
{
	std::cout << text << '\n';
}

void Assembler::SaveMemory(std::string path, Memory& memory)
{
	std::ofstream f;
	f.open(path, std::ios::binary | std::ios::out);
	if (!f.is_open())
	{
		ErrorHandler("error open file for write: " + path);
		return;
	}
	f.write((char*)memory.Data, memory.MAX_MEM * sizeof(Byte));
}

void Assembler::LoadMemory(std::string path, Memory& memory)
{
	std::ifstream f;
	f.open(path, std::ios::binary | std::ios::in);
	if (!f.is_open())
	{
		ErrorHandler("error open file for read: " + path);
		return;
	}
	f.read((char*)memory.Data, memory.MAX_MEM * sizeof(Byte));
}
