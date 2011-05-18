#include "sched.h"
#include "proc_fan.h"

void init()
{
	process_t tmp_process;

	sched_init();

	if (process_create(&tmp_process,
			"proc_fan",
			proc_fan_init,
			proc_fan_fini,
			proc_fan_periodic_task,
			proc_fan_packet_recv) == -EOK)
	{
		sched_add_process(1, tmp_process);
	}

	sched_start();
}

#ifdef SIMULATION
int main(int argc, char** argv)
{
	init();
	return 0;
}
#endif