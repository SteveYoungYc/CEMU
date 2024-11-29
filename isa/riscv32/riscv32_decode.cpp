#include <limits>
#include <riscv32/riscv32_decode.h>
#include <simulator.h>

using namespace std;

RISCV32_Decoder::RISCV32_Decoder()
{
    info = make_unique<RISCV32_DecodeInfo>();
}

bool RISCV32_Decoder::HandleDiv(bool isUnsigned)
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

bool RISCV32_Decoder::HandleRem(bool isUnsigned)
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

void RISCV32_Decoder::HandleFTrace(uint32_t addr)
{
    if (info->inst.val == 0x00008067)   // ret
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

static def_DopHelper(i)
{
    op->imm = val;
}

static def_DopHelper(r)
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
        op->preg = &simulator.cpu->gpr[val];
    }
}

void RISCV32_Decoder::decode_R(int width)
{
    decode_op_r(id_src1, info->inst.r.rs1, false);
    decode_op_r(id_src2, info->inst.r.rs2, false);
    decode_op_r(id_dest, info->inst.r.rd, true);
}

void RISCV32_Decoder::decode_I(int width)
{
    decode_op_r(id_src1, info->inst.i.rs1, false);
    decode_op_i(id_src2, info->inst.i.simm11_0, false);
    decode_op_r(id_dest, info->inst.i.rd, true);
}

void RISCV32_Decoder::decode_U(int width)
{
    decode_op_i(id_src1, info->inst.u.imm31_12 << 12, true);
    decode_op_r(id_dest, info->inst.u.rd, true);
}

void RISCV32_Decoder::decode_J(int width)
{
    sword_t simm = info->inst.j.simm10_1 << 1 | info->inst.j.simm11 << 11 |
                   info->inst.j.simm19_12 << 12 | info->inst.j.simm20 << 20;
    decode_op_i(id_src1, simm, true);
    decode_op_r(id_dest, info->inst.j.rd, true);
}

void RISCV32_Decoder::decode_S(int width)
{
    decode_op_r(id_src1, info->inst.s.rs1, false);
    sword_t simm = (info->inst.s.simm11_5 << 5) | info->inst.s.imm4_0;
    decode_op_i(id_src2, simm, false);
    decode_op_r(id_dest, info->inst.s.rs2, false);
}

void RISCV32_Decoder::decode_B(int width)
{
    decode_op_r(id_src1, info->inst.b.rs1, false);
    decode_op_r(id_src2, info->inst.b.rs2, false);
    sword_t simm = (info->inst.b.imm11 << 11) | (info->inst.b.imm4_1 << 1) |
                   (info->inst.b.simm10_5 << 5) | (info->inst.b.simm12 << 12);
    decode_op_i(id_dest, simm, false);
}

DecoderFunc RISCV32_Decoder::decoderTable[] = {
    &RISCV32_Decoder::decode_R,
    &RISCV32_Decoder::decode_I,
    &RISCV32_Decoder::decode_S,
    &RISCV32_Decoder::decode_B,
    &RISCV32_Decoder::decode_U,
    &RISCV32_Decoder::decode_J,
};

InstEntry RISCV32_Decoder::cal[] = {
    {0b00000000000000000000000000000000, calMask, &RISCV32_Decoder::op_add},
    {0b01000000000000000000000000000000, calMask, &RISCV32_Decoder::op_sub},
    {0b00000000000000000100000000000000, calMask, &RISCV32_Decoder::op_xor},
    {0b00000000000000000110000000000000, calMask, &RISCV32_Decoder::op_or},
    {0b00000000000000000111000000000000, calMask, &RISCV32_Decoder::op_and},
    {0b00000000000000000001000000000000, calMask, &RISCV32_Decoder::op_sll},
    {0b00000000000000000101000000000000, calMask, &RISCV32_Decoder::op_srl},
    {0b01000000000000000101000000000000, calMask, &RISCV32_Decoder::op_sra},
    {0b00000000000000000010000000000000, calMask, &RISCV32_Decoder::op_slt},
    {0b00000000000000000011000000000000, calMask, &RISCV32_Decoder::op_sltu},
    {0b00000010000000000000000000000000, calMask, &RISCV32_Decoder::op_mul},
    {0b00000010000000000001000000000000, calMask, &RISCV32_Decoder::op_mulh},
    {0b00000010000000000010000000000000, calMask, &RISCV32_Decoder::op_mulsu},
    {0b00000010000000000011000000000000, calMask, &RISCV32_Decoder::op_mulu},
    {0b00000010000000000100000000000000, calMask, &RISCV32_Decoder::op_div},
    {0b00000010000000000101000000000000, calMask, &RISCV32_Decoder::op_divu},
    {0b00000010000000000110000000000000, calMask, &RISCV32_Decoder::op_rem},
    {0b00000010000000000111000000000000, calMask, &RISCV32_Decoder::op_remu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::lui[] = {
    {0, 0, &RISCV32_Decoder::op_lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::load[] = {
    {0b00000000000000000000000000000000, loadMask, &RISCV32_Decoder::op_lb},
    {0b00000000000000000001000000000000, loadMask, &RISCV32_Decoder::op_lh},
    {0b00000000000000000010000000000000, loadMask, &RISCV32_Decoder::op_lw},
    {0b00000000000000000100000000000000, loadMask, &RISCV32_Decoder::op_lbu},
    {0b00000000000000000101000000000000, loadMask, &RISCV32_Decoder::op_lhu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::store[] = {
    {0b00000000000000000000000000000000, storeMask, &RISCV32_Decoder::op_sb},
    {0b00000000000000000001000000000000, storeMask, &RISCV32_Decoder::op_sh},
    {0b00000000000000000010000000000000, storeMask, &RISCV32_Decoder::op_sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::imm[] = {
    {0b00000000000000000000000000000000, immMask0, &RISCV32_Decoder::op_addi},
    {0b00000000000000000100000000000000, immMask0, &RISCV32_Decoder::op_xori},
    {0b00000000000000000110000000000000, immMask0, &RISCV32_Decoder::op_ori},
    {0b00000000000000000111000000000000, immMask0, &RISCV32_Decoder::op_andi},
    {0b00000000000000000001000000000000, immMask1, &RISCV32_Decoder::op_slli},
    {0b00000000000000000101000000000000, immMask1, &RISCV32_Decoder::op_srli},
    {0b01000000000000000101000000000000, immMask1, &RISCV32_Decoder::op_srai},
    {0b00000000000000000010000000000000, immMask0, &RISCV32_Decoder::op_slti},
    {0b00000000000000000011000000000000, immMask0, &RISCV32_Decoder::op_sltiu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::branch[] = {
    {0b00000000000000000000000000000000, branchMask, &RISCV32_Decoder::op_beq},
    {0b00000000000000000001000000000000, branchMask, &RISCV32_Decoder::op_bne},
    {0b00000000000000000100000000000000, branchMask, &RISCV32_Decoder::op_blt},
    {0b00000000000000000101000000000000, branchMask, &RISCV32_Decoder::op_bge},
    {0b00000000000000000110000000000000, branchMask, &RISCV32_Decoder::op_bltu},
    {0b00000000000000000111000000000000, branchMask, &RISCV32_Decoder::op_bgeu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jump[] = {
    {0b00000000000000000000000000000000, jumpMask, &RISCV32_Decoder::op_jalr},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::auipc[] = {
    {0, 0, &RISCV32_Decoder::op_auipc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jal[] = {
    {0, 0, &RISCV32_Decoder::op_jal},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::csr[] = {
    {0b00000000000000000001000000000000, csrMask, &RISCV32_Decoder::op_csrrw},
    {0b00000000000000000010000000000000, csrMask, &RISCV32_Decoder::op_csrrs},
    {0b00000000000000000011000000000000, csrMask, &RISCV32_Decoder::op_csrrc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};


InstEntry RISCV32_Decoder::cemu_trap[] = {
    {0, 0, &RISCV32_Decoder::op_trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::systemInst[] = {
    {0b00000000000000000000000001110011, 0, &RISCV32_Decoder::op_ecall},
    {0b00110000001000000000000001110011, 0, &RISCV32_Decoder::op_mret}
};


OpcodeEntry RISCV32_Decoder::opcodeTable[] = {
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
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), InstKind::S, nullptr}
};

// System instruction
void RISCV32_Decoder::op_ecall()
{
    simulator.cpu->mepc = pc;
    simulator.cpu->mcause = 8;
    rtl_j(simulator.cpu->mtvec);
}

void RISCV32_Decoder::op_mret()
{
    rtl_j(simulator.cpu->mepc);
}

// CSR
void RISCV32_Decoder::op_csrrw()
{
    word_t *regAddr = simulator.cpu->GetCSRRegister(id_src2->imm);
    *regs0 = *regAddr;
    *regAddr = *dsrc1;
    *ddest = *regs0;
}

void RISCV32_Decoder::op_csrrs()
{
    word_t *regAddr = simulator.cpu->GetCSRRegister(id_src2->imm);
    *regs0 = *regAddr;
    *regAddr |= *dsrc1;
    *ddest = *regs0;
}

void RISCV32_Decoder::op_csrrc()
{
    word_t *regAddr = simulator.cpu->GetCSRRegister(id_src2->imm);
    *regs0 = *regAddr;
    *regAddr &= ~(*dsrc1);
    *ddest = *regs0;
}

uint32_t RISCV32_Decoder::GetInstVal()
{
    return info->inst.val;
}

void RISCV32_Decoder::SetInstVal(uint32_t val)
{
    info->inst.val = val;
}

uint32_t RISCV32_Decoder::DecodeAndExecute()
{
    for (auto sysInst : systemInst)
    {
        if (info->inst.val == sysInst.pattern)
        {
            dnpc = snpc;
            (this->*sysInst.InstExe)();
            return 0;
        }
    }
    for (OpcodeEntry *opEntry = opcodeTable; opEntry->instPtr != nullptr; opEntry++)
    {
        uint32_t masked = info->inst.val & opEntry->mask;
        if (masked == opEntry->pattern)
        {
            InstEntry *instTable = opEntry->instPtr;
            for (InstEntry *instEntry = instTable; instEntry->InstExe != nullptr; instEntry++)
            {
                uint32_t masked = info->inst.val & instEntry->mask;
                if (masked == instEntry->pattern)
                {
                    dnpc = snpc;
                    (this->*decoderTable[opEntry->kind])(4);
                    (this->*instEntry->InstExe)();
                    return 0;
                }
            }
            InfoPrint("No instrunction found. (0x%x)\n", info->inst.val);
            return 1;
        }
    }
    InfoPrint("No opcode found. (0x%x)\n", info->inst.val);
    return 1;
}
