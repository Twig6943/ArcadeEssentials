#pragma once
#include "CActorComponent.hpp"

inline auto CarsVehiclePlayer_SetControlEnabled = (void(__thiscall*)(class CarsVehiclePlayer*, bool))(0x006cba80);

class CarsVehiclePlayer : public CActorComponent {
public:
	inline void SetControlEnabled(bool enabled) {
		CarsVehiclePlayer_SetControlEnabled(this, enabled);
	}
};