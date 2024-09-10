#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <riscv32/riscv32_cpu.h>

class Simulator
{
private:
    RISCV32_CPU *cpu;
    RISCV32_Decoder *decoder;
    const char *imgFile;

public:
    static CEMU_Status status;

    Simulator();
    ~Simulator();

    long LoadImg(const char *imgFile);
    void Run(uint64_t n);
};

#endif
