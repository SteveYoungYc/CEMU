#include <simulator.h>
#include <memory.h>
#include <sdb.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
Memory memory;
Debugger sdb;

int main(int argc, char *argv[])
{
    const char *imgFile = nullptr;
    if (argc == 2)
    {
        imgFile = argv[1];
    }
    memory.Init();
    simulator.Init();
    sdb.Init();

    simulator.LoadImg(imgFile);
    sdb.Run();
    return 0;
}
