#include <cstring>
#include <riscv32/riscv32_cpu.h>
#include <riscv32/riscv32_decode.h>

using namespace std;

const char *RISCV32_CPU::regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void RISCV32_CPU::Init()
{
    decoder = make_unique<RISCV32_Decoder>();
    Reset();
}

void RISCV32_CPU::Reset()
{
    pc = Memory::memBase;
    gpr[0] = 0;
}

union CJALInstruction {
    uint16_t raw; // 原始 16 位指令
    struct {
        uint16_t opcode : 2;    // [1:0]   操作码
        uint16_t offset5 : 1;
        uint16_t offset3_1 : 3;
        uint16_t offset7 : 1;
        uint16_t offset6 : 1;
        uint16_t offset10 : 1;
        uint16_t offset9_8 : 2;
        uint16_t offset4 : 1;
        uint16_t offset11 : 1;
        uint16_t funct3 : 3;    // [15:13] 功能码
    } fields;
};

int16_t decodeCJAL(uint16_t instr, bool &isCJAL)
{
    CJALInstruction inst;
    inst.raw = instr;

    if (inst.fields.opcode == 0b01 && inst.fields.funct3 == 0b001)
    {
        isCJAL = true;

        int16_t imm = (inst.fields.offset5 << 5) + 
                        (inst.fields.offset3_1 << 1) + 
                        (inst.fields.offset7 << 7) + 
                        (inst.fields.offset6 << 6) + 
                        (inst.fields.offset10 << 10) + 
                        (inst.fields.offset9_8 << 8) + 
                        (inst.fields.offset4 << 4) + 
                        (inst.fields.offset11 << 11);
        return imm;
    }

    isCJAL = false;
    return 0;
}

void RISCV32_CPU::Run()
{
    uint16_t instVal16 = MemPhysicalRead(decoder->snpc, sizeof(uint16_t));
    bool isCompressed;
    int16_t offset = decodeCJAL(instVal16, isCompressed);
    if (isCompressed)
    {
        decoder->dnpc += offset;
        return;
    }
    decoder->SetInstVal(Fetch(&decoder->snpc));
    DecodeAndExecute();
}

void RISCV32_CPU::DecodeAndExecute()
{
    if(decoder->DecodeAndExecute())
    {
        assert(!"Instruction not supported");
    }
}

word_t *RISCV32_CPU::GetCSRRegister(uint32_t val)
{
    switch (val)
    {
    case 0x300:
        return &mstatus;
    case 0x301:
        return &misa;
    case 0x304:
        return &mie;
    case 0x305:
        return &mtvec;
    case 0x340:
        return &mscratch;
    case 0x341:
        return &mepc;
    case 0x342:
        return &mcause;
    case 0x343:
        return &mtval;
    case 0x344:
        return &mip;
    default:
        assert(0 && "Unknown CSR address");
        return nullptr;
    }
}

void RISCV32_CPU::PrintReg()
{
    for (int i = 0; i < 32; i++)
    {
        InfoPrint("%s\t\t0x%x\n", regs[i], gpr[i]);
    }
}

word_t RISCV32_CPU::RegStrToVal(const char *s, bool *success)
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(s, regs[i]) == 0)
        {
            *success = true;
            return gpr[i];
        }
    }
    *success = false;
    return 0;
}
