#ifndef __CPU_H__
#define __CPU_H__

#include <common.h>

class ICpu
{
private:

public:
    vaddr_t pc;
    const rtlreg_t rzero = 0;
    rtlreg_t tmp_reg[4];
    
    virtual ~ICpu() {};

    uint32_t Fetch(vaddr_t *pc);
    virtual word_t RegStrToVal(const char *s, bool *success) { return 0; };
    virtual void PrintReg() {};
    virtual void Reset() {}
    virtual void Run() {}
    virtual void DecodeAndExecute() {}
};

extern ICpu *baseCPU;

#endif
