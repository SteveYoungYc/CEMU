#include <cstring>
#include <limits>
#include <riscv32/riscv32_cpu.h>

using namespace std;

SimStatus *GetStatus();

const char *RISCV32_CPU::regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

const DecoderFunc RISCV32_CPU::decoderTable[] = {
    &RISCV32_CPU::decode_R,
    &RISCV32_CPU::decode_I,
    &RISCV32_CPU::decode_S,
    &RISCV32_CPU::decode_B,
    &RISCV32_CPU::decode_U,
    &RISCV32_CPU::decode_J,
};

const InstEntry RISCV32_CPU::cal[] = {
    {0b00000000000000000000000000000000, calMask, &RISCV32_CPU::op_add},
    {0b01000000000000000000000000000000, calMask, &RISCV32_CPU::op_sub},
    {0b00000000000000000100000000000000, calMask, &RISCV32_CPU::op_xor},
    {0b00000000000000000110000000000000, calMask, &RISCV32_CPU::op_or},
    {0b00000000000000000111000000000000, calMask, &RISCV32_CPU::op_and},
    {0b00000000000000000001000000000000, calMask, &RISCV32_CPU::op_sll},
    {0b00000000000000000101000000000000, calMask, &RISCV32_CPU::op_srl},
    {0b01000000000000000101000000000000, calMask, &RISCV32_CPU::op_sra},
    {0b00000000000000000010000000000000, calMask, &RISCV32_CPU::op_slt},
    {0b00000000000000000011000000000000, calMask, &RISCV32_CPU::op_sltu},
    {0b00000010000000000000000000000000, calMask, &RISCV32_CPU::op_mul},
    {0b00000010000000000001000000000000, calMask, &RISCV32_CPU::op_mulh},
    {0b00000010000000000010000000000000, calMask, &RISCV32_CPU::op_mulsu},
    {0b00000010000000000011000000000000, calMask, &RISCV32_CPU::op_mulu},
    {0b00000010000000000100000000000000, calMask, &RISCV32_CPU::op_div},
    {0b00000010000000000101000000000000, calMask, &RISCV32_CPU::op_divu},
    {0b00000010000000000110000000000000, calMask, &RISCV32_CPU::op_rem},
    {0b00000010000000000111000000000000, calMask, &RISCV32_CPU::op_remu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::lui[] = {
    {0, 0, &RISCV32_CPU::op_lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::load[] = {
    {0b00000000000000000000000000000000, loadMask, &RISCV32_CPU::op_lb},
    {0b00000000000000000001000000000000, loadMask, &RISCV32_CPU::op_lh},
    {0b00000000000000000010000000000000, loadMask, &RISCV32_CPU::op_lw},
    {0b00000000000000000100000000000000, loadMask, &RISCV32_CPU::op_lbu},
    {0b00000000000000000101000000000000, loadMask, &RISCV32_CPU::op_lhu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::store[] = {
    {0b00000000000000000000000000000000, storeMask, &RISCV32_CPU::op_sb},
    {0b00000000000000000001000000000000, storeMask, &RISCV32_CPU::op_sh},
    {0b00000000000000000010000000000000, storeMask, &RISCV32_CPU::op_sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::imm[] = {
    {0b00000000000000000000000000000000, immMask0, &RISCV32_CPU::op_addi},
    {0b00000000000000000100000000000000, immMask0, &RISCV32_CPU::op_xori},
    {0b00000000000000000110000000000000, immMask0, &RISCV32_CPU::op_ori},
    {0b00000000000000000111000000000000, immMask0, &RISCV32_CPU::op_andi},
    {0b00000000000000000001000000000000, immMask1, &RISCV32_CPU::op_slli},
    {0b00000000000000000101000000000000, immMask1, &RISCV32_CPU::op_srli},
    {0b01000000000000000101000000000000, immMask1, &RISCV32_CPU::op_srai},
    {0b00000000000000000010000000000000, immMask0, &RISCV32_CPU::op_slti},
    {0b00000000000000000011000000000000, immMask0, &RISCV32_CPU::op_sltiu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::branch[] = {
    {0b00000000000000000000000000000000, branchMask, &RISCV32_CPU::op_beq},
    {0b00000000000000000001000000000000, branchMask, &RISCV32_CPU::op_bne},
    {0b00000000000000000100000000000000, branchMask, &RISCV32_CPU::op_blt},
    {0b00000000000000000101000000000000, branchMask, &RISCV32_CPU::op_bge},
    {0b00000000000000000110000000000000, branchMask, &RISCV32_CPU::op_bltu},
    {0b00000000000000000111000000000000, branchMask, &RISCV32_CPU::op_bgeu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::jump[] = {
    {0b00000000000000000000000000000000, jumpMask, &RISCV32_CPU::op_jalr},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::auipc[] = {
    {0, 0, &RISCV32_CPU::op_auipc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::jal[] = {
    {0, 0, &RISCV32_CPU::op_jal},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::csr[] = {
    {0b00000000000000000001000000000000, csrMask, &RISCV32_CPU::op_csrrw},
    {0b00000000000000000010000000000000, csrMask, &RISCV32_CPU::op_csrrs},
    {0b00000000000000000011000000000000, csrMask, &RISCV32_CPU::op_csrrc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::cemu_trap[] = {
    {0, 0, &RISCV32_CPU::op_trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const InstEntry RISCV32_CPU::systemInst[] = {
    {0b00000000000000000000000001110011, 0, &RISCV32_CPU::op_ecall},
    {0b00110000001000000000000001110011, 0, &RISCV32_CPU::op_mret},
    {0b00001111111100000000000000001111, 0, &RISCV32_CPU::op_fence},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}};

const OpcodeEntry RISCV32_CPU::opcodeTable[] = {
    {0b0110011, opcodeMask, InstKind::R, cal},
    {0b0110111, opcodeMask, InstKind::U, lui},
    {0b0000011, opcodeMask, InstKind::I, load},
    {0b0100011, opcodeMask, InstKind::S, store},
    {0b0010011, opcodeMask, InstKind::I, imm},
    {0b1100011, opcodeMask, InstKind::B, branch},
    {0b1100111, opcodeMask, InstKind::I, jump},
    {0b0010111, opcodeMask, InstKind::U, auipc},
    {0b1101111, opcodeMask, InstKind::J, jal},
    {0b1110011, opcodeMask, InstKind::I, csr},
    {0b1101011, opcodeMask, InstKind::U, cemu_trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), InstKind::S, nullptr}};

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

void RISCV32_CPU::decode_op_i(Operand *op, word_t val, bool flag)
{
    op->imm = val;
}

void RISCV32_CPU::decode_op_r(Operand *op, word_t val, bool flag)
{
    bool is_write = flag;
    word_t zero_null = 0;
    if (is_write && val == 0)
    {
        op->preg = &zero_null;
    }
    else
    {
        assert(val >= 0 && val < 32);
        op->preg = &gpr[val];
    }
}

void RISCV32_CPU::decode_R(int width)
{
    decode_op_r(id_src1, inst->r.rs1, false);
    decode_op_r(id_src2, inst->r.rs2, false);
    decode_op_r(id_dest, inst->r.rd, true);
}

void RISCV32_CPU::decode_I(int width)
{
    decode_op_r(id_src1, inst->i.rs1, false);
    decode_op_i(id_src2, inst->i.simm11_0, false);
    decode_op_r(id_dest, inst->i.rd, true);
}

void RISCV32_CPU::decode_U(int width)
{
    decode_op_i(id_src1, inst->u.imm31_12 << 12, true);
    decode_op_r(id_dest, inst->u.rd, true);
}

void RISCV32_CPU::decode_J(int width)
{
    sword_t simm = inst->j.simm10_1 << 1 | inst->j.simm11 << 11 |
                   inst->j.simm19_12 << 12 | inst->j.simm20 << 20;
    decode_op_i(id_src1, simm, true);
    decode_op_r(id_dest, inst->j.rd, true);
}

void RISCV32_CPU::decode_S(int width)
{
    decode_op_r(id_src1, inst->s.rs1, false);
    sword_t simm = (inst->s.simm11_5 << 5) | inst->s.imm4_0;
    decode_op_i(id_src2, simm, false);
    decode_op_r(id_dest, inst->s.rs2, false);
}

void RISCV32_CPU::decode_B(int width)
{
    decode_op_r(id_src1, inst->b.rs1, false);
    decode_op_r(id_src2, inst->b.rs2, false);
    sword_t simm = (inst->b.imm11 << 11) | (inst->b.imm4_1 << 1) |
                   (inst->b.simm10_5 << 5) | (inst->b.simm12 << 12);
    decode_op_i(id_dest, simm, false);
}

bool RISCV32_CPU::HandleDiv(bool isUnsigned)
{
    if (*dsrc2 == 0)
    {
        if (isUnsigned)
        {
            *ddest = numeric_limits<uint32_t>::max();
        }
        else
        {
            *ddest = -1;
        }
        return false;
    }
    if (!isUnsigned && (sword_t)(*dsrc1) == numeric_limits<int32_t>::min() && (sword_t)(*dsrc2) == -1)
    {
        *ddest = numeric_limits<int32_t>::min();
        return false;
    }
    return true;
}

bool RISCV32_CPU::HandleRem(bool isUnsigned)
{
    if (*dsrc2 == 0)
    {
        *ddest = *dsrc1;
        return false;
    }
    if (!isUnsigned && (sword_t)(*dsrc1) == numeric_limits<int32_t>::min() && (sword_t)(*dsrc2) == -1)
    {
        *ddest = 0;
        return false;
    }
    return true;
}

void RISCV32_CPU::HandleFTrace(uint32_t addr)
{
    if (inst->val == 0x00008067) // ret
    {
        ftrace->Return();
        return;
    }

    string name = ftrace->FindFunctionByAddress(addr);
    if (name != "")
    {
        ftrace->CallFucntion(name);
    }
}

void RISCV32_CPU::Init()
{
    decoder = make_unique<Decoder>();
    inst = make_unique<RISCV32_Inst>();
    simStatus = GetStatus();
    itrace = make_unique<ITrace>();
    ftrace = make_unique<FTrace>();
    itrace->Init("riscv32");
    ftrace->Init();
    Reset();
}

void RISCV32_CPU::Reset()
{
    pc = Memory::memBase;
    gpr[0] = 0;
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

void RISCV32_CPU::DecodeAndExecute()
{
    for (auto sysInst : systemInst)
    {
        if (inst->val == sysInst.pattern)
        {
            decoder->dnpc = decoder->snpc;
            (this->*sysInst.InstExe)();
            return;
        }
    }
    for (const OpcodeEntry *opEntry = opcodeTable; opEntry->instPtr != nullptr; opEntry++)
    {
        uint32_t masked = inst->val & opEntry->mask;
        if (masked == opEntry->pattern)
        {
            const InstEntry *instTable = opEntry->instPtr;
            for (const InstEntry *instEntry = instTable; instEntry->InstExe != nullptr; instEntry++)
            {
                uint32_t masked = inst->val & instEntry->mask;
                if (masked == instEntry->pattern)
                {
                    decoder->dnpc = decoder->snpc;
                    (this->*decoderTable[opEntry->kind])(4);
                    (this->*instEntry->InstExe)();
                    return;
                }
            }
            InfoPrint("No instrunction found. (0x%x at 0x%x)\n", inst->val, pc);
            assert(0);
        }
    }
    InfoPrint("No opcode found. (0x%x at 0x%x)\n", inst->val, pc);
    assert(0);
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
    inst->val = Fetch(&decoder->snpc);
    DecodeAndExecute();
}
