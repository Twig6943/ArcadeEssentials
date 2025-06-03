#pragma once
#include <cstdint>
#include <array>

inline auto GameProgressionManager_GetAICarCount = (int(__thiscall*)(class GameProgressionManager*))(0x004ead80);
inline auto GameProgressionManager_SetAICarCount = (void(__thiscall*)(class GameProgressionManager*, int))(0x004eadc0);
inline auto GameProgressionManager_FUN_004e8400 = (void(__thiscall*)(class GameProgressionManager*, bool))(0x004e8400);
inline auto GameProgressionManager_FUN_004ebab0 = (void(__thiscall*)(class GameProgressionManager*, int))(0x004ebab0);
inline auto GameProgressionManager_FUN_004ebaf0 = (void(__thiscall*)(class GameProgressionManager*, int))(0x004ebaf0);
inline auto GameProgressionManager_FormatStoryMission = (char* (__thiscall*)(class GameProgressionManager*, int, int))(0x004eaf90);

class GameProgressionManager {
public:
	enum class MissionMode {
		Race,
		Bomb,
		Hunter,
		Arena,
		Collect,
		Pursuit, // Unused
		Takedown,
		Tutorial,
	};
	struct MissionModeInfo {
		const char* name;
		const char* localized_name;
		MissionMode mode;
	};
	static inline MissionModeInfo* s_MissionModeInfos = reinterpret_cast<MissionModeInfo*>(0x0184cea0);
public:
	char unk[0xB8];
	int unk_index;
public:
	inline char* FormatStoryMission(int clearance_level, int mission_index) {
		return GameProgressionManager_FormatStoryMission(this, clearance_level, mission_index);
	}
	inline MissionMode GetMissionMode() {
		return *reinterpret_cast<MissionMode*>(reinterpret_cast<std::uintptr_t>(this) + 0xF8);
	}
};

inline GameProgressionManager** g_GameProgressionManager = reinterpret_cast<GameProgressionManager**>(0x018ae0f0);