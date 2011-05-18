#ifndef __PROC_FAN_H_
#define __PROC_FAN_H_

#include "process.h"

errno_t proc_fan_init ();
errno_t proc_fan_fini ();
errno_t proc_fan_periodic_task ();
errno_t proc_fan_packet_recv ();

#endif