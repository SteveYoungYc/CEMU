#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
#include <expr.h>

class Debugger;

struct CommamdTable
{
    const char *name;
    const char *description;
    uint32_t (Debugger::*handler) (char *);
};


class Debugger
{
private:
    Expression expr;

    char* ReadLine();
public:
    static const CommamdTable commamdTable[];
    uint32_t CommandHelp(char *args);
    uint32_t Command_C(char *args);
    uint32_t Command_Q(char *args);
    uint32_t Command_Si(char *args);
    uint32_t CommandInfo(char *args);
    uint32_t Command_X(char *args);
    uint32_t Command_P(char *args);
    uint32_t Command_W(char *args);
    uint32_t Command_D(char *args);
    
    void Init();
    void Run();
};

#endif
