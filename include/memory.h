#ifndef _MEMORY_H_
#define _MEMORY_H_

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

const Word START_PROGRAM = 0x0A00;
const Word STACK_POINTER = 0x0100;

struct Memory
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];
	void Init();

	/* read one byte */
	Byte operator[](u32 address) const;

	/* write one byte */
	Byte& operator[](u32 address);

	/* write two bytes*/
	void WriteWord(Word value, u32 address, u32& cycles);

	/* read two bytes*/
	Word ReadWord(u32 address, u32& cycles);
};

#endif // !_MEMORY_H_
