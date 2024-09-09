#include <riscv32/riscv32_cpu.h>
#include <cpu.h>
#include <decoder.h>
#include <memory.h>

CPU cpu;

ICpu::ICpu(Decoder *decoder)
{
    this->decoder = decoder;
}

uint32_t ICpu::Fetch(vaddr_t pc)
{
    return memory.PhysicalRead32(pc);
}


CPU::CPU()
{
    decoder = new Decoder();
    cpu = new RISCV32_CPU(decoder);
}

CPU::~CPU()
{
    delete cpu;
    delete decoder;
}

void CPU::Run(uint64_t n)
{
    for (; n > 0; n--)
    {
        decoder->pc = cpu->pc;
        decoder->snpc = cpu->pc;
        cpu->Run();
        cpu->pc = decoder->dnpc;
    }
}
