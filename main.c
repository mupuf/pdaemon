#include "sched.h"
#include "common.h"

#include "proc_fan.h"
#include "proc_sysmon.h"

void init()
{
	pid_t fan_pid, sysmon_pid;
	process_t tmp_process;

	sched_init();

	if (process_create(&tmp_process,
			"proc_fan",
			proc_fan_init,
			proc_fan_fini,
			proc_fan_periodic_task,
			proc_fan_packet_recv) == -EOK)
	{
		//sched_add_process(tmp_process, 100, &fan_pid);
	}

	if (process_create(&tmp_process,
			"proc_sysmon",
			proc_sysmon_init,
			proc_sysmon_fini,
			proc_sysmon_periodic_task,
			proc_sysmon_packet_recv) == -EOK)
	{
		sched_add_process(tmp_process, 100, &sysmon_pid);
	}

	sched_start();
}

#ifdef SIMULATION
#include "simul/nva.h"
int main(int argc, char** argv)
{
	nva_init();
	init();
	return 0;
}
#endif
