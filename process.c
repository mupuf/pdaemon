#include "process.h"
#include "string.h"

errno_t process_create(process_t *process,
			const char *id,
			process_init init,
			process_fini fini,
			process_periodic_task periodic_task,
			process_packet_recv packet_recv)
{
	strncpy(process->id, id, 20);
	process->init = init;
	process->fini = fini;
	process->periodic_task = periodic_task;
	process->packet_recv = packet_recv;

	return -EOK;
}