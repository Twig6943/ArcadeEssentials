#pragma once
#include <dinput.h>
#include "ControllerInputDriver.hpp"

class WindowsControllerInputDriver : public ControllerInputDriver {
public:
	IDirectInputDevice8A* m_device;
	int m_axisValid[8];
	bool m_observeFocus;
	// Since there are 3 alignment padding bytes between `m_observeFocus` and `m_transferLookup`, we can fit two new booleans in here without changing the layout of the class!
	bool m_hasAnalogLT;
	bool m_hasAnalogRT;
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
	wchar_t m_devicePath[260];
public:
	WindowsControllerInputDriver(HWND hWnd, IDirectInputDevice8A* joystickDevice, int deviceNumber, const char* pDeviceName);

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
	virtual bool AnyButtonPressed() override;

	virtual const char* Identify() override;
};

int __fastcall LoadProfile(WindowsControllerInputDriver* _this, std::uintptr_t edx, int deviceNumber);
WindowsControllerInputDriver* __fastcall WindowsControllerInputDriver_WindowsControllerInputDriver(WindowsControllerInputDriver* _this, std::uintptr_t edx, HWND hWnd, IDirectInputDevice8A* joystickDevice, int deviceNumber, const char* pDeviceName);

static_assert(sizeof(WindowsControllerInputDriver) == 0x5140 + 520);