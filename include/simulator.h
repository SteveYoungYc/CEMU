#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <device/dev_mgr.h>
#include <trace.h>
#include <riscv32/riscv32_cpu.h>
#include <riscv32/riscv32_decode.h>

class Simulator
{
private:
    const char *imgFile;

public:
    std::shared_ptr<RISCV32_CPU> cpu;

    std::unique_ptr<NormalMemory> memory;
    std::unique_ptr<IOMemory> ioMem;

    std::unique_ptr<Args> args;
    std::unique_ptr<DeviceManager> devManager;

    SimStatus simStatus;
    ITrace itrace;
    FTrace ftrace;

    Simulator();
    ~Simulator();

    ITrace *GetITrace();

    void Init(int argc, char *argv[]);
    long LoadImg();
    void Run(uint64_t n);
    void SetStatus(CEMU_Status status, uint32_t haltPC, int32_t retVal);
};

extern Simulator simulator;

#endif
