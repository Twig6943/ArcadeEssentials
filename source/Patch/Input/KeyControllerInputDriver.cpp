#include "KeyControllerInputDriver.hpp"

KeyControllerInputDriver::KeyControllerInputDriver(SystemInputDriver* sysInput, const char* mapName) : ControllerInputDriver() {
	m_sysInput = sysInput;
	LoadButtonMap(mapName);
	Initialize();
	readSettings();
}

void KeyControllerInputDriver::BeginInput() {
	// SetAnalog(&m_stick[std::to_underlying(AnalogAxis::X1)], m_buttonMap[std::to_underlying(ButtonMap::DPadRight)], m_buttonMap[std::to_underlying(ButtonMap::DPadLeft)]);
	// SetAnalog(&m_stick[std::to_underlying(AnalogAxis::Y1)], m_buttonMap[std::to_underlying(ButtonMap::DPadUp)], m_buttonMap[std::to_underlying(ButtonMap::DPadDown)]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::X1)], m_buttonMap[std::to_underlying(ButtonMap::Stick1Right)], m_buttonMap[std::to_underlying(ButtonMap::Stick1Left)]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::Y1)], m_buttonMap[std::to_underlying(ButtonMap::Stick1Up)], m_buttonMap[std::to_underlying(ButtonMap::Stick1Down)]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::X2)], m_buttonMap[std::to_underlying(ButtonMap::Stick2Right)], m_buttonMap[std::to_underlying(ButtonMap::Stick2Left)]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::Y2)], m_buttonMap[std::to_underlying(ButtonMap::Stick2Up)], m_buttonMap[std::to_underlying(ButtonMap::Stick2Down)]);

	if (m_bMouseLook != 0) {
		processMouseInput();
	}

	m_stick[6] = 0.0;
	m_stick[5] = 0.0;

	KCSetState(ControllerButton::Up, m_buttonMap[std::to_underlying(ButtonMap::DPadUp)]);
	KCSetState(ControllerButton::Down, m_buttonMap[std::to_underlying(ButtonMap::DPadDown)]);
	KCSetState(ControllerButton::Left, m_buttonMap[std::to_underlying(ButtonMap::DPadLeft)]);
	KCSetState(ControllerButton::Right, m_buttonMap[std::to_underlying(ButtonMap::DPadRight)]);
	KCSetState(ControllerButton::Square, m_buttonMap[std::to_underlying(ButtonMap::Square)]);
	KCSetState(ControllerButton::Cross, m_buttonMap[std::to_underlying(ButtonMap::Cross)]);
	KCSetState(ControllerButton::Circle, m_buttonMap[std::to_underlying(ButtonMap::Circle)]);
	KCSetState(ControllerButton::Triangle, m_buttonMap[std::to_underlying(ButtonMap::Triangle)]);
	KCSetState(ControllerButton::L1, m_buttonMap[std::to_underlying(ButtonMap::L1)]);
	KCSetState(ControllerButton::R1, m_buttonMap[std::to_underlying(ButtonMap::R1)]);
	KCSetState(ControllerButton::L2, m_buttonMap[std::to_underlying(ButtonMap::L2)]);
	KCSetState(ControllerButton::R2, m_buttonMap[std::to_underlying(ButtonMap::R2)]);
	KCSetState(ControllerButton::Stick1, m_buttonMap[std::to_underlying(ButtonMap::ButtonStick1)]);
	KCSetState(ControllerButton::Stick2, m_buttonMap[std::to_underlying(ButtonMap::ButtonStick2)]);
	KCSetState(ControllerButton::Start, m_buttonMap[std::to_underlying(ButtonMap::Start)]);
	KCSetState(ControllerButton::Select, m_buttonMap[std::to_underlying(ButtonMap::Select)]);

	// Fixes an issue where you wouldn't be able to drive at all when using a keyboard.
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::L2)], m_buttonMap[std::to_underlying(ButtonMap::L2)], BC_INVALID);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::R2)], m_buttonMap[std::to_underlying(ButtonMap::R2)], BC_INVALID);
}

KeyControllerInputDriver* __fastcall KeyControllerInputDriver_KeyControllerInputDriver(KeyControllerInputDriver* _this, std::uintptr_t edx, SystemInputDriver* sysInput, const char* mapName) {
	new (_this) KeyControllerInputDriver(sysInput, mapName);
	return _this;
}