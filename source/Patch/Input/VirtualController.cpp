#include "VirtualController.hpp"

VirtualController::VirtualController() {
	m_lastPacket = {};
	m_wasFed = false;
}

void VirtualController::Feed(ControllerPacket& newPacket) {
	m_wasFed = true;
	m_lastPacket = newPacket;
}

VirtualController::~VirtualController() {
}

void VirtualController::Initialize() {
}

bool VirtualController::SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) {
	return false;
}

bool VirtualController::Connected() {
	return true;
}

void VirtualController::BeginInput() {
	if (m_wasFed) {
		m_state[std::to_underlying(ControllerButton::Left)].last = m_lastPacket.leftLast;
		m_state[std::to_underlying(ControllerButton::Left)].now = m_lastPacket.leftNow;
		m_state[std::to_underlying(ControllerButton::Down)].last = m_lastPacket.downLast;
		m_state[std::to_underlying(ControllerButton::Down)].now = m_lastPacket.downNow;
		m_state[std::to_underlying(ControllerButton::Right)].last = m_lastPacket.rightLast;
		m_state[std::to_underlying(ControllerButton::Right)].now = m_lastPacket.rightNow;
		m_state[std::to_underlying(ControllerButton::Up)].last = m_lastPacket.upLast;
		m_state[std::to_underlying(ControllerButton::Up)].now = m_lastPacket.upNow;

		m_state[std::to_underlying(ControllerButton::Start)].last = m_lastPacket.startLast;
		m_state[std::to_underlying(ControllerButton::Start)].now = m_lastPacket.startNow;
		m_state[std::to_underlying(ControllerButton::Select)].last = m_lastPacket.selectLast;
		m_state[std::to_underlying(ControllerButton::Select)].now = m_lastPacket.selectNow;

		m_state[std::to_underlying(ControllerButton::Stick1)].last = m_lastPacket.l3Last;
		m_state[std::to_underlying(ControllerButton::Stick1)].now = m_lastPacket.l3Now;
		m_state[std::to_underlying(ControllerButton::Stick2)].last = m_lastPacket.r3Last;
		m_state[std::to_underlying(ControllerButton::Stick2)].now = m_lastPacket.r3Now;

		m_state[std::to_underlying(ControllerButton::Square)].last = m_lastPacket.squareLast;
		m_state[std::to_underlying(ControllerButton::Square)].now = m_lastPacket.squareNow;
		m_state[std::to_underlying(ControllerButton::Cross)].last = m_lastPacket.crossLast;
		m_state[std::to_underlying(ControllerButton::Cross)].now = m_lastPacket.crossNow;
		m_state[std::to_underlying(ControllerButton::Circle)].last = m_lastPacket.circleLast;
		m_state[std::to_underlying(ControllerButton::Circle)].now = m_lastPacket.circleNow;
		m_state[std::to_underlying(ControllerButton::Triangle)].last = m_lastPacket.triangleLast;
		m_state[std::to_underlying(ControllerButton::Triangle)].now = m_lastPacket.triangleNow;

		m_state[std::to_underlying(ControllerButton::L1)].last = m_lastPacket.l1Last;
		m_state[std::to_underlying(ControllerButton::L1)].now = m_lastPacket.l1Now;
		m_state[std::to_underlying(ControllerButton::R1)].last = m_lastPacket.r1Last;
		m_state[std::to_underlying(ControllerButton::R1)].now = m_lastPacket.r1Now;

		m_state[std::to_underlying(ControllerButton::L2)].last = m_lastPacket.l2Last;
		m_state[std::to_underlying(ControllerButton::L2)].now = m_lastPacket.l2Now;
		m_state[std::to_underlying(ControllerButton::R2)].last = m_lastPacket.r2Last;
		m_state[std::to_underlying(ControllerButton::R2)].now = m_lastPacket.r2Now;

		m_rawStick[std::to_underlying(AnalogAxis::X1)] = m_lastPacket.axis[0] / 65535.0f;
		m_rawStick[std::to_underlying(AnalogAxis::Y1)] = m_lastPacket.axis[1] / 65535.0f;
		m_rawStick[std::to_underlying(AnalogAxis::X2)] = m_lastPacket.axis[2] / 65535.0f;
		m_rawStick[std::to_underlying(AnalogAxis::Y2)] = m_lastPacket.axis[3] / 65535.0f;
		m_rawStick[std::to_underlying(AnalogAxis::L2)] = m_lastPacket.axis[4] / 65535.0f;
		m_rawStick[std::to_underlying(AnalogAxis::R2)] = m_lastPacket.axis[5] / 65535.0f;
		
		m_wasFed = false;
	}
	else {
		ControllerInputDriver::BeginInput();
	}
}

void VirtualController::DoneInput() {
}

bool VirtualController::AnyButtonPressed() {
	return false;
}
