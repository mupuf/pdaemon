all: run nva3_pdaemon.out.fuc
.PHONY: clean

ET=../new_envytools
RM=/bin/rm

CFLAGS= -I$(ET)/include -Wall -Wno-unused-function -O2 
LDFLAGS= -L$(ET)/build/nva -lnva -lpciaccess
ED2AS=$(ET)/build/envydis/envyas -w -m fuc -V nva3
ED2DIS=$(ET)/build/envydis/envydis -w -m fuc -V nva3

clean:
	-$(RM) nva3_pdaemon.fuc.h

nva3_pdaemon.fuc.h: clean nva3_pdaemon.fuc
	$(ED2AS) -a nva3_pdaemon.fuc -o nva3_pdaemon.fuc.h

nva3_pdaemon.out.fuc: nva3_pdaemon.fuc
	$(ED2AS) nva3_pdaemon.fuc | $(ED2DIS) -d 24 > out.fuc

run: run.c nva3_pdaemon.fuc.h
	$(CC) run.c -g -o run $(CFLAGS) $(LDFLAGS)
