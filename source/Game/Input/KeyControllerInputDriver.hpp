#pragma once
#include "ControllerInputDriver.hpp"
#include "SystemInputDriver.hpp"

inline auto KeyControllerInputDriver_SetAnalog = (void(__thiscall*)(class KeyControllerInputDriver*, float*, int, int))(0x00812c70);

class KeyControllerInputDriver : public ControllerInputDriver {
public:
	int m_iLastMouseX;
	int m_iLastMouseY;
	int m_bMouseLook;
	int m_bInvertMouseLook;
	int m_iMouseSensitivity;
	SystemInputDriver* m_sysInput;
	float m_mouseSensitivity;
	float m_keyboardSensitivity;
	int m_buttonMap[27];
public:
	inline void SetAnalog(float* stick, int positiveButtonCode, int negativeButtonCode) {
		KeyControllerInputDriver_SetAnalog(this, stick, positiveButtonCode, negativeButtonCode);
	}
};