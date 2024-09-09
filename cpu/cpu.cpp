#include <cpu.h>
#include <memory.h>

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = memory.PhysicalRead32(*pc);
    (*pc) += sizeof(uint32_t);
    return instVal;
}
