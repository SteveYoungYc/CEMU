#include <arg.h>

const struct option Args::table[] = {
    {"batch", no_argument, NULL, 'b'},
    {"log", required_argument, NULL, 'l'},
    {"img", required_argument, NULL, 'i'},
    {"diff", optional_argument, NULL, 'D'},
    {"port", optional_argument, NULL, 'p'},
    {"help", optional_argument, NULL, 'h'},
    {"debug", no_argument, NULL, 'd'},
    {"itrace", no_argument, NULL, 'I'},
    {0, 0, NULL, 0},
};

int32_t Args::ParseArgs(int argc, char *argv[])
{
    int o;
    while ((o = getopt_long(argc, argv, "bhl:i:D::p:dI", Args::table, NULL)) != -1)
    {
        switch (o)
        {
        case 'b':
            printf("Run in batch mode\n");
            isBatch = true;
            break;
        case 'p':
            // sscanf(optarg, "%d", &difftest_port);
            break;
        case 'l':
            logFile = optarg;
            printf("logFile: %s\n", logFile);
            break;
        case 'i':
            imgFile = optarg;
            printf("imgFile: %s\n", imgFile);
        case 'D':
            // diff_so_file = optarg;
            break;
        case 'd':
            printf("Debug print enabled\n");
            isDebug = true;
            break;
        case 'I':
            printf("itrace enabled\n");
            isItrace = true;
            break;
        default:
            printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
            printf("\t-b, --batch              run with batch mode\n");
            printf("\t-l, --log=FILE           output log to FILE\n");
            printf("\t-i, --img=FILE           specify the image file\n");
            printf("\t-D, --diff=REF_SO        run DiffTest with reference REF_SO\n");
            printf("\t-p, --port=PORT          run DiffTest with port PORT\n");
            printf("\t-h, --help               display this help message\n");
            printf("\t-d, --debug              enable debug print\n");
            printf("\t-I, --itrace             enable itrace\n");
            printf("\n");
            exit(0);
        }
    }
    return 0;
}
