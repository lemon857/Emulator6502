#include "disasm.hpp"
#include "cpu.hpp"

#include <vector>
#include <stdio.h>

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
        case CPU::INS_LDA_IM:
            out_data[ptr] = "lda #$" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDA_ZP:
            out_data[ptr] = "lda $" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDA_ZPX:
            out_data[ptr] = "lda $" + WordToStr(memory[ptr+1]) + ", X";
            break;
        case CPU::INS_LDA_ABS:
            out_data[ptr] = "lda $" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDA_ABSX:
            out_data[ptr] = "lda $" + WordToStr(memory[ptr+1]) + ", X";
            break;
        case CPU::INS_LDA_ABSY:
            out_data[ptr] = "lda $" + WordToStr(memory[ptr+1]) + ", Y";
            break;
        case CPU::INS_LDA_INDX:
            out_data[ptr] = "lda ($" + WordToStr(memory[ptr+1]) + "), X";
            break;
        case CPU::INS_LDA_INDY:
            out_data[ptr] = "lda ($" + WordToStr(memory[ptr+1]) + "), Y";
            break;

        case CPU::INS_LDX_IM:
            out_data[ptr] = "ldx #$" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDX_ZP:
            out_data[ptr] = "ldx $" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDX_ZPY:
            out_data[ptr] = "ldx $" + WordToStr(memory[ptr+1]) + ", Y";
            break;
        case CPU::INS_LDX_ABS:
            out_data[ptr] = "ldx $" + WordToStr(memory[ptr+1]);
            break;
        case CPU::INS_LDX_ABSY:
            out_data[ptr] = "ldx $" + WordToStr(memory[ptr+1]) + ", Y";
            break;
        
        default:
            out_data[ptr] = WordToStr(memory[ptr]);
            break;
        }
        ptr++;
    }    
}

void Disassembler::Init()
{
    for (int i = 0; i < Memory::MAX_MEM; i++)
    {
        out_data[i] = "";
    }
}

std::string Disassembler::WordToStr(Word value)
{
    std::string out = "";
    std::vector<char> tmp;
	int i = 0;
	do
	{
		tmp.push_back(NumToHex(value % 16));
        value /= 16;
	} while (value != 0);
    for (int i = tmp.size() - 1; i >= 0; i--)
    {
        out += tmp[i];
    }
	return out;
}

char Disassembler::NumToHex(Byte num)
{
    if (num < 10) return num + '0';
    return num + 0x57; 
}