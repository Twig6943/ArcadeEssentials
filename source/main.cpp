#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <shlwapi.h>
#include <sunset.hpp>
#include <d3d9.h>
#include "pentane.hpp"
#include "Game/GameSpecificFlashImpl.hpp"
#include "Game/Genie/String.hpp"

static std::atomic<bool> IS_PC = false;

inline auto WindowsSystemInputDriver_LockPlayerToController = (void(__thiscall*)(std::uintptr_t, int, int))(0x00816dd0);
inline auto WindowsSystemInputDriver_IsControllerLocked = (bool(__thiscall*)(std::uintptr_t, int))(0x00816ed0);
inline auto FUN_00ef3a30 = (void(__thiscall*)(std::uintptr_t))(0x00ef3a30);
inline auto FUN_0060e7d0 = (void(__thiscall*)(std::uintptr_t, int, int))(0x0060e7d0);
inline auto FUN_0060ee20 = (void**(__thiscall*)(std::uintptr_t, void*, int))(0x0060ee20);
inline auto FUN_00613000 = (void*(__thiscall*)(std::uintptr_t, void*))(0x00613000);
inline auto _CarsFrontEnd_SetScreen = (void(__thiscall*)(void*, int, const char*, bool))(0x004c1440);
inline auto _CarsFrontEnd_SetLevelAndUnk = (void(__thiscall*)(void*, void*))(0x004c0780);
inline auto _CarsFrontEnd_UnkHandleTrackLengthType = (void(__thiscall*)(void*, char*))(0x004c0800);
inline auto _CarsFrontEnd_SetGameModeIndex = (void(__thiscall*)(void*, char*))(0x004c2dc0);
inline auto _CarsFrontEnd_UNK_004c3b70 = (void(__thiscall*)(void*))(0x004c3b70);
inline auto Flash_Movie_CallFlashFunction = (void(__cdecl*)(std::uintptr_t, const char*, ...))(0x01168690);
inline auto _CMessageDispatcher_SendMessageToAll = (void(__thiscall*)(void*, const char*, unsigned int, unsigned int))(0x00cef410);
inline auto GameProgressionManager_GetSomethingWeDontKnow = (int(__thiscall*)(std::uintptr_t))(0x004ead80);
inline auto GameProgressionManager_SetSomethingWeDontKnow = (void(__thiscall*)(std::uintptr_t, int))(0x004eadc0);
inline auto GameProgressionManager_FUN_004e8400 = (void(__thiscall*)(std::uintptr_t, bool))(0x004e8400);
inline auto GameProgressionManager_FUN_004ebab0 = (void(__thiscall*)(std::uintptr_t, int))(0x004ebab0);
inline auto GameProgressionManager_FUN_004ebaf0 = (void(__thiscall*)(std::uintptr_t, int))(0x004ebaf0);
inline auto GameProgressionManager_FormatStoryMission = (char* (__thiscall*)(std::uintptr_t, int, int))(0x004eaf90);
inline auto Flash_EngineTextureLoader_LoadTextureSet = (void* (__thiscall*)(void*, const char*, bool, int))(0x0116cd20);
inline auto CTranslator_Translate = (char*(__thiscall*)(void*, void*, bool))(0x00cf6dd0);
inline auto UnkExcelDataBase_GetUnk = (void(__thiscall*)(void*, void*, void*))(0x0052cd70);
inline auto UnkExcelDataBase_GetUnk1 = (void(__thiscall*)(void*, void*, void*))(0x0052ce80);
inline auto UnkExcelDataBase_GetUnk2 = (void(__thiscall*)(void*, void*, void*, float*, float*))(0x0052cfe0);
inline auto UnkExcelDataBase_GetUnk3 = (void(__thiscall*)(void*, std::uint32_t, void*, void*))(0x0052af10);
inline auto PersistentData_GetGlobal = (int(__thiscall*)(void*, const char*))(0x00cf0e20);
inline auto PersistentData_SetGlobal = (void(__thiscall*)(void*, const char*, std::uint32_t))(0x00cf0e40);
inline auto FUN_00f675d0 = (std::uint32_t(__thiscall*)(void*, const char*, std::uint32_t))(0x00f675d0);
inline auto FUN_00f66d60 = (std::uint32_t(__thiscall*)(void*, std::uint32_t))(0x00f66d60);
inline auto GameCommon_SetPlayerSfxVolume = (void(__thiscall*)(void*, float, char))(0x00eb48b0);
inline auto GameCommon_SetPlayerMusicVolume = (void(__thiscall*)(void*, float, char))(0x00eb49a0);
inline auto GameCommon_SetPlayerDialogueVolume = (void(__thiscall*)(void*, float, char))(0x00eb4a90);

enum CarsFrontEndScreen {
	Invalid = 0,
	ExitToWindows = 1,
	AutoSaveWarning = 2,
	TitleMenu = 3,
	UnkSubMenuGears = 4, // FIXME
	SaveFileLoading = 5, // FIXME
	SaveSlots = 6, // Causes a game crash!
	MainMenu_Extras = 7,
	Extras_Options = 8,
	Extras_EnterCode = 9,
	Extras_Cheats = 10,
	Extras_Credits = 11,
	MT_FrontEnd = 12,
	MainMenu_StoryMissions = 13,
	MissionSelect = 14,
	MissionDetails = 15,
	MainMenu_Badges = 16,
	BadgeMenu_Badges = 17,
	BadgeMenu_Crests = 18,
	BadgeMenu_Suitcases = 19, // Unused in both PC and Arcade
	MainMenu_CustomMissions = 20,
	MT_Hunter = 21,
	MainMenu_CustomMissions2 = 22,
	MT_Hunter2 = 23,
	MissionSettings_Unk = 24, // FIXME
	CustomSquadSeries = 25,
	MissionSettings_Unk2 = 26, // FIXME
	GarageConnect = 27,
	WorldOfCarsConnect = 28,
	MainMenu_Garage = 29,
	GarageDetails = 30,
	WorldOfCars = 31, // Causes a game crash!
	WaitingForChallengers = 32, // FIXME
	UnkSubMenuGears2 = 33, // FIXME
	UnkSubMenuGears3 = 34, // FIXME
	CarSelect = 35,
};

enum class ControllerButton {
	Invalid,
	Left,
	Down,
	Right,
	Up,
	Square,
	Cross,
	Circle,
	Triangle,
	L1,
	R1,
	L2,
	R2,
	Start,
	Select,
	Stick1,
	Stick2,
	Back,
	Escape,
	Unk1,
	Unk2,
	Unk3,
	Unk4,
	Unk5,
	Unk6,
	Max
};

inline std::uintptr_t* g_GameProgressionManager = reinterpret_cast<std::uintptr_t*>(0x018ae0f0);
inline std::uintptr_t* g_InputPtr = reinterpret_cast<std::uintptr_t*>(0x018d3244);
inline std::uintptr_t* g_PopupCallback = reinterpret_cast<std::uintptr_t*>(0x01929b60);
inline void** g_PersistentData = reinterpret_cast<void**>(0x01926ef8);
inline void** g_Game = reinterpret_cast<void**>(0x01929bfc);

void __fastcall InitClearanceLevelData(std::uintptr_t unk) {
	auto iVar2 = FUN_00f675d0(*reinterpret_cast<void**>(0x0192c5ec), "Stat_SPY_POINTS", 1);
	auto local_c = FUN_00f66d60(*reinterpret_cast<void**>(0x0192c5ec), iVar2);
	iVar2 = FUN_00f675d0(*reinterpret_cast<void**>(0x0192c5ec), "Stat_CLEARANCE_LEVEL", 1);
	auto local_8 = FUN_00f66d60(*reinterpret_cast<void**>(0x0192c5ec), iVar2);

	int local_14 = 0, local_18 = 0;
	UnkExcelDataBase_GetUnk3(*reinterpret_cast<void**>(0x018ae110), local_8, &local_14, &local_18);
	float local_10 = static_cast<float>(local_c - local_14) / static_cast<float>(local_18 - local_14);
	if (0.0 <= local_10) {
		if (1.0 < local_10) {
			local_10 = 1.0;
		}
	}
	else {
		local_10 = 0.0;
	}
	if (static_cast<int>(local_8) < 0) {
		local_8 = 0;
	}
	else if (6 < static_cast<int>(local_8)) {
		local_8 = 6;
	}

	Flash_Movie_CallFlashFunction(*reinterpret_cast<std::uintptr_t*>(unk), "SetClearanceLevelData", 0, static_cast<double>(static_cast<int>(local_8)), static_cast<double>(local_10), static_cast<double>(local_c));
}

DefineInlineHook(SetInitialScreenState) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		void* _this = *reinterpret_cast<void**>(ctx.ebp.unsigned_integer - 0x194);
		_CarsFrontEnd_SetScreen(_this, CarsFrontEndScreen::TitleMenu, nullptr, true);
	}
};

DefineReplacementHook(CarsFrontEnd_SetScreen) {
	static void __fastcall callback(void* _this, uintptr_t edx, int unk, char* unk_name, unsigned char unk2) {
		if (unk_name != nullptr) {
			logger::log_format("[CarsFrontEnd::SetScreen] {}, {}, {}", unk, unk_name, unk2);
		}
		else {
			logger::log_format("[CarsFrontEnd::SetScreen] {}, nullptr, {}", unk, unk2);
		}
		original(_this, edx, unk, unk_name, unk2);
	}
};

DefineReplacementHook(CarsFrontEnd_GoBack) {
	static void __fastcall callback(void* _this) {
		if (IS_PC) {
			auto value = *reinterpret_cast<int*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x420) + 8);
			logger::log_format("[CarsFrontEnd::GoBack] Going back to {}...", value);
		}
		else {
			auto value = *reinterpret_cast<int*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x3FC) + 8);
			logger::log_format("[CarsFrontEnd::GoBack] Going back to {}...", value);
		}
		original(_this);
	}
};

DefineReplacementHook(RegisterGoBack) {
	static void __fastcall callback(std::uintptr_t _this) {
		GameSpecificFlashImpl* impl = reinterpret_cast<GameSpecificFlashImpl*>(_this + 4);
		GameSpecificFlashFunction* _callback = reinterpret_cast<GameSpecificFlashFunction*>((*reinterpret_cast<uintptr_t*>(_this + 0x3C)) + 0x258);
		impl->SetFlashVariableFunc("GoBack", SetFlashVariDef::ArgumentType::Null, _callback);
		original(_this);
	}
};

DefineReplacementHook(OnConfirmHook) {
	static void __fastcall callback(void* _this, std::uintptr_t edx, char* _selected_menu, std::uintptr_t unk_menu) {
		std::string selected_menu = _selected_menu;
		logger::log_format("[CarsFrontEnd::OnConfirm] {}", selected_menu);
		*(reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7CC)) += 1;
		if (std::fabs(std::fmod(static_cast<double>(*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7CC)), 3.0)) < 1E-07) {
			*(reinterpret_cast<const char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x598)) = "Click_Select";
		}
		bool should_set_screen = false;

		std::int32_t menu_state = *(reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xA8));
		switch (menu_state) {
		case ExitToWindows:
			break;

		case AutoSaveWarning:
			_CarsFrontEnd_SetScreen(_this, TitleMenu, nullptr, true);
			break;

		case TitleMenu:
			// FIXME: PC does a whole lot more with the global input driver, that we do not.
			WindowsSystemInputDriver_LockPlayerToController(*g_InputPtr, 0, 0); // g_InputPtr->LockPlayerToController(0, 0);
			// WindowsSystemInputDriver_LockPlayerToController(*g_InputPtr, 1, 1); // g_InputPtr->LockPlayerToController(1, 1);
			*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7DC) = 0; // this->controller_index[0] = 0;
			// Turns off the Globe as the game transitions to the main menu.
			_CMessageDispatcher_SendMessageToAll(*reinterpret_cast<void**>(0x01926ef0), "GlobeOff", 0, 0);
			_CarsFrontEnd_SetScreen(_this, SaveFileLoading, nullptr, true);
			*reinterpret_cast<bool*>(*g_PopupCallback + 0x10C) = true;
			break;
		case SaveSlots:
			break;

		case MainMenu_Extras:
			if (selected_menu == "FE_EX_Options") {
				_CarsFrontEnd_SetScreen(_this, Extras_Options, _selected_menu, true);
			}
			else if (selected_menu == "FE_EX_EnterCode") {
				_CarsFrontEnd_SetScreen(_this, Extras_EnterCode, _selected_menu, true);
			}
			else if (selected_menu == "FE_EX_Credits") {
				_CarsFrontEnd_SetScreen(_this, Extras_Credits, nullptr, true);
			}
			else if (selected_menu == "FE_EX_Cheats") {
				_CarsFrontEnd_SetScreen(_this, Extras_Cheats, nullptr, true);
			}
			break;

		case MT_FrontEnd:
			if (selected_menu == "FE_MM_Garage") {
				_CarsFrontEnd_SetScreen(_this, MainMenu_Garage, _selected_menu, true);
			}
			else if (selected_menu == "FE_MM_StoryMissions") {
				_CarsFrontEnd_SetScreen(_this, MainMenu_StoryMissions, _selected_menu, true);
			}
			else if (selected_menu == "FE_MM_CustomMissions") {
				_CarsFrontEnd_SetScreen(_this, MainMenu_CustomMissions, _selected_menu, true);
			}
			else if (selected_menu == "FE_MM_Badges") {
				_CarsFrontEnd_SetScreen(_this, MainMenu_Badges, _selected_menu, true);
			}
			else if (selected_menu == "FE_MM_Extras") {
				_CarsFrontEnd_SetScreen(_this, MainMenu_Extras, _selected_menu, true);
			}
			if (*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xE4) != 0) {
				InitClearanceLevelData(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xE4));
			}
			break;

		case MainMenu_StoryMissions:
		{
			if (selected_menu.size() > 15) {
				char number = selected_menu[15];
				if (number < '0' || number > '9') {
					*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424) = 0;
				}
				else {
					*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424) = number - '0';
				}
				_CarsFrontEnd_SetScreen(_this, MissionSelect, _selected_menu, true);
			}
			else {
				*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424) = 0;
				_CarsFrontEnd_SetScreen(_this, MissionSelect, _selected_menu, true);
			}
		}
		break;

		case MissionSelect:
		{
			int unk_format_var = 0;
			if (selected_menu[0] == 'S') {
				*reinterpret_cast<int*>(*g_GameProgressionManager + 0xb8) = 0;
			}
			else {
				*reinterpret_cast<int*>(*g_GameProgressionManager + 0xb8) = -1;
				unk_format_var = std::atoi(_selected_menu);
			}
			int8_t unk_story_mission_index = *reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424);
			*reinterpret_cast<char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x414) = GameProgressionManager_FormatStoryMission(*g_GameProgressionManager, unk_story_mission_index, unk_format_var);
			_CarsFrontEnd_SetScreen(_this, MissionDetails, nullptr, true);
		}
		break;

		case MissionDetails:
			should_set_screen = true;
			break;

		case MainMenu_Badges:
		{
			const char* oct_name = nullptr;
			if (selected_menu[6] == 'B') {
				_CarsFrontEnd_SetScreen(_this, BadgeMenu_Badges, _selected_menu, true);
				oct_name = "ui/badges.oct";
			}
			else if (selected_menu[6] == 'C') {
				_CarsFrontEnd_SetScreen(_this, BadgeMenu_Crests, _selected_menu, true);
				oct_name = "ui/crests.oct";
			}
			else if (selected_menu[6] == 'S') {
				_CarsFrontEnd_SetScreen(_this, BadgeMenu_Suitcases, _selected_menu, true);
				oct_name = "ui/suitcases.oct";
			}
			else {
				_CarsFrontEnd_SetScreen(_this, BadgeMenu_Badges, _selected_menu, true);
				oct_name = "ui/badges.oct";
			}
			void* engine_texture_loader = *reinterpret_cast<void**>(0x0192e194);
			if (*reinterpret_cast<void**>(reinterpret_cast<std::uintptr_t>(_this) + 0xCC) == nullptr) {
				*reinterpret_cast<void**>(reinterpret_cast<std::uintptr_t>(_this) + 0xCC) = Flash_EngineTextureLoader_LoadTextureSet(engine_texture_loader, oct_name, false, 0);
			}
		}
		break;

		case BadgeMenu_Badges:
		case BadgeMenu_Crests:
			{
				char buffer[64] = {};
				Genie::String badge_id{ _selected_menu };
				Genie::String localization_label{};
				Genie::String description_localization_label{};
				Genie::String unk4{}; // No idea what this is, seems to contain the same contents as `description_localization_label`.
				float unk5 = 0.0f;
				float unk6 = 0.0f;
				UnkExcelDataBase_GetUnk(*reinterpret_cast<void**>(0x018ae110), &badge_id, &localization_label);
				UnkExcelDataBase_GetUnk1(*reinterpret_cast<void**>(0x018ae110), &badge_id, &description_localization_label);
				UnkExcelDataBase_GetUnk2(*reinterpret_cast<void**>(0x018ae110), &badge_id, &unk4, &unk5, &unk6);
				int local_94 = static_cast<int>(std::floor(static_cast<double>(unk5 * 10.0)));
				int local_4c = static_cast<int>(std::fmod(static_cast<double>(local_94), 10.0));
				if (unk5 < unk6) {
					if (local_4c == 0) {
						sprintf_s(buffer, "%d", static_cast<int>(unk5));
					}
					else {
						float local_2a4 = unk5;
						if (unk6 - unk5 <= 0.05) {
							local_2a4 = unk6 - 0.06;
						}
						sprintf_s(buffer, "%.1f", local_2a4);
					}
				}
				else {
					sprintf_s(buffer, "%d", static_cast<int>(unk6));
				}
				char description[512] = {};
				char* description_format = CTranslator_Translate(reinterpret_cast<void*>(0x0192674c), description_localization_label.data, true);
				sprintf_s(description, description_format, static_cast<int>(unk6));
				if (menu_state == BadgeMenu_Badges) {
					unk6 = -1.0;
				}
				if (unk6 < unk5) {
					unk5 = unk6;
				}
				Flash_Movie_CallFlashFunction(unk_menu, "SetBadgeInfo", 0, localization_label.data, description, static_cast<double>(unk5), static_cast<double>(unk6), buffer);
			}
			break;

		case MainMenu_CustomMissions:
			_CarsFrontEnd_SetGameModeIndex(_this, _selected_menu);
			if (selected_menu[6] == 'S') {
				_CarsFrontEnd_SetScreen(_this, CustomSquadSeries, _selected_menu, true);
			}
			else {
				*reinterpret_cast<int*>(*g_GameProgressionManager + 0xb8) = -1;
				_CarsFrontEnd_SetScreen(_this, MT_Hunter, _selected_menu, true);
			}
			break;
		case MT_Hunter:
		case MT_Hunter2:
			if (_selected_menu[0] == 'T') {
				_CarsFrontEnd_SetLevelAndUnk(_this, _selected_menu);
				_CarsFrontEnd_UnkHandleTrackLengthType(_this, _selected_menu);
			}
			else {
				_CarsFrontEnd_SetLevelAndUnk(_this, _selected_menu);
			}

			if (menu_state == MT_Hunter2) {
				_CarsFrontEnd_SetScreen(_this, MissionSettings_Unk, nullptr, true);
			}
			else {
				int we_dont_know = GameProgressionManager_GetSomethingWeDontKnow(*g_GameProgressionManager);
				GameProgressionManager_SetSomethingWeDontKnow(*g_GameProgressionManager, we_dont_know);
				_CarsFrontEnd_SetScreen(_this, MissionSettings_Unk2, nullptr, true);
			}
			break;
		case MainMenu_CustomMissions2:
			_CarsFrontEnd_SetGameModeIndex(_this, _selected_menu);
			_CarsFrontEnd_SetScreen(_this, MT_Hunter2, nullptr, true);
			break;

		case MissionSettings_Unk:
			{
				GameProgressionManager_FUN_004e8400(*g_GameProgressionManager, false);
				GameProgressionManager_FUN_004ebaf0(*g_GameProgressionManager, *(reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x46C)));
				std::uintptr_t local_34 = *(reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x3FC));
				while (local_34 != 0 && *reinterpret_cast<int*>(local_34 + 8) != CustomSquadSeries) {
					FUN_00ef3a30(reinterpret_cast<std::uintptr_t>(_this) + 0x3E8);
					char* dest = *reinterpret_cast<char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x408);
					local_34 = *(reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x3FC));
					char* suffix = strrchr(dest, '-');
					int prefix_len = -1;
					if (suffix != nullptr) {
						prefix_len = suffix - dest;
					}
					FUN_0060e7d0(reinterpret_cast<std::uintptr_t>(_this) + 0x408, prefix_len, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x408) - 0xC) - prefix_len);
				}
				char* dest = *reinterpret_cast<char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x408);
				char* suffix = strrchr(dest, '-');
				int prefix_len = -1;
				if (suffix != nullptr) {
					prefix_len = suffix - dest;
				}
				std::uintptr_t result[10] = {};
				void* ppvVar6 = (void*)FUN_0060ee20(reinterpret_cast<std::uintptr_t>(_this) + 0x408, &result, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x408) - 0xC) - (prefix_len + 1));
				FUN_00613000(reinterpret_cast<std::uintptr_t>(_this) + 0x40C, ppvVar6);
				Genie_String_Destructor(&result);
				FUN_0060e7d0(reinterpret_cast<std::uintptr_t>(_this) + 0x408, prefix_len, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x408) - 0xC) - prefix_len);
				FUN_00ef3a30(reinterpret_cast<std::uintptr_t>(_this) + 0x3E8);
				_CarsFrontEnd_SetScreen(_this, CustomSquadSeries, "FE_MT_SquadSeries", false);
			}
			break;

		case CustomSquadSeries:
			{
				int index = std::atoi(_selected_menu);
				*(reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x46C)) = index;
				GameProgressionManager_FUN_004ebab0(*g_GameProgressionManager, index);
				_CarsFrontEnd_SetScreen(_this, MainMenu_CustomMissions2, nullptr, true);
			}
			break;

		case MissionSettings_Unk2:
			should_set_screen = true;
			GameProgressionManager_FUN_004e8400(*g_GameProgressionManager, false);
			break;

		case GarageConnect:
			if (_stricmp(_selected_menu, "DLC_Connect") == 0) {
				*(reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x800)) = 1;
			}
			break;

		case WorldOfCarsConnect:
			break;

		case MainMenu_Garage:
			if (selected_menu.starts_with("CAR_")) {
				void* dest = (reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(_this) + 0x114));
				Genie_String_Assign(dest, _selected_menu);
				_CarsFrontEnd_SetScreen(_this, GarageDetails, nullptr, true);
			}
			*(reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7A4)) = 16.0f; // FIXME
			*(reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7A8)) = 8.0f; // FIXME
			break;

		default:
			std::uintptr_t movie = *(reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xbc));
			if (movie != 0) {
				Flash_Movie_CallFlashFunction(movie, "ItemSelected", 0);
				*(reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xC0)) = 1;
			}
			break;
		}
		if (should_set_screen) {
			/*
			int locked_controllers = 0;
			for (std::size_t i = 0; i < 11; i++) {
				if (WindowsSystemInputDriver_IsControllerLocked(*g_InputPtr, i)) {
					locked_controllers += 1;
				}
			}
			PersistentData_SetGlobal(*g_PersistentData, "NumPlayers", locked_controllers);
			PersistentData_SetGlobal(*g_PersistentData, "MultiPlayer", locked_controllers > 1);
			PersistentData_SetGlobal(*g_PersistentData, "ScreenFormat", locked_controllers > 4 ? 1 : locked_controllers);
			*/
 			_CarsFrontEnd_SetScreen(_this, CarSelect, nullptr, true);
		}
		return;

	}
};

DefineReplacementHook(FlashControlMapper_ButtonPressedHook) {
	static bool __fastcall callback(void* _this, uintptr_t edx, int button, bool ignore_focus) {
		// QUICK HACK TO GET IT WORKING FOR MAXIMILIAN
		/*
		if (button == 56) {
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (button == 47) {
			if (GetAsyncKeyState(VK_UP) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (button == 48) {
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (button == 49) {
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (button == 50) {
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return original(_this, edx, button, ignore_focus);
		}
		*/
		/*
		if (button == 55) {
			if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
				return true;
			}
			else {
				return false;
			}
		}
		*/
		auto pressed = original(_this, edx, button, ignore_focus);
		if (pressed)
			logger::log_format("[Flash::FlashControlMapper::ButtonPressed] {}, {}", button, pressed);
		return pressed;
	}
};

DefineReplacementHook(SetButtonLayout) {
	static void __fastcall callback(std::uintptr_t _this, std::uintptr_t edx, int scheme) {
		logger::log_format("[Flash::FlashControlMapper::SetButtonLayout] Scheme: {}, ButtonCount: {}", scheme, *reinterpret_cast<int*>(_this + 0xA8));
		original(_this, edx, scheme);
		int* button_map = *reinterpret_cast<int**>(_this + 0xB0);
		button_map[55] = 6;
		/*
		for (std::size_t i = 0; i < *reinterpret_cast<int*>(_this + 0xA8); i++) {
			if (button_map[i] != 0)
				logger::log_format("[Flash::FlashControlMapper::SetButtonLayout] Mapped FlashButton {} -> ControllerButton {}", i, button_map[i]);
		}
		*/
	}
};

DefineReplacementHook(CallFlashVariableFuncHook) {
	static void __fastcall callback(void* _this, uintptr_t edx, struct Movie* movie, char* method, void* args) {
		logger::log_format("[GameSpecificFlashImpl::CallFlashVariableFunc] {}", method);
		original(_this, edx, movie, method, args);
	}
};

DefineReplacementHook(ExternalInterfaceHandler_Callback) {
	static void __fastcall callback(void* _this, uintptr_t edx, struct Movie* movie, char* method, void* args, unsigned int arg_count) {
		logger::log_format("[Flash::Movie::ExternalInterfaceHandler::Callback] {}", method);
		original(_this, edx, movie, method, args, arg_count);
	}
};

DefineReplacementHook(CallFlashFunction) {
	static void __fastcall callback(void* _this, uintptr_t edx, void** active_function, char* function_name, void* return_value, va_list * arg_list) {
		logger::log_format("[Flash::Movie::CallFlashFunction] {}", function_name);
		original(_this, edx, active_function, function_name, return_value, arg_list);
	}
};

DefineReplacementHook(ToggleStateFlag) {
	static void __fastcall callback(std::uintptr_t _this) {
		original(_this);
		*reinterpret_cast<bool*>(_this + 0x10C) = true;
	}
};

DefineReplacementHook(ToggleStateFlag2) {
	static void __fastcall callback(std::uintptr_t _this) {
		original(_this);
		*reinterpret_cast<bool*>(_this + 0x10C) = false;
	}
};

constexpr auto W = 3840;
constexpr auto H = 2160;

DefineInlineHook(ChangeSwapChainDesc) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0x80) = W;
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0x84) = H;
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0xB8) = W;
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0xBC) = H;
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0xF4) = W;
		*reinterpret_cast<int*>(ctx.eax.unsigned_integer + 0xF8) = H;
	}
};


std::atomic<bool> FIRST_TICK = true;
DefineInlineHook(ForceInitializeLocalPlayerOnFirstTick) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		if (FIRST_TICK) {
			*reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x10) = 1;
			FIRST_TICK = false;
		}
	}
};

DefineReplacementHook(ShouldPauseHook) {
	static bool _cdecl callback() {
		return *reinterpret_cast<bool*>(0x018743e0);
	}
};

DefineReplacementHook(SetPauseFlag0) {
	static void _fastcall callback(void* _this, std::uintptr_t edx, std::uintptr_t unk) {
		*reinterpret_cast<bool*>(0x018743e0) = false;
		original(_this, edx, unk);
	}
};

DefineReplacementHook(SetPauseFlag1) {
	static void _fastcall callback(void* _this) {
		*reinterpret_cast<bool*>(0x018743e0) = true;
		original(_this);
	}
};

DefineReplacementHook(GameCommonLoop_PauseMessage_HandleMessage) {
	static void __fastcall callback(std::uintptr_t _this, std::uintptr_t edx, int* data, std::uint32_t actor_handle, std::uint32_t posted) {
		if (*data != 1 && *data != 2) {
			if (*data == 3) {
				std::uintptr_t* inst = *reinterpret_cast<std::uintptr_t**>(_this + 0x14);
				auto func = *reinterpret_cast<void(__thiscall**)(void*, void*)>(*inst + 0x44);
				func(inst, data);
			}
			else {
				std::uintptr_t* inst = *reinterpret_cast<std::uintptr_t**>(_this + 0x14);
				auto func = *reinterpret_cast<void(__thiscall**)(void*, void*)>(*inst + 0x4C);
				func(inst, data);
			}
		}
	}
};

void __fastcall InterceptSetAutoManDrift(void* data, std::uintptr_t edx, const char* name, std::uint32_t default_value) {
	int value = PersistentData_GetGlobal(data, name);
	PersistentData_SetGlobal(data, name, (value == 1) ? 0 : 1);
}

DefineInlineHook(SetAutoManDriftLocalVarFix) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		if (*reinterpret_cast<bool*>(ctx.ebp.unsigned_integer - 0x21)) {
			ctx.edx.unsigned_integer = PersistentData_GetGlobal(*g_PersistentData, reinterpret_cast<const char*>(ctx.ebp.unsigned_integer - 0x6C));
		}
	}
};

DefineInlineHook(GetAutoManDriftFix) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		const char* label = reinterpret_cast<const char*>(ctx.ebp.unsigned_integer - 0x6C);
		if (_strnicmp(label, "AutoDrift_", 10) == 0) {
			int value = PersistentData_GetGlobal(*g_PersistentData, label);
			*reinterpret_cast<std::uint8_t*>(ctx.ebp.unsigned_integer - 0x22) = value;
		}
	}
};

DefineInlineHook(OnSaveLoaded) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		void* _this = *reinterpret_cast<void**>(ctx.ebp.unsigned_integer - 0x28);
		_CarsFrontEnd_UNK_004c3b70(_this);
		_CarsFrontEnd_SetScreen(_this, MT_FrontEnd, nullptr, false);
	}
};

struct ArcadeSetting {
	unsigned int type;
	int value;
	char string_value[80];
	unsigned int value_offset;
	bool initialized;
	char* key;
};

static_assert(sizeof(ArcadeSetting) == 0x64);

DefineInlineHook(OverrideVolumeConfig) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		ArcadeSetting* settings = reinterpret_cast<ArcadeSetting*>(ctx.eax.unsigned_integer + 0xA80);
		settings[0x15].value = 10;
		settings[0x16].value = 10;
	}
};

// #define VANILLA_ARCADE 1

extern "C" void __stdcall Pentane_Main() {
	// FIXME: link against Pentane.lib properly instead of this bullshit!!!!
	Pentane_LogUTF8 = reinterpret_cast<void(*)(PentaneCStringView*)>(GetProcAddress(GetModuleHandleA("Pentane.dll"), "Pentane_LogUTF8"));
	wchar_t exe_name[1024];
	GetModuleFileNameW(nullptr, exe_name, 1024);
	PathStripPathW(exe_name);
	if (!std::wstring_view(exe_name).contains(L"daemon")) {
		IS_PC = true;
	}
	if (IS_PC) {
		logger::log("[ArcadeEssentials::Pentane_Main] WARN: Arcade executable not detected! Assuming Cars 2: The Video Game (PC)...");
		/* DEBUGGING HOOKS START */
		// FlashControlMapper_ButtonPressedHook::install_at_ptr(0x010d6930);
		ExternalInterfaceHandler_Callback::install_at_ptr(0x010dc930);
		CallFlashFunction::install_at_ptr(0x010dae50);
		CarsFrontEnd_SetScreen::install_at_ptr(0x0048c910);
		CarsFrontEnd_GoBack::install_at_ptr(0x00489af0);
		/* DEBUGGING HOOKS END */
	}
	else {
#ifndef VANILLA_ARCADE
		// Set's ArcadeManager's initial VideoState to 16 (GAME_START), in order to force the game to skip all intro cutscenes.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004512ad), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<char*>(0x004512ad) = 16;

		// Replaces a call to CMessageDispatcher::SendMessageToAll that shuts off the Globe for a call to CarsFrontEnd::SetScreen to allow the game to boot directly to the title screen.
		sunset::inst::nop(reinterpret_cast<void*>(0x004ba81c), 19);
		SetInitialScreenState::install_at_ptr(0x004ba81c);

		static const char GLOBE_ON[] = "GlobeOn\0";
		// Removes a call to CMessageDispatcher::SendMessageToAll from a CarsFrontEnd member function that shuts off the Globe on the title screen.
		sunset::inst::nop(reinterpret_cast<void*>(0x004ba108), 19);
		// Swaps out the "GlobeOff" string for "GlobeOn" in a call to CMessageDispatcher::SendMessageToAll from within the title screen case inside CarsFrontEnd::SetScreen.
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c1c6e), reinterpret_cast<uintptr_t>(&GLOBE_ON));
		// Swaps out the "GlobeOff" string for "GlobeOn" in a call to CMessageDispatcher::SendMessageToAll.
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c39f6), reinterpret_cast<uintptr_t>(&GLOBE_ON));
		// No-ops the code for menu state UnkMenuEmpty that forces the machine into ExitToWindows.
		sunset::inst::nop(reinterpret_cast<void*>(0x004c170f), 14);

		// Registers the otherwise-missing `GoBack` callback inside CarsFrontEndFlashCallbacks::FrontendFlashFunctions::SetupFlashFunctions.
		RegisterGoBack::install_at_ptr(0x004c93e0);
		// Implements most of the logic for transitioning from screen to screen.
		OnConfirmHook::install_at_ptr(0x004be010);
		// Allows the game to transition from SaveFileLoading to MT_FrontEnd.
		sunset::inst::nop(reinterpret_cast<void*>(0x004c3b33), 0x33);
		OnSaveLoaded::install_at_ptr(0x004c3b33);
		// Forcibly maps the A/Cross button to 55, allowing menu navigation with A/Cross.
		SetButtonLayout::install_at_ptr(0x01163f70);
#endif

		/* DEBUGGING HOOKS START */
		// FlashControlMapper_ButtonPressedHook::install_at_ptr(0x01164250);
		ExternalInterfaceHandler_Callback::install_at_ptr(0x0116a080);
		CallFlashFunction::install_at_ptr(0x01168710);
		CarsFrontEnd_SetScreen::install_at_ptr(0x004c1440);
		CarsFrontEnd_GoBack::install_at_ptr(0x004be200);
		/* DEBUGGING HOOKS END */
#ifndef VANILLA_ARCADE
		// Redirects the game's WinArcadeInputDriver to the otherwise-unused WindowsSystemInputDriver (likely a leftover from the PC port).
		sunset::inst::push_u32(reinterpret_cast<void*>(0x0080cf64), 0x500); // Patch argument to operator.new
		sunset::inst::call(reinterpret_cast<void*>(0x0080cf8d), reinterpret_cast<void*>(0x00814fa0)); // Replace call to constructor
		sunset::inst::call(reinterpret_cast<void*>(0x0080da31), reinterpret_cast<void*>(0x008156f0)); // Replace call to ::Initialize member function
		// No-ops code that floods the logger with "dpad pressed" messages.
		sunset::inst::nop(reinterpret_cast<void*>(0x00814a55), 0x3D);
#endif
		// Stubs the function that otherwise triggers a a system reboot (What the actual fuck RT??)
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00458680), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x00458680) = 0xC3;
		// Disables AutoPilot
		sunset::inst::nop(reinterpret_cast<void*>(0x004f3c67), 0x2A);
		// Disables "System going down for Maintenance" message.
		sunset::inst::nop(reinterpret_cast<void*>(0x004530c9), 0xF);

#ifndef VANILLA_ARCADE
		// Fixes an issue where ending an event sent you to the attract menus.
		sunset::inst::nop(reinterpret_cast<void*>(0x004fe25d), 0x66);
		sunset::inst::nop(reinterpret_cast<void*>(0x004fe2fa), 0x20);
		// Allows the StorageStateMachine to bring the user back to race select after finishing an event.
		ToggleStateFlag::install_at_ptr(0x00e9a5c0);
		// I'll be honest, I have no idea what this does. Hopefully it fixes something!
		ToggleStateFlag2::install_at_ptr(0x00e9a770);
#endif
		// Prevents the QR code image from being generated.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00466000), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint32_t*>(0x00466000) = 0x00000CC2;
		
		ChangeSwapChainDesc::install_at_ptr(0x0083425d);

		// Force initialize the local player struct on the first call to ArcadeManager::UpdateFrontEnd by setting a local variable to 1.
		// As this expects the first tick to send the player straight to the title screen, this approach will NOT work if attract videos are re-enabled.
		ForceInitializeLocalPlayerOnFirstTick::install_at_ptr(0x0045218b);

		// Instead of reading from the pause global variable like PC does, Arcade instead... checks the lower bits of g_Game???
		// Whatever, this fixes that.
		ShouldPauseHook::install_at_ptr(0x00ecade0);
		// Patches two functions to properly update the pause flag.
		SetPauseFlag0::install_at_ptr(0x00551ab0);
		SetPauseFlag1::install_at_ptr(0x004fd0b0);

		// Allows the pause/unpause messages to properly propogate through the message dispatcher.
		GameCommonLoop_PauseMessage_HandleMessage::install_at_ptr(0x00eb66d0);

		// Forces the AutoDrift_ PersistentData globals to toggle between 1 and 0 instead of being forced to 1 no matter what.
		sunset::inst::call(reinterpret_cast<void*>(0x004cd3e6), InterceptSetAutoManDrift);
		// Updates a local variable with the last set PersistentData value for AutoDrift_.
		SetAutoManDriftLocalVarFix::install_at_ptr(0x004cd3fc);
		// Fixes an issue where the UI doesn't update until the second X press.
		GetAutoManDriftFix::install_at_ptr(0x004cd4f7);

		// Un-stubs the function responsible for initializing the clearance level data and passing it over to Flash.
		sunset::inst::jmp(reinterpret_cast<void*>(0x004b8790), InitClearanceLevelData);

		// NOTE: This might cause unintended side effects.
		// Sets the default values for both volumes to the maximum.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0045B3CE), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x0045B3CE) = 10;
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0045B3E8), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x0045B3E8) = 10;
		// Overrides the volume values read from the save files with the maximum values.
		OverrideVolumeConfig::install_at_ptr(0x00450791);

		logger::log("[ArcadeEssentials::Pentane_Main] Installed hooks!");
	}
}