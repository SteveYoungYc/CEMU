#ifndef __RISCV32_CPU_H__
#define __RISCV32_CPU_H__

#include <cpu.h>

class RISCV32_CPU : public ICpu
{
private:
    static const char *regs[];
public:
    word_t gpr[32];
    word_t mtvec;
    word_t mepc;
    word_t mstatus;
    word_t mcause;
    word_t mscratch;
    word_t mie;
    word_t mip;
    word_t mtval;
    word_t misa;

    void Init();

    inline word_t *GetReg(uint32_t idx)
    {
        assert(idx >= 0 && idx < 32);
        return gpr;
    }

    word_t *GetCSRRegister(uint32_t val);
    void PrintReg() override;
    word_t RegStrToVal(const char *s, bool *success) override;
    void Reset() override;
    void Run() override;
    void DecodeAndExecute() override;
};

#endif
