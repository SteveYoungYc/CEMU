#ifndef __REGION_H__
#define __REGION_H__

#include <common.h>

class Device;

class MemRegion
{
public:
    const char *name;
    paddr_t low;
    paddr_t high;
    uint8_t *space;

    Device* device;

    MemRegion(Device* d, const char *n, paddr_t l, paddr_t h) : name(n), low(l), high(h), device(d) {}
};

#endif
