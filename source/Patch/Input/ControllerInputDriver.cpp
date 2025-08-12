#include "ControllerInputDriver.hpp"

ControllerInputDriver::ControllerInputDriver() {
	m_allowVibration = true;
	m_headSetStatus = 0;
	m_headSetPort = -1;
	m_bInvalidController = false;
	m_iApplicationControllerID = -1;
	m_controller = nullptr;
	m_pointerValid = false;
	m_allowConnection = true;
	m_pSharedController = nullptr;

	for (auto i = 0; i < std::to_underlying(ControllerButton::Max); i++) {
		m_state[i].last = false;
		m_state[i].now = false;
	}
	for (auto i = 0; i < std::to_underlying(AnalogAxis::Max); i++) {
		m_stick[i] = 0.0f;
		m_rawStick[i] = 0.0f;
		m_axisPairs[i].axis1 = AnalogAxis::Invalid;
	}
	for (auto i = 0; i < sizeof(m_accelerometer) / sizeof(Vector3); i++) {
		m_accelerometer[i].x = 0;
		m_accelerometer[i].y = -1;
		m_accelerometer[i].z = 0;
	}
	m_pointer.x = 0;
	m_pointer.y = 0;
	m_horizon.x = 0;
	m_horizon.y = 0;

	m_uiType = UIControllerType::Xbox360Controller;
}

ControllerInputDriver::~ControllerInputDriver() {
}

bool ControllerInputDriver::ButtonPressed(ControllerButton button, bool bCheckShared) {
	bool ret = (m_state[std::to_underlying(button)].last == false && m_state[std::to_underlying(button)].now == true);
	if (bCheckShared && m_pSharedController != nullptr) {
		ret |= m_pSharedController->ButtonPressed(button, false);
	}
	return ret;
}

bool ControllerInputDriver::ButtonReleased(ControllerButton button, bool bCheckShared) {
	bool ret = (m_state[std::to_underlying(button)].last == true && m_state[std::to_underlying(button)].now == false);
	if (bCheckShared && m_pSharedController != nullptr) {
		ret |= m_pSharedController->ButtonPressed(button, false);
	}
	return ret;
}

bool ControllerInputDriver::ButtonDown(ControllerButton button, bool bCheckShared) {
	bool ret = m_state[std::to_underlying(button)].now;
	if (bCheckShared && m_pSharedController != nullptr) {
		ret |= m_pSharedController->ButtonDown(button, false);
	}
	return ret;
}

void ControllerInputDriver::BeginInput() {
	for (auto i = 0; i < std::to_underlying(ControllerButton::Max); i++) {
		m_state[i].last = m_state[i].now;
	}
}

void ControllerInputDriver::SetSecondaryController(ControllerInputDriver* controller) {
	if (controller != nullptr){
		if (controller->Connected() && controller != this) {
			m_controller = controller;
			return;
		}
	}
	m_controller = nullptr;
}

ControllerPacket ControllerInputDriver::SerializePacket() {
	ControllerPacket outPacket{};
	outPacket.leftLast = m_state[std::to_underlying(ControllerButton::Left)].last;
	outPacket.leftNow = m_state[std::to_underlying(ControllerButton::Left)].now;
	outPacket.downLast = m_state[std::to_underlying(ControllerButton::Down)].last;
	outPacket.downNow = m_state[std::to_underlying(ControllerButton::Down)].now;
	outPacket.rightLast = m_state[std::to_underlying(ControllerButton::Right)].last;
	outPacket.rightNow = m_state[std::to_underlying(ControllerButton::Right)].now;
	outPacket.upLast = m_state[std::to_underlying(ControllerButton::Up)].last;
	outPacket.upNow = m_state[std::to_underlying(ControllerButton::Up)].now;

	outPacket.startLast = m_state[std::to_underlying(ControllerButton::Start)].last;
	outPacket.startNow = m_state[std::to_underlying(ControllerButton::Start)].now;
	outPacket.selectLast = m_state[std::to_underlying(ControllerButton::Select)].last;
	outPacket.selectNow = m_state[std::to_underlying(ControllerButton::Select)].now;

	outPacket.l3Last = m_state[std::to_underlying(ControllerButton::Stick1)].last;
	outPacket.l3Now = m_state[std::to_underlying(ControllerButton::Stick1)].now;
	outPacket.r3Last = m_state[std::to_underlying(ControllerButton::Stick2)].last;
	outPacket.r3Now = m_state[std::to_underlying(ControllerButton::Stick2)].now;

	outPacket.squareLast = m_state[std::to_underlying(ControllerButton::Square)].last;
	outPacket.squareNow = m_state[std::to_underlying(ControllerButton::Square)].now;
	outPacket.crossLast = m_state[std::to_underlying(ControllerButton::Cross)].last;
	outPacket.crossNow = m_state[std::to_underlying(ControllerButton::Cross)].now;
	outPacket.circleLast = m_state[std::to_underlying(ControllerButton::Circle)].last;
	outPacket.circleNow = m_state[std::to_underlying(ControllerButton::Circle)].now;
	outPacket.triangleLast = m_state[std::to_underlying(ControllerButton::Triangle)].last;
	outPacket.triangleNow = m_state[std::to_underlying(ControllerButton::Triangle)].now;

	outPacket.l1Last = m_state[std::to_underlying(ControllerButton::L1)].last;
	outPacket.l1Now = m_state[std::to_underlying(ControllerButton::L1)].now;
	outPacket.r1Last = m_state[std::to_underlying(ControllerButton::R1)].last;
	outPacket.r1Now = m_state[std::to_underlying(ControllerButton::R1)].now;

	outPacket.l2Last = m_state[std::to_underlying(ControllerButton::L2)].last;
	outPacket.l2Now = m_state[std::to_underlying(ControllerButton::L2)].now;
	outPacket.r2Last = m_state[std::to_underlying(ControllerButton::R2)].last;
	outPacket.r2Now = m_state[std::to_underlying(ControllerButton::R2)].now;

	outPacket.axis[0] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::X1)] * 65535);
	outPacket.axis[1] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::Y1)] * 65535);
	outPacket.axis[2] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::X2)] * 65535);
	outPacket.axis[3] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::Y2)] * 65535);
	outPacket.axis[4] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::L2)] * 65535);
	outPacket.axis[5] = static_cast<unsigned short>(m_rawStick[std::to_underlying(AnalogAxis::R2)] * 65535);

	return outPacket;
}