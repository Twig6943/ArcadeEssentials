#include <Windows.h>
#include <winerror.h>
#include <Xinput.h>
#include <algorithm>
#include "XInputInputDriver.hpp"

XInputInputDriver::XInputInputDriver(unsigned long userIndex) : ControllerInputDriver() {
	m_dwUserIndex = userIndex;
	Initialize();
	m_vibrationEndTime = 0;
	m_vibrationIntensity = 0;
}

XInputInputDriver::~XInputInputDriver() {
}

void XInputInputDriver::Initialize() {
	for (auto axis = 0; axis < std::to_underlying(AnalogAxis::Max); axis++) {
		float deadZonePercent = 32;
		if (axis == std::to_underlying(AnalogAxis::X1) || axis == std::to_underlying(AnalogAxis::Y1)) {
			deadZonePercent = (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.0f) * 100;
		}
		else if (axis == std::to_underlying(AnalogAxis::X2) || axis == std::to_underlying(AnalogAxis::Y2)) {
			deadZonePercent = (XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.0f) * 100;
		}
		else if (axis == std::to_underlying(AnalogAxis::L2) || axis == std::to_underlying(AnalogAxis::R2)) {
			deadZonePercent = 0;
		}
		SetupAxis(static_cast<AnalogAxis>(axis), AxesTransferFunction::Linear, 1, deadZonePercent, 4);
	}
}

bool XInputInputDriver::SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) {
	GenerateTransferFunction(m_transferLookup[std::to_underlying(axis)], 256, transferFunction, factor, deadZonePercent, clampZonePercent);
	return true;
}

bool XInputInputDriver::Connected() {
	XINPUT_CAPABILITIES capabilities{};
	return XInputGetCapabilities(m_dwUserIndex, XINPUT_FLAG_GAMEPAD, &capabilities) == ERROR_SUCCESS;
}

void XInputInputDriver::BeginInput() {
	ControllerInputDriver::BeginInput();

	XINPUT_STATE inputState{};

	if (XInputGetState(m_dwUserIndex, &inputState) == 0) {
		SetState(ControllerButton::Left, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
		SetState(ControllerButton::Down, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
		SetState(ControllerButton::Right, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
		SetState(ControllerButton::Up, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);

		SetState(ControllerButton::Start, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
		SetState(ControllerButton::Select, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);

		SetState(ControllerButton::Stick1, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
		SetState(ControllerButton::Stick2, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);

		SetState(ControllerButton::Square, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
		SetState(ControllerButton::Cross, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
		SetState(ControllerButton::Circle, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
		SetState(ControllerButton::Triangle, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
		SetState(ControllerButton::Back, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);

		SetState(ControllerButton::L1, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
		SetState(ControllerButton::R1, (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
		SetState(ControllerButton::L2, (inputState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
		SetState(ControllerButton::R2, (inputState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));

		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::X1)], 256, AnalogAxis::X1, (inputState.Gamepad.sThumbLX / 256) + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::Y1)], 256, AnalogAxis::Y1, (inputState.Gamepad.sThumbLY / 256) + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::X2)], 256, AnalogAxis::X2, (inputState.Gamepad.sThumbRX / 256) + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::Y2)], 256, AnalogAxis::Y2, (inputState.Gamepad.sThumbRY / 256) + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::L2)], 256, AnalogAxis::L2, (inputState.Gamepad.bLeftTrigger / 2) + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::R2)], 256, AnalogAxis::R2, (inputState.Gamepad.bRightTrigger / 2) + 128);
		SimulatePointer();
		UpdateAxisPairs();
	}
}

void XInputInputDriver::DoneInput() {
	if (m_vibrationEndTime > 0 && m_vibrationEndTime <= *reinterpret_cast<unsigned int*>(0x01925e38)) {
		ClearVibration(true);
	}
}

void XInputInputDriver::Vibration(bool allow) {
	ControllerInputDriver::Vibration(allow);
	
	if ((!m_allowVibration || !*reinterpret_cast<bool*>(0x0185ddf5)) && m_vibrationIntensity != 0) {
		ClearVibration(true);
	}
}

bool XInputInputDriver::SetVibration(unsigned int intensity, VibratePan pan) {
	if ((!m_allowVibration || !*reinterpret_cast<bool*>(0x0185ddf5)) && intensity != 0) {
		return false;
	}
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = static_cast<unsigned short>(65535 * ((intensity & 0xFF) / 200.0f));
	vibration.wRightMotorSpeed = ((intensity & 256) != 0) ? 65535 : 0;
	if (XInputSetState(m_dwUserIndex, &vibration) == ERROR_SUCCESS) {
		m_vibrationIntensity = intensity;
		m_vibrationEndTime = 0;
		return true;
	}

	return false;
}

bool XInputInputDriver::SetVibrationDuration(unsigned int durationMilliseconds, unsigned int intensity, VibratePan pan) {
	if (SetVibration(intensity, pan)) {
		m_vibrationEndTime = *reinterpret_cast<unsigned int*>(0x01925e38) + durationMilliseconds;
		return true;
	}

	return false;
}

bool XInputInputDriver::ClearVibration(bool force) {
	if (m_vibrationIntensity == 0) {
		return false;
	}
	if (m_vibrationEndTime == 0 || force) {
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		if (XInputSetState(m_dwUserIndex, &vibration) == ERROR_SUCCESS) {
			m_vibrationIntensity = 0;
			m_vibrationEndTime = 0;
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

bool XInputInputDriver::GetVibration(unsigned int& intensity) {
	if (m_vibrationIntensity == 0) {
		return false;
	}
	intensity = m_vibrationIntensity;
	return true;
}

void XInputInputDriver::SimulatePointer() {
	float x = m_stick[std::to_underlying(AnalogAxis::X2)] * 0.025f;
	float y = m_stick[std::to_underlying(AnalogAxis::Y2)] * 0.025f;

	Vector2 newPointer{};
	newPointer.X(std::clamp(m_pointer.X() + x, -1.0f, 1.0f));
	newPointer.Y(std::clamp(m_pointer.Y() - y, -1.0f, 1.0f));

	m_pointer = newPointer;
	m_horizon = Vector2();
	m_pointerValid = true;
}
