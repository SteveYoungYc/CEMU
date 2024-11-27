#include <log.h>
#include <simulator.h>

Logger::~Logger()
{
    if (logFile != nullptr)
    {
        fclose(logFile);
    }
}

void Logger::Init()
{
    isDebug = simulator.args->isDebug;
    logFile = nullptr;
    if (simulator.args->logFile != nullptr)
    {
        logFile = fopen(simulator.args->logFile, "a");
        if (logFile == nullptr)
        {
            printf("Error opening log file.\n");
        }
    }
}

void Logger::InfoPrintf(const char *format, ...)
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

void Logger::DebugPrintf(const char *format, ...)
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
