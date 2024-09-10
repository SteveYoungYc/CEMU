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

    static inline void add() {}
    static inline void sub() {}
    static inline void _xor() {}
    static inline void _or() {}
    static inline void _and() {}
    static inline void sll() {}
    static inline void srl() {}
    static inline void sra() {}
    static inline void slt() {}
    static inline void sltu() {}
    static inline void mul() {}
    static inline void mulh() {}
    static inline void mulsu() {}
    static inline void mulu() {}
    static inline void div() {}
    static inline void divu() {}
    static inline void rem() {}
    static inline void remu() {}

    static inline void _lui()
    {
        printf("lui!\n");
    }


    static inline void lb() {}
    static inline void lh() {}
    static inline void lw()
    {
        printf("lw!\n");
    }
    static inline void lbu() {}
    static inline void lhu() {}

    static inline void sb() {}
    static inline void sh() {}
    static inline void sw()
    {
        printf("sw!\n");
    }

    static inline void addi() {}
    static inline void xori() {}
    static inline void ori() {}
    static inline void andi() {}
    static inline void slli() {}
    static inline void srli() {}
    static inline void srai() {}
    static inline void slti() {}
    static inline void sltiu() {}

    static inline void beq() {}
    static inline void bne() {}
    static inline void blt() {}
    static inline void bge() {}
    static inline void bltu() {}
    static inline void bgeu() {}

    static inline void jalr() {}

    static inline void _auipc() {}
    static inline void _jal() {}
    static inline void trap()
    {
        *status = STOP;
        printf("cemu_trap!\n");
    }
};

#endif
