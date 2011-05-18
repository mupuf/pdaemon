#ifndef __COMMON_H_
#define __COMMON_H_

#define NULL 0

#define PROCESS_ID_LENGTH 20
#define PROCESS_MAX 5

typedef enum {
	EOK = 0,
	EPERM = 1,		/* Operation not permitted */
	ENOENT = 2,		/* No such file or directory */
	ESRCH = 3,		/* No such process */
	EINTR = 4,		/* Interrupted system call */
	EIO = 5,			/* I/O error */
	ENXIO = 6,		/* No such device or address */
	E2BIG = 7,		/* Argument list too long */
	ENOEXEC = 8,		/* Exec format error */
	EBADF = 9,		/* Bad file number */
	ECHILD = 10,		/* No child processes */
	EAGAIN = 11,		/* Try again */
	ENOMEM = 12,		/* Out of memory */
	EACCES = 13,		/* Permission denied */
	EFAULT = 14,		/* Bad address */
	ENOTBLK = 15,	/* Block device required */
	EBUSY = 16,		/* Device or resource busy */
} errno_t;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#ifdef SIMULATION
	#include <stdio.h>
#else
	void printf(const char *blabla, ...) { }
#endif


#ifdef SIMULATION
	unsigned int mmio_rd32(u32 address)
	{

	}

	unsigned int mmio_wr32(u32 address)
	{

	}

	int irq_activated = 1;
	void irq_on() { irq_activated = 1; }
	void irq_off() { irq_activated = 0; }
	
#else
	void irq_on()
	{
	}
	
	void irq_off()
	{
	}
#endif

#endif