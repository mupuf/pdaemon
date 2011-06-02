#ifndef __SCHED_H_
#define __SCHED_H_

#include "process.h"

typedef int pid_t;

void sched_init();
void sched_start();

errno_t sched_add_process(process_t process, time_t periodicity, pid_t* _pid);

errno_t sched_proc_enter_critical();
errno_t sched_proc_leave_critical();

#endif