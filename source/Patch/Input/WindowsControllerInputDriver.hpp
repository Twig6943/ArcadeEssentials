#pragma once
#include <dinput.h>
#include "../../Game/Input/ControllerInputDriver.hpp"

class WindowsControllerInputDriver : public ControllerInputDriver {
public:
	IDirectInputDevice8W* m_device;
	int m_axisValid[8];
	bool m_observeFocus;
	float m_transferLookup[std::to_underlying(AnalogAxis::Max)][256];
	char m_deviceName[128];
	int m_buttonMap[std::to_underlying(ControllerButton::Max)];
	int m_axisMap[std::to_underlying(AnalogAxis::Max)];
	AxesTransferFunction m_initTransferFunction[std::to_underlying(AnalogAxis::Max)];
	float m_initFactor[std::to_underlying(AnalogAxis::Max)];
	float m_initDeadZone[std::to_underlying(AnalogAxis::Max)];
	float m_initClampZone[std::to_underlying(AnalogAxis::Max)];
	bool m_invertAxis[std::to_underlying(AnalogAxis::Max)];
	int m_iAxisDeadZoneValue;
public:
	WindowsControllerInputDriver(HWND hWnd, IDirectInputDevice8W* joystickDevice, int deviceNumber, const char* pDeviceName);

	bool SetupDevice(HWND hWnd);
	int GetProfileButton(const char* buttonName, int* value, char* sMapFileName);
	bool GetProfileStick(int stick, char* sMapFileName);
	int LoadProfile(int deviceNumber);

	virtual ~WindowsControllerInputDriver() override;
	virtual void Initialize() override;
	virtual const char* DeviceName() const override;
	virtual bool SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) override;
	virtual bool Connected() override;
	inline virtual int GetUserID() override { return 0; }
	virtual void BeginInput() override;
	virtual void DoneInput() override;
	virtual void Activate(bool active) override;
	virtual bool ObserveFocus() override;
};

static_assert(sizeof(WindowsControllerInputDriver) == 0x5140);