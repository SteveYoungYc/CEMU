#include <cpu.h>
#include <memory.h>
#include <log.h>

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = memory.PhysicalRead32(*pc);
    if (args.isItrace)
    {
        InfoPrint("[itrace] pc=0x%x\t inst=0x%x\n", *pc, instVal);
    }
    (*pc) += sizeof(uint32_t);
    return instVal;
}
