#include <cpu.h>
#include <memory.h>
#include <log.h>
#include <utils.h>

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = memory.PhysicalRead32(*pc);
    char instBuf[64];
    if (args.isItrace)
    {
        disassemble(instBuf, 64, *pc, (uint8_t*)&instVal, 4);
        InfoPrint("[itrace] 0x%x\t\t%s\n", *pc, instBuf);
    }
    (*pc) += sizeof(uint32_t);
    return instVal;
}
