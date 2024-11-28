#include <decoder.h>

using namespace std;

SimStatus *GetStatus();

Decoder::Decoder()
{
    simStatus = GetStatus();
    itrace = make_unique<ITrace>();
    ftrace = make_unique<FTrace>();
    itrace->Init("riscv32");
    ftrace->Init();
}
