#ifndef _DISASM_HPP_
#define _DISASM_HPP_

#include "memory.hpp"
#include <string>
#include <vector>

struct Disassembler
{
    std::string out_data[Memory::MAX_MEM];
    void Disassembly(Memory& memory); 
    void Init();
    const char* operator[](Word address)
    {
        return out_data[address].c_str();
    }
};


#endif // _DISASM_HPP_