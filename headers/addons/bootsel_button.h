#ifndef _BootselButton_H
#define _BootselButton_H

#include "gpaddon.h"
#include "gamepad.h"

// BootselButton Module Name
#define BootselButtonName "BootselButton"

class BootselButtonAddon : public GPAddon {
public:
    virtual bool available();
    virtual void setup();       // BootselButton Setup
    virtual void process() {}     // BootselButton Process
    virtual void preprocess();
    virtual void postprocess(bool sent) {}
    virtual void reinit() {}
    virtual std::string name() { return BootselButtonName; }
private:	
    bool isBootselPressed();
    uint32_t bootselButtonMap;
};

#endif  // _BootselButton_H_
