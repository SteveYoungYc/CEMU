#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdio>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

enum CEMU_Status
{
    RUNNING,
    STOP,
    END,
    ABORT,
    QUIT
};

typedef uint32_t word_t;
typedef int32_t  sword_t;
typedef uint32_t vaddr_t;
typedef uint32_t paddr_t;
typedef uint16_t ioaddr_t;

typedef word_t rtlreg_t;

#endif
