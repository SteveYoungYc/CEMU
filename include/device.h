#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <common.h>
#include <region.h>
#include <vector>

class Device;

typedef void (Device::*CallbackFunc)(uint32_t, int, bool);

class Device
{
protected:
    std::vector<MemRegion*> regions;
    std::vector<CallbackFunc> callbacks;

public:
    virtual void Init() = 0;
    void ExecuteCallbacks(uint32_t ioOffset, int len, bool isWrite)
    {
        for (auto callback : callbacks)
        {
            (this->*callback)(ioOffset, len, isWrite);
        }
    }
};


#endif
