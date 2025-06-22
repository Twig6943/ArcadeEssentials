#pragma once
#include <dinput.h>
#include "../../Game/Input/SystemInputDriver.hpp"

inline class WindowsSystemInputDriver** g_InputPtr = reinterpret_cast<class WindowsSystemInputDriver**>(0x018d3244);

//inline auto WindowsSystemInputDriver_LockPlayerToController = (void(__thiscall*)(class WindowsSystemInputDriver*, int, int))(0x00816dd0);
//inline auto WindowsSystemInputDriver_ControllerLocked = (bool(__thiscall*)(class WindowsSystemInputDriver*, int))(0x00816ea0);
//inline auto WindowsSystemInputDriver_IsControllerLocked = (bool(__thiscall*)(class WindowsSystemInputDriver*, int))(0x00816ed0);
//inline auto WindowsSystemInputDriver_GetUnlockedController = (void* (__thiscall*)(class WindowsSystemInputDriver*, int))(0x00817050);


class WindowsSystemInputDriver : public SystemInputDriver {
public:
	char m_cKeyboardBuffer[256];
	IDirectInput8W* dInput;
	IDirectInputDevice8W* m_pMouse;
	IDirectInputDevice8W* m_pKeyboard;
	IDirectInputDevice8W* m_DevHdl;
	HWND hWnd;
	short keyToButtonMap[256];
	int mapEntries;
	IDirectInputDevice8W* d_pKeyboard;
	IDirectInputDevice8W* d_pSpaceBall;
	int controllers;
	ControllerInputDriver* controller[11];
	int lockedControllerIndex[11];
	int numLockedControllers;
public:
};

static_assert(sizeof(WindowsSystemInputDriver) == 1120);

void __fastcall BeginInput(class WindowsSystemInputDriver* _this);