#ifndef _Tilt_H
#define _Tilt_H

#include "gpaddon.h"

#include "GamepadEnums.h"

// Tilt Module Name
#define TiltName "Tilt"

class TiltInput : public GPAddon {
public:
	virtual bool available();
	virtual void setup();       // Tilt Setup
	virtual void process();     // Tilt Process
	virtual void preprocess();  // Tilt Pre-Process (Cheat)
	virtual std::string name() { return TiltName; }
private:
	void SOCDTiltClean(SOCDMode);
	uint8_t SOCDCombine(SOCDMode, uint8_t);
	uint8_t SOCDGamepadClean(uint8_t);
	void OverrideGamepad(Gamepad*, uint8_t, uint8_t);
	uint8_t tiltLeftState;          // Tilt State
	uint8_t tiltRightState;          // Tilt Right Analog State
	DpadDirection lastGPUD; // Gamepad Last Up-Down
	DpadDirection lastGPLR; // Gamepad Last Left-Right
	DpadDirection leftLastTiltUD; // Tilt Last Up-Down
	DpadDirection leftLastTiltLR; // Gamepad Last Left-Right
	DpadDirection rightLastTiltUD; // Tilt Last Up-Down
	DpadDirection rightLastTiltLR; // Gamepad Last Left-Right
	uint32_t dpadTime[4];
	uint8_t pinTilt1;
	uint8_t tilt1FactorLeftX;
    uint8_t tilt1FactorLeftY;
    uint8_t tilt1FactorRightX;
    uint8_t tilt1FactorRightY;
	uint8_t pinTilt2;
    uint8_t tilt2FactorLeftX;
    uint8_t tilt2FactorLeftY;
    uint8_t tilt2FactorRightX;
    uint8_t tilt2FactorRightY;
	uint8_t pinTiltLeftAnalogDown;
	uint8_t pinTiltLeftAnalogUp;
	uint8_t pinTiltLeftAnalogLeft;
	uint8_t pinTiltLeftAnalogRight;
	uint8_t pinTiltRightAnalogDown;
	uint8_t pinTiltRightAnalogUp;
	uint8_t pinTiltRightAnalogLeft;
	uint8_t pinTiltRightAnalogRight;
	SOCDMode tiltSOCDMode;
};

#endif  // _Tilt_H
