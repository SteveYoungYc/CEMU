#include <arg.h>
#include <device/dev_mgr.h>
#include <log.h>
#include <memory.h>
#include <sdb.h>
#include <simulator.h>

Simulator simulator;

int main(int argc, char *argv[])
{
    Args::Instance().ParseArgs(argc, argv);
    Logger::Instance().Init();
    Debugger sdb;
    simulator.Init();
    DeviceManager::Instance().Init();
    sdb.Init();
    sdb.Run();
    return 0;
}
