#include <ctime>
#include <device/timer.h>
#include <memory.h>

void Timer::Init()
{
    region = ioMem.IOMap(this, "Timer", ioMem.IOBase + TIMER_OFFSET, 8);
    region->space = ioMem.GetBasePtr() + TIMER_OFFSET;
}

void Timer::Callback(uint32_t ioOffset, int len, bool is_write)
{
    uint32_t offset = ioOffset - TIMER_OFFSET;
    assert(offset == 0 || offset == 4);
    if (!is_write && offset == 4)
    {
        uint64_t us = GetTime();
        *(uint32_t*)region->space = (uint32_t)us;
        *(uint32_t*)(region->space + 4) = us >> 32;
    }
}
