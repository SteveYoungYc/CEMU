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
    {0b00000000000000000000000000000000, calMask, RISCV32_Decoder::add},
    {0b01000000000000000000000000000000, calMask, RISCV32_Decoder::sub},
    {0b00000000000000000100000000000000, calMask, RISCV32_Decoder::_xor},
    {0b00000000000000000110000000000000, calMask, RISCV32_Decoder::_or},
    {0b00000000000000000111000000000000, calMask, RISCV32_Decoder::_and},
    {0b00000000000000000001000000000000, calMask, RISCV32_Decoder::sll},
    {0b00000000000000000101000000000000, calMask, RISCV32_Decoder::srl},
    {0b01000000000000000101000000000000, calMask, RISCV32_Decoder::sra},
    {0b00000000000000000010000000000000, calMask, RISCV32_Decoder::slt},
    {0b00000000000000000011000000000000, calMask, RISCV32_Decoder::sltu},
    {0b00000010000000000000000000000000, calMask, RISCV32_Decoder::mul},
    {0b00000010000000000001000000000000, calMask, RISCV32_Decoder::mulh},
    {0b00000010000000000010000000000000, calMask, RISCV32_Decoder::mulsu},
    {0b00000010000000000011000000000000, calMask, RISCV32_Decoder::mulu},
    {0b00000010000000000100000000000000, calMask, RISCV32_Decoder::div},
    {0b00000010000000000101000000000000, calMask, RISCV32_Decoder::divu},
    {0b00000010000000000110000000000000, calMask, RISCV32_Decoder::rem},
    {0b00000010000000000111000000000000, calMask, RISCV32_Decoder::remu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::lui[] = {
    {0, 0, RISCV32_Decoder::_lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::load[] = {
    {0b00000000000000000000000000000000, loadMask, RISCV32_Decoder::lb},
    {0b00000000000000000001000000000000, loadMask, RISCV32_Decoder::lh},
    {0b00000000000000000010000000000000, loadMask, RISCV32_Decoder::lw},
    {0b00000000000000000100000000000000, loadMask, RISCV32_Decoder::lbu},
    {0b00000000000000000101000000000000, loadMask, RISCV32_Decoder::lhu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::store[] = {
    {0b00000000000000000000000000000000, storeMask, RISCV32_Decoder::sb},
    {0b00000000000000000001000000000000, storeMask, RISCV32_Decoder::sh},
    {0b00000000000000000010000000000000, storeMask, RISCV32_Decoder::sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::imm[] = {
    {0b00000000000000000000000000000000, immMask0, RISCV32_Decoder::addi},
    {0b00000000000000000100000000000000, immMask0, RISCV32_Decoder::xori},
    {0b00000000000000000110000000000000, immMask0, RISCV32_Decoder::ori},
    {0b00000000000000000111000000000000, immMask0, RISCV32_Decoder::andi},
    {0b00000000000000000001000000000000, immMask1, RISCV32_Decoder::slli},
    {0b00000000000000000101000000000000, immMask1, RISCV32_Decoder::srli},
    {0b01000000000000000101000000000000, immMask1, RISCV32_Decoder::srai},
    {0b00000000000000000010000000000000, immMask0, RISCV32_Decoder::slti},
    {0b00000000000000000011000000000000, immMask0, RISCV32_Decoder::sltiu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::branch[] = {
    {0b00000000000000000000000000000000, branchMask, RISCV32_Decoder::beq},
    {0b00000000000000000001000000000000, branchMask, RISCV32_Decoder::bne},
    {0b00000000000000000100000000000000, branchMask, RISCV32_Decoder::blt},
    {0b00000000000000000101000000000000, branchMask, RISCV32_Decoder::bge},
    {0b00000000000000000110000000000000, branchMask, RISCV32_Decoder::bltu},
    {0b00000000000000000111000000000000, branchMask, RISCV32_Decoder::bgeu},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jump[] = {
    {0b00000000000000000000000000000000, jumpMask, RISCV32_Decoder::jalr},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::auipc[] = {
    {0, 0, RISCV32_Decoder::_auipc},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

InstEntry RISCV32_Decoder::jal[] = {
    {0, 0, RISCV32_Decoder::_jal},
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
            printf("No instrunction found. (0x%x)\n", info->inst.val);
            return 1;
        }
    }
    printf("No opcode found. (0x%x)\n", info->inst.val);
    return 1;
}
