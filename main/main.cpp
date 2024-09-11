#include <monitor.h>
#include <cpu.h>
#include <simulator.h>
#include <memory.h>

int main(int argc, char *argv[])
{
    simulator.LoadImg("/home/chen/emu/ics2021/am-kernels/tests/cpu-tests/build/quick-sort-riscv32-nemu.bin");
    simulator.Run(-1);
    return 0;
}
