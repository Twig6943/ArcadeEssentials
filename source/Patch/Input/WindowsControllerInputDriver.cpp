#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <io.h>
#include "WindowsControllerInputDriver.hpp"

WindowsControllerInputDriver::WindowsControllerInputDriver(HWND hWnd, IDirectInputDevice8W* joystickDevice, int deviceNumber, const char* pDeviceName) {
	m_device = joystickDevice;
	for (auto i = 0; i < sizeof(m_axisValid) / sizeof(int); i++) {
		m_axisValid[i] = false;
	}
	m_iAxisDeadZoneValue = 20;
	m_bInvalidController = SetupDevice(hWnd);
	if (!m_bInvalidController) {
		m_bInvalidController = LoadProfile(deviceNumber);
		Initialize();
	}
	if (pDeviceName) {
		strcpy(m_deviceName, pDeviceName);
	}
	else {
		strcpy(m_deviceName, "NoNameDevice");
	}
}

bool WindowsControllerInputDriver::SetupDevice(HWND hWnd) {
	if (FAILED(m_device->EnumObjects(reinterpret_cast<LPDIENUMDEVICEOBJECTSCALLBACKW>(0x00813950), this, DIDFT_ALL))) {
		return true;
	}
	return false;
}

bool WindowsControllerInputDriver::SetupAxis(DWORD diAxis) {
	return false;
}

int WindowsControllerInputDriver::LoadProfile(int deviceNumber) {
	for (auto i = 0; i < std::to_underlying(ControllerButton::Max); i++) {
		m_buttonMap[i] = -1;
	}

	char sMapFileName[260]{};
	std::array<char[260], 11>* s_buttonMapFile = reinterpret_cast<std::array<char[260], 11>*>(0x018d2608);
	strcpy(sMapFileName, reinterpret_cast<const char*>(0x018d3138));
	strcat(sMapFileName, s_buttonMapFile->at(deviceNumber));

	if (_access(sMapFileName, 0) < 0) {
		return 1;
	}

	m_iAxisDeadZoneValue = GetPrivateProfileIntA("Button Map", "AXIS_DEAD_ZONE", 20, sMapFileName);
	m_iApplicationControllerID = GetPrivateProfileIntA("Button Map", "FORCE APPLICATION CONTROLLER ID", -1, sMapFileName);

	m_buttonMap[std::to_underlying(ControllerButton::Up)] = 100;
	m_buttonMap[std::to_underlying(ControllerButton::Down)] = 102;
	m_buttonMap[std::to_underlying(ControllerButton::Left)] = 103;
	m_buttonMap[std::to_underlying(ControllerButton::Right)] = 101;

	int iInvalidKeys = 0;
	iInvalidKeys += GetProfileButton("BUTTON_X", &m_buttonMap[std::to_underlying(ControllerButton::Cross)], sMapFileName);
	iInvalidKeys += GetProfileButton("BUTTON_O", &m_buttonMap[std::to_underlying(ControllerButton::Circle)], sMapFileName);
	iInvalidKeys += GetProfileButton("BUTTON_TRI", &m_buttonMap[std::to_underlying(ControllerButton::Triangle)], sMapFileName);
	iInvalidKeys += GetProfileButton("BUTTON_SQUARE", &m_buttonMap[std::to_underlying(ControllerButton::Square)], sMapFileName);
	GetProfileButton("BUTTON_L1", &m_buttonMap[std::to_underlying(ControllerButton::L1)], sMapFileName);
	GetProfileButton("BUTTON_R1", &m_buttonMap[std::to_underlying(ControllerButton::R1)], sMapFileName);
	GetProfileButton("BUTTON_L2", &m_buttonMap[std::to_underlying(ControllerButton::L2)], sMapFileName);
	GetProfileButton("BUTTON_R2", &m_buttonMap[std::to_underlying(ControllerButton::R2)], sMapFileName);
	GetProfileButton("BUTTON_START", &m_buttonMap[std::to_underlying(ControllerButton::Start)], sMapFileName);
	GetProfileButton("BUTTON_SELECT", &m_buttonMap[std::to_underlying(ControllerButton::Select)], sMapFileName);
	GetProfileButton("BUTTON_STICK1", &m_buttonMap[std::to_underlying(ControllerButton::Stick1)], sMapFileName);
	GetProfileButton("BUTTON_STICK2", &m_buttonMap[std::to_underlying(ControllerButton::Stick2)], sMapFileName);
	GetProfileButton("BUTTON_START", &m_buttonMap[std::to_underlying(ControllerButton::Start)], sMapFileName);
	GetProfileButton("BUTTON_SELECT", &m_buttonMap[std::to_underlying(ControllerButton::Select)], sMapFileName);
	if (iInvalidKeys > 0) {
		return 1;
	}

	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		m_axisMap[i] = -1;
	}

	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		GetProfileStick(i, sMapFileName);
	}

	return 0;
}

bool WindowsControllerInputDriver::GetProfileButton(const char* buttonName, int* value, char* sMapFileName) {
	return false;
}
bool WindowsControllerInputDriver::GetProfileStick(int stick, char* sMapFileName) {
	return false;
}


WindowsControllerInputDriver::~WindowsControllerInputDriver() {
	if (m_device != nullptr) {
		m_device->Unacquire();
		m_device->Release();
	}
}

void WindowsControllerInputDriver::Initialize() {
	if (!m_bInvalidController) {
		for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
			GenerateTransferFunction(m_transferLookup[i], 256, m_initTransferFunction[i], m_initFactor[i], m_initDeadZone[i], m_initClampZone[i]);
		}
	}
}

const char* WindowsControllerInputDriver::DeviceName() const {
	return m_deviceName;
}

bool WindowsControllerInputDriver::SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) {
	return false;
}

bool WindowsControllerInputDriver::Connected() {
	return true;
}

void WindowsControllerInputDriver::BeginInput() {
}

void WindowsControllerInputDriver::DoneInput() {
}

void WindowsControllerInputDriver::Activate(bool active) {
	if (m_device == nullptr) {
		return;
	}
	if (active) {
		m_device->Acquire();
	}
	else {
		m_device->Unacquire();
	}
}

bool WindowsControllerInputDriver::ObserveFocus() {
	return m_observeFocus;
}
