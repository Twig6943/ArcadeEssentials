#pragma once
#include "DBlock.hpp"
#include "../CActor.hpp"

inline auto Cars2VehicleDBlock_Get = (class Cars2VehicleDBlock*(__thiscall*)(CActor*, int, int))(0x00e19820);


// 00e19820
class Cars2VehicleDBlock : public DBlock {
public:
	inline static Cars2VehicleDBlock* Get(CActor& actor) {
		return Cars2VehicleDBlock_Get(&actor, 0x13, -1);
	}
};