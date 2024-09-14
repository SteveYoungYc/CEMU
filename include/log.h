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
    ~Logger()
    {
        if (logFile != nullptr)
        {
            fclose(logFile);
        }
    }

    void Init()
    {
        isDebug = args.isDebug;
        logFile = nullptr;
        if (args.logFile != nullptr)
        {
            logFile = fopen(args.logFile, "a");
            if (logFile == nullptr)
            {
                printf("Error opening log file.\n");
            }
        }
    }

    void InfoPrintf(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        if (logFile != nullptr)
        {
            vfprintf(logFile, format, args);
        }
        va_end(args);
    }

    void DebugPrintf(const char *format, ...)
    {
        if (isDebug)
        {
            va_list args;
            va_start(args, format);
            vprintf(format, args);
            if (logFile != nullptr)
            {
                vfprintf(logFile, format, args);
            }
            va_end(args);
        }
    }
};

extern Logger cemuLog;

#define InfoPrint(format, ...)  cemuLog.InfoPrintf(format, ##__VA_ARGS__)
#define DebugPrint(format, ...) cemuLog.DebugPrintf(format, ##__VA_ARGS__)

#endif
