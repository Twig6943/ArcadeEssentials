#pragma once
#include "../CActor.hpp"

class CActorComponent {
public:
	CActor* actor;
	int priority;
	bool active;
	bool initialized;
	bool suspended;
	bool markedForDeath;
public:
	inline virtual ~CActorComponent() {
	}

	inline CActor& GetActor() {
		return *actor;
	}
};