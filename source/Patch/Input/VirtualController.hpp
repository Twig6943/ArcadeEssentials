#include "ControllerInputDriver.hpp"

class VirtualController : public ControllerInputDriver {
protected:
	ControllerPacket m_lastPacket;
	bool m_wasFed;
public:
	VirtualController();

	void Feed(ControllerPacket& newPacket);

	virtual ~VirtualController() override;
	virtual void Initialize() override;
	inline virtual const char* DeviceName() const override {
		return "Virtual";
	}
	virtual bool SetupAxis(AnalogAxis axis, AxesTransferFunction transferFunction, float factor, float deadZonePercent, float clampZonePercent)  override;
	virtual bool Connected() override;
	inline virtual int GetUserID() override {
		return -1;
	}
	virtual void BeginInput() override;
	virtual void DoneInput() override;
	virtual bool AnyButtonPressed() override;

	inline virtual const char* Identify() override {
		return "Virtual";
	}
	inline virtual ControllerPacket SerializePacket() override {
		throw std::exception();
	}
};