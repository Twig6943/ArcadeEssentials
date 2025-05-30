#pragma once
#include "CActorComponent.hpp"

inline auto CarsVehicle_BumpBashHandle = (void(_cdecl*)(CActor*, CActor*, bool))(0x006be990);
inline auto CarEnergy_AddEnergy = (void(__thiscall*)(class CarEnergy*, float, bool, bool, int, const char*, int, bool))(0x00690590);
inline auto CarsControlMapper_SetVibrationDuration = (void(__thiscall*)(class CarsControlMapper*, std::uint32_t, std::uint32_t, int))(0x006646f0);

class CarsControlMapper {
public:
	inline void SetVibrationDuration(std::uint32_t duration_ms, std::uint32_t intensity, int pan = 0) {
		CarsControlMapper_SetVibrationDuration(this, duration_ms, intensity, pan);
	}
};

class CarEnergy {
public:
	inline void AddEnergy(float energy, bool one_shot, bool play_burst, int cho, const char* anchor, int control_index, bool force_my_cho) {
		CarEnergy_AddEnergy(this, energy, one_shot, play_burst, cho, anchor, control_index, force_my_cho);
	}
};
class CBulletVehicle : public CActorComponent {
};
// static_assert(sizeof(CBulletVehicle) == 0x890);

class CarsVehicle : public CBulletVehicle {
public:
	static void BumpBashHandle(CActor* actor, CActor* victim, bool unk) {
		CarsVehicle_BumpBashHandle(actor, victim, unk);
	}
	inline CarEnergy& GetCarEnergy() {
		return *reinterpret_cast<CarEnergy*>(reinterpret_cast<std::uintptr_t>(this) + 0x11F0);
	}
	inline bool GetInTheZone() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0xE4C) != 0;
	}
};
// static_assert(sizeof(CarsVehicle) == 0x1800);