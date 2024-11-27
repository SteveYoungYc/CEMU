#ifndef __LOG_H__
#define __LOG_H__

#include <common.h>
#include <cstdarg>
#include <arg.h>


class Logger
{
private:
    FILE *logFile = nullptr;
    bool isDebug;

public:
    ~Logger();
    void Init();
    void InfoPrintf(const char *format, ...);
    void DebugPrintf(const char *format, ...);
};

extern Logger cemuLog;

#define InfoPrint(format, ...)  cemuLog.InfoPrintf(format, ##__VA_ARGS__)
#define DebugPrint(format, ...) cemuLog.DebugPrintf(format, ##__VA_ARGS__)

#endif
