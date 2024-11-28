#include <simulator.h>

using namespace std;

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = simulator.memory->PhysicalRead32(*pc);
    if (Args::Instance().isItrace)
    {
        simulator.itrace.Record(*pc, (uint8_t*)&instVal);
    }
    (*pc) += sizeof(uint32_t);
    return instVal;
}
