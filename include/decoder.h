#ifndef __DECODER_H__
#define __DECODER_H__

#include <common.h>


#define id_src1 (&src1)
#define id_src2 (&src2)
#define id_dest (&dest)

typedef union
{
    rtlreg_t *preg;
    word_t imm;
    sword_t simm;
} Operand;

class IDecodeInfo
{
public:

};

class Decoder
{

public:
    vaddr_t pc;
    vaddr_t snpc; // static next pc
    vaddr_t dnpc; // dynamic next pc
    Operand dest, src1, src2;

    virtual ~Decoder() {};

    virtual uint32_t Decode() = 0;
};

#endif
