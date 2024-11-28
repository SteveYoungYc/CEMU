#include <arg.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>
#include <log.h>
#include <device/dev_mgr.h>

Simulator simulator;
Logger cemuLog;

int main(int argc, char *argv[])
{
    Debugger sdb;
    simulator.Init(argc, argv);
    cemuLog.Init();
    sdb.Init();
    sdb.Run();
    return 0;
}
