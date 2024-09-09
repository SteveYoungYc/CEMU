#include <memory.h>

Memory memory;

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

uint16_t Memory::HostRead16(uint8_t *ptr)
{
    return *(uint16_t *)ptr;
}

uint32_t Memory::HostRead32(uint8_t *ptr)
{
    return *(uint32_t *)ptr;
}

uint64_t Memory::HostRead64(uint8_t *ptr)
{
    return *(uint64_t *)ptr;
}

void Memory::HostWrite08(uint8_t *ptr, uint8_t data)
{
    *ptr = data;
}

void Memory::HostWrite16(uint8_t *ptr, uint16_t data)
{
    *(uint16_t *)ptr = data;
}

void Memory::HostWrite32(uint8_t *ptr, uint32_t data)
{
    *(uint32_t *)ptr = data;
}

void Memory::HostWrite64(uint8_t *ptr, uint64_t data)
{
    *(uint64_t *)ptr = data;
}


bool Memory::IsValidPA(paddr_t pa)
{
    return (pa >= base) && (pa < (base + size));
}

uint8_t Memory::PhysicalRead08(paddr_t pa)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    return HostRead08(GuestToHost(pa));
}

uint16_t Memory::PhysicalRead16(paddr_t pa)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    return HostRead16(GuestToHost(pa));
}

uint32_t Memory::PhysicalRead32(paddr_t pa)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    return HostRead32(GuestToHost(pa));
}

uint64_t Memory::PhysicalRead64(paddr_t pa)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    return HostRead64(GuestToHost(pa));
}

void Memory::PhysicalWrite08(paddr_t pa, uint8_t data)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    HostWrite08(GuestToHost(pa), data);
}

void Memory::PhysicalWrite16(paddr_t pa, uint16_t data)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    HostWrite16(GuestToHost(pa), data);
}

void Memory::PhysicalWrite32(paddr_t pa, uint32_t data)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    HostWrite32(GuestToHost(pa), data);
}

void Memory::PhysicalWrite64(paddr_t pa, uint64_t data)
{
    if (!IsValidPA(pa))
    {
        assert(0);
    }
    HostWrite64(GuestToHost(pa), data);
}
