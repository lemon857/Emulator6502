#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "cpu.hpp"
#include "memory.hpp"
#include "disasm.hpp"

struct GUI
{
    static void DrawStateCPU(CPU& cpu, Memory& memory, u32 cycles);
    static void LiveExecute(CPU& cpu, Memory& memory, u32 cycles);
    static void ClearScreen();
};

#endif // _GUI_HPP_
