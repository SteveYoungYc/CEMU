#ifndef __RISCV32_DECODE_H__
#define __RISCV32_DECODE_H__

#include <common.h>
#include <cpu.h>
#include <decoder.h>
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
        concat(rtl_, name##i)(ddest, dsrc1, id_src2->imm); \
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

class RISCV32_Decoder;

typedef void (RISCV32_Decoder::*DecoderFunc)(int);

struct InstEntry
{
    const uint32_t pattern;
    const uint32_t mask;
    void (RISCV32_Decoder::*InstExe)();
};

struct OpcodeEntry
{
    const uint32_t pattern;
    const uint32_t mask;
    InstKind kind;
    InstEntry *instPtr;
};

class RISCV32_DecodeInfo : public IDecodeInfo
{
public:
    union
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
    } inst;
};

class RISCV32_Decoder : public Decoder
{
private:
    static const uint32_t opcodeMask = 0b1111111;
    static const uint32_t calMask    = 0b11111110000000000111000000000000;
    static const uint32_t loadMask   = 0b00000000000000000111000000000000;
    static const uint32_t storeMask  = 0b00000000000000000111000000000000;
    static const uint32_t immMask0   = 0b00000000000000000111000000000000;
    static const uint32_t immMask1   = 0b11111110000000000111000000000000;
    static const uint32_t branchMask = 0b00000000000000000111000000000000;
    static const uint32_t jumpMask   = 0b00000000000000000111000000000000;
    static const uint32_t csrMask    = 0b00000000000000000111000000000000;

    static OpcodeEntry opcodeTable[];
    static InstEntry cal[];
    static InstEntry lui[];
    static InstEntry load[];
    static InstEntry jump[];
    static InstEntry store[];
    static InstEntry imm[];
    static InstEntry branch[];
    static InstEntry auipc[];
    static InstEntry jal[];
    static InstEntry csr[];
    static InstEntry cemu_trap[];

    static DecoderFunc decoderTable[];
    static InstEntry systemInst[];

    std::unique_ptr<RISCV32_DecodeInfo> info;

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
        rtl_sltui(ddest, dsrc1, id_src2->imm);
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
        InfoPrint("mulsu\n");
    }
    inline void op_mulu()
    {
        rtl_mulu_hi(ddest, dsrc1, dsrc2);
    }
    inline void op_div()
    {
        rtl_divs_q(ddest, dsrc1, dsrc2);
    }
    inline void op_divu()
    {
        rtl_divu_q(ddest, dsrc1, dsrc2);
    }
    inline void op_rem()
    {
        rtl_divs_r(ddest, dsrc1, dsrc2);
    }
    inline void op_remu()
    {
        rtl_divu_r(ddest, dsrc1, dsrc2);
    }

    // Load
    inline void op_lb()
    {
        rtl_lms(ddest, dsrc1, id_src2->imm, 1);
    }
    inline void op_lh()
    {
        rtl_lms(ddest, dsrc1, id_src2->imm, 2);
    }
    inline void op_lw()
    {
        rtl_lms(ddest, dsrc1, id_src2->imm, 4);
    }
    inline void op_lbu()
    {
        rtl_lm(ddest, dsrc1, id_src2->imm, 1);
    }
    inline void op_lhu()
    {
        rtl_lm(ddest, dsrc1, id_src2->imm, 2);
    }

    // Store
    inline void op_sb()
    {
        rtl_sm(ddest, dsrc1, id_src2->imm, 1);
    }
    inline void op_sh()
    {
        rtl_sm(ddest, dsrc1, id_src2->imm, 2);
    }
    inline void op_sw()
    {
        rtl_sm(ddest, dsrc1, id_src2->imm, 4);
    }

    // Control
    inline void op_beq()
    {
        rtl_jrelop(RELOP_EQ, dsrc1, dsrc2, pc + id_dest->imm);
    }
    inline void op_bne()
    {
        rtl_jrelop(RELOP_NE, dsrc1, dsrc2, pc + id_dest->imm);
    }
    inline void op_blt()
    {
        rtl_jrelop(RELOP_LT, dsrc1, dsrc2, pc + id_dest->imm);
    }
    inline void op_bge()
    {
        rtl_jrelop(RELOP_GE, dsrc1, dsrc2, pc + id_dest->imm);
    }
    inline void op_bltu()
    {
        rtl_jrelop(RELOP_LTU, dsrc1, dsrc2, pc + id_dest->imm);
    }
    inline void op_bgeu()
    {
        rtl_jrelop(RELOP_GEU, dsrc1, dsrc2, pc + id_dest->imm);
    }

    inline void op_jalr()
    {
        *regs0 = *dsrc1;
        *ddest = pc + 4;
        rtl_j(*regs0 + id_src2->imm);
        HandleFTrace(dnpc);
    }
    inline void op_lui()
    {
        *ddest = id_src1->imm;
    }
    inline void op_auipc()
    {
        *ddest = pc + id_src1->imm;
    }
    inline void op_jal()
    {
        *ddest = pc + 4;
        rtl_j(pc + id_src1->imm);
        HandleFTrace(dnpc);
    }

    // System instruction
    void op_ecall();
    void op_mret();

    // CSR
    void op_csrrw();
    void op_csrrs();
    void op_csrrc();

    // CEMU trap
    inline void op_trap()
    {
        InfoPrint("CEMU Trap!\n");
        rtl_hostcall(HOSTCALL_EXIT, nullptr, nullptr, nullptr, id_src1->imm >> 12);
    }

    void HandleFTrace(uint32_t addr);

public:
    RISCV32_Decoder();
    uint32_t DecodeAndExecute() override;
    uint32_t GetInstVal() override;
    void SetInstVal(uint32_t val) override;
};

#endif
