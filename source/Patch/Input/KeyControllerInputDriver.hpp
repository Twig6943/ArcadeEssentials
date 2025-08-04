#pragma once
#include "ControllerInputDriver.hpp"
#include "../../Game/Input/SystemInputDriver.hpp"

inline auto KeyControllerInputDriver_SetAnalog = (void(__thiscall*)(class KeyControllerInputDriver*, float*, int, int))(0x00812c70);
inline auto KeyControllerInputDriver_processMouseInput = (void(__thiscall*)(class KeyControllerInputDriver*))(0x00812d50);
inline auto KeyControllerInputDriver_readSettings = (void(__thiscall*)(class KeyControllerInputDriver*))(0x00812e50);
inline auto KeyControllerInputDriver_LoadButtonMap = (bool(__thiscall*)(class KeyControllerInputDriver*, const char*))(0x00812860);

class KeyControllerInputDriver : public ControllerInputDriver {
public:
	enum class ButtonMap : int {
		DPadUp = 0,
		DPadDown,
		DPadLeft,
		DPadRight,
		Stick1Up,
		Stick1Down,
		Stick1Left,
		Stick1Right,
		ButtonStick1,
		Stick2Up,
		Stick2Down,
		Stick2Left,
		Stick2Right,
		ButtonStick2,
		Cross,
		Circle,
		Triangle,
		Square,
		L1,
		R1,
		L2,
		R2,
		Start,
		Select,
		Walk,
		CenterView,
		FirstPerson,
		Max
	};
public:
	int m_iLastMouseX;
	int m_iLastMouseY;
	int m_bMouseLook;
	int m_bInvertMouseLook;
	int m_iMouseSensitivity;
	SystemInputDriver* m_sysInput;
	float m_mouseSensitivity;
	float m_keyboardSensitivity;
	ButtonCode m_buttonMap[std::to_underlying(ButtonMap::Max)];
public:
	KeyControllerInputDriver(SystemInputDriver* sysInput, const char* mapName);
	inline virtual ~KeyControllerInputDriver() override {}
	inline virtual void Initialize() override {}
	inline virtual const char* DeviceName() const override {
		return "Keyboard";
	}
	inline virtual bool SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent) override {
		return true;
	}
	inline virtual bool Connected() override {
		return true;
	}
	inline virtual int GetUserID() override {
		return 0;
	};
	virtual void BeginInput() override;
	inline virtual void DoneInput() override {}
	inline virtual bool AnyButtonPressed() override {
		for (int i = BC_A; i < BC_BUTTONCODES; i++) {
			if (m_sysInput->ButtonPressed(static_cast<ButtonCode>(i))) {
				return true;
			}
		}
		return false;
	}
	inline virtual const char* Identify() override {
		return "Keyboard";
	}
public:
	inline void SetAnalog(float* stick, int positiveButtonCode, int negativeButtonCode) {
		KeyControllerInputDriver_SetAnalog(this, stick, positiveButtonCode, negativeButtonCode);
	}
	inline void processMouseInput() {
		KeyControllerInputDriver_processMouseInput(this);
	}
	inline void readSettings() {
		KeyControllerInputDriver_readSettings(this);
	}
	inline bool LoadButtonMap(const char* map) {
		return KeyControllerInputDriver_LoadButtonMap(this, map);
	}
	inline void KCSetState(ControllerButton cb, ButtonCode button) {
		m_state[std::to_underlying(cb)].last = m_state[std::to_underlying(cb)].now;
		m_state[std::to_underlying(cb)].now = m_sysInput->ButtonDown(button);
	}
};

KeyControllerInputDriver* __fastcall KeyControllerInputDriver_KeyControllerInputDriver(KeyControllerInputDriver* _this, std::uintptr_t edx, SystemInputDriver* sysInput, const char* mapName);