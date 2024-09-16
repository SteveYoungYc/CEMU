#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <common.h>
#include <region.h>


class Device
{
protected:
    MemRegion *region;
public:
    virtual void Init() {}
    virtual void Callback(uint32_t, int, bool) {}
};


#endif
