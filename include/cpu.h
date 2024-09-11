#ifndef __CPU_H__
#define __CPU_H__

#include <common.h>
#include <decoder.h>

class ICpu
{
private:

public:
    const rtlreg_t rzero = 0;
    rtlreg_t tmp_reg[4];
    
    virtual ~ICpu() {};

    virtual void Reset() = 0;
    virtual void Run() = 0;

    uint32_t Fetch(vaddr_t *pc);
    virtual void Decode() = 0;
    virtual void Execute() = 0;
};

#endif
