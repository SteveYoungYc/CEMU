#include <readline/history.h>
#include <readline/readline.h>
#include <simulator.h>
#include <sdb.h>

#define NR_CMD ARRLEN(Debugger::commamdTable)

const CommamdTable Debugger::commamdTable[] = {
    { "help", "Display informations about all supported commands", &Debugger::CommandHelp },
    { "c", "Continue the execution of the program", &Debugger::Command_C },
    { "q", "Exit CEMU", &Debugger::Command_Q },
    { "si", "Execute single step", &Debugger::Command_Si },
    { "info", "Print status", &Debugger::CommandInfo },
    { "x", "Scan memory", &Debugger::Command_X },
    { "p", "Calculate expression", &Debugger::Command_P },
    { "w", "Add watch point", &Debugger::Command_W },
    { "d", "Delete Watch point", &Debugger::Command_D }
};

uint32_t Debugger::Command_C(char *args)
{
    simulator.Run(-1);
    return 0;
}

uint32_t Debugger::Command_Q(char *args)
{
    simulator.simStatus.status = CEMU_Status::QUIT;
    return 1;
}

uint32_t Debugger::Command_Si(char *args)
{
    int n = 1;
    if (args != NULL)
    {
        n = atoi(args);
    }
    simulator.Run(n);
    return 0;
}

uint32_t Debugger::CommandInfo(char *args)
{
    if (args == NULL)
    {
        InfoPrint("usage: 'info r' or 'info w'\n");
        return 0;
    }
    if (strcmp(args, "r") == 0)
    {
        simulator.cpu->PrintReg();
    }
    return 0;
}

uint32_t Debugger::Command_X(char *args)
{
    if (args == NULL)
    {
        InfoPrint("usage: 'x N EXPR'\n");
        return 0;
    }
    const char *delim = " ";
    char *p;
    int n = atoi(strtok(args, delim));
    n *= 4;
    word_t regAddr = 0;
    while ((p = strtok(NULL, delim)))
    {
        if (strncmp(p, "0x", 2) != 0 || p + 2 == NULL || *(p + 2) == '\0')
        {
            InfoPrint("usage: 'x N EXPR'\n");
            return 0;
        }
        regAddr = strtol(p, NULL, 16);
    }
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && i % 4 == 0)
        {
            InfoPrint("\n");
        }
        InfoPrint("[0x%x] 0x%x\t", regAddr, simulator.memory->PhysicalRead(regAddr, 1));
        regAddr++;
    }
    InfoPrint("\n");
    return 0;
}

uint32_t Debugger::Command_P(char *args)
{
    if (args == NULL)
    {
        InfoPrint("usage: 'p expression'\n");
        return 0;
    }
    bool success;
    int res = expr.Run(args, &success);
    if (success)
    {
        InfoPrint("0x%x\n", res);
    }
    return 0;
}

uint32_t Debugger::Command_W(char *args)
{
    return 0;
}

uint32_t Debugger::Command_D(char *args)
{
    return 0;
}

uint32_t Debugger::CommandHelp(char *args)
{
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL)
    {
        /* no argument given */
        for (i = 0; i < NR_CMD; i++)
        {
            InfoPrint("\033[34m%s - %s\033[0m\n", commamdTable[i].name, commamdTable[i].description);
        }
    }
    else
    {
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(arg, commamdTable[i].name) == 0)
            {
                InfoPrint("%s - %s\n", commamdTable[i].name, commamdTable[i].description);
                return 0;
            }
        }
        InfoPrint("Unknown command '%s'\n", arg);
    }
    return 0;
}

char *Debugger::ReadLine()
{
    static char *line_read = NULL;

    if (line_read)
    {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("CEMU $ ");

    if (line_read && *line_read)
    {
        add_history(line_read);
    }

    return line_read;
}

void Debugger::Init()
{
    mode = Args::Instance().isBatch ? Mode::Batch : Mode::Debug;
    expr.Init();
}

void Debugger::Run()
{
    if (mode == Mode::Batch)
    {
        simulator.Run(-1);
        return;
    }
    
    for (char *str; (str = ReadLine()) != NULL;)
    {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL)
        {
            continue;
        }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end)
        {
            args = NULL;
        }

        int i;
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(cmd, commamdTable[i].name) == 0)
            {
                if ((this->*commamdTable[i].handler)(args) != 0)
                {
                    return;
                }
                break;
            }
        }

        if (i == NR_CMD)
        {
            InfoPrint("Unknown command '%s'\n", cmd);
        }
    }
}
