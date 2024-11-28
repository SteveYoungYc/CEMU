#include <decoder.h>
#include <simulator.h>

using namespace std;

Decoder::Decoder()
{
    simStatus = &simulator.simStatus;
    itrace = make_unique<ITrace>();
    ftrace = make_unique<FTrace>();
    itrace->Init("riscv32");
    ftrace->Init();
}
