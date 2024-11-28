#ifndef __ARG_H__
#define __ARG_H__

#include <common.h>
#include <getopt.h>

class Args
{
private:
    bool screenEnabled = false;
    Args() = default;
    ~Args() = default;

public:
    static Args &Instance()
    {
        static Args instance;
        return instance;
    }
    Args(const Args &) = delete;
    Args &operator=(const Args &) = delete;

    static const struct option table[];

    const char *imgFile = nullptr;
    const char *logFile = nullptr;
    bool isBatch = false;
    bool isDebug = false;
    bool isItrace = false;

    int32_t ParseArgs(int argc, char *argv[]);
};

#endif
