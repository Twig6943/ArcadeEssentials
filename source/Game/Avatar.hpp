#pragma once
#include "CActor.hpp"

class Avatar : public CActor {
public:
	static Avatar* FromCActor(CActor* actor) {
		Avatar* avatar = reinterpret_cast<Avatar*>(actor);
		if (avatar == nullptr) {
			return nullptr;
		}
		std::uint32_t flag = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(avatar) + 8);
		if ((flag & *reinterpret_cast<std::uint32_t*>(0x018ac8e0)) != *reinterpret_cast<std::uint32_t*>(0x018ac8dc)) {
			return nullptr;
		}
		return avatar;
	}
	inline int GetPlayerId() {
		return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x300);

	}
	inline bool IsPlayerControlled() {
		return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x300) != -1;
	}
};