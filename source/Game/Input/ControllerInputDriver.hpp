#pragma once
#include <cstdint>

class ControllerInputDriver {
public:
	virtual ~ControllerInputDriver();
public:
	inline void SetSharedController(ControllerInputDriver* sharedController) {
		*reinterpret_cast<ControllerInputDriver**>(reinterpret_cast<std::uintptr_t>(this) + 0x264) = sharedController;
	}
	inline ControllerInputDriver* GetSharedController() {
		return *reinterpret_cast<ControllerInputDriver**>(reinterpret_cast<std::uintptr_t>(this) + 0x264);
	}
};