#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <riscv32/riscv32_cpu.h>

class Simulator
{
private:
    RISCV32_CPU *cpu;
    RISCV32_Decoder *decoder;
public:
    Simulator();
    ~Simulator();
    void Run(uint64_t n);
};

extern Simulator simulator;

#endif
