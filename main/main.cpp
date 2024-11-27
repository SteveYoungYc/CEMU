#include <arg.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>
#include <log.h>
#include <device/dev_mgr.h>

Simulator simulator;
Debugger sdb;
Logger cemuLog;

int main(int argc, char *argv[])
{
    simulator.Init(argc, argv);
    cemuLog.Init();
    sdb.Init();
    sdb.Run();
    return 0;
}
