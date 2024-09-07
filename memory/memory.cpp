#include <memory.h>

Memory::Memory()
{
    pmem = (uint8_t *)malloc(size);
    if (pmem == nullptr)
    {
        assert(0);
    }
}

Memory::~Memory()
{
    free(pmem);
    pmem = nullptr;
}

uint8_t *Memory::GuestToHost(paddr_t pa)
{
    return pmem + (pa - base);
}

paddr_t Memory::HostToGuest(uint8_t *ptr)
{
    return base + (ptr - pmem);
}

uint8_t Memory::HostRead08(uint8_t *ptr)
{
    return *ptr;
}

void Memory::HostWrite08(uint8_t *ptr, uint8_t data)
{
    *ptr = data;
}

uint8_t Memory::PhysicalRead08(paddr_t pa)
{
    assert(pa >= base);
    assert(pa < base + size);
    return HostRead08(GuestToHost(pa));
}

void Memory::PhysicalWrite08(paddr_t pa, uint8_t data)
{
    assert(pa >= base);
    assert(pa < base + size);
    HostWrite08(GuestToHost(pa), data);
}
