#include <arg.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>
#include <log.h>
#include <device/dev_mgr.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
NormalMemory memory;
IOMemory ioMem;
Debugger sdb;
Args args;
Logger cemuLog;
DeviceManager devManager;

int main(int argc, char *argv[])
{
    args.ParseArgs(argc, argv);
    cemuLog.Init();
    memory.Init();
    ioMem.Init();
    simulator.Init();
    sdb.Init();
    devManager.Init();

    simulator.LoadImg();
    sdb.Run();
    return 0;
}
