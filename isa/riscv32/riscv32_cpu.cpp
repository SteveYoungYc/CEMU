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
    Decode();
    Execute();
}

void RISCV32_CPU::Decode()
{

}

void RISCV32_CPU::Execute()
{

}
