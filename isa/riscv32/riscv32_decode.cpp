#include <riscv32/riscv32_decode.h>
#include <riscv32/riscv32_inst.h>
#include <simulator.h>


RISCV32_Decoder::RISCV32_Decoder()
{
    info = new RISCV32_DecodeInfo();
}

RISCV32_Decoder::~RISCV32_Decoder()
{
    delete info;
}

static def_DopHelper(i)
{
    op->imm = val;
}

static def_DopHelper(r)
{
    bool is_write = flag;
    word_t zero_null = 0;
    word_t reg = simulator.cpu->GetReg(val);
    op->preg = (is_write && val == 0) ? &zero_null : &reg;
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

InstEntry RISCV32_Decoder::cal[] = {
    {0b00000000000000000000000000000000, calMask, RISCV32_Inst::add},
    {0b01000000000000000000000000000000, calMask, RISCV32_Inst::sub},
    {0b00000000000000000100000000000000, calMask, RISCV32_Inst::_xor},
    {0b00000000000000000110000000000000, calMask, RISCV32_Inst::_or},
    {0b00000000000000000111000000000000, calMask, RISCV32_Inst::_and},
    {0b00000000000000000001000000000000, calMask, RISCV32_Inst::sll},
    {0b00000000000000000101000000000000, calMask, RISCV32_Inst::srl},
    {0b01000000000000000101000000000000, calMask, RISCV32_Inst::sra},
    {0b00000000000000000010000000000000, calMask, RISCV32_Inst::slt},
    {0b00000000000000000011000000000000, calMask, RISCV32_Inst::sltu},
    {0b00000010000000000000000000000000, calMask, RISCV32_Inst::mul},
    {0b00000010000000000001000000000000, calMask, RISCV32_Inst::mulh},
    {0b00000010000000000010000000000000, calMask, RISCV32_Inst::mulsu},
    {0b00000010000000000011000000000000, calMask, RISCV32_Inst::mulu},
    {0b00000010000000000100000000000000, calMask, RISCV32_Inst::div},
    {0b00000010000000000101000000000000, calMask, RISCV32_Inst::divu},
    {0b00000010000000000110000000000000, calMask, RISCV32_Inst::rem},
    {0b00000010000000000111000000000000, calMask, RISCV32_Inst::remu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::lui[] = {
    {0, 0, RISCV32_Inst::_lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::load[] = {
    {0b00000000000000000000000000000000, loadMask, RISCV32_Inst::lb},
    {0b00000000000000000001000000000000, loadMask, RISCV32_Inst::lh},
    {0b00000000000000000010000000000000, loadMask, RISCV32_Inst::lw},
    {0b00000000000000000100000000000000, loadMask, RISCV32_Inst::lbu},
    {0b00000000000000000101000000000000, loadMask, RISCV32_Inst::lhu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::store[] = {
    {0b00000000000000000000000000000000, storeMask, RISCV32_Inst::sb},
    {0b00000000000000000001000000000000, storeMask, RISCV32_Inst::sh},
    {0b00000000000000000010000000000000, storeMask, RISCV32_Inst::sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::imm[] = {
    {0b00000000000000000000000000000000, immMask0, RISCV32_Inst::addi},
    {0b00000000000000000100000000000000, immMask0, RISCV32_Inst::xori},
    {0b00000000000000000110000000000000, immMask0, RISCV32_Inst::ori},
    {0b00000000000000000111000000000000, immMask0, RISCV32_Inst::andi},
    {0b00000000000000000001000000000000, immMask1, RISCV32_Inst::slli},
    {0b00000000000000000101000000000000, immMask1, RISCV32_Inst::srli},
    {0b01000000000000000101000000000000, immMask1, RISCV32_Inst::srai},
    {0b00000000000000000010000000000000, immMask0, RISCV32_Inst::slti},
    {0b00000000000000000011000000000000, immMask0, RISCV32_Inst::sltiu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::branch[] = {
    {0b00000000000000000000000000000000, branchMask, RISCV32_Inst::beq},
    {0b00000000000000000001000000000000, branchMask, RISCV32_Inst::bne},
    {0b00000000000000000100000000000000, branchMask, RISCV32_Inst::blt},
    {0b00000000000000000101000000000000, branchMask, RISCV32_Inst::bge},
    {0b00000000000000000110000000000000, branchMask, RISCV32_Inst::bltu},
    {0b00000000000000000111000000000000, branchMask, RISCV32_Inst::bgeu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jump[] = {
    {0b00000000000000000000000000000000, jumpMask, RISCV32_Inst::jalr},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::auipc[] = {
    {0, 0, RISCV32_Inst::_auipc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jal[] = {
    {0, 0, RISCV32_Inst::_jal},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::cemu_trap[] = {
    {0, 0, RISCV32_Inst::trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
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
    {0b1101011, opcodeMask, InstKind::S, cemu_trap},
};

uint32_t RISCV32_Decoder::Decode()
{
    uint32_t opcodeLen = sizeof(opcodeTable) / sizeof(OpcodeEntry);
    for (uint32_t i = 0; i < opcodeLen; i++)
    {
        uint32_t masked = info->inst.val & opcodeTable[i].mask;
        if (masked == opcodeTable[i].pattern)
        {
            InstEntry *instTable = opcodeTable[i].instPtr;
            for (InstEntry *inst = instTable; instTable->InstExe != nullptr; inst++)
            {
                uint32_t masked = info->inst.val & inst->mask;
                if (masked == inst->pattern)
                {
                    switch (opcodeTable[i].kind)
                    {
                    case R:
                        decode_R(4);
                    case I:
                        decode_I(4);
                    case S:
                        decode_S(4);
                    case B:
                        decode_B(4);
                    case U:
                        decode_U(4);
                    case J:
                        decode_J(4);
                        break;
                    }
                    inst->InstExe(this);
                    return 0;
                }
            }
            printf("No instrunction found. (0x%x)\n", info->inst.val);
            return 1;
        }
    }
    printf("No opcode found. (0x%x)\n", info->inst.val);
    return 1;
}
