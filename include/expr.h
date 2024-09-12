#ifndef __EXPR_H__
#define __EXPR_H__

#include <common.h>
#include <regex.h>

struct Rule
{
    const char *regex;
    int token_type;
};

struct Token
{
    int type;
    char str[32];
};


enum {
    TK_NOTYPE = 256,
    TK_EQ,
    TK_NOTEQ,
    TK_LAND,
    TK_NUM,
    TK_LP,
    TK_RP,
    TK_DEREF,
    TK_HEX,
    TK_REG,
    TK_MULT,
    TK_DIVI,
    TK_ADD,
    TK_MINUS
};

class Expression
{
private:
    static const Rule rules[];
    static regex_t re[];
    static Token tokens[1024];
    static int nrToken;

    bool MakeToken(char *e);
    int CheckParentheses(int p, int q);
    int Calculate(int p, int q);

public:
    void Init();
    int Run(char *e, bool *success);
};

#endif
