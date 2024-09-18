#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <device.h>

#define KEYBAORD_OFFSET    0x60

class KeyBoard : public Device
{
private:

public:
    void Init() override;
    void Callback(uint32_t, int, bool);
};

extern void send_key(uint8_t scancode, bool is_keydown);

#endif
