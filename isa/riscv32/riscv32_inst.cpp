#include <riscv32/riscv32_inst.h>

namespace RISCV32_Inst
{

    
def_DopHelper(i)
{
    op->imm = val;
}

def_DopHelper(r)
{
    bool is_write = flag;
    word_t zero_null = 0;
    op->preg = (is_write && val == 0) ? &zero_null : &gpReg(val);
}

def_DHelper(R)
{
    decode_op_r(s, id_src1, s->info->inst.r.rs1, false);
    decode_op_r(s, id_src2, s->info->inst.r.rs2, false);
    decode_op_r(s, id_dest, s->info->inst.r.rd, true);
}

def_DHelper(I)
{
    decode_op_r(s, id_src1, s->info->inst.i.rs1, false);
    decode_op_i(s, id_src2, s->info->inst.i.simm11_0, false);
    decode_op_r(s, id_dest, s->info->inst.i.rd, true);
}

def_DHelper(U)
{
    decode_op_i(s, id_src1, s->info->inst.u.imm31_12 << 12, true);
    decode_op_r(s, id_dest, s->info->inst.u.rd, true);
}

def_DHelper(J)
{
    sword_t simm = s->info->inst.j.simm10_1 << 1 | s->info->inst.j.simm11 << 11 |
                   s->info->inst.j.simm19_12 << 12 | s->info->inst.j.simm20 << 20;
    decode_op_i(s, id_src1, simm, true);
    decode_op_r(s, id_dest, s->info->inst.j.rd, true);
}

def_DHelper(S)
{
    decode_op_r(s, id_src1, s->info->inst.s.rs1, false);
    sword_t simm = (s->info->inst.s.simm11_5 << 5) | s->info->inst.s.imm4_0;
    decode_op_i(s, id_src2, simm, false);
    decode_op_r(s, id_dest, s->info->inst.s.rs2, false);
}

def_DHelper(B)
{
    decode_op_r(s, id_src1, s->info->inst.b.rs1, false);
    decode_op_r(s, id_src2, s->info->inst.b.rs2, false);
    sword_t simm = (s->info->inst.b.imm11 << 11) | (s->info->inst.b.imm4_1 << 1) |
                   (s->info->inst.b.simm10_5 << 5) | (s->info->inst.b.simm12 << 12);
    decode_op_i(s, id_dest, simm, false);
}



}; // namespace RISCV32_Inst
