#include <device/serial.h>
#include <memory.h>
#include <log.h>

void Serial::PutChar(char c)
{
    putchar(c);
}

void Serial::Init()
{
    region = ioMem.IOMap(this, "Serial", ioMem.IOBase + SERIAL_OFFSET, 8);
    region->space = ioMem.GetBasePtr() + SERIAL_OFFSET;
}

void Serial::Callback(uint32_t ioOffset, int len, bool is_write)
{
    uint32_t offset = ioOffset - SERIAL_OFFSET;
    assert(len == 1);
    switch (offset)
    {
    case CH_OFFSET:
        if (is_write)
            PutChar(region->space[0]);
        else
            assert("do not support read");
        break;
    default:
        assert("do not support offset");
    }
}
