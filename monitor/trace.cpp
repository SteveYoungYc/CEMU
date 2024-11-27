#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include <elf.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>

#include <log.h>
#include <simulator.h>
#include <trace.h>

using namespace std;

static llvm::MCDisassembler *gDisassembler = nullptr;
static llvm::MCSubtargetInfo *gSTI = nullptr;
static llvm::MCInstPrinter *gIP = nullptr;

void signalHandler(int signal)
{
    InfoPrint("Got SIGABRT.\n");
    simulator.itrace.Print();
    simulator.ftrace.Print();
    exit(0);
}

void ITrace::Init(const char *triple)
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllDisassemblers();

    string errstr;
    string gTriple(triple);

    llvm::MCInstrInfo *gMII = nullptr;
    llvm::MCRegisterInfo *gMRI = nullptr;
    auto target = llvm::TargetRegistry::lookupTarget(gTriple, errstr);
    if (!target)
    {
        llvm::errs() << "Can't find target for " << gTriple << ": " << errstr << "\n";
        assert(0);
    }

    llvm::MCTargetOptions MCOptions;
    gSTI = target->createMCSubtargetInfo(gTriple, "", "");
    string isa = target->getName();
    if (isa == "riscv32" || isa == "riscv64")
    {
        gSTI->ApplyFeatureFlag("+m");
        gSTI->ApplyFeatureFlag("+a");
        gSTI->ApplyFeatureFlag("+c");
        gSTI->ApplyFeatureFlag("+f");
        gSTI->ApplyFeatureFlag("+d");
    }
    gMII = target->createMCInstrInfo();
    gMRI = target->createMCRegInfo(gTriple);
    auto AsmInfo = target->createMCAsmInfo(*gMRI, gTriple, MCOptions);
#if LLVM_VERSION_MAJOR >= 13
    auto llvmTripleTwine = llvm::Twine(triple);
    auto llvmtriple = llvm::Triple(llvmTripleTwine);
    auto Ctx = new llvm::MCContext(llvmtriple, AsmInfo, gMRI, nullptr);
#else
    auto Ctx = new llvm::MCContext(AsmInfo, gMRI, nullptr);
#endif
    gDisassembler = target->createMCDisassembler(*gSTI, *Ctx);
    gIP = target->createMCInstPrinter(llvm::Triple(gTriple),
                                      AsmInfo->getAssemblerDialect(), *AsmInfo, *gMII, *gMRI);
    gIP->setPrintImmHex(true);
    gIP->setPrintBranchImmAsAddress(true);

    mode = ITraceMode::FailedInst;
}

void ITrace::DoDisassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte)
{
    llvm::MCInst inst;
    llvm::ArrayRef<uint8_t> arr(code, nbyte);
    uint64_t dummy_size = 0;
    gDisassembler->getInstruction(inst, dummy_size, arr, pc, llvm::nulls());

    string s;
    llvm::raw_string_ostream os(s);
    gIP->printInst(&inst, pc, "", *gSTI, os);

    int skip = s.find_first_not_of('\t');
    const char *p = s.c_str() + skip;
    assert((int)s.length() - skip < size);
    strcpy(str, p);
}

void ITrace::Disassemble(uint64_t pc, uint8_t *code)
{
    DoDisassemble(instBuf, INST_BUF, pc, code, 4);
}

void ITrace::Record(uint64_t pc, uint8_t *code)
{
    ITraceData itraceData;

    Disassemble(pc, code);
    if (mode == ITraceMode::AllInst)
    {
        InfoPrint("[itrace] 0x%x\t\t%s\n", pc, instBuf);
    }
    else if (mode == ITraceMode::FailedInst)
    {
        itraceData.pc = pc;
        strcpy(itraceData.str, instBuf);
        if (itraceRingBuf.size() >= ITRACE_RING_BUG_SZIE)
        {
            itraceRingBuf.pop();
        }
        itraceRingBuf.push(itraceData);
    }
    else
    {
        assert(0);
    }
}

void ITrace::Print()
{
    if (mode == ITraceMode::FailedInst)
    {
        while (!itraceRingBuf.empty())
        {
            ITraceData &itraceData = itraceRingBuf.front();
            InfoPrint("[itrace] 0x%x\t\t%s\n", itraceData.pc, itraceData.str);
            itraceRingBuf.pop();
        }
    }
}

void FTrace::Init()
{
    ParseElf("/home/chen/emu/nanos-lite/build/ramdisk.img");
    ParseElf("/home/chen/emu/nanos-lite/build/nanos-lite-riscv32-nemu.elf");
    start = false;
}

void FTrace::ParseElf(const char *filename)
{
    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        InfoPrint("ELF library initialization failed.\n");
        return;
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0)
    {
        InfoPrint("Failed to open file: %s\n", filename);
        return;
    }

    Elf *elf = elf_begin(fd, ELF_C_READ, nullptr);
    if (!elf)
    {
        InfoPrint("elf_begin() failed.\n");
        close(fd);
        return;
    }

    Elf_Scn *section = nullptr;
    while ((section = elf_nextscn(elf, section)) != nullptr)
    {
        GElf_Shdr shdr;
        if (!gelf_getshdr(section, &shdr))
        {
            InfoPrint("gelf_getshdr() failed.\n");
            continue;
        }

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM)
        {
            Elf_Data *data = elf_getdata(section, nullptr);
            size_t count = shdr.sh_size / shdr.sh_entsize;

            for (size_t i = 0; i < count; ++i)
            {
                GElf_Sym sym;
                if (!gelf_getsym(data, i, &sym))
                {
                    InfoPrint("gelf_getsym() failed.\n");
                    continue;
                }

                if (ELF32_ST_TYPE(sym.st_info) == STT_FUNC && sym.st_value != 0)
                {
                    const char *name = elf_strptr(elf, shdr.sh_link, sym.st_name);
                    if (name)
                    {
                        functions.emplace_back((uint32_t)sym.st_value, name);
                    }
                }
            }
        }
    }
    // TODO: handle the last one
    std::sort(functions.begin(), functions.end(),
              [](const auto &a, const auto &b)
              { return a.first < b.first; });

    elf_end(elf);
    close(fd);
}

string FTrace::FindFunctionByAddress(uint32_t address)
{
    auto it = upper_bound(functions.begin(), functions.end(), address,
                          [](uint32_t addr, const pair<uint32_t, string> &func)
                          {
                              return addr < func.first;
                          });

    if (it == functions.begin())
    {
        return ""; // Not found
    }

    --it;
    return it->second;
}

void FTrace::CallFucntion(string &funcName)
{
    if (!start && funcName == "call_main")
    {
        start = true;
    }

    if (!start)
    {
        return;
    }

    if (!callStack.empty() && callStack.top() == funcName)
    {
        return;
    }
    callStack.push(funcName);
    DebugPrint("[FTrace] Function %s is called\n", funcName.c_str());
}

void FTrace::Return()
{
    if (!start)
    {
        return;
    }
    assert(!callStack.empty());
    if (!callStack.empty())
    {
        DebugPrint("[FTrace] Function %s returns\n", callStack.top().c_str());
        callStack.pop();
    }
}

void FTrace::Print()
{
    InfoPrint("[FTrace] Calling stack:\n");
    while (!callStack.empty())
    {
        InfoPrint("[FTrace] Function %s\n", callStack.top().c_str());
        callStack.pop();
    }
}
