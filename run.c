#include <stdio.h>
#include <unistd.h>
#include "nva.h"
#include "nva3_pdaemon.fuc.h"

static void data_segment_dump(unsigned int cnum, uint16_t base, uint32_t length)
{
	uint32_t reg, i;

	base &= 0xfffc;

	printf("Data segment dump: base = %x, length = %x", base, length);
	nva_wr32(cnum,0x10a1c8, 0x02000000 | base);
	for (i = 0; i < length / 4; i++) {
		if (i % 4 == 0)
			printf("\n%08x: ",  base + i * 4);
		reg = nva_rd32(cnum, 0x10a1cc);
		printf("%08x ",  reg);
	}
	printf("\n");
}

static void data_segment_upload(unsigned int cnum, uint16_t base,
				uint32_t *data, uint16_t length)
{
	uint32_t i;
	base &= 0xfffc; /* make sure it is 32 bits aligned */

	nva_wr32(cnum,0x10a1c8, 0x01000000 | base);
	for (i = 0; i < length; i++)
		nva_wr32(cnum, 0x10a1cc, data[i]);
}

static void pdaemon_upload(unsigned int cnum) {
	int i;

	/* reboot PDAEMON */
	nva_mask(cnum, 0x200, 0x2000, 0);
	nva_mask(cnum, 0x022210, 0x1, 0x0);
	nva_mask(cnum, 0x022210, 0x1, 0x1);
	nva_mask(cnum, 0x200, 0x2000, 0x2000);
	nva_wr32(cnum, 0x10a014, 0xffffffff); /* disable all interrupts */

	/* data upload */
	data_segment_upload(cnum, 0, nva3_pdaemon_data,
			  sizeof(nva3_pdaemon_data)/sizeof(*nva3_pdaemon_data));

	/* code upload */
	nva_wr32(cnum, 0x10a180, 0x01000000);
	for (i = 0; i < sizeof(nva3_pdaemon_code)/sizeof(*nva3_pdaemon_code); ++i) {
		if (i % 64 == 0)
			nva_wr32(cnum, 0x10a188, i >> 6);
		nva_wr32(cnum, 0x10a184, nva3_pdaemon_code[i]);
	}

	/* launch */
	nva_wr32(cnum, 0x10a104, 0x0);
	nva_wr32(cnum, 0x10a10c, 0x0);
	nva_wr32(cnum, 0x10a100, 0x2);
}

static void pdaemon_RB_state_dump(unsigned int cnum)
{
	printf("PDAEMON's FIFO 0 state: Get(%08x) Put(%08x)\n",
	       nva_rd32(cnum, 0x10a4b0), nva_rd32(cnum, 0x10a4a0));
	data_segment_dump(cnum, nva3_pdaemon_ptrs[3], nva3_pdaemon_ptrs[4] - nva3_pdaemon_ptrs[3]);
	printf("\n");
}

static void pdaemon_send_cmd(unsigned int cnum, uint8_t pid, uint32_t *data, uint16_t length)
{
	uint32_t put = nva_rd32(cnum, 0x10a4a0);
	uint32_t next_put = nva3_pdaemon_ptrs[3] + ((put + 4) % 0x40);
	uint32_t header = ((pid & 0xf) << 28) | ((length & 0xfff) << 16) | (nva3_pdaemon_ptrs[4] & 0xffff);

	/* TODO: Find an available space or wait for GET == PUT */

	/* wait for some space in the ring buffer */
	while (next_put == nva_rd32(cnum, 0x10a4b0));

	/* push the commands */
	data_segment_upload(cnum, put, &header, 1);
	nva_wr32(cnum, 0x10a4a0, next_put);
}

int main(int argc, char **argv)
{
	if (nva_init()) {
		fprintf (stderr, "PCI init failure!\n");
		return 1;
	}
	int c;
	int cnum =0;
	while ((c = getopt (argc, argv, "c:")) != -1)
		switch (c) {
			case 'c':
				sscanf(optarg, "%d", &cnum);
				break;
		}
	if (cnum >= nva_cardsnum) {
		if (nva_cardsnum)
			fprintf (stderr, "No such card.\n");
		else
			fprintf (stderr, "No cards found.\n");
		return 1;
	}

	pdaemon_upload(cnum);
	usleep(1000);

	pdaemon_RB_state_dump(cnum);
	data_segment_dump(cnum, 0x0, 0x10);

	printf("\n");
	pdaemon_send_cmd(cnum, 1, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);
	pdaemon_send_cmd(cnum, 1, NULL, 0);
	pdaemon_send_cmd(cnum, 2, NULL, 0);

	usleep(10000);
	printf("\n");

	pdaemon_RB_state_dump(cnum);
	data_segment_dump(cnum, 0x0, 0x10);

	while(1) {
		printf("PDAEMON: status = %x, intr = %x\n", nva_rd32(cnum, 0x10a04c), nva_rd32(cnum, 0x10a008));
		data_segment_dump(cnum, 0x0, 0x10);
		usleep(1000000);
	}
	return 0;
}
