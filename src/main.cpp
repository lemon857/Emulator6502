#include "asm.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "gui.hpp"

#include <iostream>
#include <string>

std::string get_path(std::string exePath)
{
#if defined(_WIN64)
	int pos = exePath.rfind('\\');
	return exePath.substr(0, pos);
#elif defined(__linux__)
	int pos = exePath.rfind('/');
	return exePath.substr(0, pos);
#endif
}

int main(int argc, char** argv)
{
	Memory mem;
	CPU my_cpu;
	my_cpu.Reset(mem);

	std::string path = get_path(argv[0]);


#if defined(_WIN64)
	Assembler::Compile(path + "\\test.asm6502", mem);
	my_cpu.Execute(128, mem);
	Assembler::SaveMemory(path + "\\test.mem", mem);
#elif defined(__linux__)
	Assembler::Compile(path + "/test.asm6502", mem);
	GUI::LiveExecute(my_cpu, mem, 128);
	//my_cpu.Execute(128, mem);
	Assembler::SaveMemory(path + "/test.mem", mem);
#endif
	//GUI::DrawState(my_cpu);
	//Assembler::LoadMemory("path", mem); 
	//// start program
	//mem[0xFFFC] = CPU::INS_JMP;
	//mem[0xFFFD] = 0xE0;
	//mem[0xFFFE] = 0xFF;
	//// main
	//mem[0xFFE0] = CPU::INS_LDA_IM;
	//mem[0xFFE1] = 0x0A;
	//mem[0xFFE2] = CPU::INS_AND_IM;
	//mem[0xFFE3] = 0x01;
	////mem[0xFFE4] = 0xC0;
	////mem[0xFFE5] = 0xFF;
	//// end program
	return 0;
}
