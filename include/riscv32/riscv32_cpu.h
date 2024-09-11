#ifndef __RISCV32_CPU_H__
#define __RISCV32_CPU_H__

#include <riscv32/riscv32_decode.h>

class RISCV32_CPU : public ICpu
{
private:

public:
    word_t gpr[32];
    vaddr_t pc;
    RISCV32_Decoder *decoder;

    RISCV32_CPU(RISCV32_Decoder *decoder)
    {
        this->decoder = decoder;
    }

    inline word_t GetReg(uint32_t idx)
    {
        assert(idx >= 0 && idx < 32);
        return gpr[idx];
    }

    void Reset() override;
    void Run() override;

    void Decode() override;
    void Execute() override;
};

#endif
