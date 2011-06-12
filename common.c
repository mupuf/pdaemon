#include "common.h"
#include "simul/nva.h"
#include "regs.h"

#ifdef SIMULATION
#define NVA_CARD 0
	u32 mmio_rd32(u32 address)
	{
		return nva_rd32(NVA_CARD, address);
	}

	void mmio_wr32(u32 address, u32 value)
	{
		nva_wr32(NVA_CARD, address, value);
	}

	int irq_activated = 1;
	void irq_on() { irq_activated = 1; }
	void irq_off() { irq_activated = 0; }
#else
	void printf(const char *blabla, ...) { }

	void irq_on()
	{
	}

	void irq_off()
	{
	}
#endif

time_t get_time()
{
	time_t low;

	/* From kmmio dumps on nv28 this looks like how the blob does this.
	* It reads the high dword twice, before and after.
	* The only explanation seems to be that the 64-bit timer counter
	* advances between high and low dword reads and may corrupt the
	* result. Not confirmed.
	*/
	time_t high2 = mmio_rd32(NV04_PTIMER_TIME_1);
	time_t high1;
	do {
		high1 = high2;
		low = mmio_rd32(NV04_PTIMER_TIME_0);
		high2 = mmio_rd32(NV04_PTIMER_TIME_1);
	} while (high1 != high2);
	return ((((time_t)high2) << 32) | (time_t)low >> 5)*10;
}