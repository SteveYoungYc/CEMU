#include <map.h>

MemMap::MemMap()
{

}

MemMap::~MemMap()
{
    for (auto region : regions)
    {
        delete region;
    }
}

bool MemMap::InBound(paddr_t pa, MemRegion *region)
{
    return (pa >= region->low && pa < region->high);
}

bool MemMap::CheckIntersection(paddr_t low, paddr_t high)
{
    for (auto region : regions)
    {
        if (InBound(low, region))
        {
            return true;
        }
        if (InBound(high, region))
        {
            return true;
        }
    }
    return false;
}

// [low, high)
MemRegion *MemMap::AddMap(Device *device, const char *name, paddr_t low, paddr_t high)
{
    assert(low < high);
    if (CheckIntersection(low, high))
    {
        return nullptr;
    }
    MemRegion *region = new MemRegion(device, name, low, high);
    regions.push_back(region);
    return region;
}

int32_t MemMap::RemoveMap(const char *name, paddr_t low)
{
    return 0;
}

MemRegion *MemMap::FindMap(paddr_t pa)
{
    for (auto region : regions)
    {
        if (InBound(pa, region))
        {
            return region;
        }
    }
    return nullptr;
}
