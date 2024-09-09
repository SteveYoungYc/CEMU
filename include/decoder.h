#ifndef __DECODER_H__
#define __DECODER_H__

#include <common.h>

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

    virtual ~Decoder() {};

    virtual uint32_t Decode() = 0;
};

#endif
