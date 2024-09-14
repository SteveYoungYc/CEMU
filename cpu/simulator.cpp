#include <riscv32/riscv32_cpu.h>
#include <cpu.h>
#include <decoder.h>
#include <memory.h>
#include <simulator.h>
#include <string.h>
#include <log.h>


static const uint32_t img [] = {
    0x800002b7,  // lui t0,0x80000
    0x0002a023,  // sw  zero,0(t0)
    0x0002a503,  // lw  a0,0(t0)
    0x0000006b,  // cemu_trap
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
    imgFile = args.imgFile;
}

long Simulator::LoadImg()
{
    if (imgFile == NULL)
    {
        InfoPrint("No image is given. Use the default build-in image.\n");
        memcpy(memory.GuestToHost(Memory::GetBase()), img, sizeof(img));
        return 4096; // built-in image size
    }

    FILE *fp = fopen(imgFile, "rb");
    if (fp == nullptr)
    {
        assert(!"No such file.");
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    InfoPrint("The image is %s, size = %ld\n", imgFile, size);

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
        if (simStatus.status != RUNNING)
            break;
        cpu->pc = decoder->dnpc;
    }

    switch (simStatus.status)
    {
    case CEMU_Status::RUNNING:
        simStatus.status = CEMU_Status::STOP;
        break;
    case STOP:
    case END:
    case ABORT:
    case QUIT:
        if (simStatus.retVal != 0)
        {
            InfoPrint("Fuck! ret=0x%x\n", simStatus.retVal);
            assert(0);
        }
        InfoPrint("Good!\n");
        break;
    }
}

void Simulator::SetStatus(CEMU_Status status, uint32_t haltPC, int32_t retVal)
{
    simStatus.status = status;
    simStatus.haltPC = haltPC;
    simStatus.retVal = retVal;
}

