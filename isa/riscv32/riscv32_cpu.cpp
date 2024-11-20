#include <cstring>
#include <riscv32/riscv32_cpu.h>
#include <memory.h>

const char *RISCV32_CPU::regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void RISCV32_CPU::Reset()
{
    pc = Memory::memBase;
    riscv32Reg.gpr[0] = 0;
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

void RISCV32_CPU::PrintReg()
{
    for (int i = 0; i < 32; i++)
    {
        InfoPrint("%s\t\t0x%x\n", regs[i], riscv32Reg.gpr[i]);
    }
}

word_t RISCV32_CPU::RegStrToVal(const char *s, bool *success)
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(s, regs[i]) == 0)
        {
            *success = true;
            return riscv32Reg.gpr[i];
        }
    }
    *success = false;
    return 0;
}
