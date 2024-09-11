#ifndef __RISCV32_DECODE_H__
#define __RISCV32_DECODE_H__

#include <common.h>
#include <cpu.h>
#include <decoder.h>
#include <rtl.h>

#define def_DopHelper(name) void concat(decode_op_, name)(Operand * op, word_t val, bool flag)

enum InstKind
{
    R,
    I,
    S,
    B,
    U,
    J,
};

class RISCV32_Decoder;

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
    static InstEntry cemu_trap[];

    void decode_R(int width);
    void decode_I(int width);
    void decode_S(int width);
    void decode_B(int width);
    void decode_U(int width);
    void decode_J(int width);

    inline void add()
    {
        printf("add\n");
        // RTL::rtl_add(ddest, dsrc1, dsrc2);
    }
    inline void sub()
    {
        printf("sub\n");
    }
    inline void _xor()
    {
        printf("xor\n");
    }
    inline void _or()
    {
        printf("or\n");
    }
    inline void _and()
    {
        printf("and\n");
    }
    inline void sll()
    {
        printf("sll\n");
    }
    inline void srl()
    {
        printf("srl\n");
    }
    inline void sra()
    {
        printf("sra\n");
    }
    inline void slt()
    {
        printf("slt\n");
    }
    inline void sltu()
    {
        printf("sltu\n");
    }
    inline void mul()
    {
        printf("mul\n");
    }
    inline void mulh()
    {
        printf("mulh\n");
    }
    inline void mulsu()
    {
        printf("mulsu\n");
    }
    inline void mulu()
    {
        printf("mulu\n");
    }
    inline void div()
    {
        printf("div\n");
    }
    inline void divu()
    {
        printf("divu\n");
    }
    inline void rem()
    {
        printf("rem\n");
    }
    inline void remu()
    {
        printf("remu\n");
    }

    inline void _lui()
    {
        printf("lui\n");
    }

    inline void lb()
    {
        printf("lb\n");
    }
    inline void lh()
    {
        printf("lh\n");
    }
    inline void lw()
    {
        printf("lw\n");
    }
    inline void lbu()
    {
        printf("lbu\n");
    }
    inline void lhu()
    {
        printf("lhu\n");
    }

    inline void sb()
    {
        printf("sb\n");
    }
    inline void sh()
    {
        printf("sh\n");
    }
    inline void sw()
    {
        printf("sw\n");
    }

    inline void addi()
    {
        printf("addi\n");
    }
    inline void xori()
    {
        printf("xori\n");
    }
    inline void ori()
    {
        printf("ori\n");
    }
    inline void andi()
    {
        printf("andi\n");
    }
    inline void slli()
    {
        printf("slli\n");
    }
    inline void srli()
    {
        printf("srli\n");
    }
    inline void srai()
    {
        printf("srai\n");
    }
    inline void slti()
    {
        printf("slti\n");
    }
    inline void sltiu()
    {
        printf("sltiu\n");
    }

    inline void beq()
    {
        printf("beq\n");
    }
    inline void bne()
    {
        printf("bne\n");
    }
    inline void blt()
    {
        printf("blt\n");
    }
    inline void bge()
    {
        printf("bge\n");
    }
    inline void bltu()
    {
        printf("bltu\n");
    }
    inline void bgeu()
    {
        printf("bgeu\n");
    }

    inline void jalr()
    {
        printf("jalr\n");
    }

    inline void _auipc()
    {
        printf("auipc\n");
    }
    inline void _jal()
    {
        printf("jal\n");
    }
    inline void trap()
    {
        printf("CEMU Trap!\n");
    }

public:
    RISCV32_DecodeInfo *info;

    RISCV32_Decoder();
    ~RISCV32_Decoder();
    uint32_t Decode() override;
};

#endif
