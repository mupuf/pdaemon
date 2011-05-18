#include "common.h"
#include "sched.h"

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
	priority_t priority;	/* Important for message queuing */
	status_t status;
	process_t process;
} sched_process_t;

sched_process_t processes[PROCESS_MAX];
sched_process_t *cur_proc = NULL;

errno_t sched_proc_pause(pid_t pid)
{
	sched_process_t *process = &processes[pid];
	
	if (process->status == INIT)
		return -ESRCH;
	else if (process->status == CRITICAL)
		return -EACCES;

	process->status = PAUSED;

	return -EOK;
}

void sched_init()
{
	int i;
	for(i = 0; i < PROCESS_MAX; i++) {
		processes[i].status = INIT;
	}
}

void sched_start()
{
	int i;
	while (1) {
		for(i = 0; i < PROCESS_MAX; i++) {
			cur_proc = &processes[i];

			if (cur_proc->status == STARTED) {
				cur_proc->process.periodic_task();

				if (cur_proc->status != STARTED) {
					printf("Proc %s exited with the status %i\n", cur_proc->process.id,
							cur_proc->status);
				}
				cur_proc->status = STARTED;
			}

			cur_proc = NULL;
		}
	}
}

errno_t sched_add_process(pid_t pid, process_t process)
{
	if (processes[pid].status != INIT)
		return -EACCES;

	if (!process.init ||
		!process.fini ||
		!process.periodic_task ||
		!process.packet_recv)
		return -EFAULT;

	processes[pid].process = process;

	if (processes[pid].process.init)
		processes[pid].process.init();

	processes[pid].status = STARTED;

	return -EOK;
}

errno_t sched_proc_enter_critical()
{
	irq_off();

	if (cur_proc)
		cur_proc->status = CRITICAL;
}

errno_t sched_proc_leave_critical()
{
	irq_on();

	if (cur_proc)
		cur_proc->status = STARTED;
}