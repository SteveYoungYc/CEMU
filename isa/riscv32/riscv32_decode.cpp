#include <riscv32/riscv32_decode.h>
#include <simulator.h>

CEMU_Status *RISCV32_Decoder::status;

RISCV32_Decoder::RISCV32_Decoder(CEMU_Status *_status)
{
    status = _status;
    info = new RISCV32_DecodeInfo();
}

RISCV32_Decoder::~RISCV32_Decoder()
{
    delete info;
}

InstEntry RISCV32_Decoder::cal[] = {
    {0b00000000000000000000000000000000, 0b11111110000000000111000000000000, RISCV32_Decoder::add},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::lui[] = {
    {0, 0, RISCV32_Decoder::_lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::load[] = {
    {0b00000000000000000010000000000000, 0b00000000000000000111000000000000, RISCV32_Decoder::lw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::store[] = {
    {0b00000000000000000010000000000000, 0b00000000000000000111000000000000, RISCV32_Decoder::sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::imm[] = {
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::branch[] = {
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jump[] = {
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::auipc[] = {
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jal[] = {
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::cemu_trap[] = {
    {0, 0, RISCV32_Decoder::trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};


OpcodeEntry RISCV32_Decoder::opcodeTable[] = {
    {0b0110011, opcodeMask, InstKind::R, cal},
    {0b0110111, opcodeMask, InstKind::U, lui},
    {0b0000011, opcodeMask, InstKind::I, load},
    {0b0100011, opcodeMask, InstKind::S, store},
    {0b0010011, opcodeMask, InstKind::I, imm},
    {0b1100011, opcodeMask, InstKind::B, branch},
    {0b1100111, opcodeMask, InstKind::I, jump},
    {0b0010111, opcodeMask, InstKind::U, auipc},
    {0b1101111, opcodeMask, InstKind::J, jal},
    {0b1101011, opcodeMask, InstKind::S, cemu_trap},
};

uint32_t RISCV32_Decoder::Decode()
{
    uint32_t opcodeLen = sizeof(opcodeTable) / sizeof(OpcodeEntry);
    for (uint32_t i = 0; i < opcodeLen; i++)
    {
        uint32_t masked = info->inst.val & opcodeTable[i].mask;
        if (masked == opcodeTable[i].pattern)
        {
            InstEntry *instTable = opcodeTable[i].instPtr;
            for (InstEntry *inst = instTable; instTable->InstExe != nullptr; inst++)
            {
                uint32_t masked = info->inst.val & inst->mask;
                if (masked == inst->pattern)
                {
                    inst->InstExe();
                    return 0;
                }
            }
            printf("No instrunction found.\n");
            return 1;
        }
    }
    printf("No opcode found.\n");
    return 1;
}
