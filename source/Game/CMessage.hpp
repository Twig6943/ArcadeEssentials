#pragma once
#include "Stage/StageEntity.hpp"

inline auto CMessageDispatcher_SendMessageToAll = (void(__thiscall*)(class CMessageDispatcher*, const char*, void*, ActorHandle))(0x00cef410);
inline auto CMessageDispatcher_SendMessageToActor = (bool(__thiscall*)(class CMessageDispatcher*, const char*, void*, ActorHandle, ActorHandle))(0x00cef520);

class CMessageDispatcher {
public:
	void SendMessageToAll(const char* name, void* data, ActorHandle sender) {
		CMessageDispatcher_SendMessageToAll(this, name, data, sender);
	}
	void SendMessageToActor(const char* name, void* data, ActorHandle sender, ActorHandle actor) {
		CMessageDispatcher_SendMessageToActor(this, name, data, sender, actor);
	}
};

inline CMessageDispatcher** g_MessageDispatcher = reinterpret_cast<CMessageDispatcher**>(0x01926ef0);