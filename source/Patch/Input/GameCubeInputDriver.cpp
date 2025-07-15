#include <algorithm>
#include "../../pentane.hpp"
#include "GameCubeInputDriver.hpp"

GameCubeInputDriver::GameCubeInputDriver(unsigned long port, GameCubeAdapter* adapter) : ControllerInputDriver() {
	m_port = port;
	m_adapter = adapter;
	m_uiType = UIControllerType::GameCubeController;
	Initialize();
}

GameCubeInputDriver::~GameCubeInputDriver()
{
}

void GameCubeInputDriver::Initialize()
{
	for (auto axis = 0; axis < std::to_underlying(AnalogAxis::Max); axis++) {
		float deadZonePercent = 10;
		if (axis == std::to_underlying(AnalogAxis::X1) || axis == std::to_underlying(AnalogAxis::Y1)) {
			deadZonePercent = 10;
		}
		else if (axis == std::to_underlying(AnalogAxis::X2) || axis == std::to_underlying(AnalogAxis::Y2)) {
			deadZonePercent = 10;
		}
		else if (axis == std::to_underlying(AnalogAxis::L2) || axis == std::to_underlying(AnalogAxis::R2)) {
			deadZonePercent = 0;
		}
		SetupAxis(static_cast<AnalogAxis>(axis), AxesTransferFunction::Linear, 1, deadZonePercent, 4);
	}
}

bool GameCubeInputDriver::SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) {
	GenerateTransferFunction(m_transferLookup[std::to_underlying(axis)], 256, transferFunction, factor, deadZonePercent, clampZonePercent);
	return true;
}

bool GameCubeInputDriver::Connected() {
	return m_adapter->IsConnected(m_port);
}

void GameCubeInputDriver::BeginInput()
{
	ControllerInputDriver::BeginInput();

	if (m_adapter->IsConnected(m_port)) {
		GameCubeControllerState inputState = m_adapter->GetState(m_port);
		
		// Input Filtering (Me)
		inputState.leftTrigger = std::clamp((static_cast<double>(inputState.leftTrigger - 30) / 202.0 * 255.0), 0.0, 255.0);
		inputState.rightTrigger = std::clamp((static_cast<double>(inputState.rightTrigger - 27) / 207.0 * 255.0), 0.0, 255.0);
		inputState.cStickX = std::clamp((static_cast<double>(inputState.cStickX - 32) / 187.0 * 255.0), 0.0, 255.0);
		inputState.cStickY = std::clamp((static_cast<double>(inputState.cStickY - 55) / 185.0 * 255.0), 0.0, 255.0);
		inputState.leftStickX = std::clamp((static_cast<double>(inputState.leftStickX - 23) / 203.0 * 255.0), 0.0, 255.0);
		inputState.leftStickY = std::clamp((static_cast<double>(inputState.leftStickY) / 201.0 * 255.0), 0.0, 255.0);

		/*
		inputState.leftTrigger = std::clamp((static_cast<double>(inputState.leftTrigger - 19) / 208.0 * 255.0), 0.0, 255.0);
		inputState.rightTrigger = std::clamp((static_cast<double>(inputState.rightTrigger - 33) / 208.0 * 255.0), 0.0, 255.0);
		inputState.leftStickX = std::clamp((static_cast<double>(inputState.leftStickX - 22) / 209.0 * 255.0), 0.0, 255.0);
		inputState.leftStickY = std::clamp((static_cast<double>(inputState.leftStickY - 10) / 213.0 * 255.0), 0.0, 255.0);
		inputState.cStickX = std::clamp((static_cast<double>(inputState.cStickX - 34) / 192.0 * 255.0), 0.0, 255.0);
		inputState.cStickY = std::clamp((static_cast<double>(inputState.cStickY - 13) / 196.0 * 255.0), 0.0, 255.0);
		*/

		SetState(ControllerButton::Left, (inputState.buttons & GAMECUBE_LEFT) != 0);
		SetState(ControllerButton::Down, (inputState.buttons & GAMECUBE_DOWN) != 0);
		SetState(ControllerButton::Right, (inputState.buttons & GAMECUBE_RIGHT) != 0);
		SetState(ControllerButton::Up, (inputState.buttons & GAMECUBE_UP) != 0);

		SetState(ControllerButton::Start, (inputState.buttons & GAMECUBE_START) != 0);
		SetState(ControllerButton::Select, false);

		SetState(ControllerButton::Stick1, false);
		SetState(ControllerButton::Stick2, false);

		SetState(ControllerButton::Square, (inputState.buttons & GAMECUBE_X) != 0);
		SetState(ControllerButton::Cross, (inputState.buttons & GAMECUBE_A) != 0);
		SetState(ControllerButton::Circle, (inputState.buttons & GAMECUBE_B) != 0);
		SetState(ControllerButton::Triangle, (inputState.buttons & GAMECUBE_Y) != 0);
		SetState(ControllerButton::Back, (inputState.buttons & GAMECUBE_B) != 0);

		SetState(ControllerButton::L1, false);
		SetState(ControllerButton::R1, (inputState.buttons & GAMECUBE_Z) != 0);
		// SetState(ControllerButton::L2, (inputState.buttons & GAMECUBE_LEFT_TRIGGER) != 0);
		// SetState(ControllerButton::R2, (inputState.buttons & GAMECUBE_RIGHT_TRIGGER) != 0);
		SetState(ControllerButton::L2, inputState.leftTrigger > 30);
		SetState(ControllerButton::R2, inputState.rightTrigger > 30);

		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::X1)], 256, AnalogAxis::X1, inputState.leftStickX);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::Y1)], 256, AnalogAxis::Y1, inputState.leftStickY);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::X2)], 256, AnalogAxis::X2, inputState.cStickX);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::Y2)], 256, AnalogAxis::Y2, inputState.cStickY);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::L2)], 256, AnalogAxis::L2, inputState.leftTrigger / 2 + 128);
		SetStick(m_transferLookup[std::to_underlying(AnalogAxis::R2)], 256, AnalogAxis::R2, inputState.rightTrigger / 2 + 128);

		// logger::log_format("{},{},{},{},{},{}", (int)inputState.leftStickX, (int)inputState.leftStickY, (int)inputState.cStickX, (int)inputState.cStickY, (int)inputState.leftTrigger, (int)inputState.rightTrigger);
		UpdateAxisPairs();
	}
}

void GameCubeInputDriver::DoneInput()
{
}

void GameCubeInputDriver::Vibration(bool allow)
{
}

bool GameCubeInputDriver::SetVibration(unsigned int intensity, VibratePan pan)
{
	return false;
}

bool GameCubeInputDriver::SetVibrationDuration(unsigned int durationMilliseconds, unsigned int intensity, VibratePan pan)
{
	return false;
}

bool GameCubeInputDriver::ClearVibration(bool force)
{
	return false;
}

bool GameCubeInputDriver::GetVibration(unsigned int& intensity)
{
	return false;
}
