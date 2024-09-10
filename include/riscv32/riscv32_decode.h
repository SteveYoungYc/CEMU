#ifndef __RISCV32_DECODE_H__
#define __RISCV32_DECODE_H__

#include <common.h>
#include <cpu.h>
#include <decoder.h>

class Simulator;

enum InstKind
{
    R,
    I,
    S,
    B,
    U,
    J,
};

struct InstEntry
{
    const uint32_t pattern;
    const uint32_t mask;
    void (*InstExe)();
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
    union {
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t rd        : 5;
            uint32_t funct3    : 3;
            uint32_t rs1       : 5;
            uint32_t rs2       : 5;
            uint32_t funct7    : 7;
        } r;
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t rd        : 5;
            uint32_t funct3    : 3;
            uint32_t rs1       : 5;
            int32_t  simm11_0  :12;
        } i;
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t imm4_0    : 5;
            uint32_t funct3    : 3;
            uint32_t rs1       : 5;
            uint32_t rs2       : 5;
            int32_t  simm11_5  : 7;
        } s;
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t imm11     : 1;
            uint32_t imm4_1    : 4;
            uint32_t funct3    : 3;
            uint32_t rs1       : 5;
            uint32_t rs2       : 5;
            uint32_t simm10_5  : 6;
            int32_t  simm12    : 1;
        } b;
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t rd        : 5;
            uint32_t imm31_12  :20;
        } u;
        struct {
            uint32_t opcode1_0 : 2;
            uint32_t opcode6_2 : 5;
            uint32_t rd        : 5;
            uint32_t simm19_12 : 8;
            uint32_t simm11    : 1;
            uint32_t simm10_1  :10;
            int32_t  simm20    : 1;
        } j;
        uint32_t val;
    } inst;
};

class RISCV32_Decoder : public Decoder
{
private:
    static CEMU_Status *status;

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

public:
    RISCV32_DecodeInfo *info;

    RISCV32_Decoder(CEMU_Status *_status);
    ~RISCV32_Decoder();
    uint32_t Decode() override;

    static inline void add() { printf("add!\n"); }
    static inline void sub() { printf("sub!\n"); }
    static inline void _xor() { printf("xor!\n"); }
    static inline void _or() { printf("or!\n"); }
    static inline void _and() { printf("and!\n"); }
    static inline void sll() { printf("sll!\n"); }
    static inline void srl() { printf("srl!\n"); }
    static inline void sra() { printf("sra!\n"); }
    static inline void slt() { printf("slt!\n"); }
    static inline void sltu() { printf("sltu!\n"); }
    static inline void mul() { printf("mul!\n"); }
    static inline void mulh() { printf("mulh!\n"); }
    static inline void mulsu() { printf("mulsu!\n"); }
    static inline void mulu() { printf("mulu!\n"); }
    static inline void div() { printf("div!\n"); }
    static inline void divu() { printf("divu!\n"); }
    static inline void rem() { printf("rem!\n"); }
    static inline void remu() { printf("remu!\n"); }

    static inline void _lui()
    {
        printf("lui!\n");
    }

    static inline void lb() { printf("lb!\n"); }
    static inline void lh() { printf("lh!\n"); }
    static inline void lw()
    {
        printf("lw!\n");
    }
    static inline void lbu() { printf("lbu!\n"); }
    static inline void lhu() { printf("lhu!\n"); }

    static inline void sb() { printf("sb!\n"); }
    static inline void sh() { printf("sh!\n"); }
    static inline void sw()
    {
        printf("sw!\n");
    }

    static inline void addi() { printf("addi!\n"); }
    static inline void xori() { printf("xori!\n"); }
    static inline void ori() { printf("ori!\n"); }
    static inline void andi() { printf("andi!\n"); }
    static inline void slli() { printf("slli!\n"); }
    static inline void srli() { printf("srli!\n"); }
    static inline void srai() { printf("srai!\n"); }
    static inline void slti() { printf("slti!\n"); }
    static inline void sltiu() { printf("sltiu!\n"); }

    static inline void beq() { printf("beq!\n"); }
    static inline void bne() { printf("bne!\n"); }
    static inline void blt() { printf("blt!\n"); }
    static inline void bge() { printf("bge!\n"); }
    static inline void bltu() { printf("bltu!\n"); }
    static inline void bgeu() { printf("bgeu!\n"); }

    static inline void jalr() { printf("jalr!\n"); }

    static inline void _auipc() { printf("auipc!\n"); }
    static inline void _jal() { printf("jal!\n"); }

    static inline void trap()
    {
        *status = STOP;
        printf("cemu_trap!\n");
    }
};

#endif
