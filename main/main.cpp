#include <simulator.h>
#include <memory.h>

int main(int argc, char *argv[])
{
    memory.Init();
    simulator.Init();
    simulator.LoadImg("/home/chen/emu/ics2021/am-kernels/tests/cpu-tests/build/quick-sort-riscv32-nemu.bin");
    simulator.Run(-1);
    return 0;
}
