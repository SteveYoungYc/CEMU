#ifndef __TRACE_H__
#define __TRACE_H__

#include <common.h>
#include <csignal>
#include <queue>

#define INST_BUF 32
#define ITRACE_RING_BUG_SZIE 128

struct ITraceData
{
    uint32_t pc;
    char str[INST_BUF];
};

class ITrace
{
private:
    char instBuf[INST_BUF];
    std::queue<ITraceData> itraceRingBuf;

    void DoDisassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

public:
    void Init(const char *triple);
    void Disassemble(uint64_t pc, uint8_t *code);
    void Record(uint64_t pc, uint8_t *code);
    void Print();
};

void signalHandler(int signal);

#endif
