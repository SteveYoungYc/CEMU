#include <riscv32/riscv32_cpu.h>
#include <cpu.h>
#include <decoder.h>
#include <memory.h>
#include <simulator.h>

Simulator simulator;

Simulator::Simulator()
{
    decoder = new RISCV32_Decoder();
    cpu = new RISCV32_CPU(decoder);
    cpu->Reset();
}

Simulator::~Simulator()
{
    delete cpu;
    delete decoder;
}

void Simulator::Run(uint64_t n)
{
    for (; n > 0; n--)
    {
        decoder->pc = cpu->pc;
        decoder->snpc = cpu->pc;
        cpu->Run();
        cpu->pc = decoder->dnpc;
    }
}
