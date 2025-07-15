#include "KeyControllerInputDriver.hpp"

KeyControllerInputDriver::KeyControllerInputDriver(SystemInputDriver* sysInput, const char* mapName) : ControllerInputDriver() {
	m_sysInput = sysInput;
	LoadButtonMap(mapName);
	Initialize();
	readSettings();
}

void KeyControllerInputDriver::BeginInput() {
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::X1)], m_buttonMap[3], m_buttonMap[2]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::Y1)], m_buttonMap[0], m_buttonMap[1]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::X2)], m_buttonMap[0xc], m_buttonMap[0xb]);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::Y2)], m_buttonMap[9], m_buttonMap[10]);
	
	if (m_bMouseLook != 0) {
		processMouseInput();
	}
	
	m_stick[6] = 0.0;
	m_stick[5] = 0.0;

	KCSetState(ControllerButton::Up, m_buttonMap[0]);
	KCSetState(ControllerButton::Down, m_buttonMap[1]);
	KCSetState(ControllerButton::Left, m_buttonMap[2]);
	KCSetState(ControllerButton::Right, m_buttonMap[3]);
	KCSetState(ControllerButton::Square, m_buttonMap[0x11]);
	KCSetState(ControllerButton::Cross, m_buttonMap[0xe]);
	KCSetState(ControllerButton::Circle, m_buttonMap[0xf]);
	KCSetState(ControllerButton::Triangle, m_buttonMap[0x10]);
	KCSetState(ControllerButton::L1, m_buttonMap[0x12]);
	KCSetState(ControllerButton::R1, m_buttonMap[0x13]);
	KCSetState(ControllerButton::L2, m_buttonMap[0x14]);
	KCSetState(ControllerButton::R2, m_buttonMap[0x15]);
	KCSetState(ControllerButton::Stick1, m_buttonMap[8]);
	KCSetState(ControllerButton::Stick2, m_buttonMap[0xd]);
	KCSetState(ControllerButton::Start, m_buttonMap[0x16]);
	KCSetState(ControllerButton::Select, m_buttonMap[0x17]);

	// Fixes an issue where you wouldn't be able to drive at all when using a keyboard.
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::L2)], m_buttonMap[20], BC_INVALID);
	SetAnalog(&m_stick[std::to_underlying(AnalogAxis::R2)], m_buttonMap[21], BC_INVALID);
}

KeyControllerInputDriver* __fastcall KeyControllerInputDriver_KeyControllerInputDriver(KeyControllerInputDriver* _this, std::uintptr_t edx, SystemInputDriver* sysInput, const char* mapName) {
	new (_this) KeyControllerInputDriver(sysInput, mapName);
	return _this;
}