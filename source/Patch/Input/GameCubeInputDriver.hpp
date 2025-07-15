#pragma once
#include "ControllerInputDriver.hpp"
#include "GameCubeAdapter.hpp"

class GameCubeInputDriver : public ControllerInputDriver {
public:
	int m_port;
	GameCubeAdapter* m_adapter;
	float m_transferLookup[19][256];
public:
	GameCubeInputDriver(unsigned long port, GameCubeAdapter* adapter);
	virtual ~GameCubeInputDriver() override;

	virtual void Initialize() override;
	inline virtual const char* DeviceName() const override {
		return "Nintendo GameCube Controller";
	}
	virtual bool SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent)  override;
	virtual bool Connected() override;
	inline virtual int GetUserID() override {
		return m_port;
	}
	virtual void BeginInput() override;
	virtual void DoneInput() override;

	virtual void Vibration(bool allow) override;
	virtual bool SetVibration(unsigned int intensity, VibratePan pan) override;
	virtual bool SetVibrationDuration(unsigned int durationMilliseconds, unsigned int intensity, VibratePan pan) override;
	virtual bool ClearVibration(bool force = false) override;
	virtual bool GetVibration(unsigned int& intensity) override;
};