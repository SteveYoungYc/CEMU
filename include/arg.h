#ifndef __ARG_H__
#define __ARG_H__

#include <common.h>
#include <getopt.h>

enum ArgType {
    SIMPLE,
    UNSIGN,
    LAST
};

struct ArgEntry
{
    const char *name;
    const char *description;
    const enum ArgType type;
};


class Args
{

public:
    static const struct option table[];

    const char *imgFile = nullptr;
    const char *logFile = nullptr;
    bool isBatch = false;
    bool isDebug = false;
    bool isItrace = false;

    int32_t ParseArgs(int argc, char *argv[]);
};

#endif
