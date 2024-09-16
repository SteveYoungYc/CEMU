#ifndef __DEVICE_TIMER_H__
#define __DEVICE_TIMER_H__

#include <device.h>

#define TIMER_OFFSET    0x48

class Timer : public Device
{
private:
    uint64_t bootTime = 0;
    uint64_t GetTime();
    uint64_t GetTimeInternal();

public:
    void Init() override;
    void Callback(uint32_t, int, bool) override;
};

#endif
