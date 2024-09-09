#ifndef __CPU_H__
#define __CPU_H__

#include <common.h>
#include <decoder.h>

class ICpu
{
private:

public:
    word_t gpr[32];
    vaddr_t pc;
    Decoder *decoder;

    ICpu(Decoder *decoder);
    virtual ~ICpu() {};

    virtual void Reset() = 0;
    virtual void Run() = 0;

    uint32_t Fetch(vaddr_t pc);
    virtual void Decode() = 0;
    virtual void Execute() = 0;
};

class CPU
{
private:
    ICpu *cpu;
    Decoder *decoder;
public:
    CPU();
    ~CPU();
    void Run(uint64_t n);
};

extern CPU cpu;

#endif
