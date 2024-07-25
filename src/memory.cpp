#include "memory.hpp"

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
	return Data[address];
}

Byte& Memory::operator[](u32 address)
{
	// assert here if address > MAX_MEMORY
	PushRes(address);
	return Data[address];
}

void Memory::WriteWord(Word value, u32 address, u32& cycles)
{
	Data[address] = value & 0xFF;
	Data[address + 1] = (value >> 8);
	PushRes(address);
	PushRes(address+1);
	cycles -= 2;
}

Word Memory::ReadWord(u32 address, u32& cycles)
{
	Word value = Data[address];
	value |= (Data[address + 1] << 8);
	PushRes(address);
	PushRes(address+1);
	cycles -= 2;
	return value;
}