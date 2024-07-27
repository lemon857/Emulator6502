#include "disasm.hpp"
#include "cpu.hpp"

#include <vector>
#include <stdio.h>

#define INS_WITHOUT_ARGS(INS, str) {\
    case CPU::INS_##INS: \
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str; \
            break;\
}

#define INS_BRANCH(INS, str) {\
    case CPU::INS_##INS: \
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(ptr + (signed char)memory.SafeGetByte(ptr+1)); \
            nextIsValue++; \
            break;\
}

#define INS_WITH_ABS_ZP(INS, str) {\
        case CPU::INS_##INS##_ZP:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
            break;\
        case CPU::INS_##INS##_ABS:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1));\
            nextIsValue+=2; \
            break;\
}

#define INS_WITH_ABS_END(INS, str, XorY, XorYstr) {\
        case CPU::INS_##INS##_ZP:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
            break;\
        case CPU::INS_##INS##_ZP##XorY:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1)) + ", " XorYstr;\
            nextIsValue++; \
            break;\
        case CPU::INS_##INS##_ABS:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1));\
            nextIsValue+=2; \
            break;\
}

#define INS_WITH_ABS_XYEND(INS, str, XorY, XorYstr){ \
        case CPU::INS_##INS##_IM: \
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " #$" + WordToStr(memory.SafeGetByte(ptr+1)); \
            nextIsValue++; \
            break; \
        case CPU::INS_##INS##_ZP:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
            break;\
        case CPU::INS_##INS##_ZP##XorY:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1)) + ", " XorYstr;\
            nextIsValue++; \
            break;\
        case CPU::INS_##INS##_ABS:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1));\
            nextIsValue+=2; \
            break;\
        case CPU::INS_##INS##_ABS##XorY:\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1)) + ", " XorYstr;\
            nextIsValue+=2; \
            break;\
}

#define INS_WITH_INDIR(INS, str) {\
        case CPU::INS_##INS##_IM: {\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " #$" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
        }break;\
        case CPU::INS_##INS##_ZP:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
        }break; \
        case CPU::INS_##INS##_ZPX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1)) + ", X";\
            nextIsValue++; \
        }break; \
        case CPU::INS_##INS##_ABS:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; }\
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1));\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_ABSX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1)) + ", X";\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_ABSY:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1)) + ", Y";\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_INDX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " ($" + WordToStr(memory.SafeGetByte(ptr+1)) + "), X";\
            nextIsValue++; \
        }break;\
        case CPU::INS_##INS##_INDY:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " ($" + WordToStr(memory.SafeGetByte(ptr+1)) + "), Y";\
            nextIsValue++; \
        }break;\
}
        
#define INS_WITH_INDIR_WITHOUT_IM(INS, str) {\
        case CPU::INS_##INS##_ZP:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1));\
            nextIsValue++; \
        }break; \
        case CPU::INS_##INS##_ZPX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetByte(ptr+1)) + ", X";\
            nextIsValue++; \
        }break; \
        case CPU::INS_##INS##_ABS:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1));\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_ABSX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1)) + ", X";\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_ABSY:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " $" + WordToStr(memory.SafeGetWord(ptr+1)) + ", Y";\
            nextIsValue+=2; \
        }break;\
        case CPU::INS_##INS##_INDX:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " ($" + WordToStr(memory.SafeGetByte(ptr+1)) + "), X";\
            nextIsValue++; \
        }break;\
        case CPU::INS_##INS##_INDY:{\
            if (nextIsValue > 0)  { DefaultHandle(memory, ptr); break; } \
            out_data[ptr] = str " ($" + WordToStr(memory.SafeGetByte(ptr+1)) + "), Y";\
            nextIsValue++; \
        }break;\
}

// Follow rules, defined in memory.hpp about mark memory
void Disassembler::Disassembly(Memory &memory)
{
    u32 ptr = START_PROGRAM;
    while (ptr < Memory::MAX_MEM)
    {
        switch (memory.SafeGetByte(ptr))
        {
            INS_WITHOUT_ARGS(TAX, "tax");
            INS_WITHOUT_ARGS(TXA, "txa");
            INS_WITHOUT_ARGS(TAY, "tay");
            INS_WITHOUT_ARGS(TYA, "tya");
            INS_WITHOUT_ARGS(DEX, "dex");
            INS_WITHOUT_ARGS(DEY, "dey");
            INS_WITHOUT_ARGS(INX, "inx");
            INS_WITHOUT_ARGS(INY, "iny");

            INS_WITHOUT_ARGS(CLC, "clc");
            INS_WITHOUT_ARGS(SEC, "sec");
            INS_WITHOUT_ARGS(CLV, "clv");

            INS_WITHOUT_ARGS(TXS, "txs");
            INS_WITHOUT_ARGS(TSX, "tsx");
            INS_WITHOUT_ARGS(PHA, "pha");
            INS_WITHOUT_ARGS(PLA, "pla");

            INS_BRANCH(BEQ, "beq");
            INS_BRANCH(BNE, "bne");
            INS_BRANCH(BCS, "bcs");
            INS_BRANCH(BCC, "bcc");

            INS_WITH_INDIR(CMP, "cmp");

            INS_WITH_ABS_ZP(CPX, "cpx");
            INS_WITH_ABS_ZP(CPY, "cpy");

            INS_WITH_INDIR(LDA, "lda");

            INS_WITH_ABS_XYEND(LDX, "ldx", Y, "Y");
            INS_WITH_ABS_XYEND(LDY, "ldy", X, "X");

            INS_WITH_INDIR(ADC, "adc");
            INS_WITH_INDIR(SBC, "sbc");

            INS_WITH_INDIR(AND, "and");
            INS_WITH_INDIR(ORA, "ora");
            INS_WITH_INDIR(EOR, "eor");

            INS_WITH_INDIR_WITHOUT_IM(STA, "sta");
            INS_WITH_ABS_END(STX, "stx", Y, "Y");
            INS_WITH_ABS_END(STY, "sty", X, "X");

        case CPU::INS_JMP_ABS: {
            if (nextIsValue > 0) { DefaultHandle(memory, ptr); break; }
            out_data[ptr] = "jmp $" + WordToStr(memory.SafeGetWord(ptr + 1));
            nextIsValue += 2;
        }break;
        case CPU::INS_JMP_IND: {
            if (nextIsValue > 0) { DefaultHandle(memory, ptr); break; }
            out_data[ptr] = "jmp ($" + WordToStr(memory.SafeGetWord(ptr + 1)) + ")";
            nextIsValue += 2;
        }break;

        default:
            DefaultHandle(memory, ptr);
            break;
        }
        ptr++;
    }    
}

void Disassembler::Init()
{
    nextIsValue = 0;
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
    if (out.length() == 1) out = "0" + out;
	return out;
}

char Disassembler::NumToHex(Byte num)
{
    if (num < 10) return num + '0';
    return num + 0x57; 
}

void Disassembler::DefaultHandle(Memory& memory, u32& ptr)
{
    out_data[ptr] = WordToStr(memory.SafeGetByte(ptr));
    if (nextIsValue > 0) nextIsValue--;
}
