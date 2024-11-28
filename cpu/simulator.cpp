#include <simulator.h>
#include <device/dev_mgr.h>

using namespace std;

void signalHandler(int signal);

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
    memory = make_shared<NormalMemory>();
    ioMem = make_shared<IOMemory>();
    cpu = make_shared<RISCV32_CPU>();

    memory->Init();
    ioMem->Init();
    cpu->Init();

    signal(SIGABRT, signalHandler);

    LoadImg();  // After memory init
}

long Simulator::LoadImg()
{
    imgFile = Args::Instance().imgFile;
    if (imgFile == NULL)
    {
        InfoPrint("No image is given. Use the default build-in image.\n");
        memcpy(memory->GuestToHost(Memory::memBase), img, sizeof(img));
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
    int ret = fread(memory->GuestToHost(Memory::memBase), size, 1, fp);
    assert(ret == 1);

    fclose(fp);
    return size;
}

void Simulator::Run(uint64_t n)
{
    for (; n > 0; n--)
    {
        cpu->decoder->pc = cpu->pc;
        cpu->decoder->snpc = cpu->pc;
        cpu->Run();
        if (simStatus.status != RUNNING)
            break;
        cpu->pc = cpu->decoder->dnpc;
        DeviceManager::Instance().Update();
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

void signalHandler(int signal)
{
    InfoPrint("Got SIGABRT.\n");
    simulator.cpu->decoder->itrace->Print();
    simulator.cpu->decoder->ftrace->Print();
    exit(0);
}

std::shared_ptr<NormalMemory> GetMemory()
{
    return simulator.memory;
}

std::shared_ptr<IOMemory> GetIOMemory()
{
    return simulator.ioMem;
}

word_t PhysicalRead(paddr_t pa, uint32_t len)
{
    if (likely(simulator.memory->IsValidPA(pa)))
    {
        return simulator.memory->PhysicalRead(pa, len);
    }
    if (likely(simulator.ioMem->IsValidPA(pa)))
    {
        return simulator.ioMem->PhysicalRead(pa, len);
    }
    InfoPrint("Invalid PA=0x%x\n", pa);
    assert(0);
}

word_t MemPhysicalRead(paddr_t pa, uint32_t len)
{
    if (likely(simulator.memory->IsValidPA(pa)))
    {
        return simulator.memory->PhysicalRead(pa, len);
    }
    InfoPrint("Invalid memory PA=0x%x\n", pa);
    assert(0);
}

word_t IOPhysicalRead(paddr_t pa, uint32_t len)
{
    if (likely(simulator.ioMem->IsValidPA(pa)))
    {
        return simulator.ioMem->PhysicalRead(pa, len);
    }
    InfoPrint("Invalid IO PA=0x%x\n", pa);
    assert(0);
}

void PhysicalWrite(paddr_t pa, uint64_t data, uint32_t len)
{
    if (likely(simulator.memory->IsValidPA(pa)))
    {
        simulator.memory->PhysicalWrite(pa, data, len);
        return;
    }
    if (likely(simulator.ioMem->IsValidPA(pa)))
    {
        simulator.ioMem->PhysicalWrite(pa, data, len);
        return;
    }
    InfoPrint("Invalid PA=0x%x, data=0x%x\n", pa, data);
    assert(0);
}

void MemPhysicalWrite(paddr_t pa, uint64_t data, uint32_t len)
{
    if (likely(simulator.memory->IsValidPA(pa)))
    {
        simulator.memory->PhysicalWrite(pa, data, len);
    }
    else
    {
        InfoPrint("Invalid memory PA=0x%x, data=0x%x\n", pa, data);
        assert(0);
    }
}

void IOPhysicalWrite(paddr_t pa, uint64_t data, uint32_t len)
{
    if (likely(simulator.ioMem->IsValidPA(pa)))
    {
        simulator.ioMem->PhysicalWrite(pa, data, len);
    }
    else
    {
        InfoPrint("Invalid IO PA=0x%x, data=0x%x\n", pa, data);
        assert(0);
    }
}

