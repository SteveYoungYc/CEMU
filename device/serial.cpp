#include <device/serial.h>
#include <memory.h>

void Serial::PutChar(char c)
{
    putchar(c);
}

void Serial::Init()
{
    MemRegion *region = GetIOMemory()->IOMap(this, "Serial", GetIOMemory()->IOBase + SERIAL_OFFSET, 8);
    region->space = GetIOMemory()->GetBasePtr() + SERIAL_OFFSET;
    regions.emplace_back(region);
    callbacks.emplace_back(static_cast<CallbackFunc>(&Serial::Callback));
}

void Serial::Callback(uint32_t ioOffset, int len, bool is_write)
{
    uint32_t offset = ioOffset - SERIAL_OFFSET;
    assert(len == 1);
    switch (offset)
    {
    case CH_OFFSET:
        if (is_write)
            PutChar(regions[0]->space[0]);
        else
            assert("do not support read");
        break;
    default:
        assert("do not support offset");
    }
}
