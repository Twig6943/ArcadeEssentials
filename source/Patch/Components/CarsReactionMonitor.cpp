#include "../../Game/Components/CarsReactionMonitor.hpp"
#include "../../Game/Components/CarsVehicle.hpp"
#include "../../Game/Components/CarsVehiclePlayer.hpp"
#include "../../Game/Stage/ActorCommands.hpp"
#include "../../Game/Stage/Cars2VehicleDBlock.hpp"
#include "../../Game/Utils/AnimEventDispatcher.hpp"

inline auto FUN_00e256f0 = (Vector3 * (__thiscall*)(std::uintptr_t, Vector3*))(0x00e256f0);
inline auto SkidMark_Enable = (void(__thiscall*)(CActorComponent*, int, int))(0x00677110);
inline auto ParticleManager_CreateEffect = (void(__thiscall*)(std::uintptr_t, const char*, Vector3*, Vector3*, unsigned int))(0x00d8cf30);

inline auto g_ParticleManager = reinterpret_cast<std::uintptr_t*>(0x0192879c);

void __fastcall HandleWipeout(CarsReactionMonitor* _this, std::uintptr_t edx, int unk, int reactIndex, ActorHandle sender) {
	float frameTime = *reinterpret_cast<float*>(0x01925e40);
	float timeDiff = (frameTime - _this->m_wipeoutReactHitTimes[reactIndex]);

	if (_this->m_remainingWipeoutInvulnerableTime > 0.0f) {
		_this->m_remainingWipeoutInvulnerableTime -= frameTime;
		return;
	}
	else if (_this->m_remainingWipeoutInvulnerableTime < 0.0f) {
		_this->m_remainingWipeoutInvulnerableTime = 0.0f;
	}
	if (timeDiff > CarsReactionMonitor::s_reactionInfoEntries[reactIndex].wipeoutBetweenHitTime) {
		_this->m_wipeoutReactHitCount[reactIndex] = 1;
	}
	else {
		_this->m_wipeoutReactHitCount[reactIndex]++;
	}
	_this->m_wipeoutReactHitTimes[reactIndex] = frameTime;
	if ((_this->m_wipeoutReactHitCount[reactIndex] >= CarsReactionMonitor::s_reactionInfoEntries[reactIndex].wipeoutHitCount) && (_this->m_inReactionRemainingTime <= 0.0f)) {
		HandleCommenceWipeout(_this);
		if (CarsReactionMonitor::s_reactionInfoEntries[reactIndex].wipeoutOptionalDamage > 0.0f) {
			ActorCommands::DamageActor(_this->GetActor().GetHandle(), CarsReactionMonitor::s_reactionInfoEntries[ reactIndex ].wipeoutOptionalDamage, "spno", CActor::FromHandle(sender));
		}
	}
}

void __fastcall HandleCommenceWipeout(CarsReactionMonitor* _this) {
	CarsVehicle* vehicle = Cars2VehicleDBlock::Get(*_this->actor)->m_carsVehicle;
    if (vehicle == nullptr || !vehicle->GetInTheZone()) {
        AnimEventDispatcher::SendEvent("React_SpinOut", _this->GetActor().GetHandle());
        if (vehicle != nullptr) {
            vehicle->SetPowerTrainOverride(true, 2.0f, 0.0f, 2.0f);
        }
		if (_this->m_carsVehiclePlayerComponent != nullptr) {
			_this->m_carsVehiclePlayerComponent->SetControlEnabled(false);
		}
		Vector3 translation{};
		Vector3 ejectVelocity{};
		FUN_00e256f0(reinterpret_cast<std::uintptr_t>(_this->actor) + 0x16C, &translation);
		ParticleManager_CreateEffect(*g_ParticleManager, "SpinOut_Smoke", &translation, &ejectVelocity, 0);
		CActorComponent* skidMark = _this->actor->GetComponentByName("SkidMark");
		if (skidMark != nullptr) {
			SkidMark_Enable(skidMark, 0, 2);
			SkidMark_Enable(skidMark, 1, 2);
			SkidMark_Enable(skidMark, 2, 2);
			SkidMark_Enable(skidMark, 3, 2);
		}
		_this->m_wipingOut = true;
		_this->m_wipeOutRemainingTime = 2.0;
		_this->m_inReactionRemainingTime = 0.75;
    }
    else {
        vehicle->CancelInTheZone(false);
        AnimEventDispatcher::SendEvent("React_SpinOut_ITZ", _this->GetActor().GetHandle());
        _this->m_wipingOut = true;
        _this->m_wipeOutRemainingTime = 1.5;
        _this->m_inReactionRemainingTime = 1.5;
    }
}