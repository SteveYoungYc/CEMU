#include <memory.h>

Memory::Memory()
{
}

Memory::~Memory()
{
    free(pmem);
    pmem = nullptr;
}

void Memory::Init()
{
    pmem = (uint8_t *)malloc(size);
    if (pmem == nullptr)
    {
        assert(0);
    }
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

void Memory::AddrCheck(paddr_t pa)
{
    bool valid = (pa >= base) && (pa < (base + size));
    if (unlikely(!valid))
    {
        assert(0);
    }
}

uint8_t Memory::PhysicalRead08(paddr_t pa)
{
    AddrCheck(pa);
    return HostRead08(GuestToHost(pa));
}

uint16_t Memory::PhysicalRead16(paddr_t pa)
{
    AddrCheck(pa);
    return HostRead16(GuestToHost(pa));
}

uint32_t Memory::PhysicalRead32(paddr_t pa)
{
    AddrCheck(pa);
    return HostRead32(GuestToHost(pa));
}

uint64_t Memory::PhysicalRead64(paddr_t pa)
{
    AddrCheck(pa);
    return HostRead64(GuestToHost(pa));
}

word_t Memory::PhysicalRead(paddr_t pa, uint32_t len)
{
    switch (len)
    {
        case 1: return PhysicalRead08(pa); break;
        case 2: return PhysicalRead16(pa); break;
        case 4: return PhysicalRead32(pa); break;
        case 8: return PhysicalRead64(pa); break;
        default:
            assert(0);
            return 0;
    }
}

word_t Memory::VirtualRead(paddr_t pa, uint32_t len)
{
    return PhysicalRead(pa, len);
}


void Memory::PhysicalWrite08(paddr_t pa, uint8_t data)
{
    AddrCheck(pa);
    HostWrite08(GuestToHost(pa), data);
}

void Memory::PhysicalWrite16(paddr_t pa, uint16_t data)
{
    AddrCheck(pa);
    HostWrite16(GuestToHost(pa), data);
}

void Memory::PhysicalWrite32(paddr_t pa, uint32_t data)
{
    AddrCheck(pa);
    HostWrite32(GuestToHost(pa), data);
}

void Memory::PhysicalWrite64(paddr_t pa, uint64_t data)
{
    AddrCheck(pa);
    HostWrite64(GuestToHost(pa), data);
}

void Memory::PhysicalWrite(paddr_t pa, uint64_t data, uint32_t len)
{
    switch (len)
    {
        case 1: PhysicalWrite08(pa, data); break;
        case 2: PhysicalWrite16(pa, data); break;
        case 4: PhysicalWrite32(pa, data); break;
        case 8: PhysicalWrite64(pa, data); break;
        default:
            assert(0);
    }
}

void Memory::VirtualWrite(paddr_t pa, uint64_t data, uint32_t len)
{
    PhysicalWrite(pa, data, len);
}
