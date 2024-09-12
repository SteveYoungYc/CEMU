#ifndef __CPU_H__
#define __CPU_H__

#include <common.h>

class ICpu
{
private:

public:
    const rtlreg_t rzero = 0;
    rtlreg_t tmp_reg[4];
    
    virtual ~ICpu() {};

    virtual void PrintReg() {};
    virtual void Reset() {}
    virtual void Run() {}

    uint32_t Fetch(vaddr_t *pc);
    virtual void DecodeAndExecute() {}
};

extern ICpu *baseCPU;

#endif
