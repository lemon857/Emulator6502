#include "disasm.hpp"
#include "cpu.hpp"

// Follow rules, defined in memory.hpp about mark memory
void Disassembler::Disassembly(Memory& memory)
{
    Word ptr = START_PROGRAM;
    while (ptr < Memory::MAX_MEM)
    {
        switch (memory[ptr])
        {
        case CPU::INS_TAX:
            out_data[ptr] = "tax";
            break;
        case CPU::INS_TXA:
            out_data[ptr] = "txa";
            break;
        case CPU::INS_TAY:
            out_data[ptr] = "tay";
            break;
        case CPU::INS_TYA:
            out_data[ptr] = "tya";
            break;
        case CPU::INS_DEX:
            out_data[ptr] = "dex";
            break;
        case CPU::INS_INX:
            out_data[ptr] = "inx";
            break;
        case CPU::INS_DEY:
            out_data[ptr] = "dey";
            break;
        case CPU::INS_INY:
            out_data[ptr] = "iny";
            break;
        
        default:
            out_data[ptr] = std::to_string(memory[ptr]);
            break;
        }
    }
    
}

void Disassembler::Init()
{
    for (int i = 0; i < Memory::MAX_MEM; i++)
    {
        out_data[i] = "";
    }
}