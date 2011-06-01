#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "common.h"

typedef errno_t (*process_init) ();
typedef errno_t (*process_fini) ();
typedef errno_t (*process_periodic_task) ();
typedef errno_t (*process_packet_recv) ();
	
typedef struct
{
	char id[PROCESS_ID_LENGTH + 1];
	process_init init;
	process_fini fini;
	process_periodic_task periodic_task; // The task should last less than 10ms
	process_packet_recv packet_recv;
} process_t;

errno_t process_create(process_t *process,
			const char *id,
			process_init init,
			process_fini fini,
			process_periodic_task periodic_task,
			process_packet_recv packet_recv);

#endif