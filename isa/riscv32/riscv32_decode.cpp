#include <riscv32/riscv32_decode.h>
#include <simulator.h>

RISCV32_Decoder::RISCV32_Decoder()
{
    info = new RISCV32_DecodeInfo();
}

RISCV32_Decoder::~RISCV32_Decoder()
{
    delete info;
}

void add()
{
    printf("add!\n");
}

void _lui()
{
    printf("lui!\n");
}

void lw()
{
    printf("lw!\n");
}

void sw()
{
    printf("sw!\n");
}

void cemu_trap()
{
    Simulator::status = STOP;
    printf("cemu_trap!\n");
}

Level2 base_cal[] = {
    {0b00000000000000000000000000000000, 0b11111110000000000111000000000000, add},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

Level2 lui[] = {
    {0, 0, _lui},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

Level2 load[] = {
    {0b00000000000000000010000000000000, 0b00000000000000000111000000000000, lw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

Level2 store[] = {
    {0b00000000000000000010000000000000, 0b00000000000000000111000000000000, sw},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

Level2 _cemu_trap[] = {
    {0, 0, cemu_trap},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};


uint32_t l1Mask = 0b1111111;

Level1 level1Table[] = {
    {0b0110011, l1Mask, InstKind::R, base_cal},
    {0b0110111, l1Mask, InstKind::U, lui},
    {0b0000011, l1Mask, InstKind::I, load},
    {0b0100011, l1Mask, InstKind::S, store},
    {0b1101011, l1Mask, InstKind::S, _cemu_trap},
//   {0b0000011, InstKind::I, load},
//   {0b0010011, InstKind::I, imm},
//   {0b1100111, InstKind::I, jump},
//   {0b0100011, InstKind::S, store},
//   {0b1100011, InstKind::B, branch},
//   {0b0110111, InstKind::U, lui},
//   {0b0010111, InstKind::U, auipc},
//   {0b1101111, InstKind::J, jal},
};

uint32_t RISCV32_Decoder::Decode()
{
    uint32_t l1Len = sizeof(level1Table) / sizeof(Level1);
    for (uint32_t i = 0; i < l1Len; i++)
    {
        uint32_t masked = info->inst.val & level1Table[i].mask;
        if (masked == level1Table[i].pattern)
        {
            Level2 *l2Table = level1Table[i].l2Ptr;
            for (Level2 * j = l2Table; l2Table->InstExe != nullptr; j++)
            {
                uint32_t masked = info->inst.val & j->mask;
                if (masked == j->pattern)
                {
                    j->InstExe();
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
