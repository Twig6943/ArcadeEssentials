#pragma once
#include <cstdint>
#include "../Game/Stage/StageEntity.hpp"
#include "../Game/Stage/ActivityDBlock.hpp"
#include "../Game/Utils/Vector3.hpp"

inline auto CActor_FromHandle = (class CActor*(_cdecl*)(ActorHandle))(0x00e19e20);

class CActor : public CStageEntity, public ActivityDBlock {
public:
	char reserved[12];
public:
	inline virtual bool IsUnstableCollisionObject(void) override {
		return true;
	}

	static inline CActor* FromHandle(ActorHandle handle) {
		return CActor_FromHandle(handle);
	}

	inline Vector3 GetBodyInWorld() {
		return *reinterpret_cast<Vector3*>(reinterpret_cast<std::uintptr_t>(this) + 0x58);
	}

	inline ActorHandle GetHandle() {
		return *reinterpret_cast<ActorHandle*>(reinterpret_cast<std::uintptr_t>(this) + 0x24);
	}
};