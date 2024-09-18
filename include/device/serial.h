#ifndef __DEVICE_SERIEL_H__
#define __DEVICE_SERIEL_H__

#include <device.h>

#define CH_OFFSET       0
#define SERIAL_OFFSET   0x3f8

class Serial : public Device
{
public:
    void PutChar(char c);

    void Init() override;
    void Callback(uint32_t, int, bool);
};

#endif
