#include "memory.hpp"

void Memory::Init()
{
	for (u32 i = 0; i < MAX_MEM; i++)
	{
		Data[i] = 0;
	}
}

Byte Memory::operator[](u32 address) const
{
	// assert here if address > MAX_MEMORY
	return Data[address];
}

Byte& Memory::operator[](u32 address)
{
	// assert here if address > MAX_MEMORY
	return Data[address];
}

void Memory::WriteWord(Word value, u32 address, u32& cycles)
{
	Data[address] = value & 0xFF;
	Data[address + 1] = (value >> 8);
	cycles -= 2;
}

Word Memory::ReadWord(u32 address, u32& cycles)
{
	Word value = Data[address];
	value |= (Data[address + 1] << 8);
	cycles -= 2;
	return value;
}