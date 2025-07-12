#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <io.h>
#include "../../pentane.hpp"
#include "WindowsControllerInputDriver.hpp"

inline auto WindowsControllerInputDriver_GetProfileButton = (int(__thiscall*)(WindowsControllerInputDriver*, const char*, int*, char*))(0x00814000);

std::array<std::string_view, 9> c_DefaultAxisName = {
	"Axis LX",
	"Axis LY",
	"Axis LZ",
	"Axis RX",
	"Axis RY",
	"Axis RZ",
	"slider0",
	"slider1",
	"----"
};

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
	GenerateTransferFunction(m_transferLookup[std::to_underlying(transferFunction)], 256, transferFunction, factor, deadZonePercent, clampZonePercent);
	return true;
}

bool WindowsControllerInputDriver::Connected() {
	return true;
}

void WindowsControllerInputDriver::BeginInput() {
	if (m_device == nullptr) {
		return;
	}
	
	if (FAILED(m_device->Poll())) {
		HRESULT ret = m_device->Acquire();
		while (ret == DIERR_INPUTLOST) {
			ret = m_device->Acquire();
		}
		return;
	}

	DIJOYSTATE2 state{};
	if (FAILED(m_device->GetDeviceState(sizeof(state), &state))) {
		return;
	}

	ControllerInputDriver::BeginInput();
	
	std::array<int, 8> axis = {
		state.lX,
		state.lY,
		state.lZ,
		state.lRx,
		state.lRy,
		state.lRz,
		state.rglSlider[0],
		state.rglSlider[1],
	};

	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		if (m_axisMap[i] >= 0) {
			if (m_axisValid[m_axisMap[i]] == 0) {
				m_stick[i] = 0.0;
			}
			else if (m_invertAxis[i] == false) {
				m_stick[i] = m_transferLookup[i][axis[m_axisMap[i]]];
			}
			else {
				m_stick[i] = m_transferLookup[i][255 - axis[m_axisMap[i]]];
			}
		}
	}

	std::array<bool, 4> povHat = {};
	if ((state.rgdwPOV[0] & 0xFFFF) != 0xFFFF) {
		int pos = state.rgdwPOV[0] / 100;
		povHat[0] = (pos > 300 || pos < 60);
		povHat[1] = (pos > 30 && pos < 150);
		povHat[2] = (pos > 120 && pos < 240);
		povHat[3] = (pos > 210 && pos < 330);
	}

	for (auto b = 0; b < std::to_underlying(ControllerButton::Max); b++) {
		if (m_buttonMap[b] != -1) {
			if (m_buttonMap[b] >= 0 && m_buttonMap[b] < 100) {
				SetState(static_cast<ControllerButton>(b), (state.rgbButtons[m_buttonMap[b]] & 0x80) != 0);
			}
			else if (m_buttonMap[b] > 0 && m_buttonMap[b] < 1000) {
				SetState(static_cast<ControllerButton>(b), povHat[m_buttonMap[b] - 100]);
			}
			else if (m_buttonMap[b] >= 1000 && m_buttonMap[b] <= 1007) {
				SetState(static_cast<ControllerButton>(b), (axis[m_buttonMap[b] - 1000] > 128 + 80));
			}
			else if (m_buttonMap[b] <= -1000 && m_buttonMap[b] >= -1007) {
				SetState(static_cast<ControllerButton>(b), (axis[-m_buttonMap[b] - 1000] < 128 - 80));
			}
		}
	}
}

void WindowsControllerInputDriver::DoneInput() {}

bool WindowsControllerInputDriver::ObserveFocus() {
	return m_observeFocus;
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

bool WindowsControllerInputDriver::SetupDevice(HWND hWnd) {
	if (FAILED(m_device->EnumObjects(reinterpret_cast<LPDIENUMDEVICEOBJECTSCALLBACKW>(0x00813950), this, DIDFT_ALL))) {
		return true;
	}
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
		logger::log_format("[WindowsControllerInputDriver::LoadProfile] Unable to access folder, aborting...");
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
		logger::log_format("[WindowsControllerInputDriver::LoadProfile] Invalid binds, aborting... ({})", iInvalidKeys);
		return 1;
	}

	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		m_axisMap[i] = -1;
	}

	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		if (!GetProfileStick(i, sMapFileName)) {
			logger::log_format("[WindowsControllerInputDriver::LoadProfile] Failed to read axis: {}", i);
		}
	}

	return 0;
}

int WindowsControllerInputDriver::GetProfileButton(const char* buttonName, int* value, char* sMapFileName) {
	return WindowsControllerInputDriver_GetProfileButton(this, buttonName, value, sMapFileName);
}

bool WindowsControllerInputDriver::GetProfileStick(int stick, char* sMapFileName) {
	m_initTransferFunction[stick] = AxesTransferFunction::Linear;
	m_initDeadZone[stick] = static_cast<float>(m_iAxisDeadZoneValue);
	m_initClampZone[stick] = 5.0f;
	m_initFactor[stick] = 1.5f;
	m_invertAxis[stick] = false;

	std::string_view section = "invalid";
	switch (static_cast<AnalogAxis>(stick))
	{
	case AnalogAxis::X1:
		section = "STICK1_LEFT";
		break;
	case AnalogAxis::Y1:
		section = "STICK1_DOWN";
		break;
	case AnalogAxis::X2:
		section = "STICK2_LEFT";
		break;
	case AnalogAxis::Y2:
		section = "STICK2_DOWN";
		break;
	case AnalogAxis::Throttle:
		section = "THROTTLE";
		break;
	case AnalogAxis::Rudder:
		section = "RUDDER";
		break;
	case AnalogAxis::L2:
		section = "ANALOG_L2";
		break;
	case AnalogAxis::R2:
		section = "ANALOG_R2";
		break;
	}

	char value[64]{};
	DWORD ret = GetPrivateProfileStringA("Button Map", section.data(), "", value, sizeof(value), sMapFileName);
	std::string_view valueView = value;
	if (ret != 0 && !valueView.empty() && valueView != "not set") {
		int iAxisID = 0;
		for (; iAxisID < 8; iAxisID++) {
			if (valueView.contains(c_DefaultAxisName[iAxisID])) {
				m_axisMap[stick] = iAxisID;
				break;
			}
		}
		if (iAxisID >= 8) {
			return false;
		}
	}
	else {
		return false;
	}
	if (valueView[0] == '+') {
		m_invertAxis[stick] = true;
	}
	return true;
}