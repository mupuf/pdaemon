#include "proc_fan.h"
#include <sys/time.h>
#include <unistd.h>

errno_t proc_fan_init ()
{
	printf("Proc fan init\n");
}

errno_t proc_fan_fini ()
{
	printf("Proc fan fini\n");
}

time_t time_diff_us(struct timeval start, struct timeval end)
{
	return ((end.tv_sec) - (start.tv_sec))*1000000 + ((end.tv_usec) - (start.tv_usec));
}

errno_t proc_fan_periodic_task ()
{
	struct timeval start, end;
	time_t t_start, t_end;
	
	t_start = get_time();
	gettimeofday(&start, NULL);

	while (get_time() - t_start < 1000000);
	
	gettimeofday(&end, NULL);

	printf("1,000,000 cycles took: %d µs\n", time_diff_us(start, end));

	gettimeofday(&start, NULL);
	t_start = get_time();
	
	do
	{
		gettimeofday(&end, NULL);
	} while (time_diff_us(start, end) < 1000000);

	gettimeofday(&end, NULL);
	t_end = get_time();

	printf("1s = %d cycles\n\n", t_end - t_start);
}

errno_t proc_fan_packet_recv ()
{
	printf("Proc fan packet receive\n");
}