#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <riscv32/riscv32_cpu.h>

class Simulator
{
private:
    const char *imgFile;

public:
    std::shared_ptr<CPU> cpu;
    std::shared_ptr<NormalMemory> memory;
    std::shared_ptr<IOMemory> ioMem;

    SimStatus simStatus;

    Simulator();
    ~Simulator();

    void Init();
    long LoadImg();
    void Run(uint64_t n);
    void SetStatus(CEMU_Status status, uint32_t haltPC, int32_t retVal);
};

extern Simulator simulator;

#endif
