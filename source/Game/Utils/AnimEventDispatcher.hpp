#pragma once
#include "../Stage/StageEntity.hpp"

inline auto AnimEventDispatcher_SendEvent = (void(_cdecl*)(const char*, AgentID, void*))(0x00f0f7a0);

class AnimEventDispatcher {
public:
	static inline void SendEvent(const char* event, AgentID target, void* data = nullptr) {
		AnimEventDispatcher_SendEvent(event, target, data);
	}
};