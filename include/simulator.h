#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <riscv32/riscv32_cpu.h>

class Simulator
{
private:
    const char *imgFile;

public:
    RISCV32_CPU *cpu;
    RISCV32_Decoder *decoder;
    CEMU_Status status;

    Simulator();
    ~Simulator();

    void Init();
    long LoadImg(const char *imgFile);
    void Run(uint64_t n);
};

extern Simulator simulator;

#endif
