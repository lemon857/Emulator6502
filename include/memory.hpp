#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <queue>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

// 0x0000 - 0x00FF - Zero Page
// 0x0100 - 0x01FF - Is a stack page
// 0x0200 - 0x02FF - I use as data storage
const Word START_PROGRAM = 0x0300; // 0x0300 - MAX_MEM - our possible using memory

struct Memory
{
	static constexpr u32 MAX_MEM = 0x0800;
	std::queue<u32> resently;
	u32 countResently;
	Byte Data[MAX_MEM];
	void Init();

	/* read one byte */
	//Byte operator[](u32 address);

	void PushRes(u32 addr);

	void PopRes();

	Byte& SafeGetByte(u32 address);
	Word& SafeGetWord(u32 address);

	/* write one byte */
	Byte& operator[](u32 address);

	/* write two bytes*/
	void WriteWord(Word value, u32 address, u32 &cycles);

	/* read two bytes*/
	Word ReadWord(u32 address, u32 &cycles);
};

#endif // _MEMORY_HPP_
