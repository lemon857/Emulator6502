#include "memory.hpp"

// 6502 is LITLE ENDIAN!!!
void Memory::Init()
{
	countResently = 0;
	for (u32 i = 0; i < MAX_MEM; i++)
	{
		Data[i] = 0;
	}
}

/*Byte Memory::operator[](u32 address)
{
	// assert here if address > MAX_MEMORY
	resently.push(address);
	return Data[address];
}*/
void Memory::PushRes(u32 addr)
{
	resently.push(addr);
	countResently++;
}

void Memory::PopRes()
{
	resently.pop();
	countResently--;
}

Byte& Memory::SafeGetByte(u32 address)
{
	// 6502 is LITLE ENDIAN!!!
	return Data[address];
}

Word& Memory::SafeGetWord(u32 address)
{
	// 6502 is LITLE ENDIAN!!!
	Word value = Data[address + 1];
	value |= (Data[address] << 8);
	return value;
}

Byte& Memory::operator[](u32 address)
{
	// assert here if address > MAX_MEMORY
	PushRes(address);
	return Data[address];
}

void Memory::WriteWord(Word value, u32 address, u32 &cycles)
{
	// 6502 is LITLE ENDIAN!!!
	Data[address] = (value >> 8);
	Data[address + 1] = (value & 0xFF);
	PushRes(address);
	PushRes(address+1);
	cycles -= 2;
}

Word Memory::ReadWord(u32 address, u32 &cycles)
{
	// 6502 is LITLE ENDIAN!!!
	Word value = Data[address + 1];
	value |= (Data[address] << 8);
	PushRes(address);
	PushRes(address+1);
	cycles -= 2;
	return value;
}