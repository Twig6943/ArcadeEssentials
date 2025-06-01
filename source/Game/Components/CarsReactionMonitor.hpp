#pragma once
#include "CActorComponent.hpp"

class CarsReactionMonitor : public CActorComponent {
public:
	char unknown[0x164];
	int m_wipeoutReactHitCount[26];
	float m_wipeoutReactHitTimes[26];
	float m_remainingWipeoutInvulnerableTime;
	bool m_wipingOut;
	float m_wipeOutRemainingTime;
	float m_inReactionRemainingTime;
	float m_lastJitterTime;
	class CarsVehiclePlayer* m_carsVehiclePlayerComponent;
public:
	enum class WeaponReaction : int {
		Unk0 = 0,
		Max = 26,
	};

	struct ReactionInfoEntry {
		char reaction4CC[4];
		bool vehicleSlowDown;
		float slowDownDuration;
		float slowDownParam1;
		float slowDownParam2;
		bool hideCar;
		float humanPlayerMeterShake;
		bool applyRandomForce;
		float scaleForce;
		bool drainEnergy;
		bool applyJitter;
		int wipeoutHitCount;
		float wipeoutBetweenHitTime;
		float wipeoutInvulnerableTime;
		float wipeoutOptionalDamage;
		float timedFatality;
		float timedFadeOut;
		float timedFadeIn;
		bool interruptable;
		char emitterControl[64];
		char emitterName[64];
		char startMessage[64];
		char stopMessage[64];
		float camShake_Scale;
		float camShake_Duration;
		float camShake_Amplitude;
		float camShake_Speed;
		bool camShake_NoiseFunc;
		float camShake_FwdSense;
		float camShake_UpSense;
		float camShake_SideSense;
		int camShake_Rumble;
	};

	static inline ReactionInfoEntry* s_reactionInfoEntries = reinterpret_cast<ReactionInfoEntry*>(0x018c6190);
	static inline std::uint32_t* s_reactionInfoCount = reinterpret_cast<std::uint32_t*>(0x018c5248);
};

static_assert(sizeof(CarsReactionMonitor::ReactionInfoEntry) == 0x16C);

void __fastcall HandleWipeout(CarsReactionMonitor* _this, std::uintptr_t edx, int unk, int reactIndex, ActorHandle sender);
void __fastcall HandleCommenceWipeout(CarsReactionMonitor* _this);