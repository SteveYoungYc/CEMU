#ifndef __RISCV32_CPU_H__
#define __RISCV32_CPU_H__

#include <common.h>
#include <cpu.h>
#include <decoder.h>

class RISCV32_DecodeInfo : public IDecodeInfo
{
public:

};

class RISCV32_CPU : public ICpu
{
private:

public:
    RISCV32_CPU(Decoder *decoder) : ICpu(decoder) {};

    void Reset() override;
    void Run() override;

    void Decode() override;
    void Execute() override;
};

#endif
