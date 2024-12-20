#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdio>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <memory>


#define concat(x, y)  x##y

#if !defined(likely)
#define likely(cond)   __builtin_expect(cond, 1)
#define unlikely(cond) __builtin_expect(cond, 0)
#endif

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

#define MAP(c, f) c(f)

#define ROUND_UP(x, n)     (((x) + (n)-1) & ~((n)-1))
#define ROUND_DOWN(x, n)   ((x) & ~((n)-1))

enum CEMU_Status
{
    RUNNING,
    STOP,
    END,
    ABORT,
    QUIT
};

struct SimStatus
{
    CEMU_Status status;
    uint32_t haltPC;
    int32_t retVal;
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

extern uint64_t GetTime();

#endif
