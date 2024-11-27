#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <trace.h>
#include <riscv32/riscv32_cpu.h>

class Simulator
{
private:
    const char *imgFile;

public:
    RISCV32_CPU *cpu;
    RISCV32_Decoder *decoder;
    SimStatus simStatus;
    ITrace itrace;
    FTrace ftrace;

    Simulator();
    ~Simulator();

    ITrace *GetITrace();

    void Init();
    long LoadImg();
    void Run(uint64_t n);
    void SetStatus(CEMU_Status status, uint32_t haltPC, int32_t retVal);
};

extern Simulator simulator;

#endif
