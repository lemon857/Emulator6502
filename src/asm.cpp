#include <fstream>
#include <iostream>
#include <queue>
#include <map>

#include "asm.h"
#include "cpu.h"

static std::map<std::string, Word> pointNames;	// if line start '.' it's point name
static std::queue<RequirePointName> pountReq;	// if args start '.' it's point name

void Assembler::Compile(std::string sourceCodePath, Memory& memory)
{
	std::ifstream f;
	f.open(sourceCodePath);
	if (!f.is_open())
	{
		std::cout << "Failed to open file\n";
		return;
	}

	std::string line;
	Word prgpos = START_PROGRAM;
	while (std::getline(f, line))
	{
		int p = 0;
		// delete comment
		if ((p = line.find(';')) != -1) line = line.substr(0, p-1);
		// clear end spaces
		while (!line.empty() && line[line.length() - 1] == ' ') line = line.substr(0, line.length() - 2);
		if (line.empty()) continue;
		// save point ptr
		if (line[0] == '.') 
		{
			pointNames.emplace(line, prgpos++);
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
			arg1 = line.substr(posSpace + 1);
			arg2 = line.substr(posPoint + 1);
			while (arg2[0] == ' ') { arg2 = arg2.substr(1); }
			HandleIns(ins, arg1, arg2, memory, prgpos);
		}
	}
	
	while (!pountReq.empty())
	{
		RequirePointName ptr = pountReq.front();

		auto it = pointNames.find(ptr.name);
		if (it != pointNames.end())
		{
			Word A = it->second;
			memory[ptr.posReq] = (A & 0xFF00) >> 8;
			memory[ptr.posReq+1] = (A & 0x00FF);
		}
		else
			ErrorHandler("Undefined name: " + ptr.name);
		pountReq.pop();
	}
}
/*else if (arg[0] == '.')
			{
				auto it = pointNames.find(arg);
				if (it != pointNames.end())
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
					pountReq.push(rpn);
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
					ErrorHandler("error too much simple value at: " + std::to_string(pos));
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
					ErrorHandler("error too much address at: " + std::to_string(pos));
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
		else if (ins == "jmp")
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
					memory[pos++] = CPU::INS_JMP;
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
				auto it = pointNames.find(arg);
				if (it != pointNames.end())
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
					pountReq.push(rpn);
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
				auto it = pointNames.find(arg);
				if (it != pointNames.end())
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
					pountReq.push(rpn);
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
	}
	else
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

void Assembler::ErrorHandler(std::string text)
{
	std::cout << text << '\n';
}

void Assembler::SaveMemory(std::string path, Memory& memory)
{
	std::ofstream f;
	f.open(path, std::ios::binary);
	f.write((char*)memory.Data, memory.MAX_MEM * sizeof(Byte));
}

void Assembler::LoadMemory(std::string path, Memory& memory)
{
	std::ifstream f;
	f.open(path, std::ios::binary);
	f.read((char*)memory.Data, memory.MAX_MEM * sizeof(Byte));
}
