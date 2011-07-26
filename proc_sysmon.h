#ifndef __PROC_SYSMON_H_
#define __PROC_SYSMON_H_

#include "process.h"

static inline errno_t proc_sysmon_init ()
{
        printf("Proc sysmon init\n");
        return EOK;
}

static inline errno_t proc_sysmon_fini ()
{
        printf("Proc sysmon fini\n");
        return EOK;
}

static inline errno_t proc_sysmon_periodic_task ()
{
        static int id = 0;
        u64 samples = 0, count = 0;
        ptime_t start = get_time();

        while(get_time() - start < 1000000) // 1ms
        {
                samples++;
                count += mmio_rd32(0x400700) & 0x1;
        }

        printf("%u, %llu\n", id++, count*100/samples);

        return EOK;
}

static inline errno_t proc_sysmon_packet_recv ()
{
        printf("Proc sysmon packet receive\n");
        return EOK;
}

#endif
