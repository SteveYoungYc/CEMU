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

    Logger() = default;
    ~Logger();

public:
    static Logger &Instance()
    {
        static Logger instance;
        return instance;
    }
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void Init();
    void InfoPrintf(const char *format, ...);
    void DebugPrintf(const char *format, ...);
};

#define InfoPrint(format, ...)  Logger::Instance().InfoPrintf(format, ##__VA_ARGS__)
#define DebugPrint(format, ...) Logger::Instance().DebugPrintf(format, ##__VA_ARGS__)

#endif
