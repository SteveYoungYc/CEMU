#include <arg.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>
#include <log.h>
#include <device/serial.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
Memory memory;
Debugger sdb;
Args args;
Logger cemuLog;

Serial serial;

int main(int argc, char *argv[])
{
    args.ParseArgs(argc, argv);
    cemuLog.Init();
    memory.Init();
    simulator.Init();
    sdb.Init();
    serial.Init();

    simulator.LoadImg();
    sdb.Run();
    return 0;
}
