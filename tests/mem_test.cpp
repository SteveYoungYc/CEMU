#include <memory.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    Memory Memory;
    vector<paddr_t> paList = {0x80000000, 0x80000FFF, 0x87000000};
    uint8_t max08 = ~0;
    uint16_t max16 = ~0;
    uint32_t max32 = ~0;
    uint64_t max64 = ~0;
    for (uint32_t i = 0; i < max08; i++)
    {
        for (auto pa : paList)
        {
            Memory.PhysicalWrite08(pa, i);
            uint8_t data = Memory.PhysicalRead08(pa);
            // printf("08 data: %u\n", data);
            assert(data == i);
        }
    }
    printf("Physical R/W 08 passed!\n");

    for (uint32_t i = 0; i < max16; i++)
    {
        for (auto pa : paList)
        {
            Memory.PhysicalWrite16(pa, i);
            uint16_t data = Memory.PhysicalRead16(pa);
            // printf("16 data: %u\n", data);
            assert(data == i);
        }
    }
    printf("Physical R/W 16 passed!\n");

    for (uint64_t i = 0; i < max32; i = i + max16)
    {
        for (auto pa : paList)
        {
            Memory.PhysicalWrite32(pa, i);
            uint32_t data = Memory.PhysicalRead32(pa);
            // printf("32 data: %u\n", data);
            assert(data == i);
        }
    }
    printf("Physical R/W 32 passed!\n");

    uint64_t interval = max64 / 512;
    for (uint64_t i = 0; i < max64 / 2; i += interval)
    {
        for (auto pa : paList)
        {
            Memory.PhysicalWrite64(pa, i);
            uint64_t data = Memory.PhysicalRead64(pa);
            // printf("64 data: %lu\n", data);
            assert(data == i);
        }
    }
    printf("Physical R/W 64 passed!\n");
    return 0;
}
