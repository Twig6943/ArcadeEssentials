#pragma once
#include <cstdint>
#include "../CActor.hpp"

class DBlock {
public:
	static constexpr unsigned int DefaultID = -1;
	static constexpr unsigned int Flag_MarkedForDeath = 1;

	enum class BlockType : unsigned short {
		Activity,
		Components,
		Net,
		Properties,
		Sound,
		Num_BlockTypes,
		Custom,
	};
public:
	class CActor* actor;
	BlockType block_ype;
	unsigned short flags;
	unsigned int id;

	inline bool IsMarkedForDeath(void) const {
		return (flags & Flag_MarkedForDeath) == 1;
	}
	inline void MarkForDeath(void) {
		flags |= Flag_MarkedForDeath;
	}
	inline virtual ~DBlock() {
	}
};