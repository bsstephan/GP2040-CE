#ifndef _PlayerNum_H
#define _PlayerNum_H

#include "gpaddon.h"

#include "GamepadEnums.h"

#include "BoardConfig.h"

// Analog Module Name
#define PlayerNumName "PlayerNum"

class PlayerNumAddon : public GPAddon {
public:
	virtual bool available();
	virtual void setup();       // Analog Setup
	virtual void process();     // Analog Process
	virtual void preprocess() {}
    virtual std::string name() { return PlayerNumName; }
private:
	void handleLED(int);
	uint8_t assigned;
	uint8_t playerNum;
	uint8_t xinputIDs[4];
};

#endif  // _PlayerNum_H
