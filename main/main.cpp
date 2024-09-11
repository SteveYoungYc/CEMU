#include <simulator.h>
#include <memory.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
Memory memory;

int main(int argc, char *argv[])
{
    memory.Init();
    simulator.Init();
    simulator.LoadImg("/home/chen/emu/ics2021/am-kernels/tests/cpu-tests/build/quick-sort-riscv32-nemu.bin");
    simulator.Run(-1);
    return 0;
}
