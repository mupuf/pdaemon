#ifndef __COMMON_H_
#define __COMMON_H_

#include "regs.h"

#define NULL 0

#define PROCESS_ID_LENGTH 20
#define PROCESS_MAX 5

typedef enum {
	EOK = 0,
	EPERM = 1,		/* Operation not permitted */
	ENOENT = 2,		/* No such file or directory */
	ESRCH = 3,		/* No such process */
	EINTR = 4,		/* Interrupted system call */
	EIO = 5,		/* I/O error */
	ENXIO = 6,		/* No such device or address */
	E2BIG = 7,		/* Argument list too long */
	ENOEXEC = 8,		/* Exec format error */
	EBADF = 9,		/* Bad file number */
	ECHILD = 10,		/* No child processes */
	EAGAIN = 11,		/* Try again */
	ENOMEM = 12,		/* Out of memory */
	EACCES = 13,		/* Permission denied */
	EFAULT = 14,		/* Bad address */
	ENOTBLK = 15,		/* Block device required */
	EBUSY = 16,		/* Device or resource busy */
} errno_t;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
typedef u64 ptime_t;

#ifdef SIMULATION
#define NVA_CARD 0
#include <stdio.h>
#include "simul/nva.h"
        static inline u32 mmio_rd32(u32 address)
        {
                return nva_rd32(NVA_CARD, address);
        }

        static inline void mmio_wr32(u32 address, u32 value)
        {
                nva_wr32(NVA_CARD, address, value);
        }

        int irq_activated = 1;
        void irq_on() { irq_activated = 1; }
        void irq_off() { irq_activated = 0; }
#else
        int printf(const char *blabla, ...) { return 0; }
        static inline void mmio_wr32(u32 addr, u32 val)
        {
            __asm__ volatile ("iowr\tI[%0] %1" :: "r"(addr << 6), "r"(val));
        }

        static inline u32 mmio_rd32(u32 addr)
        {
            int x;
            __asm__ volatile ("iord\t%0 I[%1]" :"=r"(x) : "r"(addr << 6));
            return x;
        }

        static inline void mmio_wr32_i(u32 addr, u32 i, u32 val)
        {
            __asm__ volatile ("iowr\tI[%0] %1" :: "r"(addr << 6 + i * 4), "r"(val));
        }

        static inline u32 mmio_rd32_i(u32 addr, u32 i)
        {
            int x;
            __asm__ volatile ("iord\t%0 I[%1]" :"=r"(x) : "r"(addr << 6 + i * 4));
            return x;
        }

        void irq_on()
        {
        }

        void irq_off()
        {
        }
#endif

static inline ptime_t get_time()
{
        ptime_t low;

        /* From kmmio dumps on nv28 this looks like how the blob does this.
        * It reads the high dword twice, before and after.
        * The only explanation seems to be that the 64-bit timer counter
        * advances between high and low dword reads and may corrupt the
        * result. Not confirmed.
        */
        ptime_t high2 = mmio_rd32(NV04_PTIMER_TIME_1);
        ptime_t high1;
        do {
                high1 = high2;
                low = mmio_rd32(NV04_PTIMER_TIME_0);
                high2 = mmio_rd32(NV04_PTIMER_TIME_1);
        } while (high1 != high2);
        return ((((ptime_t)high2) << 32) | (ptime_t)low >> 5);
}

#endif
