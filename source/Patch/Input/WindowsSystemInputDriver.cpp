#pragma once
#include <cstdint>
#include <array>
#include <thread>
#include <atomic>
#include <cwctype>
#include <Windows.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <dbt.h>
#include <hidsdi.h>
#include <Hidclass.h>
#include <Xinput.h>
#include "WindowsSystemInputDriver.hpp"
#include "WindowsControllerInputDriver.hpp"
#include "XInputInputDriver.hpp"
#include "../../pentane.hpp"

inline auto CMasterTimer_GetOSTime = (std::uint32_t(_cdecl*)())(0x00770280);
inline auto IsXInputDevice = (bool(_cdecl*)(const GUID*))(0x00816850);
inline auto getINIFileName = (void(_cdecl*)(char*))(0x00814eb0);

bool FIRST_INIT = true;
wchar_t DEVICE_PATH_DROPPED[260] = {};
std::atomic<bool> CHECK_FOR_DI_NEW_CONTROLLERS = true;
std::atomic<bool> DEVICE_DROPPED = false;
std::atomic<std::uint32_t> LAST_DI_CHECK_TIMESTAMP = 0;
std::atomic<std::uint32_t> LAST_XI_CHECK_TIMESTAMP = 0;
std::atomic<HCMNOTIFICATION> DI_CHECK_NOTIFICATION_HANDLE{};

bool iequals(std::wstring_view lhs, std::wstring_view rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	for (std::size_t i = 0; i < lhs.size(); ++i) {
		if (std::towlower(lhs[i]) != std::towlower(rhs[i])) {
			return false;
		}
	}
	return true;
}

int __stdcall EnumJoysticks(const DIDEVICEINSTANCEA* devInst, void* udata) {

	WindowsSystemInputDriver* _this = reinterpret_cast<WindowsSystemInputDriver*>(udata);

	if (IsXInputDevice(&devInst->guidProduct)) {
		return DIENUM_CONTINUE;
	}

	if (strstr(devInst->tszProductName, "Nintendo RVL-CNT") != nullptr) {
		return DIENUM_CONTINUE;
	}

	DWORD devType = devInst->dwDevType & 0xff;
	if (devType != DI8DEVTYPE_GAMEPAD && devType != DI8DEVTYPE_JOYSTICK) {
		return DIENUM_CONTINUE;
	}

	if (_this->controllers >= 11) {
		return DIENUM_STOP;
	}

	for (int i = 0; i < _this->controllers; i++) {
		const char* identity = _this->controller[i]->Identify();
		if (std::string_view(identity) == "DirectInput") {
			DIDEVICEINSTANCEA currInst{};
			currInst.dwSize = sizeof(DIDEVICEINSTANCEA);
			reinterpret_cast<WindowsControllerInputDriver*>(_this->controller[i])->m_device->GetDeviceInfo(&currInst);
			if (currInst.guidInstance == devInst->guidInstance) {
				logger::log_format("[WindowsSystemInputDriver::EnumJoysticks] Device: {} ({}) already added, skipping...", devInst->tszProductName, i);
				return DIENUM_CONTINUE;
			}
		}
	}

	int iControllerUniqueIndex = 0;
	char sDeviceName[256]{};
	strcpy_s(sDeviceName, devInst->tszInstanceName);
	getINIFileName(sDeviceName);
	strcat_s(sDeviceName, "_");
	std::array<char[260], 11>* s_buttonMapFile = reinterpret_cast<std::array<char[260], 11>*>(0x018d2608);

	for (int i = 0; i < _this->controllers; i++) {
		if (strstr(s_buttonMapFile->at(i), sDeviceName) != nullptr) {
			iControllerUniqueIndex++;
		}
	}
	sprintf_s(sDeviceName, "%s%d", sDeviceName, 0 /* iControllerUniqueIndex */);
	strcat_s(sDeviceName, ".ini");
	strcpy_s(s_buttonMapFile->at(_this->controllers), sDeviceName);

	IDirectInputDevice8A* device = nullptr;
	if (_this->dInput->CreateDevice(devInst->guidInstance, &device, nullptr) != DI_OK) {
		return DIENUM_CONTINUE;
	}

	DIDEVCAPS caps{};
	caps.dwSize = sizeof(DIDEVCAPS);
	device->GetCapabilities(&caps);
	if (caps.dwAxes < 3) {
		device->Unacquire();
		device->Release();
		logger::log_format("[WindowsSystemInputDriver::EnumJoysticks] Device: {}_{} was ignored as it has less than 3 axes.", devInst->tszInstanceName, iControllerUniqueIndex);
		return DIENUM_CONTINUE;
	}

	if (FAILED(device->SetDataFormat(&c_dfDIJoystick2))) {
		device->Unacquire();
		device->Release();
		logger::log_format("[WindowsSystemInputDriver::EnumJoysticks] Device: {}_{} was ignored as ::SetDataFormat returned an error.", devInst->tszInstanceName, iControllerUniqueIndex);
		return DIENUM_CONTINUE;
	}

	SetActiveWindow(_this->hWnd);
	device->SetCooperativeLevel(_this->hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	_this->controller[_this->controllers] = new WindowsControllerInputDriver(_this->hWnd, device, _this->controllers, devInst->tszInstanceName);
	
	if (_this->controller[_this->controllers]->m_bInvalidController != 0) {
		delete _this->controller[_this->controllers];
		_this->controller[_this->controllers] = nullptr;
		logger::log_format("[WindowsSystemInputDriver::EnumJoysticks] Device: {}_{} was ignored as it does not have a profile saved in `AppData`.", devInst->tszInstanceName, iControllerUniqueIndex);
		return DIENUM_CONTINUE;
	}

	logger::log_format("[WindowsSystemInputDriver::EnumJoysticks] Device: {}_{} mapped to index: {}!", devInst->tszInstanceName, iControllerUniqueIndex, _this->controllers);
	_this->controllers++;
	return DIENUM_CONTINUE;
}

void BumpKBMToSlot0(WindowsSystemInputDriver* _this) {
	int kbm = -1;
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr && _this->controller[i] == _this->d_pKeyboard) {
			kbm = i;
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

void DetachSharedController(WindowsSystemInputDriver* _this, ControllerInputDriver* controller) {
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			if (_this->controller[i]->GetSharedController() == controller) {
				_this->controller[i]->SetSharedController(nullptr);
			}
		}
	}
}

void OnXInputControllerRemoved(WindowsSystemInputDriver* _this, std::uint32_t id) {
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			if (std::string_view(_this->controller[i]->Identify()) == "XInput") {
				std::uint32_t currentId = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(_this->controller[i]) + 0x26C);
				if (currentId == id) {
					logger::log_format("[WindowsSystemInputDriver::BeginInput] XUser Disconnected! ID: {}", id);
					DetachSharedController(_this, _this->controller[i]);
					delete _this->controller[i];
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

void OnDirectInputControllerRemoved(WindowsSystemInputDriver* _this, const std::wstring_view devicePath) {
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			if (std::string_view(_this->controller[i]->Identify()) == "DirectInput") {
				if (iequals(reinterpret_cast<WindowsControllerInputDriver*>(_this->controller[i])->m_devicePath, devicePath)) {
					logger::log_format("[WindowsSystemInputDriver::BeginInput] Device Disconnected! Name: {} ({})", _this->controller[i]->DeviceName(), i);
					DetachSharedController(_this, _this->controller[i]);
					delete _this->controller[i];
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
			/*
			_this->controller[i] = reinterpret_cast<ControllerInputDriver*>(operator_new(0x4e78));
			XInputInputDriver_XInputInputDriver(_this->controller[i], id);
			_this->controllers++;
			_this->d_pKeyboard->SetSharedController(_this->controller[i]);
			_this->controller[i]->SetSharedController(_this->d_pKeyboard);
			logger::log_format("[WindowsSystemInputDriver::BeginInput] Found new XInput controller! ID: {}, Assigned to: {}.", id, i);
			return;
			*/

			if (i == 0) {
				_this->controller[0] = new XInputInputDriver(id);
				// _this->controller[0]->SetSharedController(_this->d_pKeyboard);
				_this->d_pKeyboard->SetSharedController(_this->controller[0]);
				_this->controllers++;
				logger::log_format("[WindowsSystemInputDriver::BeginInput] Found new XInput controller! ID: {}, Assigned to 0.", id);
				return;
			}
			else {
				ControllerInputDriver* oldPlayerOne = _this->controller[0];
				_this->controller[0] = new XInputInputDriver(id);
				// _this->controller[0]->SetSharedController(_this->d_pKeyboard);
				_this->d_pKeyboard->SetSharedController(_this->controller[0]);
				_this->controller[i] = oldPlayerOne;
				_this->controllers++;
				logger::log_format("[WindowsSystemInputDriver::BeginInput] Found new XInput controller! ID: {}, Bumped to 0.", id);
				return;
			}
		}
	}
}

DWORD __stdcall DIDevicesChangedCallback(HCMNOTIFICATION h, PVOID context, CM_NOTIFY_ACTION action, PCM_NOTIFY_EVENT_DATA data, DWORD eventDataSize) {
	if (action == CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL) {
		if (DEVICE_DROPPED.load()) {
			DEVICE_DROPPED.wait(false);
		}
		std::wstring_view devicePathView(data->u.DeviceInterface.SymbolicLink);
		std::copy_n(data->u.DeviceInterface.SymbolicLink, (std::min)(devicePathView.size(), sizeof(DEVICE_PATH_DROPPED) / sizeof(wchar_t)), DEVICE_PATH_DROPPED);
		DEVICE_DROPPED = true;
	}
	else if (action == CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL) {
		if (CHECK_FOR_DI_NEW_CONTROLLERS.load()) {
			CHECK_FOR_DI_NEW_CONTROLLERS.wait(false);
		}
		CHECK_FOR_DI_NEW_CONTROLLERS = true;
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
	if (CHECK_FOR_DI_NEW_CONTROLLERS && (current - LAST_DI_CHECK_TIMESTAMP) >= 2000) {
		auto oldControllerCount = _this->controllers;
		_this->dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticks, _this, DIEDFL_ATTACHEDONLY);
		CHECK_FOR_DI_NEW_CONTROLLERS = false;
		LAST_DI_CHECK_TIMESTAMP = CMasterTimer_GetOSTime();
		if (_this->controllers > oldControllerCount) {
			auto newestController = _this->controller[_this->controllers - 1];
			_this->controller[_this->controllers - 1] = _this->controller[0];
			_this->controller[0] = newestController;
			logger::log_format("[WindowsSystemInputDriver::BeginInput] Device: {} bumped to slot 0!", newestController->DeviceName());
		}
	}
	if (DEVICE_DROPPED) {
		OnDirectInputControllerRemoved(_this, DEVICE_PATH_DROPPED);
		DEVICE_DROPPED = false;
	}
	if ((current - LAST_XI_CHECK_TIMESTAMP) >= 2000) {
		// Here, we try to figure out which XUsers the game is currently aware of.
		std::array<bool, 4> connectedXUsers = { false, false, false, false };
		for (int i = 0; i < 11; i++) {
			if (_this->controller[i] != nullptr) {
				if (std::string_view(_this->controller[i]->Identify()) == "XInput") {
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
				XINPUT_CAPABILITIES_EX caps{};
				if (XInputGetCapabilitiesEx(1, i, XINPUT_FLAG_GAMEPAD, &caps) != ERROR_SUCCESS) {
					OnXInputControllerRemoved(_this, i);
				}
			}
		}
		// Then, we check to see if there's any new XUsers that the game hasn't picked up yet.
		for (int i = 0; i < 4; i++) {
			if (!connectedXUsers[i]) {
				// If we manage to find one, we add it!
				XINPUT_CAPABILITIES_EX caps{};
				if (XInputGetCapabilitiesEx(1, i, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS) {
					AddXInputController(_this, i);
				}
			}
		}
		LAST_XI_CHECK_TIMESTAMP = CMasterTimer_GetOSTime();
	}
	
	for (int i = 0; i < 11; i++) {
		if (_this->controller[i] != nullptr) {
			_this->controller[i]->BeginInput();
		}
	}

	if (_this->m_pMouse != nullptr) {
		DIMOUSESTATE2 dims2{};
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