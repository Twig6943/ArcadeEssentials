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
	IDirectInput8A* dInput;
	IDirectInputDevice8A* m_pMouse;
	IDirectInputDevice8A* m_pKeyboard;
	IDirectInputDevice8A* m_DevHdl;
	HWND hWnd;
	short keyToButtonMap[256];
	int mapEntries;
	ControllerInputDriver* d_pKeyboard;
	ControllerInputDriver* d_pSpaceBall;
	int controllers;
	ControllerInputDriver* controller[11];
	int lockedControllerIndex[11];
	int numLockedControllers;
public:
	bool Initialize(HINSTANCE hInst, HWND hWnd);
};

static_assert(sizeof(WindowsSystemInputDriver) == 1120);

bool __fastcall WindowsSystemInputDriver_Initialize(class WindowsSystemInputDriver* _this, std::uintptr_t edx, HINSTANCE hInst, HWND hWnd);
void __fastcall WindowsSystemInputDriver_BeginInput(class WindowsSystemInputDriver* _this);
int __stdcall WindowsSystemInputDriver_EnumJoysticks(const DIDEVICEINSTANCEA* devInst, void* udata);