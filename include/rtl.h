#ifndef __RTL_H__
#define __RTL_H__

#include <memory.h>

// relation operation
enum {
    //            +-- unsign
    //            |   +-- sign
    //            |   |   +-- equal
    //            |   |   |   +-- invert
    //            |   |   |   |
    RELOP_FALSE = 0 | 0 | 0 | 0,
    RELOP_TRUE  = 0 | 0 | 0 | 1,
    RELOP_EQ    = 0 | 0 | 2 | 0,
    RELOP_NE    = 0 | 0 | 2 | 1,

    RELOP_LT    = 0 | 4 | 0 | 0,
    RELOP_LE    = 0 | 4 | 2 | 0,
    RELOP_GT    = 0 | 4 | 2 | 1,
    RELOP_GE    = 0 | 4 | 0 | 1,

    RELOP_LTU   = 8 | 0 | 0 | 0,
    RELOP_LEU   = 8 | 0 | 2 | 0,
    RELOP_GTU   = 8 | 0 | 2 | 1,
    RELOP_GEU   = 8 | 0 | 0 | 1,
};

static inline bool InterpretRelop(uint32_t relop, const rtlreg_t src1, const rtlreg_t src2)
{
    switch (relop)
    {
    case RELOP_FALSE:
        return false;
    case RELOP_TRUE:
        return true;
    case RELOP_EQ:
        return src1 == src2;
    case RELOP_NE:
        return src1 != src2;
    case RELOP_LT:
        return (sword_t)src1 < (sword_t)src2;
    case RELOP_LE:
        return (sword_t)src1 <= (sword_t)src2;
    case RELOP_GT:
        return (sword_t)src1 > (sword_t)src2;
    case RELOP_GE:
        return (sword_t)src1 >= (sword_t)src2;
    case RELOP_LTU:
        return src1 < src2;
    case RELOP_LEU:
        return src1 <= src2;
    case RELOP_GTU:
        return src1 > src2;
    case RELOP_GEU:
        return src1 >= src2;
    default:
        assert("Unsupport relop");
        return false;
    }
}


#define dsrc1 (id_src1->preg)
#define dsrc2 (id_src2->preg)
#define ddest (id_dest->preg)
#define regs0 (&GetBaseCPU()->tmp_reg[0])
#define regs1 (&GetBaseCPU()->tmp_reg[1])
#define regs2 (&GetBaseCPU()->tmp_reg[2])
#define regt0 (&GetBaseCPU()->tmp_reg[3])
#define rz    (&GetBaseCPU()->rzero)

#define c_shift_mask 0x1f

#define c_add(a, b)    ((a) + (b))
#define c_sub(a, b)    ((a) - (b))
#define c_and(a, b)    ((a) & (b))
#define c_or(a, b)     ((a) | (b))
#define c_xor(a, b)    ((a) ^ (b))
#define c_sll(a, b)    ((a) << ((b) & c_shift_mask))
#define c_srl(a, b)    ((a) >> ((b) & c_shift_mask))
#define c_sra(a, b)    ((sword_t)(a) >> ((b) & c_shift_mask))
#define c_slt(a, b)    (((sword_t)(a) < (sword_t)(b)) ? 1 : 0)
#define c_sltu(a, b)   (((word_t)(a) < (word_t)(b)) ? 1 : 0)
#define c_sext(a, num) (((sword_t)a << num) >> num)

#define c_mulu_lo(a, b) ((a) * (b))
#define c_mulu_hi(a, b) (((uint64_t)(a) * (uint64_t)(b)) >> 32)
#define c_muls_hi(a, b) (((int64_t)(sword_t)(a) * (int64_t)(sword_t)(b)) >> 32)

#define c_divu_q(a, b) ((a) / (b))
#define c_divu_r(a, b) ((a) % (b))
#define c_divs_q(a, b) ((sword_t)(a) / (sword_t)(b))
#define c_divs_r(a, b) ((sword_t)(a) % (sword_t)(b))

#define def_rtl(name, ...) void concat(rtl_, name)(__VA_ARGS__)

#define def_rtl_compute_reg(name)                                                           \
    static inline def_rtl(name, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) \
    {                                                                                       \
        *dest = concat(c_, name)(*src1, *src2);                                             \
    }

#define def_rtl_compute_imm(name)                                                           \
    static inline def_rtl(name##i, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) \
    {                                                                                       \
        *dest = concat(c_, name)(*src1, imm);                                               \
    }

#define def_rtl_compute_reg_imm(name) \
    def_rtl_compute_reg(name)         \
    def_rtl_compute_imm(name)


def_rtl(hostcall, uint32_t id, rtlreg_t *dest, const rtlreg_t *src1,
    const rtlreg_t *src2, word_t imm);

#endif
