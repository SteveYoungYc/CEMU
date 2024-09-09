#ifndef __DECODER_H__
#define __DECODER_H__

#include <common.h>

enum InstKind
{
    R,
    I,
    S,
    B,
    U,
    J,
};

struct Level2
{
    const uint32_t pattern;
    const uint32_t mask;
    void (*InstExe)();
};

struct Level1
{
    const uint32_t pattern;
    const uint32_t mask;
    InstKind kind;
    Level2 *l2Ptr;
};

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
