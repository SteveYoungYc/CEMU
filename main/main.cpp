#include <monitor.h>
#include <cpu.h>
#include <simulator.h>
#include <memory.h>

int main(int argc, char *argv[])
{
    Simulator simulator;
    simulator.Run(6);
    return 0;
}
