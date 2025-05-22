#pragma once
#include "Types.hpp"

class DynamicArrayBase {
protected:
	void* m_Items;
	u32 m_Count;
	u32	m_Total;
	u32	m_Zero : 1;
	u32	m_Locked : 1;
	u32	m_Aligned : 1;
	u32	m_GrowBy : 21;
	u32	m_Heap : 8;
	char m_Unk[0xC];
public:
};

template <typename T> class DynamicArray : DynamicArrayBase {
};

static_assert(sizeof(DynamicArrayBase) == 0x1C);
static_assert(sizeof(DynamicArray<void*>) == 0x1C);