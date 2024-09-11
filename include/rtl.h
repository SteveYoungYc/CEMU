#ifndef __RTL_H__
#define __RTL_H__

#include <cpu.h>
#include <decoder.h>
#include <memory.h>

namespace RTL
{

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

enum
{
    HOSTCALL_EXIT, // handling nemu_trap
    HOSTCALL_INV,  // invalid opcode
    HOSTCALL_PIO,  // port I/O
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
#define s0    (&baseCPU->tmp_reg[0])
#define s1    (&baseCPU->tmp_reg[1])
#define s2    (&baseCPU->tmp_reg[2])
#define t0    (&baseCPU->tmp_reg[3])
#define rz    (&baseCPU->rzero)

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

#define def_rtl(name, ...) void concat(op_, name)(Decoder *s, __VA_ARGS__)

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


def_rtl_compute_reg_imm(add)
def_rtl_compute_reg_imm(sub)
def_rtl_compute_reg_imm(and)
def_rtl_compute_reg_imm(or)
def_rtl_compute_reg_imm(xor)
def_rtl_compute_reg_imm(sll)
def_rtl_compute_reg_imm(srl)
def_rtl_compute_reg_imm(sra)
def_rtl_compute_reg_imm(slt)
def_rtl_compute_reg_imm(sltu)

def_rtl_compute_reg(mulu_lo)
def_rtl_compute_reg(mulu_hi)
def_rtl_compute_reg(muls_hi)
def_rtl_compute_reg(divu_q)
def_rtl_compute_reg(divu_r)
def_rtl_compute_reg(divs_q)
def_rtl_compute_reg(divs_r)

static inline def_rtl(setrelop, uint32_t relop, rtlreg_t *dest,
                    const rtlreg_t *src1, const rtlreg_t *src2)
{
    *dest = InterpretRelop(relop, *src1, *src2);
}

static inline def_rtl(setrelopi, uint32_t relop, rtlreg_t *dest,
                    const rtlreg_t *src1, sword_t imm)
{
    *dest = InterpretRelop(relop, *src1, imm);
}

static inline def_rtl(div64u_q, rtlreg_t* dest,
                    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
    uint32_t divisor = (*src2);
    *dest = dividend / divisor;
}

static inline def_rtl(div64u_r, rtlreg_t* dest,
                    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
    uint32_t divisor = (*src2);
    *dest = dividend % divisor;
}

static inline def_rtl(div64s_q, rtlreg_t* dest,
                    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
    int32_t divisor = (*src2);
    *dest = dividend / divisor;
}

static inline def_rtl(div64s_r, rtlreg_t* dest,
                    const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
{
    int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
    int32_t divisor = (*src2);
    *dest = dividend % divisor;
}

static inline def_rtl(lm, rtlreg_t *dest, const rtlreg_t* addr, word_t offset, int len)
{
    *dest = memory.PhysicalRead(*addr + offset, len);
}

static inline def_rtl(sm, const rtlreg_t *src1, const rtlreg_t* addr, word_t offset, int len)
{
    memory.PhysicalWrite(*addr + offset, len, *src1);
}

static inline def_rtl(lms, rtlreg_t *dest, const rtlreg_t* addr, word_t offset, int len)
{
    word_t val = memory.PhysicalRead(*addr + offset, len);
    switch (len)
    {
        case 4: *dest = (sword_t)(int32_t)val; return;
        case 1: *dest = (sword_t)( int8_t)val; return;
        case 2: *dest = (sword_t)(int16_t)val; return;
        default: assert(0); return;
    }
}

static inline def_rtl(host_lm, rtlreg_t* dest, const void *addr, int len)
{
    switch (len)
    {
        case 4: *dest = *(uint32_t *)addr; return;
        case 1: *dest = *( uint8_t *)addr; return;
        case 2: *dest = *(uint16_t *)addr; return;
        default: assert(0); return;
    }
}

static inline def_rtl(host_sm, void *addr, const rtlreg_t *src1, int len)
{
    switch (len)
    {
        case 4: *(uint32_t *)addr = *src1; return;
        case 1: *( uint8_t *)addr = *src1; return;
        case 2: *(uint16_t *)addr = *src1; return;
    }
}

// control

inline def_rtl(j, vaddr_t target)
{
    s->dnpc = target;
}

static inline def_rtl(jr, rtlreg_t *target)
{
    s->dnpc = *target;
}

static inline def_rtl(jrelop, uint32_t relop,
                    const rtlreg_t *src1, const rtlreg_t *src2, vaddr_t target)
{
    bool is_jmp = InterpretRelop(relop, *src1, *src2);
    op_j(s, (is_jmp ? target : s->snpc));
}

}   // namespace RTL

#endif
