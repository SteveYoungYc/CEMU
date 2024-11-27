#ifndef __DEV_MGR_H__
#define __DEV_MGR_H__

#include <device/serial.h>
#include <device/timer.h>
#include <device/keyboard.h>
#include <device/vga.h>

// #define HAS_SCREEN_KEYBOARD

class DeviceManager
{
private:
    bool screenEnabled = false;
public:
    Serial serial;
    Timer timer;
#ifdef HAS_SCREEN_KEYBOARD
    KeyBoard keyboard;
    VGA vga;
#endif

    void Init();
    void Update();
};

#endif
