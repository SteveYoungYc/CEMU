#include <riscv32/riscv32_cpu.h>
#include <cpu.h>
#include <decoder.h>
#include <memory.h>
#include <simulator.h>
#include <string.h>


static const uint32_t img [] = {
  0x800002b7,  // lui t0,0x80000
  0x0002a023,  // sw  zero,0(t0)
  0x0002a503,  // lw  a0,0(t0)
  0x0000006b,  // nemu_trap
};

CEMU_Status Simulator::status;

Simulator::Simulator()
{
    decoder = new RISCV32_Decoder();
    cpu = new RISCV32_CPU(decoder);

    memcpy(memory.GuestToHost(Memory::GetBase()), img, sizeof(img));
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
        if (status != RUNNING)
            return;
        cpu->pc = decoder->dnpc;
    }
}
