#ifndef _ASM_H_
#define _ASM_H_

#include <string>
#include "memory.h"

struct Assembler
{
	static void Compile(std::string sourceCodePath, Memory& memory);
	static void HandleIns(std::string ins, std::string arg1, std::string arg2, Memory& memory, Word& pos);
	static Word StrToWord(std::string str);
};

#endif // !_ASM_H_
