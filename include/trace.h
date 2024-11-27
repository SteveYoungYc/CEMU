#ifndef __TRACE_H__
#define __TRACE_H__

#include <common.h>
#include <csignal>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

#define INST_BUF 32
#define ITRACE_RING_BUG_SZIE 128

struct ITraceData
{
    uint32_t pc;
    char str[INST_BUF];
};

enum ITraceMode
{
    AllInst,
    FailedInst
};

class ITrace
{
private:
    ITraceMode mode;
    char instBuf[INST_BUF];
    std::queue<ITraceData> itraceRingBuf;

    void DoDisassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

public:
    void Init(const char *triple);
    void Disassemble(uint64_t pc, uint8_t *code);
    void Record(uint64_t pc, uint8_t *code);
    void Print();
};

class FTrace
{
private:
    bool start;
    std::vector<std::pair<uint32_t, std::string>> functions;
    std::stack<std::string> callStack;

    void ParseElf(const char *filename);

public:
    void Init();
    std::string FindFunctionByAddress(uint32_t address);
    void CallFucntion(std::string &funcName);
    void Return();
    void Print();
};

void signalHandler(int signal);

#endif
