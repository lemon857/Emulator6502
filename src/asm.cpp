#include <fstream>
#include <iostream>
#include "asm.h"
#include "cpu.h"

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
		int posSpace = line.find(' ');
		std::string ins = line.substr(0, posSpace);
		int posPoint = line.find(',');
		std::string arg1;
		if (posPoint == -1)
		{
			arg1 = line.substr(posSpace + 1);
			HandleIns(ins, arg1, "", memory, prgpos);
		}
	}
}

void Assembler::HandleIns(std::string ins, std::string arg1, std::string arg2, Memory& memory, Word& pos)
{
	Word A, B;
	int p;
	if ((p = arg1.find('$')) != -1)
	{
		A = StrToWord(arg1.substr(p + 1));
	}
	if ((p = arg2.find('$')) != -1)
	{
		B = StrToWord(arg1.substr(p + 1));
	}
	if (arg2 == "")
	{
		if (ins == "lda")
		{
			if (arg1[0] == '#')
			{
				memory[pos] = CPU::INS_LDA_IM;
				pos++;
				memory[pos] = A;
				pos++;
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
