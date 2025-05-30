#pragma once
#include "Utils/Vector3.hpp"
// 
inline auto CarsCamManager_StartEyeShake = (void(__thiscall*)(class CarsCamManager*, std::uint32_t, float, float, float, bool, Vector3))(0x00488dd0);

class CarsCamManager {
public:
	void StartEyeShake(std::uint32_t id, float unk, float unk2, float unk3, bool unk4, Vector3 unk5) {
		CarsCamManager_StartEyeShake(this, id, unk, unk2, unk3, unk4, unk5);
	}
};

inline CarsCamManager** g_CarsCamManager = reinterpret_cast<CarsCamManager**>(0x018ae0dc);
