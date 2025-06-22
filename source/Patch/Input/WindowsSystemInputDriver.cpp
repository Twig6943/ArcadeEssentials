#pragma once
#include <cstdint>
#include <array>
#include <thread>
#include <atomic>
#include <Windows.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <dbt.h>
#include <hidsdi.h>
#include <Hidclass.h>
#include <Xinput.h>
#include "WindowsSystemInputDriver.hpp"
#include "../../pentane.hpp"

inline auto CMasterTimer_GetOSTime = (std::uint32_t(_cdecl*)())(0x00770280);
inline auto EnumJoysticks = (int(__stdcall*)(LPCDIDEVICEINSTANCEW, void*))(0x008162f0);
inline auto operator_new = (void* (_cdecl*)(std::size_t))(0x007b1650);
inline auto operator_delete = (void(_cdecl*)(void*))(0x007b1850);
inline auto XInputInputDriver_XInputInputDriver = (ControllerInputDriver*(__thiscall*)(ControllerInputDriver*, int))(0x00814480);

std::atomic<bool> FIRST_INIT = true;
std::atomic<bool> CHECK_FOR_DI_CONTROLLERS = true;
std::atomic<std::uint32_t> LAST_DI_CHECK_TIMESTAMP = 0;
std::atomic<std::uint32_t> LAST_XI_CHECK_TIMESTAMP = 0;
std::atomic<HCMNOTIFICATION> DI_CHECK_NOTIFICATION_HANDLE{};

void BumpKBMToSlot0(WindowsSystemInputDriver* _this) {
	int kbm = -1;
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			std::uintptr_t vt = *reinterpret_cast<std::uintptr_t*>(_this->controller[i]);
			// Check for the KeyControllerInputDriver vtable
			if (vt == 0x01617E7C) {
				kbm = i;
			}
		}
	}
	if (kbm != -1) {
		logger::log_format("[WindowsSystemInputDriver::BeginInput] Bumped Keyboard to 0!");
		ControllerInputDriver* oldPlayerOne = _this->controller[0];
		_this->controller[0] = _this->controller[kbm];
		_this->controller[kbm] = oldPlayerOne;
	}
	else {
		logger::log_format("[WindowsSystemInputDriver::BeginInput] Unable to find Keyboard Controller!");
	}
}

void OnXInputControllerRemoved(WindowsSystemInputDriver* _this, std::uint32_t id) {
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			std::uintptr_t vt = *reinterpret_cast<std::uintptr_t*>(_this->controller[i]);
			// Check for the XInputInputDriver vtable
			if (vt == 0x01617F94) {
				std::uint32_t currentId = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(_this->controller[i]) + 0x26C);
				if (currentId == id) {
					logger::log_format("[WindowsSystemInputDriver::BeginInput] XUser Disconnected! ID: {}", id);
					operator_delete(_this->controller[i]);
					_this->controller[i] = nullptr;
					_this->controllers--;
					if (i == 0) {
						BumpKBMToSlot0(_this);
					}
				}
			}
		}
	}
}

void AddXInputController(WindowsSystemInputDriver* _this, std::uint32_t id) {
	/*
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			logger::log_format("[Controller States] {}", *reinterpret_cast<std::uintptr_t*>(_this->controller[i]));
		}
		else {
			logger::log_format("[Controller States] {}", reinterpret_cast<std::uintptr_t>(_this->controller[i]));
		}
	}
	*/
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] == nullptr) {
			if (i == 0) {
				_this->controller[0] = reinterpret_cast<ControllerInputDriver*>(operator_new(0x4e78));
				XInputInputDriver_XInputInputDriver(_this->controller[0], id);
				_this->controllers++;
				logger::log_format("[WindowsSystemInputDriver::BeginInput] Found new XInput controller! ID: {}, Assigned to 0.", id);
				return;
			}
			else {
				ControllerInputDriver* oldPlayerOne = _this->controller[0];
				_this->controller[0] = reinterpret_cast<ControllerInputDriver*>(operator_new(0x4e78));
				XInputInputDriver_XInputInputDriver(_this->controller[0], id);
				_this->controller[i] = oldPlayerOne;
				_this->controllers++;
				logger::log_format("[WindowsSystemInputDriver::BeginInput] Found new XInput controller! ID: {}, Bumped to 0.", id);
				return;
			}
		}
	}
}

DWORD __stdcall DIDevicesChangedCallback(HCMNOTIFICATION h, PVOID context, CM_NOTIFY_ACTION action, PCM_NOTIFY_EVENT_DATA data, DWORD eventDataSize) {
	if (action == CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL || action == CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL) {
		CHECK_FOR_DI_CONTROLLERS = true;
	}
	return ERROR_SUCCESS;
}

void _cdecl ReleaseNotification() {
	HCMNOTIFICATION hNotify = DI_CHECK_NOTIFICATION_HANDLE;
	CM_Unregister_Notification(hNotify);
}

void __fastcall BeginInput(WindowsSystemInputDriver* _this) {
	if (FIRST_INIT) {
		CM_NOTIFY_FILTER filter = {};
		filter.cbSize = sizeof(CM_NOTIFY_FILTER);
		filter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE;
		filter.u.DeviceInterface.ClassGuid = GUID_DEVINTERFACE_HID;

		HCMNOTIFICATION hNotify = nullptr;
		CONFIGRET cr = CM_Register_Notification(&filter, nullptr, DIDevicesChangedCallback, &hNotify);
		DI_CHECK_NOTIFICATION_HANDLE = hNotify;
		FIRST_INIT = false;
		std::atexit(ReleaseNotification);
	}

	for (int i = 0; i < BC_BUTTONCODES; i++) {
		_this->state[i].last = _this->state[i].now;
	}

	if (_this->hasFocus) {
		if (_this->m_pKeyboard->GetDeviceState(sizeof(_this->m_cKeyboardBuffer), _this->m_cKeyboardBuffer) < 0) {
			_this->m_pKeyboard->Acquire();
			return;
		}

		char* dInputKeyCode = reinterpret_cast<char*>(0x0185efa8);
		for (int i = 0; i < BC_BUTTONCODES - 1; i++) {
			if (dInputKeyCode[i] != 0) {
				_this->state[i].now = ((_this->m_cKeyboardBuffer[dInputKeyCode[i]] & 0x80) != 0);
			}
		}
	}

	if (_this->state[BC_LALT].now || _this->state[BC_RALT].now) {
		_this->state[BC_ENTER].now = false;
		_this->state[BC_NUM_ENTER].now = false;
	}

	std::uint32_t current = CMasterTimer_GetOSTime();
	if (CHECK_FOR_DI_CONTROLLERS && (current - LAST_DI_CHECK_TIMESTAMP) >= 2000) {
		_this->dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticks, _this, DIEDFL_ATTACHEDONLY);
		CHECK_FOR_DI_CONTROLLERS = false;
		LAST_DI_CHECK_TIMESTAMP = CMasterTimer_GetOSTime();
	}
	if ((current - LAST_XI_CHECK_TIMESTAMP) >= 2000) {
		// Here, we try to figure out which XUsers the game is currently aware of.
		std::array<bool, 4> connectedXUsers = { false, false, false, false };
		for (int i = 0; i < 11; i++) {
			if (_this->controller[i] != nullptr) {
				std::uintptr_t vt = *reinterpret_cast<std::uintptr_t*>(_this->controller[i]);
				// Check for the XInputInputDriver vtable
				if (vt == 0x01617F94) {
					std::uint32_t id = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(_this->controller[i]) + 0x26C);
					if (id < 4) {
						connectedXUsers[id] = true;
					}
				}
			}
		}
		// Next, we check to see if any XUsers dropped in the last two seconds.
		for (int i = 0; i < 4; i++) {
			if (connectedXUsers[i]) {
				XINPUT_CAPABILITIES caps{};
				if (XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &caps) != ERROR_SUCCESS) {
					OnXInputControllerRemoved(_this, i);
				}
			}
		}
		// Then, we check to see if there's any new XUsers that the game hasn't picked up yet.
		for (int i = 0; i < 4; i++) {
			if (!connectedXUsers[i]) {
				// If we manage to find one, we add it!
				XINPUT_CAPABILITIES caps{};
				if (XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS) {
					AddXInputController(_this, i);
				}
			}
		}
		LAST_XI_CHECK_TIMESTAMP = CMasterTimer_GetOSTime();
	}
	
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			auto beginInput = *reinterpret_cast<void(__thiscall**)(ControllerInputDriver*)>(*reinterpret_cast<std::uintptr_t*>(_this->controller[i]) + 0x6C);
			beginInput(_this->controller[i]);
		}
	}

	if (_this->m_pMouse != nullptr) {
		DIMOUSESTATE2 dims2;
		memset(&dims2, 0, sizeof(dims2));
		if (_this->m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2) < 0) {
			_this->m_pMouse->Acquire();
		}
		else {
			_this->mouseX = dims2.lX;
			_this->mouseY = dims2.lY;
			if (abs(_this->mouseX) * 3 < abs(_this->mouseY)) {
				_this->mouseX = 0;
			}
			else if (abs(_this->mouseY) * 3 < abs(_this->mouseX)) {
				_this->mouseY = 0;
			}

			if (GetSystemMetrics(SM_SWAPBUTTON) != 0) {
				_this->state[BC_MOUSELEFT].now = (dims2.rgbButtons[1] & 0x80) != 0;
				_this->state[BC_MOUSERIGHT].now = (dims2.rgbButtons[0] & 0x80) != 0;
			}
			else {
				_this->state[BC_MOUSELEFT].now = (dims2.rgbButtons[0] & 0x80) != 0;
				_this->state[BC_MOUSERIGHT].now = (dims2.rgbButtons[1] & 0x80) != 0;
			}
			_this->state[BC_MOUSEMIDDLE].now = (dims2.rgbButtons[2] & 0x80) != 0;
		}
	}
}