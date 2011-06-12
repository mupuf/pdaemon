#include "proc_sysmon.h"
#include <sys/time.h>
#include <unistd.h>

errno_t proc_sysmon_init ()
{
	printf("Proc sysmon init\n");
}

errno_t proc_sysmon_fini ()
{
	printf("Proc sysmon fini\n");
}

errno_t proc_sysmon_periodic_task ()
{
	static int id = 0;
	u64 samples = 0, count = 0;
	time_t start = get_time();

	while(get_time() - start < 1000000) // 1ms
	{
		samples++;
		count += mmio_rd32(0x400700) & 0x1;
	}

	printf("%u, %u\n", id++, count*100/samples);
}

errno_t proc_sysmon_packet_recv ()
{
	printf("Proc sysmon packet receive\n");
}