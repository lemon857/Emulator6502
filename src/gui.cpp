#include "gui.hpp"

#include <iostream>

#if defined(_WIN64)
#include <windows.h>
#endif

static Disassembler disasm;

void GUI::DrawStateCPU(CPU& cpu, Memory& memory, u32 cycles)
{
    printf("Elapsed: \t%d\t|\tMemory:\n", cycles);
    printf("=====================================================\n");    
    printf("CPU reg A: \t%x\t|\t[%x]: %s\n", cpu.A, cpu.PC, disasm[cpu.PC]);
    printf("CPU reg X: \t%x\t|\t[%x]: %s\n", cpu.X, cpu.PC+1, disasm[cpu.PC+1]);
    printf("CPU reg Y: \t%x\t|\t[%x]: %s\n", cpu.Y, cpu.PC+2, disasm[cpu.PC+2]);
    printf("CPU flag Z: \t%d\t|\t[%x]: %s\n", cpu.Z, cpu.PC+3, disasm[cpu.PC+3]);
    printf("CPU flag N: \t%d\t|\t[%x]: %s\n", cpu.N, cpu.PC+4, disasm[cpu.PC+4]);
    printf("CPU flag V: \t%d\t|\t[%x]: %s\n", cpu.V, cpu.PC+5, disasm[cpu.PC+5]);
    printf("CPU flag C: \t%d\t|\t[%x]: %s\n", cpu.C, cpu.PC+6, disasm[cpu.PC+6]);
    printf("=====================================================\n");    
}
void GUI::LiveExecute(CPU& cpu, Memory& memory, u32 cycles)
{        
    disasm.Init();
    disasm.Disassembly(memory);
    u32 start_cycles = cycles;
    ClearScreen();
    printf("Press enter to next step (qq for exit)\n");
    while (cycles > 0)
    {
        if (std::cin.get() == '\n')
        {
            ClearScreen();
        }
        else if (std::cin.get() == 'q')
        {
            break;
        }
        else continue;
        DrawStateCPU(cpu, memory, start_cycles - cycles);
        printf("Press button to next step (qq for exit)\n");
        cpu.Step(cycles, memory);
    }
    
}

void GUI::ClearScreen()
{
#if defined(_WIN64)
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
#elif defined(__linux__)
    std::cout << "\x1B[2J\x1B[H";
#endif
}