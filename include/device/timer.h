#ifndef __DEVICE_TIMER_H__
#define __DEVICE_TIMER_H__

#include <device.h>

#define TIMER_OFFSET    0x48

class Timer : public Device
{
public:
    void Init() override;
    void Callback(uint32_t, int, bool);
};

#endif
