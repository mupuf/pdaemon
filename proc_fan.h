#ifndef __PROC_FAN_H_
#define __PROC_FAN_H_

#include "process.h"

static inline errno_t proc_fan_init ()
{
        printf("Proc fan init\n");
        return EOK;
}

static inline errno_t proc_fan_fini ()
{
        printf("Proc fan fini\n");
        return EOK;
}

static inline errno_t proc_fan_periodic_task ()
{
        return EOK;
}

static inline errno_t proc_fan_packet_recv ()
{
        printf("Proc fan packet receive\n");
        return EOK;
}

#endif
