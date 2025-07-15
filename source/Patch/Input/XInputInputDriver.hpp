#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "ControllerInputDriver.hpp"

typedef struct _XINPUT_CAPABILITIES_EX {
	BYTE Type;
	BYTE SubType;
	WORD Flags;
	XINPUT_GAMEPAD Gamepad;
	XINPUT_VIBRATION Vibration;
	WORD VendorId;
	WORD ProductId;
	WORD VersionNumber;
	WORD Unknown1;
	DWORD Unknown2;
} XINPUT_CAPABILITIES_EX, *PXINPUT_CAPABILITIES_EX;

DWORD WINAPI XInputGetCapabilitiesEx(DWORD dwReserved, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilitiesEx);

class XInputInputDriver : public ControllerInputDriver {
public:
	unsigned int m_dwUserIndex;
	float m_transferLookup[19][256];
	unsigned int m_vibrationEndTime;
	unsigned int m_vibrationIntensity;
public:
	XInputInputDriver(unsigned long userIndex);
	virtual ~XInputInputDriver() override;
	virtual void Initialize() override;
	inline virtual const char* DeviceName() const override {
		return "XInput";
	}
	virtual bool SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent)  override;
	virtual bool Connected() override;
	inline virtual int GetUserID() override {
		return m_dwUserIndex;
	}
	virtual void BeginInput() override;
	virtual void DoneInput() override;
	virtual void Vibration(bool allow) override;
	virtual bool SetVibration(unsigned int intensity, VibratePan pan) override;
	virtual bool SetVibrationDuration(unsigned int durationMilliseconds, unsigned int intensity, VibratePan pan) override;
	virtual bool ClearVibration(bool force = false) override;
	virtual bool GetVibration(unsigned int& intensity) override;
	virtual bool AnyButtonPressed() override;

	void SimulatePointer();

	virtual const char* Identify() override;
};

XInputInputDriver* __fastcall XInputInputDriver_XInputInputDriver(XInputInputDriver* _this, std::uintptr_t edx, unsigned long userIndex);

static_assert(sizeof(XInputInputDriver) == 0x4e78);