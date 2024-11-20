#ifndef __RISCV32_CPU_H__
#define __RISCV32_CPU_H__

#include <riscv32/riscv32_decode.h>
#include <riscv32/reg.h>

class RISCV32_CPU : public ICpu
{
private:
    static const char *regs[];
public:
    RISCV32_Decoder *decoder;
    RISCV32_REG *reg;

    RISCV32_CPU(RISCV32_Decoder *decoder, RISCV32_REG *reg)
    {
        this->decoder = decoder;
        this->reg = reg;
    }

    inline word_t *GetReg(uint32_t idx)
    {
        assert(idx >= 0 && idx < 32);
        return riscv32Reg.gpr;
    }

    void PrintReg() override;
    word_t RegStrToVal(const char *s, bool *success) override;
    void Reset() override;
    void Run() override;
    void DecodeAndExecute() override;
};

extern RISCV32_CPU riscv32CPU;

#endif
