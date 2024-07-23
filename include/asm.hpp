#ifndef _ASM_HPP_
#define _ASM_HPP_

#include <string>

#include "memory.hpp"

struct Assembler
{
	Assembler() = delete;

	static void Compile(std::string sourceCodePath, Memory& memory);
	static void HandleIns(std::string ins, std::string arg1, std::string arg2, Memory& memory, Word& pos);
	static Word StrToWord(std::string str);
	static Byte StrToSignedByte(std::string str);
	static void ErrorHandler(std::string text);

	static void SaveMemory(std::string path, Memory& memory);
	static void LoadMemory(std::string path, Memory& memory);
};

#endif // _ASM_HPP_
