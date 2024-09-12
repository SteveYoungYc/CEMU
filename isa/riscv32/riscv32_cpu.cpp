#include <riscv32/riscv32_cpu.h>
#include <memory.h>

void RISCV32_CPU::Reset()
{
    pc = Memory::GetBase();
    gpr[0] = 0;
}

void RISCV32_CPU::Run()
{
    decoder->info->inst.val = Fetch(&decoder->snpc);
    DecodeAndExecute();
}

void RISCV32_CPU::DecodeAndExecute()
{
    if(decoder->DecodeAndExecute())
    {
        assert(!"Instruction not supported");
    }
}
