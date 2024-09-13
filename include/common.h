#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdio>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


#define concat(x, y)  x##y

#if !defined(likely)
#define likely(cond)   __builtin_expect(cond, 1)
#define unlikely(cond) __builtin_expect(cond, 0)
#endif

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

enum CEMU_Status
{
    RUNNING,
    STOP,
    END,
    ABORT,
    QUIT
};

enum
{
    HOSTCALL_EXIT, // handling cemu_trap
    HOSTCALL_INV,  // invalid opcode
    HOSTCALL_PIO,  // port I/O
};

typedef uint32_t word_t;
typedef int32_t  sword_t;
typedef uint32_t vaddr_t;
typedef uint32_t paddr_t;
typedef uint16_t ioaddr_t;

typedef word_t rtlreg_t;

#endif
