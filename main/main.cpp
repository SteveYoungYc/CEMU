#include <simulator.h>
#include <memory.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
Memory memory;

int main(int argc, char *argv[])
{
    const char *imgFile = nullptr;
    if (argc == 2)
    {
        imgFile = argv[1];
    }
    memory.Init();
    simulator.Init();
    simulator.LoadImg(imgFile);
    simulator.Run(-1);
    return 0;
}
