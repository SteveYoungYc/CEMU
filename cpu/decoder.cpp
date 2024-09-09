#include "riscv32/riscv32_cpu.h"
#include <decoder.h>

Decoder::Decoder()
{
    info = new RISCV32_DecodeInfo();
}

Decoder::~Decoder()
{
    delete info;
}
