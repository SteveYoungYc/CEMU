#ifndef __DEV_MGR_H__
#define __DEV_MGR_H__

#include <device/serial.h>
#include <device/timer.h>
#include <device/keyboard.h>

class DeviceManager
{
public:
    Serial serial;
    Timer timer;
    KeyBoard keyboard;

    void Init();
    void Update();
};

extern DeviceManager devManager;

#endif
