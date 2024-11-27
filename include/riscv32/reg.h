#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

class RISCV32_REG
{
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

    word_t *GetCSRRegister(uint32_t val)
    {
        switch (val)
        {
        case 0x300:
            return &mstatus;
        case 0x301:
            return &misa;
        case 0x304:
            return &mie;
        case 0x305:
            return &mtvec;
        case 0x340:
            return &mscratch;
        case 0x341:
            return &mepc;
        case 0x342:
            return &mcause;
        case 0x343:
            return &mtval;
        case 0x344:
            return &mip;
        default:
            assert(0 && "Unknown CSR address");
            return nullptr;
        }
    }
};

#endif
