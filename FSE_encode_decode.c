#include <stdio.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#include "FSE.h"

u64 le64 (const u8 *buf, int *off)
{
	*off = *off + 8;
	return buf[*off - 7] | buf[*off - 6] << 8 | buf[*off - 5] << 16 | buf[*off - 4] << 24 | buf[*off - 3] << 32 | buf[*off - 2] << 40 | buf[*off - 1] << 48 | buf[*off] << 56;
}

u32 le32 (const u8 *buf, int *off)
{
	*off = *off + 4;
	return buf[*off - 4] | buf[*off - 3] << 8 | buf[*off - 2] << 16 | buf[*off - 1] << 24;
}

u16 le16 (const u8 *buf, int *off)
{
	*off = *off + 2;
	return buf[*off - 2] | buf[*off - 1] << 8;
}

u8 le8 (const u8 *buf, int *off)
{
	*off = *off + 1;
	return buf[(*off) - 1];
}

int main(int argc, char **argv)
{
	struct FSE_ucode code, *ucode = &code;
	int i,size;

	/* create a script */
	FSE_init(ucode);
	FSE_write(ucode, 0x12345678, 0xdeadbeef);
	FSE_write(ucode, 0x12345678, 0xef);
	FSE_mask(ucode, 0x12345678, 0x11111111, 0x10101010);
	FSE_wait(ucode, 0x12345678, 0x42424242, 0x02020202);
	FSE_delay_ns(ucode, 0x123456789abcdef);
	FSE_delay_ns(ucode, 0x42);
	FSE_delay_ns(ucode, 0x42000);
	FSE_delay_ns(ucode, 6553500);
	FSE_send_msg(ucode, 13, "Hello World!");
	FSE_fini(ucode);

	/* print the generated code */
	printf("encoded program: ucode->len = %i bytes", ucode->len);
	for (i = 0; i < ucode->len; i++) {
		if (i % 16 == 0)
			printf("\n%08x: ", i);
		printf("%01x ", ucode->ptr.u08[i]);
	}
	printf("\n\n");

	/* decode */
	printf("decoded program:\n");
	i = 0;
	while (i < ucode->len) {
		u8 opcode = le8(ucode->ptr.u08, &i);
		u64 p1, p2, p3;

		switch (opcode) {
			case 0x00:
				p1 = le64(ucode->ptr.u08, &i);
				printf("FSE_delay_full(0x%016llx)\n", p1 * 32);
				break;

			case 0x01:
				p1 = le16(ucode->ptr.u08, &i);
				printf("FSE_delay_ns(0x%04x)\n", p1 * 32);
				break;

			case 0x02:
				p1 = le16(ucode->ptr.u08, &i);
				printf("FSE_delay_us(0x%04x)\n", p1);
				break;

			case 0x10:
				p1 = le32(ucode->ptr.u08, &i);
				p2 = le32(ucode->ptr.u08, &i);
				printf("FSE_write(0x%08x, 0x%08x);\n", p1, p2);
				break;

			case 0x11:
				p1 = le32(ucode->ptr.u08, &i);
				p2 = le8(ucode->ptr.u08, &i);
				printf("FSE_write(0x%02x, 0x%08x);\n", p1, p2);
				break;

			case 0x12:
				p1 = le32(ucode->ptr.u08, &i);
				p2 = le32(ucode->ptr.u08, &i);
				p3 = le32(ucode->ptr.u08, &i);
				printf("FSE_mask(0x%08x, 0x%08x, 0x%08x);\n", p1, p2, p3);
				break;

			case 0x13:
				p1 = le32(ucode->ptr.u08, &i);
				p2 = le32(ucode->ptr.u08, &i);
				p3 = le32(ucode->ptr.u08, &i);
				printf("FSE_wait(0x%08x, 0x%08x, 0x%08x);\n", p1, p2, p3);
				break;

			case 0x20:
				size = le16(ucode->ptr.u08, &i);
				printf("FSE_send_msg(0x%04x", size);
				while(size)
				{
				      printf(", 0x%02x", le8(ucode->ptr.u08, &i));
				      size--;
				}
				printf(");\n");
				break;

			case 0xff:
				printf("exit\n");
				break;

			default:
				printf("unknown opcode %1x\n", opcode);
				break;
		}
	}


	return 0;
}
