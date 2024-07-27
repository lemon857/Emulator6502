#include "asm.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "gui.hpp"

#include <stdio.h>
#include <string>
#include <vector>

#define PRG_VERSION_STR "6502 Processor emulator V-0.0.3"


#if defined(_WIN64)
char pathSym = '\\';
#elif defined(__linux__)
char pathSym = '/';
#endif

std::string get_path(std::string exePath)
{
	int pos = exePath.rfind(pathSym);
	return pos == -1 ? "" : exePath.substr(0, pos) + pathSym;
}

void HelpMessage()
{
	printf(PRG_VERSION_STR "\n");
	printf("-h or --help used for get this message\n");
	printf("-g or --gui used for live execution program\n");
	printf("-p or --prg used for set relative path program\n");
	printf("-m or --mem used for set relative path memory for save or load\n");
	printf("-e or --exc used for exexute program for memory file\n");
	exit(0);
}

void ErrorHandler(std::string message)
{
	printf("%s\n", message.c_str());
	exit(-1);
}

int main(int argc, char** argv)
{
	Memory mem;
	CPU my_cpu;
	my_cpu.Reset(mem);

	std::string path = get_path(argv[0]);
	std::string pathToPrg = "";
	std::string pathToMem = "";
	bool guiExecute = false;
	bool fromMemoryExecute = false;
	
	// flags checking
	if (argc > 1)
	{
		for (int i = 0; i < argc - 1; i++)
		{
			std::string arg = argv[i+1];
			if (arg == "--gui" || arg == "-g") guiExecute = true;
			else if (arg == "--prg" || arg == "-p")
			{
				i++;
				if (i >= argc - 1) ErrorHandler("error not found path program file");
				pathToPrg = argv[i + 1];
			}
			else if (arg == "--mem" || arg == "-m")
			{
				i++;
				if (i >= argc - 1) ErrorHandler("error not found path memory file");
				pathToMem = argv[i + 1];
			}
			else if (arg == "--exc" || arg == "-e") fromMemoryExecute = true;
			else if (arg == "--help" || arg == "-h") HelpMessage();
			else if (arg == "--version" || arg == "-v")
			{
				printf(PRG_VERSION_STR "\n");
				exit(0);
			}
			else ErrorHandler("Invalid argument: " + arg);
		}
	}

	printf("Path: %s\nPath prg: %s\nPath mem: %s\nPress enter for continue\n", path.c_str(), pathToPrg.c_str(), pathToMem.c_str());
	getchar();
	if (fromMemoryExecute) Assembler::LoadMemory(path + pathToMem, mem);
	else Assembler::Compile(path + pathToPrg, mem);

	if (guiExecute) GUI::LiveExecute(my_cpu, mem, 128);
	else my_cpu.Execute(0x0100, mem);

	Assembler::SaveMemory(path + pathToMem, mem);

	return 0;
}
