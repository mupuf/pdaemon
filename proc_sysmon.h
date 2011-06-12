#ifndef __PROC_SYSMON_H_
#define __PROC_SYSMON_H_

#include "process.h"

errno_t proc_sysmon_init ();
errno_t proc_sysmon_fini ();
errno_t proc_sysmon_periodic_task ();
errno_t proc_sysmon_packet_recv ();

#endif