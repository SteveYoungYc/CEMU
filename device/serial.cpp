#include <device/serial.h>
#include <memory.h>
#include <log.h>

void Serial::PutChar(char c)
{
    putchar(c);
}

void Serial::Init()
{
    region = memory.IOMap(this, "Serial", memory.IOBase + SERIAL_OFFSET, 8);
    region->space = memory.pio + SERIAL_OFFSET;
}

void Serial::Callback(uint32_t ioOffset, int len, bool is_write)
{
    uint32_t offset = ioOffset - SERIAL_OFFSET;
    // InfoPrint("Offset: 0x%x\n", offset);
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
