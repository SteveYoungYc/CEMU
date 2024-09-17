#include <common.h>
#include <time.h>

static uint64_t boot_time = 0;

static uint64_t GetTimeInternal()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
    uint64_t us = now.tv_sec * 1000000 + now.tv_nsec / 1000;
    return us;
}

uint64_t GetTime()
{
    if (boot_time == 0)
        boot_time = GetTimeInternal();
    uint64_t now = GetTimeInternal();
    return now - boot_time;
}
