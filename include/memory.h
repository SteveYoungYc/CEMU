#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <common.h>

/*
Host: CEMU process virtual address space
Guest: CEMU memory physical address space
*/

class Memory
{
private:
    uint8_t *pmem;
    static const uint32_t size = 0x8000000;
    static const uint32_t base = 0x80000000;

    uint8_t HostRead08(uint8_t *ptr);
    uint16_t HostRead16(uint8_t *ptr);
    uint32_t HostRead32(uint8_t *ptr);
    uint64_t HostRead64(uint8_t *ptr);
    void HostWrite08(uint8_t *ptr, uint8_t data);
    void HostWrite16(uint8_t *ptr, uint16_t data);
    void HostWrite32(uint8_t *ptr, uint32_t data);
    void HostWrite64(uint8_t *ptr, uint64_t data);
    
    bool IsValidPA(paddr_t pa);

public:
    Memory();
    ~Memory();

    void Init();

    static uint32_t GetBase() { return base; }
    static uint32_t GetSize() { return size; }

    uint8_t *GuestToHost(paddr_t pa);
    paddr_t HostToGuest(uint8_t *ptr);

    uint8_t PhysicalRead08(paddr_t pa);
    uint16_t PhysicalRead16(paddr_t pa);
    uint32_t PhysicalRead32(paddr_t pa);
    uint64_t PhysicalRead64(paddr_t pa);
    word_t PhysicalRead(paddr_t pa, uint32_t len);
    word_t VirtualRead(paddr_t pa, uint32_t len);

    void PhysicalWrite08(paddr_t pa, uint8_t data);
    void PhysicalWrite16(paddr_t pa, uint16_t data);
    void PhysicalWrite32(paddr_t pa, uint32_t data);
    void PhysicalWrite64(paddr_t pa, uint64_t data);
    void PhysicalWrite(paddr_t pa, uint64_t data, uint32_t len);
    void VirtualWrite(paddr_t pa, uint64_t data, uint32_t len);
};

extern Memory memory;

#endif
