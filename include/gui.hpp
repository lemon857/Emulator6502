#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "cpu.hpp"
#include "memory.hpp"

class GUI
{
public:
    GUI() = delete;
    ~GUI() = delete;
    static void DrawState(CPU& cpu, Memory& memory, u32 cycles);
    static void LiveExecute(CPU& cpu, Memory& memory, u32 cycles);
private:
    static void ClearScreen();
};

#endif // _GUI_HPP_
