#ifndef __RISCV32_INST_H__
#define __RISCV32_INST_H__

#include <riscv32/riscv32_decode.h>
#include <simulator.h>


namespace RISCV32_Inst
{
#define concat_temp(x, y) x##y
#define concat(x, y)      concat_temp(x, y)

#define id_src1 (&s->src1)
#define id_src2 (&s->src2)
#define id_dest (&s->dest)

inline int check_reg_idx(int idx)
{
    assert(idx >= 0 && idx < 32);
    return idx;
}

#define gpReg(idx) (simulator.cpu->gpr[check_reg_idx(idx)])

#define def_DHelper(name) void concat(decode_, name)(RISCV32_Decoder * s, int width)

#define def_DopHelper(name) void concat(decode_op_, name)(RISCV32_Decoder * s, Operand * op, word_t val, bool flag)

def_DHelper(R);
def_DHelper(I);
def_DHelper(U);
def_DHelper(J);
def_DHelper(S);
def_DHelper(B);

inline void add(RISCV32_Decoder * s)
{
    printf("add\n");
}
inline void sub(RISCV32_Decoder * s)
{
    printf("sub\n");
}
inline void _xor(RISCV32_Decoder * s)
{
    printf("xor\n");
}
inline void _or(RISCV32_Decoder * s)
{
    printf("or\n");
}
inline void _and(RISCV32_Decoder * s)
{
    printf("and\n");
}
inline void sll(RISCV32_Decoder * s)
{
    printf("sll\n");
}
inline void srl(RISCV32_Decoder * s)
{
    printf("srl\n");
}
inline void sra(RISCV32_Decoder * s)
{
    printf("sra\n");
}
inline void slt(RISCV32_Decoder * s)
{
    printf("slt\n");
}
inline void sltu(RISCV32_Decoder * s)
{
    printf("sltu\n");
}
inline void mul(RISCV32_Decoder * s)
{
    printf("mul\n");
}
inline void mulh(RISCV32_Decoder * s)
{
    printf("mulh\n");
}
inline void mulsu(RISCV32_Decoder * s)
{
    printf("mulsu\n");
}
inline void mulu(RISCV32_Decoder * s)
{
    printf("mulu\n");
}
inline void div(RISCV32_Decoder * s)
{
    printf("div\n");
}
inline void divu(RISCV32_Decoder * s)
{
    printf("divu\n");
}
inline void rem(RISCV32_Decoder * s)
{
    printf("rem\n");
}
inline void remu(RISCV32_Decoder * s)
{
    printf("remu\n");
}

inline void _lui(RISCV32_Decoder * s)
{
    printf("lui\n");
}

inline void lb(RISCV32_Decoder * s)
{
    printf("lb\n");
}
inline void lh(RISCV32_Decoder * s)
{
    printf("lh\n");
}
inline void lw(RISCV32_Decoder * s)
{
    printf("lw\n");
}
inline void lbu(RISCV32_Decoder * s)
{
    printf("lbu\n");
}
inline void lhu(RISCV32_Decoder * s)
{
    printf("lhu\n");
}

inline void sb(RISCV32_Decoder * s)
{
    printf("sb\n");
}
inline void sh(RISCV32_Decoder * s)
{
    printf("sh\n");
}
inline void sw(RISCV32_Decoder * s)
{
    printf("sw\n");
}

inline void addi(RISCV32_Decoder * s)
{
    printf("addi\n");
}
inline void xori(RISCV32_Decoder * s)
{
    printf("xori\n");
}
inline void ori(RISCV32_Decoder * s)
{
    printf("ori\n");
}
inline void andi(RISCV32_Decoder * s)
{
    printf("andi\n");
}
inline void slli(RISCV32_Decoder * s)
{
    printf("slli\n");
}
inline void srli(RISCV32_Decoder * s)
{
    printf("srli\n");
}
inline void srai(RISCV32_Decoder * s)
{
    printf("srai\n");
}
inline void slti(RISCV32_Decoder * s)
{
    printf("slti\n");
}
inline void sltiu(RISCV32_Decoder * s)
{
    printf("sltiu\n");
}

inline void beq(RISCV32_Decoder * s)
{
    printf("beq\n");
}
inline void bne(RISCV32_Decoder * s)
{
    printf("bne\n");
}
inline void blt(RISCV32_Decoder * s)
{
    printf("blt\n");
}
inline void bge(RISCV32_Decoder * s)
{
    printf("bge\n");
}
inline void bltu(RISCV32_Decoder * s)
{
    printf("bltu\n");
}
inline void bgeu(RISCV32_Decoder * s)
{
    printf("bgeu\n");
}

inline void jalr(RISCV32_Decoder * s)
{
    printf("jalr\n");
}

inline void _auipc(RISCV32_Decoder * s)
{
    printf("auipc\n");
}
inline void _jal(RISCV32_Decoder * s)
{
    printf("jal\n");
}
inline void trap(RISCV32_Decoder * s)
{
    simulator.status = STOP;
    printf("CEMU Trap!\n");
}

}; // namespace RISCV32_Inst

#endif
