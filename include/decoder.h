#ifndef __DECODER_H__
#define __DECODER_H__

#include <common.h>
#include <rtl.h>

#define id_src1 (&src1)
#define id_src2 (&src2)
#define id_dest (&dest)

typedef union
{
    rtlreg_t *preg;
    word_t imm;
    sword_t simm;
} Operand;

class IDecodeInfo
{
public:

};

class Decoder
{

public:
    vaddr_t pc;
    vaddr_t snpc; // static next pc
    vaddr_t dnpc; // dynamic next pc
    Operand dest, src1, src2;

    SimStatus *simStatus;

    Decoder();
    virtual ~Decoder() {};

    virtual uint32_t GetInstVal() = 0;
    virtual void SetInstVal(uint32_t val) = 0;
    virtual uint32_t DecodeAndExecute() = 0;

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

    inline def_rtl(setrelop, uint32_t relop, rtlreg_t *dest,
                        const rtlreg_t *src1, const rtlreg_t *src2)
    {
        *dest = InterpretRelop(relop, *src1, *src2);
    }

    inline def_rtl(setrelopi, uint32_t relop, rtlreg_t *dest,
                        const rtlreg_t *src1, sword_t imm)
    {
        *dest = InterpretRelop(relop, *src1, imm);
    }

    inline def_rtl(div64u_q, rtlreg_t* dest,
                        const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
    {
        uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
        uint32_t divisor = (*src2);
        *dest = dividend / divisor;
    }

    inline def_rtl(div64u_r, rtlreg_t* dest,
                        const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
    {
        uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
        uint32_t divisor = (*src2);
        *dest = dividend % divisor;
    }

    inline def_rtl(div64s_q, rtlreg_t* dest,
                        const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
    {
        int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
        int32_t divisor = (*src2);
        *dest = dividend / divisor;
    }

    inline def_rtl(div64s_r, rtlreg_t* dest,
                        const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2)
    {
        int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
        int32_t divisor = (*src2);
        *dest = dividend % divisor;
    }

    inline def_rtl(lm, rtlreg_t *dest, const rtlreg_t* addr, word_t offset, int len)
    {
        *dest = PhysicalRead(*addr + offset, len);
    }

    inline def_rtl(sm, const rtlreg_t *src1, const rtlreg_t* addr, word_t offset, int len)
    {
        PhysicalWrite(*addr + offset, *src1, len);
    }

    inline def_rtl(lms, rtlreg_t *dest, const rtlreg_t* addr, word_t offset, int len)
    {
        word_t val = PhysicalRead(*addr + offset, len);
        switch (len)
        {
            case 4: *dest = (sword_t)(int32_t)val; return;
            case 1: *dest = (sword_t)( int8_t)val; return;
            case 2: *dest = (sword_t)(int16_t)val; return;
            default: assert(0); return;
        }
    }

    inline def_rtl(host_lm, rtlreg_t* dest, const void *addr, int len)
    {
        switch (len)
        {
            case 4: *dest = *(uint32_t *)addr; return;
            case 1: *dest = *( uint8_t *)addr; return;
            case 2: *dest = *(uint16_t *)addr; return;
            default: assert(0); return;
        }
    }

    inline def_rtl(host_sm, void *addr, const rtlreg_t *src1, int len)
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
        dnpc = target;
    }

    inline def_rtl(jr, rtlreg_t *target)
    {
        dnpc = *target;
    }

    inline def_rtl(jrelop, uint32_t relop,
                        const rtlreg_t *src1, const rtlreg_t *src2, vaddr_t target)
    {
        bool is_jmp = InterpretRelop(relop, *src1, *src2);
        rtl_j((is_jmp ? target : snpc));
    }

    def_rtl(hostcall, uint32_t id, rtlreg_t *dest, const rtlreg_t *src1,
            const rtlreg_t *src2, word_t imm)
    {
        switch (id)
        {
        case HOSTCALL_EXIT:
            simStatus->status = CEMU_Status::END;
            simStatus->haltPC = pc;
            simStatus->retVal = imm;
            break;
        case HOSTCALL_INV:
            assert("invalid_instr");
            break;
        default:
            assert("Unsupport hostcall ID");
            break;
        }
    }
};

#endif
