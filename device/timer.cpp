#include <ctime>
#include <device/timer.h>
#include <memory.h>

static uint64_t boot_time = 0;

static uint64_t get_time_internal()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
    uint64_t us = now.tv_sec * 1000000 + now.tv_nsec / 1000;
    return us;
}

uint64_t get_time()
{
    if (boot_time == 0)
        boot_time = get_time_internal();
    uint64_t now = get_time_internal();
    return now - boot_time;
}

void Timer::Init()
{
    region->space = ioMem.GetBasePtr() + TIMER_OFFSET;
}

void Timer::Callback(uint32_t ioOffset, int len, bool is_write)
{
    uint32_t offset = ioOffset - TIMER_OFFSET;
    assert(offset == 0 || offset == 4);
    if (!is_write && offset == 4)
    {
        uint64_t us = get_time();
        *(uint32_t*)region->space = (uint32_t)us;
        *(uint32_t*)(region->space + 4) = us >> 32;
    }
}
