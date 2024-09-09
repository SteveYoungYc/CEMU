#include <riscv32/riscv32_decode.h>

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
    printf("Add!\n");
}

Level2 base_cal[] = {
    {0b00000000000000000000000000000000, 0b11111110000000000111000000000000, add},
    {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1), nullptr}
};

uint32_t l1Mask = 0b1111111;

Level1 level1Table[] = {
  {0b0110011, l1Mask, InstKind::R, base_cal},
//   {0b0000011, InstKind::I, load},
//   {0b0010011, InstKind::I, imm},
//   {0b1100111, InstKind::I, jump},
//   {0b0100011, InstKind::S, store},
//   {0b1100011, InstKind::B, branch},
//   {0b0110111, InstKind::U, lui},
//   {0b0010111, InstKind::U, auipc},
//   {0b1101111, InstKind::J, jal},
};

void RISCV32_Decoder::Decode()
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
                    return;
                }
            }
        }
    }
}
