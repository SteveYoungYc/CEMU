#ifndef __DECODER_H__
#define __DECODER_H__

#include <common.h>

class IDecodeInfo
{
public:
    union {
        uint32_t val;
    } inst;
};

class Decoder
{
public:
    vaddr_t pc;
    vaddr_t snpc; // static next pc
    vaddr_t dnpc; // dynamic next pc
    IDecodeInfo *info;

    Decoder();
    ~Decoder();
};

#endif
