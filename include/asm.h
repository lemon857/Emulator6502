#ifndef _ASM_H_
#define _ASM_H_

#include <string>
#include "memory.h"

struct Assembler
{
	void Compile(std::string sourceCode);

	void WriteProgram(Memory& memory);
};

#endif // !_ASM_H_
