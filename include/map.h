#ifndef __MAP_H__
#define __MAP_H__

#include <common.h>
#include <device.h>
#include <vector>


class MemMap
{
private:
    std::vector<MemRegion *> regions;

    bool InBound(paddr_t pa, MemRegion *area);
    bool CheckIntersection(paddr_t low, paddr_t high);
public:
    MemMap();
    ~MemMap();

    MemRegion *AddMap(Device *device, const char *name, paddr_t low, paddr_t high);
    int32_t RemoveMap(const char *name, paddr_t low);
    MemRegion *FindMap(paddr_t pa);
};

#endif
