#include <memory.h>
#include <monitor.h>

int main(int argc, char *argv[])
{
    /* Initialize the monitor. */
    Memory Memory;
    paddr_t pa = 0x80000000;
    Memory.PhysicalWrite08(pa, 6);
    printf("data: %d\n", Memory.PhysicalRead08(pa));
    return 0;
}
