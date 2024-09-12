#include <cstring>
#include <expr.h>
#include <memory.h>
#include <cpu.h>

#define NR_REGEX ARRLEN(Expression::rules)

const Rule Expression::rules[] = {
    {"\\*0[xX][0-9a-fA-F]+", TK_DEREF},
    {" +", TK_NOTYPE},    // spaces
    {"\\(", TK_LP},
    {"\\)", TK_RP},
    {"\\*", TK_MULT},
    {"/", TK_DIVI},
    {"\\+", TK_ADD},         // plus
    {"-", TK_MINUS},
    {"==", TK_EQ},        // equal
    {"!=", TK_NOTEQ},
    {"&&", TK_LAND},
    {"\\$[0-9a-zA-Z]+", TK_REG},
    {"0[xX][0-9a-fA-F]+", TK_HEX},
    {"[0-9][0-9]*", TK_NUM},
};

Token Expression::tokens[1024] __attribute__((used)) = {};
int Expression::nrToken __attribute__((used))  = 0;

regex_t Expression::re[NR_REGEX] = {

};

void Expression::Init()
{
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            assert("regex compilation failed\n");
        }
    }
}

bool Expression::MakeToken(char *e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;

    nrToken = 0;

    while (e[position] != '\0' && e[position] != '\n')
    {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++)
        {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                int substr_len = pmatch.rm_eo;
                position += substr_len;
                switch (rules[i].token_type)
                {
                case TK_NOTYPE:
                    break;
                case TK_NUM:
                {
                    tokens[nrToken].type = TK_NUM;
                    strncpy(tokens[nrToken].str, e + position - substr_len, substr_len);
                    tokens[nrToken].str[substr_len] = '\0';
                    nrToken++;
                    break;
                }
                case TK_HEX:
                {
                    tokens[nrToken].type = TK_HEX;
                    strncpy(tokens[nrToken].str, e + position - substr_len + 2, substr_len - 2); // remove "0x" prefix
                    tokens[nrToken].str[substr_len - 2] = '\0';
                    nrToken++;
                    break;
                }
                case TK_REG:
                {
                    tokens[nrToken].type = TK_REG;
                    strncpy(tokens[nrToken].str, e + position - substr_len + 1, substr_len - 1);
                    tokens[nrToken].str[substr_len - 1] = '\0';
                    nrToken++;
                    break;
                }
                case TK_DEREF:
                {
                    tokens[nrToken].type = TK_DEREF;
                    strncpy(tokens[nrToken].str, e + position - substr_len + 3, substr_len - 3);
                    tokens[nrToken].str[substr_len - 3] = '\0';
                    nrToken++;
                    break;
                }
                case TK_ADD:
                case TK_MULT:
                case TK_MINUS:
                case TK_DIVI:
                case TK_EQ:
                case TK_NOTEQ:
                case TK_LAND:
                case TK_LP:
                case TK_RP:
                {
                    tokens[nrToken].type = rules[i].token_type;
                    nrToken++;
                    break;
                }
                    // default: TODO();
                }

                break;
            }
        }

        if (i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

int Expression::CheckParentheses(int p, int q)
{
    int left = p, right = q;
    while (tokens[left].type == TK_NOTYPE)
        left++;
    while (tokens[right].type == TK_NOTYPE)
        right--;

    if (right <= left)
        return -1;
    if (tokens[left].type != TK_LP || tokens[right].type != TK_RP)
        return 0;
    left++;
    right--;

    // printf("left: %d, right: %d\n", left, right);
    int stack_count = 0;
    for (int i = left; i <= right; i++)
    {
        if (tokens[i].type == TK_LP)
        {
            stack_count++;
        }
        else if (tokens[i].type == TK_RP)
        {
            if (stack_count > 0)
                stack_count--;
            else
                return 0;
        }
    }
    if (stack_count > 0)
        return -1;
    return 1;
}

int Expression::Calculate(int p, int q)
{
    int res;
    if (p > q)
    {
        /* Bad expression */
        printf("[invalid] p: %d, q: %d\n", p, q);
        return -1;
    }
    else if (p == q)
    {
        /* Single token.
         * For now this token should be a number.
         * Return the value of the number.
         */
        word_t val = 0;
        bool success;
        switch (tokens[p].type)
        {
        case TK_NUM:
            val = strtol(tokens[p].str, NULL, 10);
            break;
        case TK_HEX:
            val = strtol(tokens[p].str, NULL, 16);
            break;
        case TK_REG:
            val = baseCPU->RegStrToVal(tokens[p].str, &success);
            if (!success)
            {
                if (strcmp(tokens[p].str, "pc") != 0)
                {
                    printf("[invalid] cannot find reg\n");
                    return -1;
                }
                val = baseCPU->pc;
            }
            break;
        case TK_DEREF:
            val = memory.PhysicalRead(strtol(tokens[p].str, NULL, 16), 1);
            break;
        default:
            printf("[invalid]\n");
            return -1;
        }
        return val;
    }
    else if ((res = CheckParentheses(p, q)) == 1)
    {
        /* The expression is surrounded by a matched pair of parentheses.
         * If that is the case, just throw away the parentheses.
         */
        // printf("p: %d, q: %d is valid\n", p, q);
        return Calculate(p + 1, q - 1);
    }
    else
    {
        if (res == -1)
        {
            printf("[invalid] parentheses");
            return -1;
        }
        word_t op = -1;
        int precedence = 4;
        int stack_count = 0;
        for (int i = p; i <= q; i++)
        {
            if (tokens[i].type == TK_LP)
            {
                stack_count++;
            }
            else if (tokens[i].type == TK_RP)
            {
                if (stack_count > 0)
                    stack_count--;
            }
            if (stack_count > 0)
                continue;
            if (tokens[i].type != TK_MULT && tokens[i].type != TK_DIVI && tokens[i].type != TK_ADD && tokens[i].type != TK_MINUS && tokens[i].type != TK_EQ && tokens[i].type != TK_NOTEQ && tokens[i].type != TK_LAND)
                continue;
            switch (tokens[i].type)
            {
            case TK_MULT:
            case TK_DIVI:
                if (3 <= precedence)
                {
                    precedence = 3;
                    op = i;
                }
                break;
            case TK_ADD:
            case TK_MINUS:
                if (2 <= precedence)
                {
                    precedence = 2;
                    op = i;
                }
                break;
            case TK_EQ:
            case TK_NOTEQ:
                if (1 <= precedence)
                {
                    precedence = 1;
                    op = i;
                }
                break;
            case TK_LAND:
                if (0 <= precedence)
                {
                    precedence = 0;
                    op = i;
                }
                break;
            default:
            {
                printf("[invalid] op\n");
                return -1;
            }
            }
        }
        // printf("main op is %c\n", tokens[op].type);
        int val1 = Calculate(p, op - 1);
        if (val1 == -1)
        {
            printf("[invalid] val1\n");
            return -1;
        }
        int val2 = Calculate(op + 1, q);
        if (val2 == -1)
        {
            printf("[invalid] val2\n");
            return -1;
        }

        switch (tokens[op].type)
        {
        case TK_ADD:
            return val1 + val2;
        case TK_MINUS:
            return val1 - val2;
        case TK_MULT:
            return val1 * val2;
        case TK_DIVI:
        {
            if (val2 == 0)
            {
                printf("[invalid] divide by 0\n");
                return -1;
            }
            return val1 / val2;
        }
        case TK_EQ:
            return (val1 == val2) ? 1 : 0;
        case TK_NOTEQ:
            return (val1 != val2) ? 1 : 0;
        case TK_LAND:
            return val1 && val2;
        default:
            assert(0);
            return 0;
        }
    }
}

int Expression::Run(char *e, bool *success)
{
    if (!MakeToken(e))
    {
        *success = false;
        return 0;
    }
    int res = Calculate(0, nrToken - 1);
    *success = (res != -1);

    return res;
}
