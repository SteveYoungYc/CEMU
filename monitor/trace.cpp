#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include <trace.h>
#include <log.h>
#include <simulator.h>

using namespace std;
using namespace llvm;

static llvm::MCDisassembler *gDisassembler = nullptr;
static llvm::MCSubtargetInfo *gSTI = nullptr;
static llvm::MCInstPrinter *gIP = nullptr;

void signalHandler(int signal)
{
    InfoPrint("Got SIGABRT. Itrace:\n");
    simulator.GetITrace()->Print();
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

    MCTargetOptions MCOptions;
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
    auto llvmTripleTwine = Twine(triple);
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
}

void ITrace::DoDisassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte)
{
    MCInst inst;
    llvm::ArrayRef<uint8_t> arr(code, nbyte);
    uint64_t dummy_size = 0;
    gDisassembler->getInstruction(inst, dummy_size, arr, pc, llvm::nulls());

    string s;
    raw_string_ostream os(s);
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
    itraceData.pc = pc;
    strcpy(itraceData.str, instBuf);
    if (itraceRingBuf.size() >= ITRACE_RING_BUG_SZIE)
    {
        itraceRingBuf.pop();
    }
    itraceRingBuf.push(itraceData);
}

void ITrace::Print()
{
    while (!itraceRingBuf.empty())
    {
        ITraceData &itraceData = itraceRingBuf.front();
        InfoPrint("[itrace] 0x%x\t\t%s\n", itraceData.pc, itraceData.str);
        itraceRingBuf.pop();
    }
}
