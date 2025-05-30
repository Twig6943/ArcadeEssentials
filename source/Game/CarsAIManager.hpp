#pragma once
#include "Stage/StageEntity.hpp"

inline auto CarsAIManager_HandleSideBash = (void(__thiscall*)(class CarsAIManager*, ActorHandle, ActorHandle))(0x00411cb0);

class CarsAIManager {
public:
	inline void HandleSideBash(ActorHandle src, ActorHandle dst) {
		CarsAIManager_HandleSideBash(this, src, dst);
	}
};
inline CarsAIManager** g_CarsAIManager = reinterpret_cast<CarsAIManager**>(0x018ae0d4);