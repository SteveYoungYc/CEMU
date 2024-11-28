#include <arg.h>
#include <cpu.h>

using namespace std;

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = MemPhysicalRead(*pc, sizeof(uint32_t));
    if (Args::Instance().isItrace)
    {
        decoder->itrace->Record(*pc, (uint8_t *)&instVal);
    }
    (*pc) += sizeof(uint32_t);
    return instVal;
}
