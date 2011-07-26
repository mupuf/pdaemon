#ifndef __SCHED_H_
#define __SCHED_H_

#include "process.h"

typedef int pid_t;

typedef enum
{
        INIT = -1,
        STARTED = 0,
        CRITICAL = 1,
        PAUSED = 2,
} status_t;

typedef enum
{
        IDLE = 0,
        LOW = 1,
        NORMAL = 2,
        HIGH = 3,
        HIGHEST = 4
} priority_t;

typedef struct
{
        pid_t pid;
        priority_t priority;    /* Important for message queuing */
        status_t status;

        /* sched */
        ptime_t periodicity;
        ptime_t next_exec;

        process_t process;
} sched_process_t;

sched_process_t processes[PROCESS_MAX];
sched_process_t *cur_proc = NULL;

/******************************************************************************/

static inline pid_t get_first_pid()
{
        for (int i=0; i < PROCESS_MAX; i++) {
                if (processes[i].status == INIT)
                        return i;
        }
        return -1;
}

static inline  int sched_pid_is_free(pid_t pid)
{
        return pid < PROCESS_MAX && processes[pid].status == INIT;
}

/******************************************************************************/

static inline errno_t sched_proc_pause(pid_t pid)
{
        sched_process_t *process = &processes[pid];

        if (process->status == INIT)
                return -ESRCH;
        else if (process->status == CRITICAL)
                return -EACCES;

        process->status = PAUSED;

        return -EOK;
}

static inline void sched_init()
{
        int i;
        for(i = 0; i < PROCESS_MAX; i++) {
                processes[i].status = INIT;
        }
}

static inline void sched_start()
{
        int i;
        while (1) {
                for(i = 0; i < PROCESS_MAX; i++) {
                        cur_proc = &processes[i];

                        if (cur_proc->status == STARTED) {
                                cur_proc->process.periodic_task();

                                if (cur_proc->status != STARTED) {
                                        printf("Proc %s exited with status %i\n",
                                                cur_proc->process.id,
                                                cur_proc->status);
                                }
                                cur_proc->status = STARTED;
                        }

                        cur_proc = NULL;
                }
        }
}

static inline errno_t sched_add_process(process_t process, ptime_t periodicity, pid_t* _pid)
{
        pid_t pid;

        if (!pid)
                return -EACCES;

        pid = get_first_pid();
        if (!sched_pid_is_free(pid))
                return -EACCES;

        if (!process.init ||
            !process.fini ||
            !process.periodic_task ||
            !process.packet_recv)
                return -EFAULT;

        processes[pid].process = process;

        if (processes[pid].process.init)
                processes[pid].process.init();

        processes[pid].periodicity = periodicity;
        processes[pid].next_exec = get_time(); /* schedule for ASAP */
        processes[pid].status = STARTED;

        *_pid = pid;
        return -EOK;
}

static inline errno_t sched_proc_enter_critical()
{
        irq_off();

        if (cur_proc)
                cur_proc->status = CRITICAL;
}

static inline errno_t sched_proc_leave_critical()
{
        irq_on();

        if (cur_proc)
                cur_proc->status = STARTED;
}

#endif
