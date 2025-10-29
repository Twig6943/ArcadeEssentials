#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <shlwapi.h>
#include <sunset.hpp>
#include <d3d9.h>
#include <dwmapi.h>
#include "pentane.hpp"
#include "config.hpp"
#include "Game/GameSpecificFlashImpl.hpp"
#include "Game/Genie/String.hpp"
#include "Game/GameProgressionManager.hpp"
#include "Game/Stage/StageEntity.hpp"
#include "Game/Components/ActiveMoves.hpp"
#include "Game/Components/CarsReactionMonitor.hpp"
#include "Game/Stage/Cars2VehicleDBlock.hpp"
#include "Patch/Input/KeyControllerInputDriver.hpp"
#include "Patch/Input/WindowsSystemInputDriver.hpp"
#include "Patch/Input/WindowsControllerInputDriver.hpp"
#include "Patch/Input/XInputInputDriver.hpp"
#include "Patch/OptionFlashCallbacks.hpp"

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
inline auto Flash_EngineTextureLoader_LoadTextureSet = (void* (__thiscall*)(void*, const char*, bool, int))(0x0116cd20);
inline auto CTranslator_Translate = (char*(__thiscall*)(void*, void*, bool))(0x00cf6dd0);
inline auto UnkExcelDataBase_GetUnk = (void(__thiscall*)(void*, void*, void*))(0x0052cd70);
inline auto UnkExcelDataBase_GetUnk1 = (void(__thiscall*)(void*, void*, void*))(0x0052ce80);
inline auto UnkExcelDataBase_GetUnk2 = (void(__thiscall*)(void*, void*, void*, float*, float*))(0x0052cfe0);
inline auto UnkExcelDataBase_GetUnk3 = (void(__thiscall*)(void*, std::uint32_t, void*, void*))(0x0052af10);
inline auto PersistentData_GetGlobal = (int(__thiscall*)(void*, const char*))(0x00cf0e20);
inline auto PersistentData_SetGlobal = (void(__thiscall*)(void*, const char*, std::uint32_t))(0x00cf0e40);
inline auto PersistentData_IsSetGlobal = (bool(__thiscall*)(void*, const char*))(0x00cf0e70);
inline auto FUN_00f675d0 = (std::uint32_t(__thiscall*)(void*, const char*, std::uint32_t))(0x00f675d0);
inline auto FUN_00f66d60 = (std::uint32_t(__thiscall*)(void*, std::uint32_t))(0x00f66d60);
inline auto GameCommon_SetPlayerSfxVolume = (void(__thiscall*)(void*, float, char))(0x00eb48b0);
inline auto GameCommon_SetPlayerMusicVolume = (void(__thiscall*)(void*, float, char))(0x00eb49a0);
inline auto GameCommon_SetPlayerDialogueVolume = (void(__thiscall*)(void*, float, char))(0x00eb4a90);
inline auto FUN_0116d4d0 = (void(__thiscall*)(std::uintptr_t, std::uintptr_t, const char*))(0x0116d4d0);
inline auto CSaveGame_UNK_00ee99a0 = (void(__thiscall*)(std::uintptr_t, bool))(0x00ee99a0);
inline auto CSaveGame_UNK_00eea2e0 = (void(__thiscall*)(std::uintptr_t))(0x00eea2e0);
inline auto CSaveGame_ClearLoadedData = (void(__thiscall*)(std::uintptr_t))(0x00ee9ae0);
inline auto FUN_00ba0870 = (void(__thiscall*)(std::uintptr_t, std::uintptr_t))(0x00ba0870);
inline auto FUN_0080df70 = (void(__thiscall*)(std::uintptr_t))(0x0080df70);
inline auto CMasterTimer_GetOSTime = (std::uint32_t(_cdecl*)())(0x00770280);
inline auto operator_new = (void*(_cdecl*)(std::size_t))(0x007b1650);
inline auto HudPosition_Multi_HudPosition_Multi = (void*(__thiscall*)(void*, std::uint32_t, std::uint32_t))(0x0054b220);
inline auto FUN_004d0250 = (void(__thiscall*)(std::uintptr_t, int))(0x004d0250);
inline auto _Flash_Gui_AddMovie = (void*(__thiscall*)(std::uintptr_t, const char*, char, float, float))(0x01165de0);
inline auto _Flash_Movie_SetLoadMovie = (void(__thiscall*)(void*, const char*, float, float))(0x011676e0);

inline std::uintptr_t* g_PopupCallback = reinterpret_cast<std::uintptr_t*>(0x01929b60);
inline void** g_PersistentData = reinterpret_cast<void**>(0x01926ef8);
inline void** g_Game = reinterpret_cast<void**>(0x01929bfc);
inline std::uintptr_t* g_SaveGame = reinterpret_cast<std::uintptr_t*>(0x0192b8d0);
inline std::uintptr_t* g_FlashGui = reinterpret_cast<std::uintptr_t*>(0x0192e0e4);

enum CarsFrontEndScreen {
	Invalid = 0,
	ExitToWindows = 1,
	AutoSaveWarning = 2,
	TitleMenu = 3,
	UnkSubMenuGears = 4, // FIXME
	SaveFileLoading = 5,
	SaveSlots = 6,
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
#ifdef _DEBUG
		if (unk_name != nullptr) {
			logger::log_format("[CarsFrontEnd::SetScreen] {}, {}, {}", unk, unk_name, unk2);
		}
		else {
			logger::log_format("[CarsFrontEnd::SetScreen] {}, nullptr, {}", unk, unk2);
		}
#endif
		std::int32_t menu_state = *(reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xA8));
		if (menu_state == SaveSlots) {
			std::uintptr_t* mini_menu = *(reinterpret_cast<std::uintptr_t**>(reinterpret_cast<std::uintptr_t>(_this) + 0xe4));
			if (mini_menu != nullptr) {
				if (*mini_menu != 0) {
					logger::log_format("[CarsFrontEnd::SetScreen] Removing last button.");
					Flash_Movie_CallFlashFunction(*mini_menu, "RemoveLastButton", 0);
				}
			}
		}
#ifdef MP_STRATEGY_ONLINE
		if (unk == WaitingForChallengers) {
			if ((*reinterpret_cast<std::uint32_t*>(0x018aa724) & NetFlags::Active) != 0) {
				logger::log_format("[CarsFrontEnd::SetScreen] Net is enabled!");
				if (!(*g_InputPtr)->ControllerLocked(0)) {
					logger::log_format("[CarsFrontEnd::SetScreen] Locking player 0 to controller 0...");
					(*g_InputPtr)->LockPlayerToController(0, 0);
					*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7DC) = 0;
				}
				for (auto i = 0; i < 11; i++) {
					if (std::string_view((*g_InputPtr)->controller[i]->Identify()) == "Virtual") {
						logger::log_format("[CarsFrontEnd::SetScreen] Locking player 1 to controller {}...", i);
						(*g_InputPtr)->LockPlayerToController(1, i);
						break;
					}
				}
				if (!(*g_InputPtr)->ControllerLocked(1)) {
					logger::log_format("[CarsFrontEnd::SetScreen] Virtual controller was not locked!");
				}
			}
		}
#endif
		original(_this, edx, unk, unk_name, unk2);
	}
};

DefineReplacementHook(CarsFrontEnd_GoBack) {
	static void __fastcall callback(void* _this) {
		auto value = *reinterpret_cast<int*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x3FC) + 8);
		logger::log_format("[CarsFrontEnd::GoBack] Going back to {}...", value);
		original(_this);
	}
};

DefineReplacementHook(RegisterMissingFlashFuncs) {
	static void __fastcall callback(std::uintptr_t _this) {
		GameSpecificFlashImpl* impl = reinterpret_cast<GameSpecificFlashImpl*>(_this + 4);
		GameSpecificFlashFunction* _callback = reinterpret_cast<GameSpecificFlashFunction*>((*reinterpret_cast<uintptr_t*>(_this + 0x3C)) + 0x258);
		impl->SetFlashVariableFunc("GoBack", SetFlashVariDef::ArgumentType::Null, _callback);
		_callback = reinterpret_cast<GameSpecificFlashFunction*>((*reinterpret_cast<uintptr_t*>(_this + 0x3C)) + 0x1F8);
		impl->SetFlashVariableFunc("GetSpyPoints", SetFlashVariDef::ArgumentType::Number, _callback);
		original(_this);
	}
};

int GetNumUnlockedControllers() {
	int unlocked_controller_count = 0;
	for (int i = 0; i < 11; i = i + 1) {
		void* controller = (*g_InputPtr)->GetUnlockedController(i);
		if (controller != nullptr) {
			std::uintptr_t* controller_inst = *reinterpret_cast<std::uintptr_t**>(controller);
			auto func = *reinterpret_cast<bool(__thiscall**)(void*)>(*controller_inst + 0x10);
			if (func(controller)) {
				unlocked_controller_count = unlocked_controller_count + 1;
			}
		}
	}
	return unlocked_controller_count;
}

DefineReplacementHook(OnConfirmHook) {
	static void __fastcall callback(void* _this, std::uintptr_t edx, char* _selected_menu, std::uintptr_t unk_menu) {
		std::string selected_menu = _selected_menu;
		logger::log_format("[CarsFrontEnd::OnConfirm] {}", selected_menu);
		*(reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7CC)) += 1;
		if (std::fabs(std::fmod(static_cast<double>(*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7CC)), 3.0)) < 1E-07) {
			*(reinterpret_cast<const char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x598)) = "Click_Select";
		}
		bool should_set_screen = false;
		bool should_play_item_selected = true;

		std::int32_t menu_state = *(reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xA8));
		switch (menu_state) {
		case ExitToWindows:
			*reinterpret_cast<std::uint8_t*>(g_PopupCallback + 0x131) = 1;
			if (selected_menu == "SharedText_Yes") {
				std::exit(0);
			}
			else {
				_CarsFrontEnd_SetScreen(_this, MT_FrontEnd, nullptr, false);
				std::uint32_t array[2] = { 1, 0 };
				FUN_00ba0870(reinterpret_cast<std::uintptr_t>(_this) + 0x3e8, reinterpret_cast<std::uintptr_t>(&array));
			}
			should_play_item_selected = false;
			break;
		case AutoSaveWarning:
			_CarsFrontEnd_SetScreen(_this, TitleMenu, nullptr, true);
			should_play_item_selected = false;
			break;

		case TitleMenu:
			{
			// For some god-forsaken reason, the game refuses to accept player 0 input from anything but controller 0. So we silently swap whoever pressed START at the title screen with controller 0.
			int start_controller = *_selected_menu - 0x30;
			if (start_controller != 0) {
				ControllerInputDriver* old_controller_zero = (*g_InputPtr)->controller[0];
				(*g_InputPtr)->controller[0] = (*g_InputPtr)->controller[start_controller];
				(*g_InputPtr)->controller[start_controller] = old_controller_zero;
				start_controller = 0;
			}
			// If theres no controller locked to player 0, we want to lock player 0 to whoever pressed START at the title screen (due to the above hack this should always be zero anyway).
			if (!(*g_InputPtr)->ControllerLocked(0)) {
				(*g_InputPtr)->LockPlayerToController(0, start_controller);
				*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7DC) = start_controller;
			}
			// reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xOFFSET)) = 1;
			// reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xOFFSET)) = CMasterTimer_GetOSTime();
			CSaveGame_UNK_00eea2e0(*g_SaveGame);
			CSaveGame_ClearLoadedData(*g_SaveGame);
			FUN_0080df70(*reinterpret_cast<std::uintptr_t*>(0x018d323c));
			*reinterpret_cast<bool*>(*g_PopupCallback + 0x10C) = true;
			_CarsFrontEnd_SetScreen(_this, SaveFileLoading, nullptr, true);
			*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424) = static_cast<std::uint8_t>(*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(*g_GameProgressionManager) + 0x108));
			should_play_item_selected = false;
			}
			break;
		case SaveSlots:
		{
			int index = std::atoi(_selected_menu);
			if (index < 0 || index > 2) {
				index = 0;
			}
			*reinterpret_cast<int*>(*g_SaveGame + 0x60) = index;
			if (*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x4C + index * 24) == 0) {
				CSaveGame_ClearLoadedData(*g_SaveGame);
				std::uintptr_t* g_GameSettings = *reinterpret_cast<std::uintptr_t**>(0x0192b8a8);
				auto func = *reinterpret_cast<void(__thiscall**)(std::uintptr_t*)>(*g_GameSettings + 0xC);
				func(g_GameSettings);
			}
			else {
				CSaveGame_UNK_00ee99a0(*g_SaveGame, false);
			}
			std::uintptr_t game_settings = *reinterpret_cast<std::uintptr_t*>(0x0192b8a8);
			std::uintptr_t unk = *reinterpret_cast<std::uintptr_t*>(game_settings + 0x8);
			GameCommon_SetPlayerMusicVolume(*g_Game, *reinterpret_cast<float*>(unk + 4), 0);
			GameCommon_SetPlayerSfxVolume(*g_Game, *reinterpret_cast<float*>(unk + 8), 0);
			GameCommon_SetPlayerDialogueVolume(*g_Game, *reinterpret_cast<float*>(unk + 12), 0);
			_CarsFrontEnd_SetScreen(_this, MT_FrontEnd, nullptr, true);
			should_play_item_selected = false;
		}
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
			else if (selected_menu == "FE_EX_Graphics") {
				_CarsFrontEnd_SetScreen(_this, Extras_Options, _selected_menu, true);
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
			else if (selected_menu == "FE_MM_WOC") {
				_CarsFrontEnd_SetScreen(_this, WorldOfCars, _selected_menu, true);
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
				(*g_GameProgressionManager)->unk_index = 0;
			}
			else {
				(*g_GameProgressionManager)->unk_index = -1;
				unk_format_var = std::atoi(_selected_menu);
			}
			int8_t unk_story_mission_index = *reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x424);
			*reinterpret_cast<char**>(reinterpret_cast<std::uintptr_t>(_this) + 0x414) = (*g_GameProgressionManager)->FormatStoryMission(unk_story_mission_index, unk_format_var);
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
			should_play_item_selected = false;
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
				should_play_item_selected = false;
			}
			break;

		case MainMenu_CustomMissions:
#ifdef MP_STRATEGY_ONLINE
			if ((*reinterpret_cast<std::uint32_t*>(0x018aa724) & NetFlags::Active) != 0) {
				logger::log_format("[CarsFrontEnd::OnConfirm] Net is enabled!");
				if (!(*g_InputPtr)->ControllerLocked(0)) {
					logger::log_format("[CarsFrontEnd::OnConfirm] Locking player 0 to controller 0...");
					(*g_InputPtr)->LockPlayerToController(0, 0);
					*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7DC) = 0;
				}
			}
			for (auto i = 0; i < 11; i++) {
				if (std::string_view((*g_InputPtr)->controller[i]->Identify()) == "Virtual") {
					logger::log_format("[CarsFrontEnd::OnConfirm] Locking player 1 to controller {}...", i);
					(*g_InputPtr)->LockPlayerToController(1, i);
					break;
				}
			}
			if (!(*g_InputPtr)->ControllerLocked(1)) {
				logger::log_format("[CarsFrontEnd::OnConfirm] Virtual controller was not locked!");
			}
#endif
			_CarsFrontEnd_SetGameModeIndex(_this, _selected_menu);
			if (selected_menu[6] == 'S') {
				_CarsFrontEnd_SetScreen(_this, CustomSquadSeries, _selected_menu, true);
			}
			else {
				(*g_GameProgressionManager)->unk_index = -1;
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
#ifdef MP_STRATEGY_ONLINE
			if ((*reinterpret_cast<std::uint32_t*>(0x018aa724) & NetFlags::Active) != 0) {
				should_set_screen = true;
			}
			else {
#endif
				if (menu_state == MT_Hunter2) {
					_CarsFrontEnd_SetScreen(_this, MissionSettings_Unk, nullptr, true);
				}
				else {
					int ai_car_count = GameProgressionManager_GetAICarCount(*g_GameProgressionManager);
					GameProgressionManager_SetAICarCount(*g_GameProgressionManager, ai_car_count);
					_CarsFrontEnd_SetScreen(_this, MissionSettings_Unk2, nullptr, true);
				}
#ifdef MP_STRATEGY_ONLINE
			}
#endif
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
			should_play_item_selected = false;
			break;

		case WorldOfCarsConnect:
			break;

		case MainMenu_Garage:
			if (selected_menu.starts_with("CAR_")) {
				void* dest = (reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(_this) + 0x114));
				Genie_String_Assign(dest, _selected_menu);
				_CarsFrontEnd_SetScreen(_this, GarageDetails, nullptr, true);
			}
			should_play_item_selected = false;
			*(reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7A4)) = 16.0f; // FIXME
			*(reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(_this) + 0x7A8)) = 8.0f; // FIXME
			break;

		default:
			break;
		}
		if (should_set_screen) {
#ifdef MP_STRATEGY_LOCAL
			int locked_controllers = 0;
			for (std::size_t i = 0; i < 11; i++) {
				if ((*g_InputPtr)->IsControllerLocked(i)) {
					locked_controllers += 1;
				}
			}
			PersistentData_SetGlobal(*g_PersistentData, "NumPlayers", locked_controllers);
			PersistentData_SetGlobal(*g_PersistentData, "MultiPlayer", locked_controllers > 1);
			PersistentData_SetGlobal(*g_PersistentData, "ScreenFormat", locked_controllers > 4 ? 1 : locked_controllers);
#endif
 			_CarsFrontEnd_SetScreen(_this, CarSelect, nullptr, true);
		}
		std::uintptr_t movie = *(reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xbc));
		if (movie != 0 && should_play_item_selected) {
			Flash_Movie_CallFlashFunction(movie, "ItemSelected", nullptr);
			*(reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0xC0)) = 1;
		}
		return;

	}
};

DefineReplacementHook(SetButtonLayout) {
	static void __fastcall callback(std::uintptr_t _this, std::uintptr_t edx, int scheme) {
		original(_this, edx, scheme);
		int* button_map = *reinterpret_cast<int**>(_this + 0xB0);
		button_map[55] = 6;
	}
};

DefineReplacementHook(CallFlashVariableFuncHook) {
	static void __fastcall callback(void* _this, uintptr_t edx, struct Movie* movie, char* method, void* args) {
		logger::log_format("[GameSpecificFlashImpl::CallFlashVariableFunc] {}", method);
		original(_this, edx, movie, method, args);
	}
};

static bool IN_APPLY_SETTINGS = false;

DefineReplacementHook(ExternalInterfaceHandler_Callback) {
	static void __fastcall callback(void* _this, uintptr_t edx, struct Movie* movie, char* method, void* args, unsigned int arg_count) {
#ifdef _DEBUG
		logger::log_format("[Flash::Movie::ExternalInterfaceHandler::Callback] {}", method);
#endif
		if (method != nullptr) {
			if (IN_APPLY_SETTINGS && _stricmp(method, "GetMenuOptionsList") == 0) {
				HandleGetMenuOptionsList(movie);
				return;
			}
			if (IN_APPLY_SETTINGS && _stricmp(method, "SelectOption") == 0) {
				HandleSelectOption(movie);
				IN_APPLY_SETTINGS = false;
				return;
			}
			if (_stricmp(method, "GetAllResolutions") == 0) {
				HandleGetAllResolutions(movie);
				return;
			}
			else if (_stricmp(method, "GetCurrResolution") == 0) {
				HandleGetCurrResolution(movie);
				return;
			}
			else if (_stricmp(method, "GetCurrGraphicType") == 0) {
				HandleGetCurrGraphicType(movie);
				return;
			}
			else if (_stricmp(method, "SetCurrGraphicType") == 0) {
				HandleSetCurrGraphicType(movie, std::bit_cast<float>(reinterpret_cast<unsigned int*>(args)[2]));
				return;
			}
			else if (_stricmp(method, "PopupMessage") == 0) {
				HandlePopupMessage(movie);
				IN_APPLY_SETTINGS = true;
				return;
			}
			else if (_stricmp(method, "SetCurrResolution") == 0) {
				HandleSetCurrResolution(movie, std::bit_cast<float>(reinterpret_cast<unsigned int*>(args)[2]));
				return;
			}
		}
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
		if (*reinterpret_cast<bool*>((*reinterpret_cast<std::uintptr_t*>(0x018d3470)) + 0x7C)) {
			_CarsFrontEnd_SetScreen(_this, SaveSlots, nullptr, false);
		}
		else {
			_CarsFrontEnd_SetScreen(_this, MT_FrontEnd, nullptr, false);
		}
	}
};

DefineInlineHook(OverrideDefaultVolume) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<std::uint32_t*>(ctx.ecx.unsigned_integer + 0x4) = 0x3F800000;
		*reinterpret_cast<std::uint32_t*>(ctx.ecx.unsigned_integer + 0x8) = 0x3F800000;
		*reinterpret_cast<std::uint32_t*>(ctx.ecx.unsigned_integer + 0xC) = 0x3F800000;
	}
};

DefineInlineHook(FixRestartVolume) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		// Fix the stack pointer since we nop'd a call to a __thiscall function.
		ctx.esp.signed_integer += 0xc;
		*reinterpret_cast<std::uint32_t*>(ctx.ebp.unsigned_integer - 0x4) = 0x3F800000;
	}
};

DefineReplacementHook(HandleSpinOut) {
	static void __fastcall callback(std::uintptr_t _this, std::uintptr_t edx, std::uint32_t a, std::uint32_t b, std::uint32_t c) {
		CarsVehicle* vehicle = *reinterpret_cast<CarsVehicle**>(_this + 4);
		*reinterpret_cast<std::uint8_t*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(vehicle) + 0x1470) + 0x1BC) = 1;
		if (vehicle->GetActiveMoves() != nullptr) {
			bool should_set = false;
			if (vehicle->GetActiveMoves()->m_actionState == ActiveMoves::ActionState::TwoWheelLeft || vehicle->GetActiveMoves()->m_actionState == ActiveMoves::ActionState::TwoWheelRight) {
				should_set = true;
			}
			if (should_set) {
				vehicle->GetActiveMoves()->SetTwoWheelingLeft(false);
				vehicle->GetActiveMoves()->SetTwoWheelingRight(false);
			}
		}
	}
};

DefineReplacementHook(LockInBro) {
	static bool __fastcall callback(CarsVehicle* _this, std::uintptr_t edx, bool param_1) {
		bool bVar1 = false;
		if (!_this->GetInTheZone()) {
			if (_this->GetActiveMoves() == nullptr) {
				bVar1 = false;
			}
			else {
				if (!param_1) {
					if (_this->GetActiveMoves()->m_turboing == false) {
						return false;
					}
					if (_this->GetActiveMoves()->m_turboTime > 0.5) {
						return false;
					}
				}
				float local_8 = *reinterpret_cast<float*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x117C) + 0x58);
				if (!param_1) {
					local_8 = local_8 - *reinterpret_cast<float*>(*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_this) + 0x117C) + 0x54);
				}
				if (local_8 <= _this->GetCarEnergy().m_energy) {
					bVar1 = _this->GoInTheZone(false, false);
				}
				else {
					bVar1 = false;
				}
			}
		}
		else {
			bVar1 = false;
		}
		return bVar1;
	}
};

DefineReplacementHook(SideBashHandler) {
	static bool __fastcall callback(ActiveMoves* _this, std::uintptr_t edx, ActorHandle victim, bool bash_tie) {
		logger::log_format("[ActiveMoves::TriggerSideBashReactions] Attempting to handle sidebash reaction...");
		TriggerSideBashReactions(_this, edx, victim, bash_tie);
		return true;
	}
};

DefineReplacementHook(StartInvulnHook) {
	static void __fastcall callback(std::uintptr_t _this) {
		if (!*reinterpret_cast<bool*>(_this + 0x170)) {
			*reinterpret_cast<std::uint32_t*>(_this + 0x260) = 0x40200000;
		}
		else {
			*reinterpret_cast<bool*>(_this + 0x170) = 0;
		}
	}
};

DefineInlineHook(BeginFrameHook) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<bool*>(ctx.edx.unsigned_integer + 0x170) = false;
	}
};

DefineReplacementHook(FixRevITUI) {
	static void __fastcall callback(std::uintptr_t _this) {
		if (*reinterpret_cast<std::int32_t*>(_this + 0x40) == 0) {
			FUN_0116d4d0(*reinterpret_cast<std::uintptr_t*>(0x0192e19c), _this + 0x8cc, "to_energyMeter");
		}
		else {
			FUN_0116d4d0(*reinterpret_cast<std::uintptr_t*>(0x0192e19c), _this + 0x28, "init");
		}
		*reinterpret_cast<std::uint32_t*>(_this + 0x40) = 1;
	}
};

DefineInlineHook(InitHudElementsRace) {
	static void _cdecl callback(sunset::InlineCtx& ctx) {
		GameProgressionManager::MissionMode mode = *reinterpret_cast<GameProgressionManager::MissionMode*>(ctx.ebp.unsigned_integer - 0x2c);
		int* local_8 = reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x4);

#ifdef MP_STRATEGY_LOCAL
		int playerCount = GetPlayerCount();
#else
		int playerCount = 1;
#endif

		// assert(mode == GameProgressionManager::MissionMode::Race);

		// 1 Player -> 2, 2 Player -> 3, Else -> 4.
		if (playerCount == 1) {
			*local_8 = 0;
			*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 2;
		}
		else if (playerCount == 2) {
			*local_8 = 0;
			*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 3;
		}
		else {
			*local_8 = 0;
			*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 4;
		}
	}
};

DefineInlineHook(InitHudElements) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		GameProgressionManager::MissionMode mode = *reinterpret_cast<GameProgressionManager::MissionMode*>(ctx.ebp.unsigned_integer - 0x2c);
		int* local_8 = reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x4);

#ifdef MP_STRATEGY_LOCAL
		int playerCount = GetPlayerCount();
#else
		int playerCount = 1;
#endif

		if (mode == GameProgressionManager::MissionMode::Bomb || mode == GameProgressionManager::MissionMode::Hunter || mode == GameProgressionManager::MissionMode::Arena) {
			*local_8 = 7;
			// 1 Player -> 15, 2 Player -> 16, Else -> 17
			if (playerCount == 1) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 15;
			}
			else if (playerCount == 2) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 16;
			}
			else {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 17;
			}
		}
		else if (mode == GameProgressionManager::MissionMode::Collect) {
			*local_8 = 4;
			// 1 Player -> 10, 2 Player -> 11, Else -> 12
			if (playerCount == 1) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 10;
			}
			else if (playerCount == 2) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 11;
			}
			else {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 12;
			}
		}
		else if (mode == GameProgressionManager::MissionMode::Takedown) {
			*local_8 = 3;
			// 1 Player -> 5, Else -> 6
			if (playerCount == 1) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 5;
			}
			else {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 6;
			}
		}
		else if (mode == GameProgressionManager::MissionMode::Tutorial) {
			*local_8 = 1;
			// 1 Player -> 7, Else -> 8
			if (playerCount == 1) {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 7;
			}
			else {
				*reinterpret_cast<int*>(ctx.esp.unsigned_integer) = 8;
			}
		}
	}
};

DefineReplacementHook(SetVolumeLogger) {
	static std::uint32_t __fastcall callback(std::uintptr_t ecx, std::uintptr_t edx, int group, float unk1, float unk2) {
		logger::log_format("[AudioEventManager::SetVolume] {}, {}, {}", group, unk1, unk2);
		return original(ecx, edx, group, unk1, unk2);
	}
};

DefineInlineHook(SupplyFlashArgs) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<std::uintptr_t*>(ctx.esp.unsigned_integer) = ctx.ebp.unsigned_integer - 0xE8;
	}
};

DefineInlineHook(SupplyFlashArgs2) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<std::uintptr_t*>(ctx.esp.unsigned_integer) = ctx.ebp.unsigned_integer - 0x30;
	}
};

DefineInlineHook(FixTurboUI) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		std::uintptr_t _this = *reinterpret_cast<std::uintptr_t*>(ctx.ebp.unsigned_integer - 0xCC);
		int playerNumber = *reinterpret_cast<int*>(_this + 0x194);

		std::uintptr_t* inst = *reinterpret_cast<std::uintptr_t**>(ctx.ebp.unsigned_integer - 0x18);
		Genie::String* buttonTurbo = reinterpret_cast<Genie::String*>(ctx.ebp.unsigned_integer - 0x3C);
		// buttonTurbo->Append(SuffixForCT((*g_InputPtr)->GetController(playerNumber)->m_uiType).data());

		auto func = *reinterpret_cast<void(__thiscall**)(void*, char*, const char*, const char*)>(*inst + 0x50);
		func(inst, buttonTurbo->data, "turbo", "normal");
	}
};

DefineInlineHook(SRandHook) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		ctx.eax.unsigned_integer = 0;
	}
};

int _cdecl RandHook() {
	return 0x3FFF;
}

DefineInlineHook(HandleHudPositionMulti) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		if ((ctx.eax.unsigned_integer & 0x10000) != 0) {
			void* _inst = operator_new(0x74);
			HudPosition_Multi_HudPosition_Multi(_inst, ctx.edx.unsigned_integer + 0x5C, *reinterpret_cast<std::uint32_t*>(ctx.edx.unsigned_integer + 0x9c));
			*reinterpret_cast<void**>(ctx.edx.unsigned_integer + 0x34) = _inst;
		}
	}
};

// https://stackoverflow.com/questions/36543301/detecting-windows-10-version
RTL_OSVERSIONINFOW windows_version() {
	HMODULE ntdll = ::GetModuleHandleW(L"ntdll.dll");
	if (ntdll != nullptr) {
		auto rtl_get_version = reinterpret_cast<NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW)>(GetProcAddress(ntdll, "RtlGetVersion"));
		if (rtl_get_version != nullptr) {
			RTL_OSVERSIONINFOW info = { 0 };
			info.dwOSVersionInfoSize = sizeof(info);
			if (rtl_get_version(&info) == 0) {
				return info;
			}
		}
	}
	return RTL_OSVERSIONINFOW{};
}

// https://github.com/hedge-dev/UnleashedRecomp/blob/main/UnleashedRecomp/ui/game_window.cpp
DefineInlineHook(DarkModeWindowTitle) {
	static void _cdecl callback(sunset::InlineCtx& ctx) {
		RTL_OSVERSIONINFOW version = windows_version();
		if (version.dwMajorVersion >= 10 && version.dwBuildNumber >= 17763) {
			const std::uint32_t use_immersive_dark_mode = 1;
			// DWMWA_USE_IMMERSIVE_DARK_MODE
			DwmSetWindowAttribute(*reinterpret_cast<HWND*>(ctx.edx.unsigned_integer + 0x4), version.dwBuildNumber >= 18985 ? 20 : 19, &use_immersive_dark_mode, sizeof(use_immersive_dark_mode));
		}
	}
};

DefineInlineHook(CheckForExitToWindows) {
	static void _cdecl callback(sunset::InlineCtx& ctx) {
		const char* _str = *reinterpret_cast<const char**>(ctx.ebp.unsigned_integer - 0x4);
		if (_stricmp(_str, "Win32Wii_Scn_ExitToWindows") == 0) {
			FUN_004d0250(*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(ctx.ebp.unsigned_integer - 0xC8) + 4), 2);
		}
	}
};

DefineInlineHook(CheckForExitToWindows2) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		int selected_option = *reinterpret_cast<int*>(*reinterpret_cast<std::uintptr_t*>(ctx.edx.unsigned_integer + 4) + 0x18);
		if (selected_option == 2) {
			std::exit(0);
		}
	}
};

DefineInlineHook(CheckForGraphicsMenu) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		std::uintptr_t _this = *reinterpret_cast<std::uintptr_t*>(ctx.ebp.unsigned_integer - 0x2F4);
		const char* _selected_menu = *reinterpret_cast<const char**>(_this + 0x40C);
		if (_selected_menu != nullptr) {
			if (_stricmp(_selected_menu, "FE_EX_Graphics") == 0) {
				*reinterpret_cast<const char**>(ctx.esp.unsigned_integer) = "Win32Wii_Scn_Item_Resolution,Win32Wii_Scn_GraphicQuality";
			}
		}
	}
};

DefineInlineHook(CheckForGraphicsMenu2) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		const char* _selected_menu = *reinterpret_cast<const char**>(ctx.eax.unsigned_integer + 0x40C);
		if (_selected_menu != nullptr) {
			if (_stricmp(_selected_menu, "FE_EX_Graphics") == 0) {
				*reinterpret_cast<const char**>(ctx.esp.unsigned_integer) = "text,text";
			}
		}
	}
};

DefineInlineHook(CheckForGraphicsMenu3) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		std::uintptr_t _this = *reinterpret_cast<std::uintptr_t*>(ctx.ebp.unsigned_integer + -0x184);
		const char* _selected_menu = *reinterpret_cast<const char**>(_this + 0x40C);
		if (_selected_menu != nullptr) {
			if (_stricmp(_selected_menu, "FE_EX_Graphics") == 0) {
				*reinterpret_cast<const char**>(ctx.ebp.unsigned_integer + -0x14) = "pc_graphic_setting";
			}
		}
	}
};

DefineInlineHook(GetTypeCCI) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		char* screen_type = *reinterpret_cast<char**>(ctx.ebp.unsigned_integer + 0xC);
		if (_stricmp(screen_type, "FS") == 0) {
			*reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x10) = 0;
		}
		else if (_stricmp(screen_type, "HH") == 0) {
			*reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x10) = 1;
		}
		else if (_stricmp(screen_type, "HHHW") == 0) {
			*reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x10) = 2;
		}
		else {
			*reinterpret_cast<int*>(ctx.ebp.unsigned_integer - 0x10) = 0;
		}
	}
};

DefineInlineHook(FixDefaultDistance) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		*reinterpret_cast<std::uint32_t*>(ctx.eax.unsigned_integer + 0x260) = *reinterpret_cast<std::uint32_t*>(ctx.ebp.unsigned_integer - 0x24);
	}
};

DefineReplacementHook(FlashGuiImpRender) {
	static void _fastcall callback(std::uintptr_t _this, std::uintptr_t edx, void* pMovieView, void* pTexture, void* movieInfo, float x, float y, float w, float h) {
		auto* device = *reinterpret_cast<IDirect3DDevice9Ex**>(0x01906334);
		// Backup the SrgbTexture value for sampler 0.
		unsigned long srgb_sampler_state_backup = 0;
		device->GetSamplerState(0, D3DSAMP_SRGBTEXTURE, &srgb_sampler_state_backup);
		// Force SrgbTexture for sampler 0 to FALSE before rendering the UI.
		device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
		original(_this, edx, pMovieView, pTexture, movieInfo, x, y, w, h);
		// Restore sampler 0 back to its original state.
		device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, srgb_sampler_state_backup);
	}
};

struct ScaleformViewport {
	int bufferWidth;
	int bufferHeight;
	int left;
	int top;
	int width;
	int height;
	int scissorLeft;
	int scissorRight;
	int scissorWidth;
	int scissorHeight;
	float _unk;
	float _unk2;
	int flags;
};

static_assert(sizeof(ScaleformViewport) == 0x34);

DefineInlineHook(AdjustScaleformViewport) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		ScaleformViewport* view = reinterpret_cast<ScaleformViewport*>(ctx.ecx.unsigned_integer);
		if (view->bufferHeight > 720.0) {
			view->_unk = 720.0f / static_cast<float>(view->bufferHeight);
		}
	}
};

DefineReplacementHook(CarsVehicleSetSteer) {
	static void __fastcall callback(CarsVehicle * _this, std::uintptr_t edx, float steer) {
		bool isBwd = false;
		Cars2VehicleDBlock* dBlock = Cars2VehicleDBlock::Get(*_this->actor);
		if (dBlock != nullptr) {
			if (dBlock->m_activeMoves != nullptr) {
				// If the car is backwards driving, and is player-controlled, invert the steering input.
				isBwd = dBlock->m_activeMoves->m_actionState == ActiveMoves::ActionState::BackwardsDriving && dBlock->m_playerNum != -1;
			}
		}
		*reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(_this) + 0x8e8) = isBwd ? -steer : steer;
	}
};

DefineReplacementHook(MiniMenuAddButton) {
	static void __fastcall callback(std::uintptr_t* _this, std::uintptr_t edx, const char* icon_key, const char* label_key) {
		if (*_this != 0) {
			logger::log_format("[MiniMenu::AddButton] Added button: {}, {}.", icon_key, label_key);
			Flash_Movie_CallFlashFunction(*_this, "AddButton", 0, icon_key, label_key);
		}
	}
};

DefineReplacementHook(MiniMenuShowTite) {
	static void __fastcall callback(std::uintptr_t * _this) {
		if (*_this != 0) {
			Flash_Movie_CallFlashFunction(*_this, "ShowTitle", 0);
		}
	}
};

DefineReplacementHook(MiniMenuResetButtons) {
	static void __fastcall callback(std::uintptr_t * _this) {
		if (*_this != 0) {
			logger::log_format("[MiniMenu::ResetButtons] Removed all buttons.");
			logger::log_format("[MiniMenu::ResetButtons] Added `menu_button_main`, `Scn_Select`.");
			logger::log_format("[MiniMenu::ResetButtons] Added `menu_button_main-cancel`, `SharedText_Back`.");

			Flash_Movie_CallFlashFunction(*_this, "RemoveAllButtons", 0);
			Flash_Movie_CallFlashFunction(*_this, "AddButton", 0, "menu_button_main", "Scn_Select");
			Flash_Movie_CallFlashFunction(*_this, "AddButton", 0, "menu_button_back-cancel", "SharedText_Back");
		}
	}
};

DefineInlineHook(AddButtons) {
	static void _cdecl callback(sunset::InlineCtx & ctx) {
		std::uintptr_t movie = **reinterpret_cast<std::uintptr_t**>(ctx.ebp.unsigned_integer - 0xc);
		Flash_Movie_CallFlashFunction(movie, "AddButton", 0, "menu_button_main", "Scn_Select");
		Flash_Movie_CallFlashFunction(movie, "AddButton", 0, "menu_button_back-cancel", "SharedText_Back");
	}
};

#ifndef MP_STRATEGY_ONLINE
#define DISABLE_ATTRACT
#endif

extern "C" void __stdcall Pentane_Main() {
	// FIXME: link against Pentane.lib properly instead of this bullshit!!!!
	Pentane_LogUTF8 = reinterpret_cast<void(*)(PentaneCStringView*)>(GetProcAddress(GetModuleHandleA("Pentane.dll"), "Pentane_LogUTF8"));
	Pentane_IsWindowedModeEnabled = reinterpret_cast<int(*)()>(GetProcAddress(GetModuleHandleA("Pentane.dll"), "Pentane_IsWindowedModeEnabled"));

	// Make sure we're attached to Arcade, not any other game.
	const auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr)) + reinterpret_cast<IMAGE_DOS_HEADER*>(GetModuleHandleW(nullptr))->e_lfanew);
	if (nt_header->FileHeader.TimeDateStamp != 0x521E2EAF) {
		logger::log("[ArcadeEssentials::Pentane_Main] `ArcadeEssentials` is not compatble with Cars 2: The Video Game (PC)!");
	}
	else {
		if (!GLOBAL_CONFIG->read()) {
			logger::log_format("[ArcadeEssentials::AEConfig::read] `settings.toml` is missing! VSync will be enabled, and the game window will attempt to match 1280x720.");
			GLOBAL_CONFIG->finalize();
		}
		if (GLOBAL_CONFIG->vsync) {
			// Set the presentation interval to D3DPRESENT_INTERVAL_ONE if vertical sync is enabled in `settings.toml`.
			sunset::utils::set_permission(reinterpret_cast<void*>(0x008340f7 + 6), 4, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint32_t*>(0x008340f7 + 6) = D3DPRESENT_INTERVAL_ONE;
		}
		if (Pentane_IsWindowedModeEnabled() == 0) {
 			// Set the window dimensions to whatever the user set in `settings.toml`, if we're running in exclusive fullscreen.
			sunset::utils::set_permission(reinterpret_cast<void*>(0x00d70b51), 4, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint32_t*>(0x00d70b51) = static_cast<std::uint32_t>(GLOBAL_CONFIG->window_width);
			sunset::utils::set_permission(reinterpret_cast<void*>(0x00d70b58), 4, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint32_t*>(0x00d70b58) = static_cast<std::uint32_t>(GLOBAL_CONFIG->window_height);
		}

		/* DEBUGGING HOOKS START */
#ifdef _DEBUG
		// CallFlashFunction::install_at_ptr(0x01168710);
		// CarsFrontEnd_GoBack::install_at_ptr(0x004be200);
		// SetVolumeLogger::install_at_ptr(0x007e0c30);
		// init_message_logger_arcade();
#endif
		init_testing();
		/* DEBUGGING HOOKS END */

		// Change the window title from Octane2 Renderer Window -> Cars 2: Arcade
		char window_title[] = "Cars 2: Arcade";
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0161dd9c), sizeof(window_title), sunset::utils::Perm::ReadWrite);
		std::memcpy(reinterpret_cast<void*>(0x0161dd9c), window_title, sizeof(window_title));

		// Allows the window's title bar to properly respond to system-wide Dark Mode.
		DarkModeWindowTitle::install_at_ptr(0x00834b08);

#ifdef DISABLE_ATTRACT
		// Set's ArcadeManager's initial VideoState to 16 (GAME_START), in order to force the game to skip all intro cutscenes.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004512ad), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<char*>(0x004512ad) = 16;

		// Prevents the game from sending you to the attract videos at the title screen.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004bc2c5), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<char*>(0x004bc2c5) = 0xEB;
#endif
		// Kills the 16s timer for the car select and track select screens.
		sunset::inst::nop(reinterpret_cast<void*>(0x004bc7d4), 4);
		sunset::inst::nop(reinterpret_cast<void*>(0x004bc4f3), 4);
		sunset::inst::nop(reinterpret_cast<void*>(0x004bbbfe), 4);
		sunset::inst::nop(reinterpret_cast<void*>(0x004bbd1f), 8);

#ifdef DISABLE_ATTRACT
		// Replaces a call to CMessageDispatcher::SendMessageToAll that shuts off the Globe for a call to CarsFrontEnd::SetScreen to allow the game to boot directly to the title screen.
		sunset::inst::nop(reinterpret_cast<void*>(0x004ba81c), 19);
		SetInitialScreenState::install_at_ptr(0x004ba81c);
#else
		// If net is disabled, send the user to the title screen after exiting attract.
		sunset::inst::push_u8(reinterpret_cast<void*>(0x004bb6a1), TitleMenu);
		// Otherwise, send them to mission select.
		sunset::inst::push_u8(reinterpret_cast<void*>(0x004bb67e), MainMenu_CustomMissions);
		sunset::inst::push_u8(reinterpret_cast<void*>(0x004bbe40), MainMenu_CustomMissions);
#endif
		static const char GLOBE_ON[] = "GlobeOn\0";
		// Removes a call to CMessageDispatcher::SendMessageToAll from a CarsFrontEnd member function that shuts off the Globe on the title screen.
		sunset::inst::nop(reinterpret_cast<void*>(0x004ba108), 19);
		// Swaps out the "GlobeOff" string for "GlobeOn" in a call to CMessageDispatcher::SendMessageToAll from within the title screen case inside CarsFrontEnd::SetScreen.
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c1c6e), reinterpret_cast<uintptr_t>(&GLOBE_ON));
		// Swaps out the "GlobeOff" string for "GlobeOn" in a call to CMessageDispatcher::SendMessageToAll.
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c39f6), reinterpret_cast<uintptr_t>(&GLOBE_ON));
		// No-ops the code for menu state UnkMenuEmpty that forces the machine into ExitToWindows.
		sunset::inst::nop(reinterpret_cast<void*>(0x004c170f), 14);

		// Registers the otherwise-missing `GoBack` and `GetSpyPoints` callbacks inside CarsFrontEndFlashCallbacks::FrontendFlashFunctions::SetupFlashFunctions.
		RegisterMissingFlashFuncs::install_at_ptr(0x004c93e0);
		
		// Implements most of the logic for transitioning from screen to screen.
		OnConfirmHook::install_at_ptr(0x004be010);
		
		// Allows the game to transition from SaveFileLoading to MT_FrontEnd, as well as reset the volume to the user-configured values.
		sunset::inst::nop(reinterpret_cast<void*>(0x004c3b33), 0x33);
		OnSaveLoaded::install_at_ptr(0x004c3b33);
		
		// Forcibly maps the A/Cross button to 55, allowing menu navigation with A/Cross.
		SetButtonLayout::install_at_ptr(0x01163f70);

		// Redirects the game's WinArcadeInputDriver to the otherwise-unused WindowsSystemInputDriver.
		sunset::inst::push_u32(reinterpret_cast<void*>(0x0080cf64), sizeof(WindowsSystemInputDriver)); // Patch argument to operator.new
		sunset::inst::call(reinterpret_cast<void*>(0x0080cf8d), reinterpret_cast<void*>(0x00814fa0)); // Replace call to constructor
		sunset::inst::call(reinterpret_cast<void*>(0x0080da31), WindowsSystemInputDriver_Initialize); // Replace call to ::Initialize member function

		// Stubs the function that otherwise triggers a a system reboot (What the actual fuck RT??)
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00458680), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x00458680) = 0xC3;

		// Stubs the function that schedules a maintenance reboot when the framerate dips below 30.
		// (looking at u max)
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00455650), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x00455650) = 0xC3;
			
		// Disables AutoPilot
		sunset::inst::nop(reinterpret_cast<void*>(0x004f3c67), 0x2A);
		
		// Disables "System going down for Maintenance" message.
		sunset::inst::nop(reinterpret_cast<void*>(0x004530c9), 0xF);

		// Fixes an issue where ending an event sent you to the attract menus.
		sunset::inst::nop(reinterpret_cast<void*>(0x004fe25d), 0x66);
		sunset::inst::nop(reinterpret_cast<void*>(0x004fe2fa), 0x20);
		// Allows the StorageStateMachine to bring the user back to race select after finishing an event.
		ToggleStateFlag::install_at_ptr(0x00e9a5c0);
		// I'll be honest, I have no idea what this does. Hopefully it fixes something!
		ToggleStateFlag2::install_at_ptr(0x00e9a770);

		// Prevents the QR code image from being generated.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00466000), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint32_t*>(0x00466000) = 0x00000CC2;
#ifdef DISABLE_ATTRACT
		// Force initialize the local player struct on the first call to ArcadeManager::UpdateFrontEnd by setting a local variable to 1.
		// As this expects the first tick to send the player straight to the title screen, this approach will NOT work if attract videos are re-enabled.
		ForceInitializeLocalPlayerOnFirstTick::install_at_ptr(0x0045218b);
#endif
		// Instead of reading from the pause global variable like PC does, Arcade instead... checks the lower bits of g_Game? Whatever, this fixes that.
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

		// Prevents the game from using ArcadeData's master volume on restarting an event.
		sunset::inst::nop(reinterpret_cast<void*>(0x004f75a7), 5);
		FixRestartVolume::install_at_ptr(0x004f75a7);
		// Completely stub ArcadeManager::SetVolume.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004515e0), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint32_t*>(0x004515e0) = 0x900004C2;
		// Overrides the default volumes used when there is no save file with the maximum values.
		OverrideDefaultVolume::install_at_ptr(0x00e9b89b);

		// Restores the ability for cars to respond to being sidebashed.
		SideBashHandler::install_at_ptr(0x006fac40);

		// Allows the user to backwards-drive for as long as they want.
		sunset::inst::nop(reinterpret_cast<void*>(0x006f73eb), 4);
		// Prevents RaceManager from trying to inject ControllerButton::Triangle presses in the input driver.
		sunset::inst::nop(reinterpret_cast<void*>(0x004f3ecf), 2);
		
		// Prevents events from ending once the timer hits 180s.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004F3B79), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x004F3B79) = 0;

		// Restores the ability for cars to spin out.
		HandleSpinOut::install_at_ptr(0x006c3740);
		sunset::inst::jmp(reinterpret_cast<void*>(0x006d73a0), HandleWipeout);
		sunset::inst::jmp(reinterpret_cast<void*>(0x006d73b0), HandleCommenceWipeout);
		
		// Brings the ITZ initialization logic closer to the original game.
		LockInBro::install_at_ptr(0x006b3320);
		// Prevents the game from forcing ITZ on every turbo.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x006F96B2), 5, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint32_t*>(0x006F96B2) = 0x9004C483;
		*reinterpret_cast<std::uint8_t*>(0x006F96B6) = 0x90;
		sunset::inst::nop(reinterpret_cast<void**>(0x006f96bc), 6);

		// Fixes an issue where lemon cars would remain indefinetly `Invulnerable`.
		BeginFrameHook::install_at_ptr(0x006d432c);
		StartInvulnHook::install_at_ptr(0x006d5640);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x006d3898), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x006d3898) = 0;
		
		// Fixes an issue where non-race mission modes would always fall back to the same UI.
		InitHudElements::install_at_ptr(0x0055114c);
		InitHudElementsRace::install_at_ptr(0x00551054);
		sunset::inst::nop(reinterpret_cast<void*>(0x0055101b), 2);

		// Fixes the Rev-It! icon meter dissappearing after an event starts.
		FixRevITUI::install_at_ptr(0x00547ca0);
		static const char DISPLAY_BUTTON_PROMPT[] = "DisplayButtonPrompt";
		sunset::inst::push_u32(reinterpret_cast<void*>(0x0054528b), reinterpret_cast<std::uintptr_t>(&DISPLAY_BUTTON_PROMPT));
		sunset::inst::push_u32(reinterpret_cast<void*>(0x005454f6), reinterpret_cast<std::uintptr_t>(&DISPLAY_BUTTON_PROMPT));
		SupplyFlashArgs::install_at_ptr(0x0054528b);
		SupplyFlashArgs2::install_at_ptr(0x005454f6);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00545288), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x00545288) = 3;
		sunset::utils::set_permission(reinterpret_cast<void*>(0x005454f3), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x005454f3) = 3;
		// Restores the TURBO meter/icon.
		FixTurboUI::install_at_ptr(0x0068fb5b);

		// Removes a premature `jmp` that otherwise ranks AI sidestep actions as 0.0f no matter what. (WTF Microsoft? How is this code still there past the return? Whatever, thanks I guess...)
		sunset::inst::nop(reinterpret_cast<void*>(0x004215c9), 11);

#if defined(_DEBUG) || defined(MP_STRATEGY_ONLINE)
		// Sets the random seed to zero instead of the system time.
		SRandHook::install_at_ptr(0x00450c48);
		// Forces `std::rand` to always return 0x3FFF.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x01591318), sizeof(void*), sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<void**>(0x01591318) = RandHook;
#endif
		// Fixes an issue where the game would not correctly handle the `FlashMovieMultiInputEnabled` flash function.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0116A31B), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x0116A31B) = 1;

		sunset::utils::set_permission(reinterpret_cast<void*>(0x004bc9c7), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x004bc9c7) = 0xFF;

		sunset::utils::set_permission(reinterpret_cast<void*>(0x004b8772), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x004b8772) = 0;

		// Modifies the input driver to continously search for controllers and handle instances where a controller drops/reconnects.
		sunset::inst::jmp(reinterpret_cast<void*>(0x00815fb0), WindowsSystemInputDriver_BeginInput);

		// Fixes an issue where Arcade would ignore the HudPosition_Multi flag in PlayerHud::Init.
		HandleHudPositionMulti::install_at_ptr(0x0054cf1a);

		// Removes the ability to toggle between exclusive fullscreen and windowed mode.
		// We do this because the original games' implementation is very error-prone.
		sunset::inst::nop(reinterpret_cast<void*>(0x00833268), 5);

		// Allows the FE_MM_WOC menu to appear in FrontEnd.
		sunset::inst::nop(reinterpret_cast<void*>(0x004bf0fa), 7);
		
		// Adds Win32Wii_Scn_ExitToWindows to the pause menu.
		static const char PAUSE_MENU_OPTIONS_LIST_WITH_RESTART[] = "IG_PA_Resume,IG_PA_Restart,IG_PA_Exit,Win32Wii_Scn_ExitToWindows";
		static const char PAUSE_MENU_OPTIONS_LIST[] = "IG_PA_Resume,IG_PA_Exit,Win32Wii_Scn_ExitToWindows";
		sunset::inst::mov_u32(reinterpret_cast<void*>(0x004d0309), sunset::inst::RegisterIndex::Eax, reinterpret_cast<std::uintptr_t>(&PAUSE_MENU_OPTIONS_LIST_WITH_RESTART));
		sunset::inst::mov_u32(reinterpret_cast<void*>(0x004d0302), sunset::inst::RegisterIndex::Eax, reinterpret_cast<std::uintptr_t>(&PAUSE_MENU_OPTIONS_LIST));		
		CheckForExitToWindows::install_at_ptr(0x004d0596);
		CheckForExitToWindows2::install_at_ptr(0x004d091c);

		// Adds a new Graphics sub-menu to the Options menu.
		static const char EXTRAS_OPTIONS_MENU_ITEMS[] = "FE_EX_Options,FE_EX_EnterCode,FE_EX_Credits,FE_EX_Graphics";
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004bed9b), reinterpret_cast<std::uintptr_t>(&EXTRAS_OPTIONS_MENU_ITEMS));
		CheckForGraphicsMenu::install_at_ptr(0x004bee93);
		CheckForGraphicsMenu2::install_at_ptr(0x004cbcda);
		CheckForGraphicsMenu3::install_at_ptr(0x004c2394);
		ExternalInterfaceHandler_Callback::install_at_ptr(0x0116a080);

		// Prevents the game from multiplying camera distance values by 0.75.
		sunset::inst::nop(reinterpret_cast<void*>(0x0049e05c), 8);
		// Prevents the game from clamping the camera distance to 7.0.
		sunset::inst::nop(reinterpret_cast<void*>(0x0049e037), 5);

		// Changes the default camera distances.
		static const float FIVE = 5.0f;
		static const float NINE = 9.0f;
		static const float TEN = 10.0f;
		static const float FIFTEEN = 15.0f;
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00489225 + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x00489225 + 4) = reinterpret_cast<std::uintptr_t>(&FIVE);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0048a20c + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x0048a20c + 4) = reinterpret_cast<std::uintptr_t>(&FIVE);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0048b92a + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x0048b92a + 4) = reinterpret_cast<std::uintptr_t>(&NINE);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x005a07a9 + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x005a07a9 + 4) = reinterpret_cast<std::uintptr_t>(&TEN);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x00482999 + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x00482999 + 4) = reinterpret_cast<std::uintptr_t>(&TEN);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x005a07ef + 4), 4, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uintptr_t*>(0x005a07ef + 4) = reinterpret_cast<std::uintptr_t>(&FIFTEEN);
		FixDefaultDistance::install_at_ptr(0x0048a9cc);

		// Effectively removes the weird FOV scaling by setting the constant to 1.0f.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x015f21cc), 4, sunset::utils::Perm::ReadWrite);
		*reinterpret_cast<std::uint32_t*>(0x015f21cc) = 0x3F800000;

		// Allows the game to choose the correct screen type value for CarTypeProperties.
		GetTypeCCI::install_at_ptr(0x0048b4b9);

		// Fixes an issue where UI elements (both 2D and 3D) would render at a higher-than-intended gamma.
		// I'm not nearly smart enough to know if this is really the "correct" way to do it. But I don't care, it works!
		FlashGuiImpRender::install_at_ptr(0x0116da70);

		// Forces the game to use XBOX360 `.dct` files instead of WIN32 ones.
		// FIXME: We might need to revert this later.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x01647d44), 8, sunset::utils::Perm::ReadWrite);
		std::memcpy(reinterpret_cast<void*>(0x01647d44), "XBOX360", 8);	

		// Fixes UI scaling at resolutions higher than 720p.
		AdjustScaleformViewport::install_at_ptr(0x0116e28e);

		// Inverts backwards-driving controls for player-controlled cars.
		CarsVehicleSetSteer::install_at_ptr(0x006b10a0);

		// Fixes an issue where the game thinks two players are active by default.
		sunset::inst::push_u8(reinterpret_cast<void*>(0x004c5c17), 1);

		// Fixes an issue where the mini-menu would omit the button labels on the main menu.
		MiniMenuAddButton::install_at_ptr(0x004b87a0);
		MiniMenuShowTite::install_at_ptr(0x004b89a0);
		MiniMenuResetButtons::install_at_ptr(0x004b88b0);

		sunset::inst::nop(reinterpret_cast<void*>(0x004c179e), 5);

		sunset::utils::set_permission(reinterpret_cast<void*>(0x004c2a1f + 6), 1, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<unsigned char*>(0x004c2a1f + 6) = CarsFrontEndScreen::SaveSlots;

		static const char FRONTEND_NEW_GAME_BUTTON[] = "FrontEnd_NewGameButton";
		static const char FRONTEND_NEW_TITLE[] = "FrontEnd_New_Title";
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c2a28), reinterpret_cast<std::uintptr_t>(&FRONTEND_NEW_TITLE));
		sunset::inst::push_u32(reinterpret_cast<void*>(0x004c2a2d), reinterpret_cast<std::uintptr_t>(&FRONTEND_NEW_GAME_BUTTON));

		AddButtons::install_at_ptr(0x004b86aa);
		CarsFrontEnd_SetScreen::install_at_ptr(0x004c1440);

		logger::log("[ArcadeEssentials::Pentane_Main] Installed hooks!");
	} 

}
