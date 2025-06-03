#pragma once
#include "DBlock.hpp"
#include "../CActor.hpp"
#include "../Components/CarsVehiclePlayer.hpp"
#include "../Components/ActiveMoves.hpp"
#include "../Components/CarsReactionMonitor.hpp"

inline auto Cars2VehicleDBlock_Get = (class Cars2VehicleDBlock*(__thiscall*)(CActor*, int, int))(0x00e19820);

class Cars2VehicleDBlock : public DBlock {
public:
	CarsVehicle* m_carsVehicle;
	struct LapTracker* m_lapTracker;
	ActiveMoves* m_activeMoves;
	CarsVehiclePlayer* m_carsVehiclePlayer;
	CarsControlMapper* m_carsControlMapper;
	CarsReactionMonitor* m_carsReactionMonitor;
public:
	inline static Cars2VehicleDBlock* Get(CActor& actor) {
		return Cars2VehicleDBlock_Get(&actor, 0x13, -1);
	}
};

static_assert(sizeof(Cars2VehicleDBlock) == 40);