#include <arg.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>

RISCV32_Decoder riscv32Decoder;
RISCV32_CPU riscv32CPU(&riscv32Decoder);
ICpu *baseCPU = static_cast<ICpu*>(&riscv32CPU);
Simulator simulator;
Memory memory;
Debugger sdb;

static int ParseArgs(int argc, char *argv[])
{
    const struct option table[] = {
        {"batch", optional_argument, NULL, 'b'},
        {"log", optional_argument, NULL, 'l'},
        {"diff", optional_argument, NULL, 'd'},
        {"port", optional_argument, NULL, 'p'},
        {"help", optional_argument, NULL, 'h'},
        {0, 0, NULL, 0},
    };
    int o;
    while ((o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1)
    {
        switch (o)
        {
        case 'b':
            printf("Run in batch mode\n");
            sdb.SetMode(Mode::Batch);
            break;
        case 'p':
            // sscanf(optarg, "%d", &difftest_port);
            break;
        case 'l':
            // log_file = optarg;
            break;
        case 'd':
            // diff_so_file = optarg;
            break;
        case 1:
            simulator.SetImg(optarg);
            return optind - 1;
        default:
            printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
            printf("\t-b,--batch              run with batch mode\n");
            printf("\t-l,--log=FILE           output log to FILE\n");
            printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
            printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
            printf("\n");
            exit(0);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    memory.Init();
    simulator.Init();
    sdb.Init();
    ParseArgs(argc, argv);

    simulator.LoadImg();
    sdb.Run();
    return 0;
}
