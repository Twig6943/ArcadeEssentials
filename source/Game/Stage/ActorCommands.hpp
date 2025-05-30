#pragma once
#include "StageEntity.hpp"
#include "../CActor.hpp"

inline auto ActorCommands_DamageActor = (void(_cdecl*)(ActorHandle, float, const char*, CActor*))(0x00dfe2c0);

namespace ActorCommands {
	inline void DamageActor(ActorHandle target, float damage, const char* damage_type, CActor* damage_source) {
		ActorCommands_DamageActor(target, damage, damage_type, damage_source);
	}
};