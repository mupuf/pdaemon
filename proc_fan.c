#include "proc_fan.h"

errno_t proc_fan_init ()
{
	printf("Proc fan init\n");
}

errno_t proc_fan_fini ()
{
	printf("Proc fan fini\n");
}

errno_t proc_fan_periodic_task ()
{
	printf("Proc fan periodic task\n");
}

errno_t proc_fan_packet_recv ()
{
	printf("Proc fan packet receive\n");
}