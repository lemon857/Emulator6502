#ifndef _DISASM_HPP_
#define _DISASM_HPP_

#include "memory.hpp"
#include <string>
#include <queue>

struct Disassembler
{
    std::queue<Word> resMemory;
    std::string out_data[Memory::MAX_MEM];
    void Disassembly(Memory& memory); 
    void Init();
    std::string WordToStr(Word value);
    char NumToHex(Byte num);
    const char* operator[](Word address)
    {
        return out_data[address].c_str();
    }
};


#endif // _DISASM_HPP_