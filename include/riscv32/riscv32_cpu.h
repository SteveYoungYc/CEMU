#ifndef __RISCV32_CPU_H__
#define __RISCV32_CPU_H__

#include <cpu.h>
#include <log.h>

#define def_DopHelper(name) void concat(decode_op_, name)(Operand * op, word_t val, bool flag)

#define def_op(name) void concat(op_, name)()

#define def_op_compute_reg(name)                 \
    inline def_op(name)                          \
    {                                            \
        concat(rtl_, name)(ddest, dsrc1, dsrc2); \
    }

#define def_op_compute_imm(name)                           \
    inline def_op(name##i)                                 \
    {                                                      \
        concat(rtl_, name##i)(ddest, dsrc1, id_src2->simm); \
    }

enum InstKind
{
    R = 0,
    I,
    S,
    B,
    U,
    J,
};

class RISCV32_CPU;

typedef void (RISCV32_CPU::*DecoderFunc)(int);

struct InstEntry
{
    const uint32_t pattern;
    const uint32_t mask;
    void (RISCV32_CPU::*InstExe)();
};

struct OpcodeEntry
{
    const uint32_t pattern;
    const uint32_t mask;
    const InstKind kind;
    const InstEntry *instPtr;
};


union RISCV32_Inst
{
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t funct7 : 7;
    } r;
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        int32_t simm11_0 : 12;
    } i;
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t imm4_0 : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        int32_t simm11_5 : 7;
    } s;
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t imm11 : 1;
        uint32_t imm4_1 : 4;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t simm10_5 : 6;
        int32_t simm12 : 1;
    } b;
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t rd : 5;
        uint32_t imm31_12 : 20;
    } u;
    struct
    {
        uint32_t opcode1_0 : 2;
        uint32_t opcode6_2 : 5;
        uint32_t rd : 5;
        uint32_t simm19_12 : 8;
        uint32_t simm11 : 1;
        uint32_t simm10_1 : 10;
        int32_t simm20 : 1;
    } j;
    uint32_t val;
};


union CJALInstruction
{
    uint16_t raw;
    struct
    {
        uint16_t opcode : 2;
        uint16_t offset5 : 1;
        uint16_t offset3_1 : 3;
        uint16_t offset7 : 1;
        uint16_t offset6 : 1;
        uint16_t offset10 : 1;
        uint16_t offset9_8 : 2;
        uint16_t offset4 : 1;
        uint16_t offset11 : 1;
        uint16_t funct3 : 3;
    } fields;
};


class RISCV32_CPU : public CPU
{
private:
    static const char *regs[];
    static const uint32_t opcodeMask = 0b1111111;
    static const uint32_t calMask    = 0b11111110000000000111000000000000;
    static const uint32_t loadMask   = 0b00000000000000000111000000000000;
    static const uint32_t storeMask  = 0b00000000000000000111000000000000;
    static const uint32_t immMask0   = 0b00000000000000000111000000000000;
    static const uint32_t immMask1   = 0b11111110000000000111000000000000;
    static const uint32_t branchMask = 0b00000000000000000111000000000000;
    static const uint32_t jumpMask   = 0b00000000000000000111000000000000;
    static const uint32_t csrMask    = 0b00000000000000000111000000000000;

    static const OpcodeEntry opcodeTable[];
    static const InstEntry systemInst[];
    static const InstEntry cal[];
    static const InstEntry lui[];
    static const InstEntry load[];
    static const InstEntry jump[];
    static const InstEntry store[];
    static const InstEntry imm[];
    static const InstEntry branch[];
    static const InstEntry auipc[];
    static const InstEntry jal[];
    static const InstEntry csr[];
    static const InstEntry cemu_trap[];

    static const DecoderFunc decoderTable[];

    std::unique_ptr<RISCV32_Inst> inst;

    void decode_op_i(Operand * op, word_t val, bool flag);
    void decode_op_r(Operand * op, word_t val, bool flag);
    void decode_R(int width);
    void decode_I(int width);
    void decode_S(int width);
    void decode_B(int width);
    void decode_U(int width);
    void decode_J(int width);

    // Compute
    def_op_compute_reg(add)
    def_op_compute_reg(sub)
    def_op_compute_reg(xor)
    def_op_compute_reg(or)
    def_op_compute_reg(and)
    def_op_compute_reg(sll)
    def_op_compute_reg(srl)
    def_op_compute_reg(sra)
    def_op_compute_reg(slt)
    def_op_compute_reg(sltu)
    def_op_compute_imm(add)
    def_op_compute_imm(sub)
    def_op_compute_imm(xor)
    def_op_compute_imm(or)
    def_op_compute_imm(and)
    def_op_compute_imm(sll)
    def_op_compute_imm(srl)
    def_op_compute_imm(sra)
    def_op_compute_imm(slt)
    inline void op_sltiu()
    {
        rtl_sltui(ddest, dsrc1, id_src2->simm);
    }

    inline void op_mul()
    {
        rtl_mulu_lo(ddest, dsrc1, dsrc2);
    }
    inline void op_mulh()
    {
        rtl_muls_hi(ddest, dsrc1, dsrc2);
    }
    inline void op_mulsu()
    {
        rtl_mulsu_hi(ddest, dsrc1, dsrc2);
    }
    inline void op_mulu()
    {
        rtl_mulu_hi(ddest, dsrc1, dsrc2);
    }
    inline void op_div()
    {
        if (HandleDiv(false))
        {
            rtl_divs_q(ddest, dsrc1, dsrc2);
        }
    }
    inline void op_divu()
    {
        if (HandleDiv(true))
        {
            rtl_divu_q(ddest, dsrc1, dsrc2);
        }
    }
    inline void op_rem()
    {
        if (HandleRem(false))
        {
            rtl_divs_r(ddest, dsrc1, dsrc2);
        }
    }
    inline void op_remu()
    {
        if (HandleRem(true))
        {
            rtl_divu_r(ddest, dsrc1, dsrc2);
        }
    }

    // Load
    inline void op_lb()
    {
        rtl_lms(ddest, dsrc1, id_src2->simm, 1);
    }
    inline void op_lh()
    {
        rtl_lms(ddest, dsrc1, id_src2->simm, 2);
    }
    inline void op_lw()
    {
        rtl_lms(ddest, dsrc1, id_src2->simm, 4);
    }
    inline void op_lbu()
    {
        rtl_lm(ddest, dsrc1, id_src2->simm, 1);
    }
    inline void op_lhu()
    {
        rtl_lm(ddest, dsrc1, id_src2->simm, 2);
    }

    // Store
    inline void op_sb()
    {
        rtl_sm(ddest, dsrc1, id_src2->simm, 1);
    }
    inline void op_sh()
    {
        rtl_sm(ddest, dsrc1, id_src2->simm, 2);
    }
    inline void op_sw()
    {
        rtl_sm(ddest, dsrc1, id_src2->simm, 4);
    }

    // Control
    inline void op_beq()
    {
        rtl_jrelop(RELOP_EQ, dsrc1, dsrc2, pc + id_dest->simm);
    }
    inline void op_bne()
    {
        rtl_jrelop(RELOP_NE, dsrc1, dsrc2, pc + id_dest->simm);
    }
    inline void op_blt()
    {
        rtl_jrelop(RELOP_LT, dsrc1, dsrc2, pc + id_dest->simm);
    }
    inline void op_bge()
    {
        rtl_jrelop(RELOP_GE, dsrc1, dsrc2, pc + id_dest->simm);
    }
    inline void op_bltu()
    {
        rtl_jrelop(RELOP_LTU, dsrc1, dsrc2, pc + id_dest->simm);
    }
    inline void op_bgeu()
    {
        rtl_jrelop(RELOP_GEU, dsrc1, dsrc2, pc + id_dest->simm);
    }

    inline void op_jalr()
    {
        *regs0 = *dsrc1;
        rtl_addi(ddest, &pc, 4);
        rtl_addi(regs0, regs0, id_src2->simm);
        rtl_jr(regs0);
        HandleFTrace(decoder->dnpc);
    }
    inline void op_lui()
    {
        *ddest = id_src1->simm;
    }
    inline void op_auipc()
    {
        rtl_addi(ddest, &pc, id_src1->simm);
    }
    inline void op_jal()
    {
        rtl_addi(ddest, &pc, 4);
        rtl_addi(regs0, &pc, id_src1->simm);
        rtl_jr(regs0);
        HandleFTrace(decoder->dnpc);
    }

    // System instruction
    inline void op_ecall()
    {
        mepc = pc;
        mcause = 8;
        rtl_j(mtvec);
    }

    inline void op_mret()
    {
        rtl_j(mepc);
    }

    // Fence
    inline void op_fence()
    {
    }
    // CSR
    inline void op_csrrw()
    {
        word_t *regAddr = GetCSRRegister(id_src2->imm);
        *regs0 = *regAddr;
        *regAddr = *dsrc1;
        *ddest = *regs0;
    }
    inline void op_csrrs()
    {
        word_t *regAddr = GetCSRRegister(id_src2->imm);
        *regs0 = *regAddr;
        *regAddr |= *dsrc1;
        *ddest = *regs0;
    }
    inline void op_csrrc()
    {
        word_t *regAddr = GetCSRRegister(id_src2->imm);
        *regs0 = *regAddr;
        *regAddr &= ~(*dsrc1);
        *ddest = *regs0;
    }

    // CEMU trap
    inline void op_trap()
    {
        InfoPrint("CEMU Trap!\n");
        rtl_hostcall(HOSTCALL_EXIT, nullptr, nullptr, nullptr, id_src1->simm >> 12);
    }

    bool HandleDiv(bool isUnsigned);
    bool HandleRem(bool isUnsigned);
    void HandleFTrace(uint32_t addr);

public:
    word_t gpr[32];
    word_t mtvec;
    word_t mepc;
    word_t mstatus;
    word_t mcause;
    word_t mscratch;
    word_t mie;
    word_t mip;
    word_t mtval;
    word_t misa;

    inline word_t *GetReg(uint32_t idx)
    {
        assert(idx >= 0 && idx < 32);
        return gpr;
    }

    word_t *GetCSRRegister(uint32_t val);
    void PrintReg() override;
    word_t RegStrToVal(const char *s, bool *success) override;

    void Init() override;
    void Reset() override;
    void DecodeAndExecute() override;
    void Run() override;
};

#endif
