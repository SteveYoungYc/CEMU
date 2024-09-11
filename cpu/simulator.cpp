#include <riscv32/riscv32_cpu.h>
#include <cpu.h>
#include <decoder.h>
#include <memory.h>
#include <simulator.h>
#include <string.h>


static const uint32_t img [] = {
  0x800002b7,  // lui t0,0x80000
  0x0002a023,  // sw  zero,0(t0)
  0x0002a503,  // lw  a0,0(t0)
  0x0000006b,  // nemu_trap
};

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
}

void Simulator::Init()
{
    decoder = &riscv32Decoder;
    cpu = &riscv32CPU;
    cpu->Reset();
}

long Simulator::LoadImg(const char *imgFile)
{
    if (imgFile == NULL)
    {
        printf("No image is given. Use the default build-in image.");
        memcpy(memory.GuestToHost(Memory::GetBase()), img, sizeof(img));
        return 4096; // built-in image size
    }

    this->imgFile = imgFile;
    FILE *fp = fopen(imgFile, "rb");
    if (fp == nullptr)
    {
        assert(!"No such file.");
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    printf("The image is %s, size = %ld\n", imgFile, size);

    fseek(fp, 0, SEEK_SET);
    int ret = fread(memory.GuestToHost(Memory::GetBase()), size, 1, fp);
    assert(ret == 1);

    fclose(fp);
    return size;
}

void Simulator::Run(uint64_t n)
{
    for (; n > 0; n--)
    {
        decoder->pc = cpu->pc;
        decoder->snpc = cpu->pc;
        cpu->Run();
        if (status != RUNNING)
            return;
        cpu->pc = decoder->dnpc;
    }
}

