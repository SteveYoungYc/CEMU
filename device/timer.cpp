#include <ctime>
#include <device/timer.h>
#include <memory.h>

uint64_t Timer::GetTimeInternal()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
    uint64_t us = now.tv_sec * 1000000 + now.tv_nsec / 1000;
    return us;
}

uint64_t Timer::GetTime()
{
    if (bootTime == 0)
        bootTime = GetTimeInternal();
    uint64_t now = GetTimeInternal();
    return now - bootTime;
}

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
