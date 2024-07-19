#include <fstream>

#include "asm.h"
#include "cpu.h"

int main(int argc, char** argv)
{
	//std::string program;

	Memory mem;
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
