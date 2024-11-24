#include <cpu.h>
#include <cstring>
#include <memory.h>
#include <log.h>
#include <trace.h>
#include <queue>
#include <simulator.h>

using namespace std;

uint32_t ICpu::Fetch(vaddr_t *pc)
{
    uint32_t instVal = memory.PhysicalRead32(*pc);
    if (args.isItrace)
    {
        simulator.GetITrace()->Record(*pc, (uint8_t*)&instVal);
    }
    (*pc) += sizeof(uint32_t);
    return instVal;
}
