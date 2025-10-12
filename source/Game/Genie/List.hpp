#pragma once
#include "String.hpp"

namespace Genie {
	template <typename T> struct List {
		struct Node {
			Node* m_pNext;
			Node* m_pPrev;
			T value;
		};
		void* m_Unknown[5];
		Node* m_pHead;
		Node* m_pTail;
		unsigned int m_nElements;
	};
};

static_assert(sizeof(Genie::List<const Genie::String>) == 0x20);